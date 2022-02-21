#include "global.h"
#include "system_malloc.h"

UNK_TYPE4 D_80096C30 = 2;

// #ifdef NON_MATCHING
// // This needs lots of work. Mostly regalloc and getting the address of D_80096C30 placed in s5 at the beginning of the
// // function
void Load2_Relocate(void* allocatedVRamAddress, OverlayRelocationSection* overlayInfo, void* vRamAddress) {
    u32 sections[4];
    u32 relocatedValue;

    u32 relocOffset;
    u32 relocData;
    u32 unrelocatedAddress;
    u32 i;
    u32* relocDataP;
    u32* luiRefs[32];
    u32 luiVals[32];
    u32 relocatedAddress;
    u32 reloc;
    u32 vaddr;
    u32* luiInstRef;
    u32 allocu32 = (u32)allocatedVRamAddress;
    u32* regValP;
    u32 isLoNeg;

    // s32 signedOffset;
    // u32* lastInst;
    // u32* inst;
    // u32 relocation;
    // u32 relocationIndex;



    relocOffset = 0;
    relocatedValue = 0;
    unrelocatedAddress = 0;
    relocatedAddress = 0;

    // if (gOverlayLogSeverity >= 3) {
    //     osSyncPrintf("DoRelocation(%08x, %08x, %08x)\n", allocatedVRamAddress, overlayInfo, vRamAddress);
    //     osSyncPrintf("text=%08x, data=%08x, rodata=%08x, bss=%08x\n", overlayInfo->textSize, overlayInfo->dataSize,
    //                  overlayInfo->rodataSize, overlayInfo->bssSize);
    // }


    sections[0] = 0;
    sections[1] = allocu32;
    sections[2] = allocu32 + overlayInfo->textSize;
    sections[3] = sections[2] + overlayInfo->dataSize;

    for (i = 0; i < overlayInfo->nRelocations; i++) {
        reloc = overlayInfo->relocations[i];
        relocDataP = (u32*)(sections[reloc >> 0x1E] + (reloc & 0xFFFFFF));
        relocData = *relocDataP;

        switch (reloc & 0x3F000000) {
            case 0x2000000:
                /* R_MIPS_32
                 * Handles 32-bit address relocation.  Used in things such as
                 * jump tables.
                 */
                if ((*relocDataP & 0xF000000) == 0) {
                    luiInstRef = vRamAddress;
                    relocOffset = *relocDataP - (u32)luiInstRef;
                    relocatedValue = relocOffset + allocu32;
                    relocatedAddress = relocatedValue;
                    unrelocatedAddress = relocData;
                    *relocDataP = relocatedAddress;
                } else if (D_80096C30 > 2) {}
                break;

            case 0x4000000:
                /* R_MIPS_26
                 * Handles 26-bit address relocation, used for jumps and jals
                 */
                unrelocatedAddress = ((*relocDataP & 0x3FFFFFF) << 2) | 0x80000000;
                relocOffset = unrelocatedAddress - (u32)vRamAddress;
                relocatedValue = (*relocDataP & 0xFC000000) | (((allocu32 + relocOffset) & 0xFFFFFFF) >> 2);
                relocatedAddress = ((relocatedValue & 0x3FFFFFF) << 2) | 0x80000000;
                *relocDataP = relocatedValue;
                break;
            case 0x5000000:
                /* R_MIPS_HI16
                 * Handles relocation for a lui instruciton, store the reference to
                 * the instruction, and will update it in the R_MIPS_LO16 section.
                 */
                luiRefs[(*relocDataP >> 0x10) & 0x1F] = relocDataP;
                luiVals[(*relocDataP >> 0x10) & 0x1F] = *relocDataP;
                break;
            case 0x6000000:
                /* R_MIPS_LO16
                 * Updates the LUI instruction to reflect the relocated address.
                 * The full address is calculated from the LUI and lo parts, and then updated.
                 * if the lo part is negative, add 1 to the lui.
                 */
                regValP = &luiVals[((*relocDataP >> 0x15) & 0x1F)];
                vaddr = (*regValP << 0x10) + (s16)*relocDataP;
                luiInstRef = luiRefs[((*relocDataP >> 0x15) & 0x1F)];
                if ((vaddr & 0xF000000) == 0) {
                    relocOffset = vaddr - (u32)vRamAddress;
                    vaddr = (s16)relocData;
                    isLoNeg = (((relocOffset + allocu32) & 0x8000) ? 1 : 0);
                    unrelocatedAddress = (*luiInstRef << 0x10) + vaddr;
                    *luiInstRef =
                        (*luiInstRef & 0xFFFF0000) | ((((relocOffset + allocu32) >> 0x10) & 0xFFFF) + isLoNeg);
                    relocatedValue = (*relocDataP & 0xFFFF0000) | ((relocOffset + allocu32) & 0xFFFF);

                    relocatedAddress = (*luiInstRef << 0x10) + (s16)relocatedValue;
                    *relocDataP = relocatedValue;
                }
                break;
        }
    }
}
// // #else
// // #pragma GLOBAL_ASM("asm/non_matchings/boot/loadfragment2/Load2_Relocate.s")
// #endif
extern s32 gOverlayLogSeverity;

