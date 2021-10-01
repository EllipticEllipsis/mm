#include "global.h"

#pragma GLOBAL_ASM("asm/non_matchings/code/speed_meter/func_80177390.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/speed_meter/func_801773A0.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/speed_meter/func_801773C4.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/speed_meter/SpeedMeter_DrawTimeEntries.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/speed_meter/func_80177A84.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/speed_meter/func_80177AC8.s")

#pragma GLOBAL_ASM("asm/non_matchings/code/speed_meter/SpeedMeter_DrawAllocEntries.s")

// #pragma GLOBAL_ASM("asm/non_matchings/code/speed_meter/func_801780F0.s")
void func_801780F0(Mtx* mtx, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    Mtx_t* m = &mtx->m;
    u16(*m1)[4] = (u16*)(*m)[0];
    // u16* m2 = (u16*)(*m)[2];
    u32 temp;

    (*m)[0][1] = 0;
    (*m)[2][1] = 0;
    (*m)[0][3] = 0;
    (*m)[2][3] = 0;
    (*m)[1][0] = 0;

    temp = (s32)(arg1 * 0x10000);
    (*m)[0][0] = temp;

    m1[0][1] = 0;
    (*m)[2][0] = temp << 16;

    temp = (s32)(arg2 * 0x10000);
    (*m)[0][2] = temp >> 16;
    (*m)[2][2] = temp & 0xFFFF;

    temp = (s32)(arg3 * 0x10000);
    (*m)[1][1] = temp;
    m1[2][3] = 0;
    (*m)[3][1] = temp << 16;

    (*m)[3][0] = 0;

    temp = (s32)(arg4 * 0x10000);
    m1[3][0] = (temp >> 16) & 0xFFFF;
    m1[7][0] = temp & 0xFFFF;

    temp = (s32)(arg5 * 0x10000);
    m1[3][1] = (temp >> 16) & 0xFFFF;
    m1[7][1] = temp & 0xFFFF;

    temp = (s32)(arg6 * 0x10000);
    m1[3][2] = (temp >> 16) & 0xFFFF;
    m1[3][3] = 1;
    (*m)[3][3] = temp << 16;
}

// #pragma GLOBAL_ASM("asm/non_matchings/code/speed_meter/func_801781EC.s")
// void func_801781EC(Mtx* mtx, s16 angle, f32 axisX, f32 axisY, f32 axisZ);
void func_801781EC(Mtx* mtx, s32 angle, f32 axisX, f32 axisY, f32 axisZ) {
    Mtx_t* m = &mtx->m;
    u16* m1 = (u16*)(*m)[0];
    s32 temp_f8;
    f32 sp48 = Math_SinS(angle);
    f32 temp_f0 = Math_CosS(angle);
    
    f32 new_var4;
    f32 new_var3;
    f32 new_var2;
    f32 temp_f14;

    
    mtx->m[1][2] = 0;
    mtx->m[1][3] = 1;
    mtx->m[3][2] = 0;
    mtx->m[3][3] = 0;

    temp_f14 = axisX * axisX;
    temp_f8 = (((1.0f - temp_f14) * temp_f0) + temp_f14) * 0x10000;
    m1[0] = ((u32)temp_f8) >> 0x10 & 0xFFFF;
    m1[16 + 0] = temp_f8 & 0xFFFF;

    new_var4 = axisZ; // Very fake, but required for regalloc
    temp_f8 = (((axisY * axisZ) * (1.0f - temp_f0)) - (axisX * sp48)) * 0x10000;
    m1[9] = ((u32)temp_f8) >> 0x10 & 0xFFFF;
    m1[16 + 9] = temp_f8 & 0xFFFF;
    temp_f8 = ((axisX * sp48) + ((axisY * new_var4) * (1.0f - temp_f0))) * 0x10000; 

    new_var2 = (new_var4 * axisX) * (1.0f - temp_f0);
    mtx->m[0][3] = temp_f8;
    m1[7] = 0;
    mtx->m[2][3] = temp_f8 << 0x10;

    temp_f14 = axisY * axisY;
    temp_f8 = ((((1.0f - temp_f14) * temp_f0) + temp_f14) * 0x10000);
    m1[5] = ((u32)temp_f8) >> 0x10 & 0xFFFF;
    m1[16 + 5] = temp_f8 & 0xFFFF;

    temp_f8 = (new_var2 - (axisY * sp48)) * 0x10000;
    m1[8] = ((u32)temp_f8) >> 0x10 & 0xFFFF;
    m1[16 + 8] = temp_f8 & 0xFFFF;

    temp_f8 = (new_var2 + (axisY * sp48)) * 0x10000;
    mtx->m[0][1] = temp_f8;
    m1[3] = 0;
    mtx->m[2][1] = temp_f8 << 0x10;

    new_var3 = ((axisX * axisY) * (1.0f - temp_f0));

    temp_f14 = new_var4 * new_var4;
    temp_f8 = ((((1.0f - temp_f14) * temp_f0) + temp_f14) * 0x10000);
    mtx->m[1][1] = temp_f8;
    m1[11] = 0;
    mtx->m[3][1] = temp_f8 << 0x10;
    
    temp_f8 = (new_var3 - new_var4 * sp48) * 0x10000;
    m1[4] = ((u32)temp_f8) >> 0x10 & 0xFFFF;
    m1[16 + 4] = temp_f8 & 0xFFFF;

    temp_f8 = (new_var3 + new_var4 * sp48) * 0x10000;
    m1[1] = ((u32)temp_f8) >> 0x10 & 0xFFFF;
    m1[16 + 1] = temp_f8 & 0xFFFF;
}

