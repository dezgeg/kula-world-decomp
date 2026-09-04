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
extern int D_00099F68[];
int *D_00099F60[] = { D_00099F68, D_00099F68 };
int D_00099F68[] = { 0x00000666 };
int D_00099F6C[] = { 0 };
int saveRegsRenderLevelGeometry[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

INCLUDE_ASM("asm/nonmatchings/asm_render", RenderBonusBackgroundPart2);

INCLUDE_ASM("asm/nonmatchings/asm_render", RenderDispList);

INCLUDE_ASM("asm/nonmatchings/asm_render", RenderBonusBackgroundPart1);

INCLUDE_ASM("asm/nonmatchings/asm_render", RenderNonSpecialBackground);

INCLUDE_ASM("asm/nonmatchings/asm_render", TweakSkyboxColors);

INCLUDE_ASM("asm/nonmatchings/asm_render", RenderStarfield);

INCLUDE_ASM("asm/nonmatchings/asm_render", RenderLevelGeometryQuads);
