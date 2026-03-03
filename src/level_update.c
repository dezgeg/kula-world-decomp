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
extern int debugDisableTimer;
extern int drawTimerPausedWidget;
extern int levelTimeLeft;
int gameMode;
int D_000A4430;

static SVECTOR SVECTOR_000a45d8;
static SVECTOR SVECTOR_000a4638;
static SVECTOR SVECTOR_000a4640;
static SVECTOR SVECTOR_000a4738;
static SVECTOR SVECTOR_000a4740;

typedef struct EntityPos {
    MATRIX matrix;
    short posX;
    short posY;
    short posZ;
    short pad;
    byte reserved[8];
} EntityPos;

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

void FUN_00034518(VECTOR *v1, VECTOR *v2, VECTOR *v3, VECTOR *v4, VECTOR *v5, short timer, short param_7) {
    int t = timer;
    int shift = param_7;
    int inv_t = (1 << (shift * 2 / 3)) - t;
    
    int inv_t2 = inv_t * inv_t;
    int inv_t3 = inv_t2 * inv_t;
    int t2 = t * t;
    int t3 = t2 * t;
    
    int inv_t_3 = inv_t * 3;
    int t_3 = t * 3;
    
    v5->vx = (inv_t3 >> shift) * v1->vx;
    v5->vx += ((t * inv_t_3 * inv_t) >> shift) * v2->vx;
    v5->vx += ((t * t_3 * inv_t) >> shift) * v3->vx;
    v5->vx += (t3 >> shift) * v4->vx;
    v5->vx >>= shift;
    
    v5->vy = (inv_t3 >> shift) * v1->vy;
    v5->vy += ((t * inv_t_3 * inv_t) >> shift) * v2->vy;
    v5->vy += ((t * t_3 * inv_t) >> shift) * v3->vy;
    v5->vy += (t3 >> shift) * v4->vy;
    v5->vy >>= shift;
    
    v5->vz = (inv_t3 >> shift) * v1->vz;
    v5->vz += ((t * inv_t_3 * inv_t) >> shift) * v2->vz;
    v5->vz += ((t * t_3 * inv_t) >> shift) * v3->vz;
    v5->vz += (t3 >> shift) * v4->vz;
    v5->vz >>= shift;
    
    D_000A4430 = inv_t;
}


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

void GetVectorBasedOnTwoDirs(int dir1, int dir2, SVECTOR *res) {
    SVECTOR_000a4638.vz = 0;
    SVECTOR_000a4638.vy = 0;
    SVECTOR_000a4638.vx = 0;
    SVECTOR_000a4640.vz = 0;
    SVECTOR_000a4640.vy = 0;
    SVECTOR_000a4640.vx = 0;
    
    if (dir1 == 5) {
        SVECTOR_000a4638.vx = 1;
        SVECTOR_000a4640.vy = 1;
    }
    if (dir1 == 0) {
        SVECTOR_000a4638.vx = -1;
        SVECTOR_000a4640.vy = 1;
    }
    if (dir1 == 4) {
        SVECTOR_000a4638.vz = 1;
        SVECTOR_000a4640.vy = 1;
    }
    if (dir1 == 1) {
        SVECTOR_000a4638.vz = -1;
        SVECTOR_000a4640.vy = 1;
    }
    if (dir1 == 2) {
        SVECTOR_000a4638.vx = 1;
        SVECTOR_000a4640.vz = -1;
    }
    if (dir1 == 3) {
        SVECTOR_000a4638.vx = 1;
        SVECTOR_000a4640.vz = 1;
    }
    
    *res = SVECTOR_000a4640;
    if (dir2 == 2) {
        *res = SVECTOR_000a4638;
    }
    if (dir2 == 3) {
        res->vx = -SVECTOR_000a4640.vx;
        res->vy = -SVECTOR_000a4640.vy;
        res->vz = -SVECTOR_000a4640.vz;
    }
    if (dir2 == 4) {
        res->vx = -SVECTOR_000a4638.vx;
        res->vy = -SVECTOR_000a4638.vy;
        res->vz = -SVECTOR_000a4638.vz;
    }
}

INCLUDE_ASM("asm/nonmatchings/level_update", SetPlayerRotation);

