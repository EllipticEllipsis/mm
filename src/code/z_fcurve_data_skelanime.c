/**
 * File: z_fcurve_data_skelanime.c
 * Description: Curve skeleton animation system
 *
 * A curve skeleton has a fixed number of limbs, each of which has 9 propetries that may be changed by the animation:
 * - 3 scales,
 * - 3 rotations,
 * - 3 positions
 * (note the position is stored in the animations instead of being stored in the limbs like SkelAnime would). Otherwise the structure is similar to an ordinary SkelAnime-compatible skeleton.
 *
 * The animations are significantly more complex than SkelAnime. A curve animation consists of 4 parts:
 * - a header (really a footer, for it is always last in the object file)
 * - a list of counts for the 9 properties of each limb (u8)
 * - a list of interpolation data (CurveInterpKnot). The length is the sum of the counts.
 * - a list of constant data (). The length is the number of 0 in counts.
 *
 * If the interpolation count for a property is 0, the value of the property is copied from the next number in the
 * constant data; there are no gaps for nonzero interpolation count.
 * If the interpolation count N for a property is larger than 0, the next N elements of the interpolation data array
 * are used to interpolate the value of the property, using Curve_Interpolate.
 * 
 * Curve limbs may use LOD:
 * - lower detail draws only the first displaylist
 * - higher detail draws both.
 */

#include "global.h"
#include "z64curve.h"

void SkelCurve_Clear(SkelCurve* skelCurve) {
    skelCurve->limbCount = 0;
    skelCurve->skeleton = NULL;
    skelCurve->animation = NULL;
    skelCurve->curFrame = 0.0f;
    skelCurve->playSpeed = 0.0f;
    skelCurve->endFrame = 0.0f;
    skelCurve->unk_0C = 0.0f;
    skelCurve->jointTable = NULL;
}

/**
 * Initialises the SkelCurve struct and mallocs the joint data.
 *
 * @return bool always true
 */
s32 SkelCurve_Init(GlobalContext* globalCtx, SkelCurve* skelCurve, CurveSkeletonHeader* skeletonHeaderSeg,
                   CurveAnimationHeader* animation) {
    SkelCurveLimb** limbs;
    CurveSkeletonHeader* skeletonHeader = Lib_SegmentedToVirtual(skeletonHeaderSeg);

    skelCurve->limbCount = skeletonHeader->limbCount;
    skelCurve->skeleton = Lib_SegmentedToVirtual(skeletonHeader->limbs);

    skelCurve->jointTable = ZeldaArena_Malloc(sizeof(*skelCurve->jointTable) * skelCurve->limbCount);

    skelCurve->curFrame = 0.0f;
    return true;
}

/**
 * Frees the joint data.
 */
void SkelCurve_Destroy(GlobalContext* globalCtx, SkelCurve* skelCurve) {
    if (skelCurve->jointTable != NULL) {
        ZeldaArena_Free(skelCurve->jointTable);
    }
}

void SkelCurve_SetAnim(SkelCurve* skelCurve, CurveAnimationHeader* animation, f32 arg2, f32 endFrame, f32 curFrame,
                       f32 playSpeed) {
    skelCurve->unk_0C = arg2 - skelCurve->playSpeed;
    skelCurve->endFrame = endFrame;
    skelCurve->curFrame = curFrame;
    skelCurve->playSpeed = playSpeed;
    skelCurve->animation = animation;
}

typedef enum {
    /* 0 */ SKELCURVE_VEC_TYPE_SCALE,
    /* 1 */ SKELCURVE_VEC_TYPE_ROTATION,
    /* 2 */ SKELCURVE_VEC_TYPE_POSIITON,
    /* 3 */ SKELCURVE_VEC_TYPE_MAX,
} SkelCurveVecType;

#define SKELCURVE_SCALE_SCALE 1024.0f
#define SKELCURVE_SCALE_ROTATION (32768.0f / 180.0f)
#define SKELCURVE_SCALE_POSITION 100

/**
 * The only animation updating function.
 *
 * @return bool true when the animation has finished.
 */
