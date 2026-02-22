#include "common.h"

INCLUDE_ASM("asm/nonmatchings/level_init", ProcessLevelData);

INCLUDE_ASM("asm/nonmatchings/level_init", ProcessCubesIntoFaces);

INCLUDE_ASM("asm/nonmatchings/level_init", CoordHash);

INCLUDE_ASM("asm/nonmatchings/level_init", GetRandomTextureRotation);

INCLUDE_ASM("asm/nonmatchings/level_init", ParseKeysAndSpecialLevelFromItemData);

INCLUDE_ASM("asm/nonmatchings/level_init", ScanLevelDataForMovingBlocks1);

INCLUDE_ASM("asm/nonmatchings/level_init", InitLasers2);

INCLUDE_ASM("asm/nonmatchings/level_init", ScanLevelDataForCrumblingBlocks);

INCLUDE_ASM("asm/nonmatchings/level_init", ScanLevelDataForFlashingBlocks);

INCLUDE_ASM("asm/nonmatchings/level_init", SetFaceData);

INCLUDE_ASM("asm/nonmatchings/level_init", CopyQuadData);

INCLUDE_ASM("asm/nonmatchings/level_init", InitAnimatedTextureChain);

INCLUDE_ASM("asm/nonmatchings/level_init", AddQuadToAnimatedTextureChain);

INCLUDE_ASM("asm/nonmatchings/level_init", FUN_000298e0);

INCLUDE_ASM("asm/nonmatchings/level_init", InitShadowSprites);

INCLUDE_ASM("asm/nonmatchings/level_init", InitPlayerSpecularSprite);

INCLUDE_ASM("asm/nonmatchings/level_init", ScanLevelDataForBlinkingEntities);
