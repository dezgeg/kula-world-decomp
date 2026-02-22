#include "common.h"

int ballTextureIndex;
int curController;
short numCopycatMoves;
short unusedNumCopycatRounds;

int getBlockX;
int getBlockY;
int getBlockZ;
int getBlockResult;

short isPausedOrWaitingForRestart;

int DAT_000a43c4 = 0;

extern short* levelData;
extern short copycatMoves[1024];
extern short copycatNewOrCopyMoves;
extern Player thePlayer;
extern void* entityData;

void HandlePauseModeRotationEffect(Player* player);
int FUN_00033720(SVECTOR* vec, int itemdataOff, int param_3);

extern int inGetReadyScreen;
extern int isPaused;
extern int levelEndReason;

static SVECTOR SVECTOR_000a45d8;
short pauseForStartPress;

extern void ProcessEnemies(void);
extern void CalcPlayerMatrixesAndDrawPlayer(Player* player);
extern void CreateAllItemDispLists(void);

INCLUDE_ASM("asm/nonmatchings/level_update", ScanLevelDataForMovingBlocks2);

INCLUDE_ASM("asm/nonmatchings/level_update", MoveMovingPlatforms);

INCLUDE_ASM("asm/nonmatchings/level_update", FUN_00033720);

int FUN_0003382c(Player* player) {
    int blockIndex;
    int scaledIndex;

    blockIndex = player->surroundingBlocks[1][1][1];
    scaledIndex = blockIndex - 5;
    DAT_000a43c4 = scaledIndex * 128;
    if (DAT_000a43c4 < 0 || *(short*)(scaledIndex * 256 + (int)entityData) != 5) {
        blockIndex = player->surroundingBlocks[2][1][1];
        scaledIndex = blockIndex - 5;
        DAT_000a43c4 = scaledIndex * 128;
        if (DAT_000a43c4 < 0 || *(short*)(scaledIndex * 256 + (int)entityData) != 5) {
            return 0;
        }
    }
    return FUN_00033720(&player->finePos, DAT_000a43c4, 100);
}

INCLUDE_ASM("asm/nonmatchings/level_update", HandleMovingPlatforms);

INCLUDE_ASM("asm/nonmatchings/level_update", GetMovingPlatformAt);

INCLUDE_ASM("asm/nonmatchings/level_update", FUN_00033eb0);

INCLUDE_ASM("asm/nonmatchings/level_update", JumpingOnMovingPlatform);

INCLUDE_ASM("asm/nonmatchings/level_update", FUN_0003418c);

int FUN_000344b0(int a0, int a1) {
    if ((a0 == 1 || a0 == 4) && (a1 == 1 || a1 == 4)) return 1;
    if ((a0 == 2 || a0 == 3) && (a1 == 2 || a1 == 3)) return 1;
    if ((a0 == 0 || a0 == 5) && (a1 == 0 || a1 == 5)) return 1;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/level_update", FUN_00034518);

INCLUDE_ASM("asm/nonmatchings/level_update", CalcLevelBounds);

INCLUDE_ASM("asm/nonmatchings/level_update", ProcessCameraAndMovement);

INCLUDE_ASM("asm/nonmatchings/level_update", HandleDebugCamera);

INCLUDE_ASM("asm/nonmatchings/level_update", HandlePauseModeRotationEffect);

INCLUDE_ASM("asm/nonmatchings/level_update", HandlePlayerMovementStuff);

void SetInvulnerable(void) {
    thePlayer.invulnerabilityTimer = 3000;
}

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

void ProcessEnemiesRenderItemsAndCheckFellOff(void) {
    ProcessEnemies();
    if (!isPaused && levelEndReason == 0 && !inGetReadyScreen) {
        CalcPlayerMatrixesAndDrawPlayer(&thePlayer);
    }
    CreateAllItemDispLists();

    SVECTOR_000a45d8.vx = (thePlayer.finePos.vx + (thePlayer.gravityDir.vx << 8) + 0x100) >> 9;
    SVECTOR_000a45d8.vy = (thePlayer.finePos.vy + (thePlayer.gravityDir.vy << 8) + 0x100) >> 9;
    SVECTOR_000a45d8.vz = (thePlayer.finePos.vz + (thePlayer.gravityDir.vz << 8) + 0x100) >> 9;

    if (SVECTOR_000a45d8.vx < -1 || SVECTOR_000a45d8.vx > 35 || SVECTOR_000a45d8.vy < -1 || SVECTOR_000a45d8.vy > 35 || SVECTOR_000a45d8.vz < -1 || SVECTOR_000a45d8.vz > 35) {
        levelEndReason = -3;
    }

    if (pauseForStartPress == 1 && levelEndReason == 0) {
        pauseForStartPress = 0;
        isPaused = 1;
    }
}

void SetPausedOrWaitingForRestart(void) {
    isPausedOrWaitingForRestart = 1;
    HandlePauseModeRotationEffect(&thePlayer);
}

INCLUDE_ASM("asm/nonmatchings/level_update", RenderItems_);

INCLUDE_ASM("asm/nonmatchings/level_update", HandlePlayerButtons);

INCLUDE_ASM("asm/nonmatchings/level_update", CalcWhatPlayerIsStandingOn);

#define CUBE_TYPE_AT(x, y, z) levelData[(x) * 1156 + (y) * 34 + (z)]

int GetBlockAt(SVECTOR* coord) {
    getBlockX = (coord->vx + 0x100) >> 9;
    getBlockY = (coord->vy + 0x100) >> 9;
    getBlockZ = (coord->vz + 0x100) >> 9;

    if (getBlockX < 1 || getBlockY < 1 || getBlockZ < 1 || getBlockX > 32 || getBlockY > 32 || getBlockZ > 32) {
        return -1;
    } else {
        getBlockResult = CUBE_TYPE_AT(getBlockX, getBlockY, getBlockZ);
        return getBlockResult;
    }
}

INCLUDE_ASM("asm/nonmatchings/level_update", UpdateSubpixelPositions);

INCLUDE_ASM("asm/nonmatchings/level_update", MovePlayerForward);

INCLUDE_ASM("asm/nonmatchings/level_update", MovePlayerDownwards);

int GetRotationIndexFromVector(SVECTOR v) {
    if (v.vx == 1) return 1;
    if (v.vx == -1) return 4;
    if (v.vy == 1) return 2;
    if (v.vy == -1) return 3;
    if (v.vz == 1) return 5;
    if (v.vz == -1) return 0;
    return -1;
}

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

int IsPlayerInAir(Player* player) {
    if (player->howMoving198 == FALLING ||
        ((uint)player->howMoving198 < ROLLING && player->jumpingOrViewportRotationTimer > 1)) {
        return 1;
    }
    return 0;
}

int IsFallingOrJumping(Player* player) {
    if (player->howMoving198 == FALLING || (uint)player->howMoving198 < ROLLING) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/level_update", Unused_FUN_0003bdec);