#define PRINTF0(msg) 
#define PRINTF1(msg, arg1)
#define PRINTF2(msg, arg1, arg2)
#define PRINTF3(msg, arg1, arg2, arg3)
#define PRINTF4(msg, arg1, arg2, arg3, arg4)


// #ifdef NON_MATCHING
// Very minor stack stuff with a saved value

s32 Load2_LoadOverlay(u32 vRomStart, u32 vRomEnd, void* vRamStart, void* vRamEnd, void* allocatedVRamAddress) {
    // s32 pad;
    u32 end;
    u32 bssSize;
    u32 size;
    // u32 relocCnt;
    u32 ovlOffset;
    OverlayRelocationSection* ovl;

    size = vRomEnd - vRomStart;
    if (gOverlayLogSeverity >= 3) {
        PRINTF0("\nダイナミックリンクファンクションのロードを開始します\n");
    }
    if (gOverlayLogSeverity >= 3) {
        PRINTF2("TEXT,DATA,RODATA+relをＤＭＡ転送します(%08x-%08x)\n", (u32)allocatedVRamAddress,
                     (u32)allocatedVRamAddress + size);
    }

    size = vRomEnd - vRomStart;
    end = (u32)allocatedVRamAddress + size;
    DmaMgr_SendRequest0(allocatedVRamAddress, vRomStart, size);

    // ovlOffset = ((s32*)end)[-1];
    ovl = (OverlayRelocationSection*)((u32)end - ((s32*)end)[-1]);

    if (gOverlayLogSeverity >= 3) {
        PRINTF4("TEXT(%08x), DATA(%08x), RODATA(%08x), BSS(%08x)\n", ovl->textSize, ovl->dataSize, ovl->rodataSize,
                     ovl->bssSize);
    }
    if (gOverlayLogSeverity >= 3) {
        PRINTF0("リロケーションします\n");
    }

    Load2_Relocate(allocatedVRamAddress, ovl, vRamStart);

    bssSize = ovl->bssSize;
    if (ovl->bssSize != 0) {
        if (gOverlayLogSeverity >= 3) {
            PRINTF2("BSS領域をクリアします(%08x-%08x)\n", end, end + ovl->bssSize);
        }
        size = ovl->bssSize;
        bssSize = size;
        bzero(end, ovl->bssSize);
        // relocCnt = ovl->nRelocations;
    }
    if (0) {
        size = (u32)&ovl->relocations[ovl->nRelocations] - (u32)ovl;
        if (gOverlayLogSeverity >= 3) {
            PRINTF2("REL領域をクリアします(%08x-%08x)\n", ovl, (u32)ovl + size);
        }
        bzero(ovl, size);
    }
        size = (u32)vRamEnd - (u32)vRamStart;
    if (0) {}
    osWritebackDCache(allocatedVRamAddress, size);
    osInvalICache(allocatedVRamAddress, size);

    if (gOverlayLogSeverity >= 3) {
        PRINTF0("ダイナミックリンクファンクションのロードを終了します\n\n");
    }
    return size;
}
// s32 Load2_LoadOverlay(u32 vRomStart, u32 vRomEnd, void* vRamStart, void* vRamEnd, void* allocatedVRamAddr) {
//     int nbytes;
//     // s32 pad2;
//     s32 pad;
//     u32* end;
//     int size;
//     OverlayRelocationSection* overlayInfo;

//     size = vRomEnd - vRomStart;

//     if (1) {}

//     DmaMgr_SendRequest0(allocatedVRamAddr, vRomStart, size);

//     end = ((u32)allocatedVRamAddr + size);

    
//     overlayInfo = (u32)end - end[-1];

//         if (size && size) {}
//         // if (0) { !overlayInfo->bssSize && !overlayInfo->bssSize;}
//     if (1) {}

//     Load2_Relocate(allocatedVRamAddr, overlayInfo, vRamStart);

//     if (overlayInfo->bssSize != 0) {
//         bzero(end, overlayInfo->bssSize);
//     }

//     size = (u32)vRamEnd - (u32)vRamStart;

//     osWritebackDCache((void*)allocatedVRamAddr, size);
//     osInvalICache((void*)allocatedVRamAddr, size);
//     return size;
// }
// #else
// #pragma GLOBAL_ASM("asm/non_matchings/boot/loadfragment2/Load2_LoadOverlay.s")
// #endif

void* Overlay_AllocateAndLoad(u32 vRomStart, u32 vRomEnd, u32 vRamStart, u32 vRamEnd) {
    void* allocatedVRamAddr;
    size_t size;

    size = vRamEnd - vRamStart;
    allocatedVRamAddr = SystemArena_MallocR(size);
    if (allocatedVRamAddr != NULL) {
        Load2_LoadOverlay(vRomStart, vRomEnd, vRamStart, vRamEnd, (u32)allocatedVRamAddr);
    }

    return allocatedVRamAddr;
}
