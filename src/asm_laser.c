#include "common.h"

int saveRegsDrawLasers[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#ifdef NON_MATCHING
void DrawLasers() {}
#else
INCLUDE_ASM("nonmatchings/asm_laser", DrawLasers);
#endif
