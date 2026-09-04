#include "common.h"

#ifdef NON_MATCHING
void LoadScaledGteMatrix() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_gte", LoadScaledGteMatrix);
#endif

#ifdef NON_MATCHING
void GteTransformAndClipQuad() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_gte", GteTransformAndClipQuad);
#endif