#pragma GLOBAL_ASM("asm/non_matchings/code/speed_meter/func_8017842C.s")

// void func_8017842C(Mtx *mtx, f32 scaleX, f32 scaleY, f32 scaleZ, s16 angle, f32 axisX, f32 axisY, f32 axisZ, f32
// posX, f32 posY, f32 posZ) {
//     f32 sp40;
//     f32 sp28;
//     f32 sp24;
//     f32 sp20;
//     f32 sp1C;
//     f32 sp18;
//     f32 temp_f0;
//     f32 temp_f10_3;
//     f32 temp_f12;
//     f32 temp_f12_2;
//     f32 temp_f12_3;
//     f32 temp_f4;
//     f32 temp_f6;
//     f32 temp_f6_3;
//     f32 temp_f6_4;
//     f32 temp_f8;
//     f32 temp_f8_3;
//     s32 temp_f10;
//     s32 temp_f10_2;
//     s32 temp_f10_4;
//     s32 temp_f10_5;
//     s32 temp_f4_2;
//     s32 temp_f4_3;
//     s32 temp_f4_4;
//     s32 temp_f6_2;
//     s32 temp_f6_5;
//     s32 temp_f8_2;
//     s32 temp_f8_4;
//     s32 temp_t9;

//     mtx = mtx;
//     sp28 = (bitwise f32) angle;
//     sp40 = Math_SinS(angle);
//     temp_f0 = Math_CosS(angle);
//     temp_f12 = axisX * axisX;
//     temp_f6 = 1.0f - temp_f0;
//     temp_t9 = (s32) ((((1.0f - temp_f12) * temp_f0) + temp_f12) * scaleX * 0x10000);
//     mtx->m = (s16) ((u32) temp_t9 >> 0x10);
//     mtx->m[2] = (s16) temp_t9;
//     sp28 = temp_f6;
//     temp_f4 = axisY * axisZ * temp_f6;
//     temp_f8 = axisX * sp40;
//     sp24 = temp_f4;
//     sp1C = temp_f8;
//     temp_f6_2 = (s32) ((temp_f4 - sp1C) * scaleZ * 0x10000);
//     mtx->unk12 = (s16) ((u32) temp_f6_2 >> 0x10);
//     mtx->unk32 = (s16) temp_f6_2;
//     temp_f12_2 = axisY * axisY;
//     temp_f10 = (s32) ((temp_f8 + temp_f4) * scaleY * 0x10000);
//     mtx->m[0][3] = temp_f10;
//     mtx->unkE = 0;
//     mtx->m[2][3] = temp_f10 << 0x10;
//     temp_f10_2 = (s32) ((((1.0f - temp_f12_2) * temp_f0) + temp_f12_2) * scaleY * 0x10000);
//     mtx->unkA = (s16) ((u32) temp_f10_2 >> 0x10);
//     mtx->unk2A = (s16) temp_f10_2;
//     temp_f10_3 = axisZ * axisX * sp28;
//     temp_f6_3 = axisY * sp40;
//     sp20 = temp_f10_3;
//     sp18 = temp_f6_3;
//     temp_f4_2 = (s32) ((temp_f10_3 - sp18) * scaleZ * 0x10000);
//     mtx->m[1] = (s16) ((u32) temp_f4_2 >> 0x10);
//     mtx->m[3] = (s16) temp_f4_2;
//     temp_f8_2 = (s32) ((temp_f6_3 + temp_f10_3) * scaleX * 0x10000);
//     mtx->m[0][1] = temp_f8_2;
//     mtx->unk6 = 0;
//     mtx->m[2][1] = temp_f8_2 << 0x10;
//     temp_f12_3 = axisZ * axisZ;
//     temp_f4_3 = (s32) ((((1.0f - temp_f12_3) * temp_f0) + temp_f12_3) * scaleZ * 0x10000);
//     mtx->m[1][1] = temp_f4_3;
//     mtx->unk16 = 0;
//     mtx->m[3][1] = temp_f4_3 << 0x10;
//     temp_f8_3 = axisX * axisY * sp28;
//     temp_f6_4 = axisZ * sp40;
//     sp24 = temp_f8_3;
//     sp1C = temp_f6_4;
//     temp_f10_4 = (s32) ((temp_f8_3 - temp_f6_4) * scaleY * 0x10000);
//     mtx->m[0][2] = (s16) ((u32) temp_f10_4 >> 0x10);
//     mtx->m[2][2] = (s16) temp_f10_4;
//     temp_f6_5 = (s32) ((sp1C + temp_f8_3) * scaleX * 0x10000);
//     mtx->unk2 = (s16) ((u32) temp_f6_5 >> 0x10);
//     mtx->unk22 = (s16) temp_f6_5;
//     temp_f8_4 = (s32) (posX * 0x10000);
//     mtx->m[1][2] = temp_f8_4;
//     mtx->m[3][2] = (s16) temp_f8_4;
//     temp_f4_4 = (s32) (posY * 0x10000);
//     mtx->unk1A = (s16) ((u32) temp_f4_4 >> 0x10);
//     mtx->unk3A = (s16) temp_f4_4;
//     temp_f10_5 = (s32) (posZ * 0x10000);
//     mtx->m[1][3] = temp_f10_5;
//     mtx->unk1E = 1;
//     mtx->m[3][3] = temp_f10_5 << 0x10;
// }
// void func_8017842C(Mtx *mtx, f32 scaleX, f32 scaleY, f32 scaleZ, s32 angle, f32 axisX, f32 axisY, f32 axisZ, f32
// posX, f32 posY, f32 posZ) {
//     Mtx_t* m = &mtx->m;
//     u16 (*m1) = (u16*)(*m)[0];
//     // u16* m2 = (u16*)(*m)[2];