INCLUDE_ASM("asm/nonmatchings/level_update", SetPlayerMatrix6);

INCLUDE_ASM("asm/nonmatchings/level_update", HandleItemTouching);

INCLUDE_ASM("asm/nonmatchings/level_update", CreateAllItemDispLists);

void SetEntityRotation(EntityPos *pos, int param_2, int param_3, int param_4) {
    pos->matrix.m[2][2] = 0;
    pos->matrix.m[2][1] = 0;
    pos->matrix.m[2][0] = 0;
    pos->matrix.m[1][2] = 0;
    pos->matrix.m[1][1] = 0;
    pos->matrix.m[1][0] = 0;
    pos->matrix.m[0][2] = 0;
    pos->matrix.m[0][1] = 0;
    pos->matrix.m[0][0] = 0;

    if (param_3 == 4) {
        pos->matrix.m[2][0] = 0x1000;
        pos->matrix.m[1][1] = 0x1000;
        pos->matrix.m[0][2] = -0x1000;
        pos->matrix.t[0] -= param_4;
    }
    if (param_3 == 1) {
        pos->matrix.m[2][0] = -0x1000;
        pos->matrix.m[1][1] = 0x1000;
        pos->matrix.m[0][2] = 0x1000;
        pos->matrix.t[0] += param_4;
    }
    if (param_3 == 3) {
        pos->matrix.m[0][0] = 0x1000;
        pos->matrix.m[2][1] = 0x1000;
        pos->matrix.m[1][2] = -0x1000;
        pos->matrix.t[1] -= param_4;
    }
    if (param_3 == 2) {
        pos->matrix.m[0][0] = 0x1000;
        pos->matrix.m[2][1] = -0x1000;
        pos->matrix.m[1][2] = 0x1000;
        pos->matrix.t[1] += param_4;
    }
    if (param_3 == 0) {
        pos->matrix.m[0][0] = -0x1000;
        pos->matrix.m[1][1] = 0x1000;
        pos->matrix.m[2][2] = -0x1000;
        pos->matrix.t[2] -= param_4;
    }
    if (param_3 == 5) {
        pos->matrix.m[0][0] = 0x1000;
        pos->matrix.m[1][1] = 0x1000;
        pos->matrix.m[2][2] = 0x1000;
        pos->matrix.t[2] += param_4;
    }

    if (param_2 == 2) {
        SVECTOR_000a4738.vx = pos->matrix.m[0][0];
        SVECTOR_000a4738.vy = pos->matrix.m[1][0];
        SVECTOR_000a4738.vz = pos->matrix.m[2][0];

        pos->matrix.m[0][0] = -pos->matrix.m[0][1];
        pos->matrix.m[1][0] = -pos->matrix.m[1][1];
        pos->matrix.m[2][0] = -pos->matrix.m[2][1];

        pos->matrix.m[0][1] = SVECTOR_000a4738.vx;
        pos->matrix.m[1][1] = SVECTOR_000a4738.vy;
        pos->matrix.m[2][1] = SVECTOR_000a4738.vz;
    }
    if (param_2 == 3) {
        pos->matrix.m[0][0] = -pos->matrix.m[0][0];
        pos->matrix.m[1][0] = -pos->matrix.m[1][0];
        pos->matrix.m[2][0] = -pos->matrix.m[2][0];
        pos->matrix.m[0][1] = -pos->matrix.m[0][1];
        pos->matrix.m[1][1] = -pos->matrix.m[1][1];
        pos->matrix.m[2][1] = -pos->matrix.m[2][1];
    }
    if (param_2 == 4) {
        SVECTOR_000a4738.vx = pos->matrix.m[0][0];
        SVECTOR_000a4738.vy = pos->matrix.m[1][0];
        SVECTOR_000a4738.vz = pos->matrix.m[2][0];

        pos->matrix.m[0][0] = pos->matrix.m[0][1];
        pos->matrix.m[1][0] = pos->matrix.m[1][1];
        pos->matrix.m[2][0] = pos->matrix.m[2][1];

        pos->matrix.m[0][1] = -SVECTOR_000a4738.vx;
        pos->matrix.m[1][1] = -SVECTOR_000a4738.vy;
        pos->matrix.m[2][1] = -SVECTOR_000a4738.vz;
    }
}

