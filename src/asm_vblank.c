#include "common.h"

#ifdef NON_MATCHING
void WaitForVBlank() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_vblank", WaitForVBlank);
#endif