//     // f32 temp_f4;
//     // f32 temp_f6;
//     // f32 temp_f8;
//     s32 temp;
//     f32 sin; // sine
//     f32 cos; // cos

//     f32 tempXX;
//     f32 tempYY;
//     f32 tempZZ;
//     f32 tempYZ;
//     // f32 tempZX;
//     // f32 tempXY;
//     // s32 pad[2];

//     sin = Math_SinS(angle);
//     cos = Math_CosS(angle);

//     tempXX = axisX * axisX;
//     tempYY = axisY * axisY;
//     tempZZ = axisZ * axisZ;
//     // tempZX = axisZ * axisX;
//     // tempXY = axisX * axisY;

//     temp = (((1.0f - tempXX) * cos) + tempXX) * scaleX * 0x10000;
//     m1[0] = ((u32) temp >> 0x10) & 0xFFFF;
//     m1[16+0] = temp & 0xFFFF;

//     tempYZ = axisY * axisZ;
//     // temp_f6 = 1.0f - cos;
//     // temp_f4 = tempYZ * (1.0f - cos);
//     // temp_f8 = axisX * sin;
//     temp = ( (1.0f - cos) * tempYZ - axisX * sin) * scaleZ * 0x10000;
//     m1[9] = ((u32) temp >> 0x10) & 0xFFFF;
//     m1[16+9] = temp & 0xFFFF;

//     // temp_f12_2 = axisY * axisY;
//     temp = ((1.0f - cos) * tempYZ + axisX * sin) * scaleY * 0x10000;
//     (*m)[0][3] = temp;
//     m1[7] = 0;
//     (*m)[2][3] = temp << 0x10;

//     temp = (((1.0f - tempYY) * cos) + tempYY) * scaleY * 0x10000;
//     m1[5] = ((u32) temp >> 0x10) & 0xFFFF;
//     m1[16+5] = temp & 0xFFFF;

//     // temp_f4 = zx * (1.0f - cos);
//     // temp_f8 = axisY * sin;
//     temp = ((1.0f - cos) * axisZ * axisX - axisY * sin) * scaleZ * 0x10000;
//     m1[8] = ((u32) temp >> 0x10) & 0xFFFF;
//     m1[16+8] = temp & 0xFFFF;

//     temp = ((1.0f - cos) * axisZ * axisX + axisY * sin) * scaleX * 0x10000;
//     (*m)[0][1] = temp;
//     m1[3] = 0;
//     (*m)[2][1] = (temp << 0x10);

//     temp = (((1.0f - tempZZ) * cos) + tempZZ) * scaleZ * 0x10000;
//     (*m)[1][1] = temp;
//     m1[11] = 0;
//     (*m)[3][1] = temp << 0x10;

//     // temp_f4 = xy * (1.0f - cos);
//     // temp_f8 = axisZ * sin;
//     temp = ((1.0f - cos) * axisX * axisY - axisZ * sin) * scaleY * 0x10000;
//     m1[4] = ((u32) temp >> 0x10) & 0xFFFF;
//     m1[16+4] = temp & 0xFFFF;

//     temp = ((1.0f - cos) * axisX * axisY + axisZ * sin) * scaleX * 0x10000;
//     m1[1] = ((u32) temp >> 0x10) & 0xFFFF;
//     m1[16+1] = temp & 0xFFFF;

//     temp = (s32) (posX * 0x10000);
//     (*m)[1][2] = temp;
//     m1[16+12] = temp;

//     temp = (s32) (posY * 0x10000);
//     m1[13] = ((u32) temp >> 0x10) & 0xFFFF;
//     m1[16+13] = temp & 0xFFFF;

//     temp = (s32) (posZ * 0x10000);
//     (*m)[1][3] = temp;
//     m1[15] = 1;
//     (*m)[3][3] = (temp << 0x10);
// }
