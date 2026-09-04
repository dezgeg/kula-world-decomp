#include "common.h"

int saveRegsAnimateTextures[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#ifdef NON_MATCHING
void AnimateTextures() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_texture_anim", AnimateTextures);
#endif
