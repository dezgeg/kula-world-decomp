#include "common.h"

extern uint Rand(int param_1);

TgiFile* tgi;

INCLUDE_ASM("asm/nonmatchings/level_init", ProcessLevelData);

INCLUDE_ASM("asm/nonmatchings/level_init", ProcessCubesIntoFaces);

INCLUDE_ASM("asm/nonmatchings/level_init", CoordHash);


int GetRandomTextureRotation(void) {
    int ret;

    switch (tgi->randomTileRotation) {
        case 0:
            return 0;
        case 1:
            ret = Rand(2);
            if (ret == 1) {
                ret = 2;
            }
            return ret;
        default:
            return Rand(4);
    }
}

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
