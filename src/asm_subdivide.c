#include "common.h"

int saveRegsSubdivideLevelGeometry[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#ifdef NON_MATCHING
void SubdivideLevelGeometryPolys() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_subdivide", SubdivideLevelGeometryPolys);
#endif

#ifdef NON_MATCHING
void DoSubdivide() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_subdivide", DoSubdivide);
#endif

#ifdef NON_MATCHING
void FUN_00051964() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_subdivide", FUN_00051964);
#endif
