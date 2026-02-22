#include "common.h"

int ballTextureIndex;
int curController;
short numCopycatMoves;
short unusedNumCopycatRounds;

int getBlockX;
int getBlockY;
int getBlockZ;
int getBlockResult;

extern short* levelData;
extern short copycatMoves[1024];
extern short copycatNewOrCopyMoves;

INCLUDE_ASM("asm/nonmatchings/level_update", ScanLevelDataForMovingBlocks2);

INCLUDE_ASM("asm/nonmatchings/level_update", MoveMovingPlatforms);

INCLUDE_ASM("asm/nonmatchings/level_update", FUN_00033720);

INCLUDE_ASM("asm/nonmatchings/level_update", FUN_0003382c);

INCLUDE_ASM("asm/nonmatchings/level_update", HandleMovingPlatforms);

INCLUDE_ASM("asm/nonmatchings/level_update", GetMovingPlatformAt);

INCLUDE_ASM("asm/nonmatchings/level_update", FUN_00033eb0);

INCLUDE_ASM("asm/nonmatchings/level_update", JumpingOnMovingPlatform);

INCLUDE_ASM("asm/nonmatchings/level_update", FUN_0003418c);

INCLUDE_ASM("asm/nonmatchings/level_update", FUN_000344b0);

INCLUDE_ASM("asm/nonmatchings/level_update", FUN_00034518);

INCLUDE_ASM("asm/nonmatchings/level_update", CalcLevelBounds);

INCLUDE_ASM("asm/nonmatchings/level_update", ProcessCameraAndMovement);

INCLUDE_ASM("asm/nonmatchings/level_update", HandleDebugCamera);

INCLUDE_ASM("asm/nonmatchings/level_update", HandlePauseModeRotationEffect);

INCLUDE_ASM("asm/nonmatchings/level_update", HandlePlayerMovementStuff);

INCLUDE_ASM("asm/nonmatchings/level_update", SetInvulnerable);

INCLUDE_ASM("asm/nonmatchings/level_update", CreateItemsFromLevelData);

INCLUDE_ASM("asm/nonmatchings/level_update", InitLasers);

void ResetCopycatMode(int param_1) {
    int i;

    for (i = 0; i < 1024; i++) {
        copycatMoves[i] = -1;
    }

    numCopycatMoves = 1;
    curController = param_1;
    ballTextureIndex = param_1;
    unusedNumCopycatRounds = 0;
    copycatNewOrCopyMoves = 1;
}

INCLUDE_ASM("asm/nonmatchings/level_update", LevelInit);

INCLUDE_ASM("asm/nonmatchings/level_update", ProcessPlayer);

INCLUDE_ASM("asm/nonmatchings/level_update", ProcessEnemiesRenderItemsAndCheckFellOff);

INCLUDE_ASM("asm/nonmatchings/level_update", SetPausedOrWaitingForRestart);

INCLUDE_ASM("asm/nonmatchings/level_update", RenderItems_);

INCLUDE_ASM("asm/nonmatchings/level_update", HandlePlayerButtons);

INCLUDE_ASM("asm/nonmatchings/level_update", CalcWhatPlayerIsStandingOn);

#define CUBE_TYPE_AT(x, y, z) levelData[(x) * 1156 + (y) * 34 + (z)]

int GetBlockAt(SVECTOR *coord) {
    int x, y, z;
    int res;
    
    x = (coord->vx + 0x100) >> 9;
    y = (coord->vy + 0x100) >> 9;
    z = (coord->vz + 0x100) >> 9;
    
    getBlockX = x;
    getBlockY = y;
    getBlockZ = z;
    
    if (x < 1 || y < 1 || z < 1 || x > 32 || y > 32 || z > 32) {
        return -1;
    } else {
        res = CUBE_TYPE_AT(x, y, z);
        getBlockResult = res;
        return res;
    }
}

INCLUDE_ASM("asm/nonmatchings/level_update", UpdateSubpixelPositions);

INCLUDE_ASM("asm/nonmatchings/level_update", MovePlayerForward);

INCLUDE_ASM("asm/nonmatchings/level_update", MovePlayerDownwards);

INCLUDE_ASM("asm/nonmatchings/level_update", GetRotationIndexFromVector);

INCLUDE_ASM("asm/nonmatchings/level_update", GetVectorBasedOnTwoDirs);

INCLUDE_ASM("asm/nonmatchings/level_update", SetPlayerRotation);

INCLUDE_ASM("asm/nonmatchings/level_update", SetPlayerMatrix6);

INCLUDE_ASM("asm/nonmatchings/level_update", HandleItemTouching);

INCLUDE_ASM("asm/nonmatchings/level_update", CreateAllItemDispLists);

INCLUDE_ASM("asm/nonmatchings/level_update", SetEntityRotation);

INCLUDE_ASM("asm/nonmatchings/level_update", MatrixFromDirectionIndex);

INCLUDE_ASM("asm/nonmatchings/level_update", CheckForButtonEntity);

INCLUDE_ASM("asm/nonmatchings/level_update", HandleTransporter);

INCLUDE_ASM("asm/nonmatchings/level_update", HandleSpecialCubeTypes);

INCLUDE_ASM("asm/nonmatchings/level_update", SubtractLevelTimer);

INCLUDE_ASM("asm/nonmatchings/level_update", IsPlayerInAir);

INCLUDE_ASM("asm/nonmatchings/level_update", IsFallingOrJumping);

INCLUDE_ASM("asm/nonmatchings/level_update", Unused_FUN_0003bdec);
