#include "common.h"

int saveRegsUnkAsmFun[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int saveRegsRenderDispList[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
char S_MAX_SORT_OBJ_too_small[] = { "MAX_SORT_OBJ too small!\0\0\0\0" };
char S_ot_obj_overflow_MRLEN_OBJ_too_small[] = { "ot_obj overflow! MRLEN_OBJ too small!\0\0\0\0\0\0" };
int saveRegsRenderBonusBackground[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int D_00099EF8[] = { 0 };
int D_00099EFC[] = { 0 };
int saveRegsRenderNonSpecialBackground[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int D_00099F2C[] = { 0 };
int D_00099F30[] = { 0 };
int saveRegsRenderStarfield[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
extern int starfieldDummy[];
int *starfieldOtags[] = { starfieldDummy, starfieldDummy };
int starfieldDummy[] = { 0x00000666 };
int numRenderedStars[] = { 0 };
int saveRegsRenderLevelGeometry[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#ifdef NON_MATCHING
void RenderBonusBackgroundPart2() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_render", RenderBonusBackgroundPart2);
#endif

#ifdef NON_MATCHING
void RenderDispList() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_render", RenderDispList);
#endif

#ifdef NON_MATCHING
void RenderBonusBackgroundPart1() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_render", RenderBonusBackgroundPart1);
#endif

#ifdef NON_MATCHING
void RenderNonSpecialBackground() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_render", RenderNonSpecialBackground);
#endif

#ifdef NON_MATCHING
void TweakSkyboxColors() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_render", TweakSkyboxColors);
#endif

#ifdef NON_MATCHING
void RenderStarfield() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_render", RenderStarfield);
#endif

#ifdef NON_MATCHING
void RenderLevelGeometryQuads() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_render", RenderLevelGeometryQuads);
#endif
