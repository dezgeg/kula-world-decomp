#include "common.h"

int saveRegsDrawLasers[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#ifdef NON_MATCHING
void DrawLasers() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_laser", DrawLasers);
#endif
