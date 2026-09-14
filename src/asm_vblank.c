#include "common.h"

int gotVsync;

#ifdef NON_MATCHING
void WaitForVBlank() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_vblank", WaitForVBlank);
#endif