void MatrixFromDirectionIndex(MATRIX *m, int param_2, int param_3, short delta, SVECTOR *param_5) {
    m->m[0][2] = 0;
    m->m[0][1] = 0;
    m->m[0][0] = 0;
    m->m[1][2] = 0;
    m->m[1][1] = 0;
    m->m[1][0] = 0;
    m->m[2][2] = 0;
    m->m[2][1] = 0;
    m->m[2][0] = 0;

    if (param_3 == 4) {
        param_5->vx = param_5->vx - delta;
        m->m[2][0] = 0x1000;
        m->m[1][1] = 0x1000;
        m->m[0][2] = -0x1000;
    }
    if (param_3 == 1) {
        param_5->vx = param_5->vx + delta;
        m->m[2][0] = -0x1000;
        m->m[1][1] = 0x1000;
        m->m[0][2] = 0x1000;
    }
    if (param_3 == 3) {
        param_5->vy = param_5->vy - delta;
        m->m[0][0] = 0x1000;
        m->m[2][1] = 0x1000;
        m->m[1][2] = -0x1000;
    }
    if (param_3 == 2) {
        param_5->vy = param_5->vy + delta;
        m->m[0][0] = 0x1000;
        m->m[2][1] = -0x1000;
        m->m[1][2] = 0x1000;
    }
    if (param_3 == 0) {
        param_5->vz = param_5->vz - delta;
        m->m[0][0] = -0x1000;
        m->m[1][1] = 0x1000;
        m->m[2][2] = -0x1000;
    }
    if (param_3 == 5) {
        param_5->vz = param_5->vz + delta;
        m->m[0][0] = 0x1000;
        m->m[1][1] = 0x1000;
        m->m[2][2] = 0x1000;
    }

    if (param_2 == 2) {
        SVECTOR_000a4740.vx = m->m[0][0];
        SVECTOR_000a4740.vy = m->m[1][0];
        SVECTOR_000a4740.vz = m->m[2][0];

        m->m[0][0] = -m->m[0][1];
        m->m[1][0] = -m->m[1][1];
        m->m[2][0] = -m->m[2][1];

        m->m[0][1] = SVECTOR_000a4740.vx;
        m->m[1][1] = SVECTOR_000a4740.vy;
        m->m[2][1] = SVECTOR_000a4740.vz;
    }
    if (param_2 == 3) {
        m->m[0][0] = -m->m[0][0];
        m->m[1][0] = -m->m[1][0];
        m->m[2][0] = -m->m[2][0];
        m->m[0][1] = -m->m[0][1];
        m->m[1][1] = -m->m[1][1];
        m->m[2][1] = -m->m[2][1];
    }
    if (param_2 == 4) {
        SVECTOR_000a4740.vx = m->m[0][0];
        SVECTOR_000a4740.vy = m->m[1][0];
        SVECTOR_000a4740.vz = m->m[2][0];

        m->m[0][0] = m->m[0][1];
        m->m[1][0] = m->m[1][1];
        m->m[2][0] = m->m[2][1];

        m->m[0][1] = -SVECTOR_000a4740.vx;
        m->m[1][1] = -SVECTOR_000a4740.vy;
        m->m[2][1] = -SVECTOR_000a4740.vz;
    }
}

INCLUDE_ASM("asm/nonmatchings/level_update", CheckForButtonEntity);

INCLUDE_ASM("asm/nonmatchings/level_update", HandleTransporter);

INCLUDE_ASM("asm/nonmatchings/level_update", HandleSpecialCubeTypes);

void SubtractLevelTimer(int param_1) {
    if (thePlayer.faceTypePlayerStandingOn != 8 && debugDisableTimer == 0 && gameMode != 1) {
        levelTimeLeft -= param_1;
        if (levelTimeLeft < 1) {
            levelEndReason = -2;
        }
    }

    if (thePlayer.faceTypePlayerStandingOn == 8 || debugDisableTimer == 1) {
        drawTimerPausedWidget = 1;
    } else {
        drawTimerPausedWidget = 0;
    }
}

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