s32 SkelCurve_Update(GlobalContext* globalCtx, SkelCurve* skelCurve) {
    s16* jointData;
    u8* knotCounts;
    CurveAnimationHeader* animation;
    u16* constantData;
    s32 curLimb;
    s32 ret = false;
    s32 coord;
    CurveInterpKnot* startKnot;
    s32 vecType;

    animation = Lib_SegmentedToVirtual(skelCurve->animation);
    knotCounts = Lib_SegmentedToVirtual(animation->knotCounts);
    startKnot = Lib_SegmentedToVirtual(animation->interpolationData);
    constantData = Lib_SegmentedToVirtual(animation->constantData);
    jointData = *skelCurve->jointTable;

    skelCurve->curFrame += skelCurve->playSpeed * ((s32)globalCtx->state.framerateDivisor * 0.5f);

    if (((skelCurve->playSpeed >= 0.0f) && (skelCurve->curFrame > skelCurve->endFrame)) ||
        ((skelCurve->playSpeed < 0.0f) && (skelCurve->curFrame < skelCurve->endFrame))) {
        skelCurve->curFrame = skelCurve->endFrame;
        ret = true;
    }

    for (curLimb = 0; curLimb < skelCurve->limbCount; curLimb++) {
        for (vecType = SKELCURVE_VEC_TYPE_SCALE; vecType < SKELCURVE_VEC_TYPE_MAX;
             vecType++) {                         // scale/rotation/position
            for (coord = 0; coord < 3; coord++) { // x/y/z
                f32 transformValue;

                if (*knotCounts == 0) {
                    transformValue = *constantData;
                    *jointData = transformValue;
                    constantData++;
                } else {
                    transformValue = Curve_Interpolate(skelCurve->curFrame, startKnot, *knotCounts);
                    startKnot += *knotCounts;
                    if (vecType == SKELCURVE_VEC_TYPE_SCALE) {
                        // Rescaling allows for more refined scaling using an s16
                        *jointData = transformValue * SKELCURVE_SCALE_SCALE;
                    } else if (vecType == SKELCURVE_VEC_TYPE_ROTATION) {
                        // Convert value from degrees to a binary angle
                        *jointData = transformValue * SKELCURVE_SCALE_ROTATION;
                    } else { // SKELCURVE_VEC_TYPE_POSIITON
                        // Model to world scale conversion
                        *jointData = transformValue * SKELCURVE_SCALE_POSITION;
                    }
                }
                knotCounts++;
                jointData++;
            }
        }
    }

    return ret;
}

/**
 * Recursively draws limbs with appropriate properties.
 */
void SkelCurve_DrawLimb(GlobalContext* globalCtx, s32 limbIndex, SkelCurve* skelCurve,
                        OverrideCurveLimbDraw overrideLimbDraw, PostCurveLimbDraw postLimbDraw, s32 lod, Actor* thisx) {
    SkelCurveLimb* limb = Lib_SegmentedToVirtual(skelCurve->skeleton[limbIndex]);

    OPEN_DISPS(globalCtx->state.gfxCtx);

    Matrix_StatePush();

    if ((overrideLimbDraw == NULL) ||
        ((overrideLimbDraw != NULL) && overrideLimbDraw(globalCtx, skelCurve, limbIndex, thisx))) {
        Vec3f scale;
        Vec3s rot;
        Vec3f pos;
        Gfx* dList;
        s16* jointData = skelCurve->jointTable[limbIndex];

        scale.x = jointData[0] / SKELCURVE_SCALE_SCALE;
        scale.y = jointData[1] / SKELCURVE_SCALE_SCALE;
        scale.z = jointData[2] / SKELCURVE_SCALE_SCALE;
        jointData += 3;
        rot.x = jointData[0];
        rot.y = jointData[1];
        rot.z = jointData[2];
        jointData += 3;
        pos.x = jointData[0];
        pos.y = jointData[1];
        pos.z = jointData[2];

        Matrix_JointPosition(&pos, &rot);
        Matrix_Scale(scale.x, scale.y, scale.z, MTXMODE_APPLY);

        if (lod == 0) {
            s32 pad1;

            dList = limb->dList[0];
            if (dList != NULL) {
                gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx),
                          G_MTX_LOAD | G_MTX_NOPUSH | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_OPA_DISP++, dList);
            }
        } else if (lod == 1) {
            s32 pad2;

            dList = limb->dList[0];
            if (dList != NULL) {
                gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx),
                          G_MTX_LOAD | G_MTX_NOPUSH | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_OPA_DISP++, dList);
            }
            dList = limb->dList[1];
            if (dList != NULL) {
                gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx),
                          G_MTX_LOAD | G_MTX_NOPUSH | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++, dList);
            }
        }
    }

    if (postLimbDraw != NULL) {
        postLimbDraw(globalCtx, skelCurve, limbIndex, thisx);
    }

    if (limb->child != LIMB_DONE) {
        SkelCurve_DrawLimb(globalCtx, limb->child, skelCurve, overrideLimbDraw, postLimbDraw, lod, thisx);
    }

    Matrix_StatePop();

    if (limb->sibling != LIMB_DONE) {
        SkelCurve_DrawLimb(globalCtx, limb->sibling, skelCurve, overrideLimbDraw, postLimbDraw, lod, thisx);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx);
}

// The first and last arguments are used inconsistently in different actors.
void SkelCurve_Draw(Actor* actor, GlobalContext* globalCtx, SkelCurve* skelCurve,
                    OverrideCurveLimbDraw overrideLimbDraw, PostCurveLimbDraw postLimbDraw, s32 lod, Actor* thisx) {
    if (skelCurve->jointTable != NULL) {
        SkelCurve_DrawLimb(globalCtx, 0, skelCurve, overrideLimbDraw, postLimbDraw, lod, thisx);
    }
}
