#include "common.h"

int saveRegsSubdivideLevelGeometry[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

INCLUDE_ASM("asm/nonmatchings/asm_subdivide", SubdivideLevelGeometryPolys);

INCLUDE_ASM("asm/nonmatchings/asm_subdivide", DoSubdivide);

INCLUDE_ASM("asm/nonmatchings/asm_subdivide", FUN_00051964);
