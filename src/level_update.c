#include "common.h"

#define CUBE_TYPE_AT(x, y, z) levelData[(x) * 1156 + (y) * 34 + (z)]

int ballTextureIndex;
int curController;
short numCopycatMoves;
short unusedNumCopycatRounds;

int getBlockX;
int getBlockY;
int getBlockZ;
int getBlockResult;

short isPausedOrWaitingForRestart;

int DAT_000a43c4;

extern short numEntities;
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
int D_000A43E8;
int D_000A43DC;
static SVECTOR SVECTOR_000a43ec;
static SVECTOR SVECTOR_000a43e0;
static SVECTOR initPlayerFacingVec;
static SVECTOR initPlayerGravityVec;
static SVECTOR initPlayerRightVec;

extern int cameraIndex;
extern MATRIX perspMatrixes[];
extern int specialLevelType;

static SVECTOR SVECTOR_000a449c;
static VECTOR VECTOR_000a44a8;
int maxDistSquared;
int xMinPlusMax;
int yMinPlusMax;
int zMinPlusMax;
int zoomInAndOutPhase;

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

int GetMovingPlatformAt(Player* player, SVECTOR* param_2) {
    if (param_2->vx == -1) {
        SVECTOR_000a43e0.vx = player->finePos.vx - player->gravityDir.vx * 512;
        SVECTOR_000a43e0.vy = player->finePos.vy - player->gravityDir.vy * 512;
        SVECTOR_000a43e0.vz = player->finePos.vz - player->gravityDir.vz * 512;
    } else {
        SVECTOR_000a43e0 = *param_2;
    }

    D_000A43DC = (GetBlockAt(&SVECTOR_000a43e0) - 5) * 128;

    if (D_000A43DC >= 0) {
        if (*(short*)(entityData + D_000A43DC * 2) != 5) {
            goto ret_minus1;
        }

        if (FUN_000344b0(*(short*)(entityData + D_000A43DC * 2 + 4), GetRotationIndexFromVector(player->gravityDir)) != 0) {
            goto ret_minus1;
        }

        if (player->subpixelPositionOnCube.vy < 101) {
            if (FUN_00033720(&player->finePos, D_000A43DC, 0) != 0) {
                goto ret_D;
            }
            goto ret_minus1;
        }
    }

ret_minus1:
    return -1;
ret_D:
    return D_000A43DC;
}

int FUN_00033eb0(Player* player, SVECTOR* param_2) {
    int type;
    int index;
    int rotation;

    if (param_2->vx == -1) {
        SVECTOR_000a43ec = player->finePos;
    } else {
        SVECTOR_000a43ec = *param_2;
    }

    index = GetBlockAt(&SVECTOR_000a43ec) - 5;
    D_000A43E8 = index * 128;

    if (D_000A43E8 < 0 || *(short*)((index * 256) + (int)entityData) != 5) {
        SVECTOR_000a43ec.vx -= player->gravityDir.vx * 512;
        SVECTOR_000a43ec.vy -= player->gravityDir.vy * 512;
        SVECTOR_000a43ec.vz -= player->gravityDir.vz * 512;

        index = GetBlockAt(&SVECTOR_000a43ec) - 5;
        D_000A43E8 = index * 128;

        if (D_000A43E8 < 0) {
            return -1;
        }
        if (*(short*)((index * 256) + (int)entityData) != 5) {
            return -1;
        }
    }

    if (FUN_000344b0((int)*(short*)(D_000A43E8 * 2 + (int)entityData + 4), GetRotationIndexFromVector(player->gravityDir)) == 0) {
        return -1;
    }
    if (FUN_00033720(&player->finePos, D_000A43E8, 100) != 0) {
        return D_000A43E8;
    }

    return -1;
}

INCLUDE_ASM("asm/nonmatchings/level_update", JumpingOnMovingPlatform);

INCLUDE_ASM("asm/nonmatchings/level_update", FUN_0003418c);

int FUN_000344b0(int a0, int a1) {
    if ((a0 == 1 || a0 == 4) && (a1 == 1 || a1 == 4)) return 1;
    if ((a0 == 2 || a0 == 3) && (a1 == 2 || a1 == 3)) return 1;
    if ((a0 == 0 || a0 == 5) && (a1 == 0 || a1 == 5)) return 1;
    return 0;
}

void FUN_00034518(VECTOR* v1, VECTOR* v2, VECTOR* v3, VECTOR* v4, VECTOR* v5, short t, short shift) {
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

void HandlePauseModeRotationEffect(Player* player) {
    player->playerHasControl = 0;

    SVECTOR_000a449c.vx = (SVECTOR_000a449c.vx - 10) % 4096;
    SVECTOR_000a449c.vy = (SVECTOR_000a449c.vy + 4) % 4096;
    SVECTOR_000a449c.vz = (SVECTOR_000a449c.vz + 13) % 4096;

    if (specialLevelType == 0) {
        zoomInAndOutPhase = (zoomInAndOutPhase + 15) % 4096;
    }
    if (specialLevelType == 1) {
        zoomInAndOutPhase = (zoomInAndOutPhase + 70) % 4096;
    }

    RotMatrix(&SVECTOR_000a449c, &perspMatrixes[cameraIndex]);

    VECTOR_000a44a8.vx = -xMinPlusMax;
    VECTOR_000a44a8.vy = -yMinPlusMax;
    VECTOR_000a44a8.vz = -zMinPlusMax;

    ApplyMatrixLV(&perspMatrixes[cameraIndex], &VECTOR_000a44a8, &VECTOR_000a44a8);

    perspMatrixes[cameraIndex].t[0] = VECTOR_000a44a8.vx;
    perspMatrixes[cameraIndex].t[1] = VECTOR_000a44a8.vy;
    perspMatrixes[cameraIndex].t[2] = VECTOR_000a44a8.vz + maxDistSquared + 400;

    if (specialLevelType == 0) {
        perspMatrixes[cameraIndex].t[2] = VECTOR_000a44a8.vz + maxDistSquared + 1400 + ((rsin(zoomInAndOutPhase) * 1000) / 4096);
    }
    if (specialLevelType == 1) {
        perspMatrixes[cameraIndex].t[2] = VECTOR_000a44a8.vz + maxDistSquared + 3400 + ((rsin(zoomInAndOutPhase) * 3000) / 4096);
    }
}

INCLUDE_ASM("asm/nonmatchings/level_update", HandlePlayerMovementStuff);

void SetInvulnerable(void) {
    thePlayer.invulnerabilityTimer = 3000;
}

INCLUDE_ASM("asm/nonmatchings/level_update", CreateItemsFromLevelData);

void InitLasers(void) {
    int i;
    int x;
    int y;
    int z;

    for (i = 0; i < numEntities * 128; i += 128) {
        if (((short*)entityData)[i + 0] != 8)
            continue;
        x = ((short*)entityData)[i + 4];
        y = ((short*)entityData)[i + 5];
        z = ((short*)entityData)[i + 6];
        switch (((short*)entityData)[i + 2]) {
            case 1:
                levelData[x * 1156 + y * 34 + z] = 0;
                for (x = ((short*)entityData)[i + 4] + 1; x < ((short*)entityData)[i + 7]; x++) {
                    levelData[x * 1156 + y * 34 + z] = -2;
                }
                levelData[x * 1156 + y * 34 + z] = 0;
                break;
            case 2:
                levelData[x * 1156 + y * 34 + z] = 0;
                for (y = ((short*)entityData)[i + 5] + 1; y < ((short*)entityData)[i + 8]; y++) {
                    levelData[x * 1156 + y * 34 + z] = -2;
                }
                levelData[x * 1156 + y * 34 + z] = 0;
                break;
            case 5:
                levelData[x * 1156 + y * 34 + z] = 0;
                for (z = ((short*)entityData)[i + 6] + 1; z < ((short*)entityData)[i + 9]; z++) {
                    levelData[x * 1156 + y * 34 + z] = -2;
                }
                levelData[x * 1156 + y * 34 + z] = 0;
                break;
        }
    }
}

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

static short playerFinePosMod512[4];

void UpdateSubpixelPositions(Player* player) {
    playerFinePosMod512[0] = (player->finePos.vx + 0x100) & 0x1FF;
    playerFinePosMod512[1] = (player->finePos.vy + 0x100) & 0x1FF;
    playerFinePosMod512[2] = (player->finePos.vz + 0x100) & 0x1FF;

    player->svec_154 = player->subpixelPositionOnCube;

    if (player->facingDir.vx == 1) {
        player->subpixelPositionOnCube.vz = playerFinePosMod512[0];
    }
    if (player->facingDir.vx == -1) {
        player->subpixelPositionOnCube.vz = 0x200 - playerFinePosMod512[0];
    }

    if (player->facingDir.vy == 1) {
        player->subpixelPositionOnCube.vz = playerFinePosMod512[1];
    }
    if (player->facingDir.vy == -1) {
        player->subpixelPositionOnCube.vz = 0x200 - playerFinePosMod512[1];
    }

    if (player->facingDir.vz == 1) {
        player->subpixelPositionOnCube.vz = playerFinePosMod512[2];
    }
    if (player->facingDir.vz == -1) {
        player->subpixelPositionOnCube.vz = 0x200 - playerFinePosMod512[2];
    }

    if (player->gravityDir.vx == 1) {
        player->subpixelPositionOnCube.vy = playerFinePosMod512[0];
    }
    if (player->gravityDir.vx == -1) {
        player->subpixelPositionOnCube.vy = 0x200 - playerFinePosMod512[0];
    }

    if (player->gravityDir.vy == 1) {
        player->subpixelPositionOnCube.vy = playerFinePosMod512[1];
    }
    if (player->gravityDir.vy == -1) {
        player->subpixelPositionOnCube.vy = 0x200 - playerFinePosMod512[1];
    }

    if (player->gravityDir.vz == 1) {
        player->subpixelPositionOnCube.vy = playerFinePosMod512[2];
    }
    if (player->gravityDir.vz == -1) {
        player->subpixelPositionOnCube.vy = 0x200 - playerFinePosMod512[2];
    }

    if (player->rightVec.vx == 1) {
        player->subpixelPositionOnCube.vx = playerFinePosMod512[0];
    }
    if (player->rightVec.vx == -1) {
        player->subpixelPositionOnCube.vx = 0x200 - playerFinePosMod512[0];
    }

    if (player->rightVec.vy == 1) {
        player->subpixelPositionOnCube.vx = playerFinePosMod512[1];
    }
    if (player->rightVec.vy == -1) {
        player->subpixelPositionOnCube.vx = 0x200 - playerFinePosMod512[1];
    }

    if (player->rightVec.vz == 1) {
        player->subpixelPositionOnCube.vx = playerFinePosMod512[2];
    }
    if (player->rightVec.vz == -1) {
        player->subpixelPositionOnCube.vx = 0x200 - playerFinePosMod512[2];
    }

    player->svec_144.vx = player->subpixelPositionOnCube.vx - player->svec_154.vx;
    player->svec_144.vy = player->subpixelPositionOnCube.vy - player->svec_154.vy;
    player->svec_144.vz = player->subpixelPositionOnCube.vz - player->svec_154.vz;
}


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

void GetVectorBasedOnTwoDirs(int dir1, int dir2, SVECTOR* res) {
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

void SetPlayerRotation(int cubeSide, int rotation, Player* player) {
    initPlayerRightVec.vz = 0;
    initPlayerRightVec.vy = 0;
    initPlayerRightVec.vx = 0;
    initPlayerFacingVec.vz = 0;
    initPlayerFacingVec.vy = 0;
    initPlayerFacingVec.vx = 0;
    initPlayerGravityVec.vz = 0;
    initPlayerGravityVec.vy = 0;
    initPlayerGravityVec.vx = 0;

    if (cubeSide == 5) {
        initPlayerRightVec.vx = 1;
        initPlayerFacingVec.vy = 1;
        initPlayerGravityVec.vz = 1;
    }
    if (cubeSide == 0) {
        initPlayerRightVec.vx = -1;
        initPlayerFacingVec.vy = 1;
        initPlayerGravityVec.vz = -1;
    }
    if (cubeSide == 4) {
        initPlayerRightVec.vz = 1;
        initPlayerFacingVec.vy = 1;
        initPlayerGravityVec.vx = -1;
    }
    if (cubeSide == 1) {
        initPlayerRightVec.vz = -1;
        initPlayerFacingVec.vy = 1;
        initPlayerGravityVec.vx = 1;
    }
    if (cubeSide == 2) {
        initPlayerRightVec.vx = 1;
        initPlayerFacingVec.vz = -1;
        initPlayerGravityVec.vy = 1;
    }
    if (cubeSide == 3) {
        initPlayerRightVec.vx = 1;
        initPlayerFacingVec.vz = 1;
        initPlayerGravityVec.vy = -1;
    }

    player->rightVec = initPlayerRightVec;
    player->facingDir = initPlayerFacingVec;
    player->gravityDir = initPlayerGravityVec;

    if (rotation == 2) {
        player->rightVec.vx = -initPlayerFacingVec.vx;
        player->rightVec.vy = -initPlayerFacingVec.vy;
        player->rightVec.vz = -initPlayerFacingVec.vz;
        player->facingDir = initPlayerRightVec;
    }
    if (rotation == 3) {
        player->rightVec.vx = -initPlayerRightVec.vx;
        player->rightVec.vy = -initPlayerRightVec.vy;
        player->rightVec.vz = -initPlayerRightVec.vz;
        player->facingDir.vx = -initPlayerFacingVec.vx;
        player->facingDir.vy = -initPlayerFacingVec.vy;
        player->facingDir.vz = -initPlayerFacingVec.vz;
    }
    if (rotation == 4) {
        player->rightVec = initPlayerFacingVec;
        player->facingDir.vx = -initPlayerRightVec.vx;
        player->facingDir.vy = -initPlayerRightVec.vy;
        player->facingDir.vz = -initPlayerRightVec.vz;
    }
}

void SetPlayerMatrix6(Player* player) {
    player->matrix_d4.m[0][0] = player->rightVec.vx << 12;
    player->matrix_d4.m[1][0] = player->rightVec.vy << 12;
    player->matrix_d4.m[2][0] = player->rightVec.vz << 12;
    player->matrix_d4.m[0][2] = player->gravityDir.vx << 12;
    player->matrix_d4.m[1][2] = player->gravityDir.vy << 12;
    player->matrix_d4.m[2][2] = player->gravityDir.vz << 12;
    player->matrix_d4.m[0][1] = -player->facingDir.vx << 12;
    player->matrix_d4.m[1][1] = -player->facingDir.vy << 12;
    player->matrix_d4.m[2][1] = -player->facingDir.vz << 12;
}

INCLUDE_ASM("asm/nonmatchings/level_update", HandleItemTouching);

INCLUDE_ASM("asm/nonmatchings/level_update", CreateAllItemDispLists);

void SetEntityRotation(EntityPos* pos, int param_2, int param_3, int param_4) {
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

void MatrixFromDirectionIndex(MATRIX* m, int param_2, int param_3, short delta, SVECTOR* param_5) {
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

extern SVECTOR SVECTOR_000a2df4;
extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);
extern void Vibrate99(int magnitude1, int magnitude2, int count);
int IsFallingOrJumping(Player* player);

int DAT_000a4748;
int DAT_000a474c;
int DAT_000a4750;
int DAT_000a4754;

void CheckForButtonEntity(Player* player) {
    int temp;
    int temp_init;
    short* ptr;
    unsigned short* ptr0;
    unsigned short* ptr2;
    if (IsFallingOrJumping(player)) {
        player->alreadyProcessedEntityAction = 0;
    } else {
        if (player->alreadyProcessedEntityAction != 9 && player->faceTypePlayerStandingOn == 9 &&
            (u16)player->subpixelPositionOnCube.vz - 197U < 119U &&
            (u16)player->subpixelPositionOnCube.vx - 197U < 119U) {

            player->alreadyProcessedEntityAction = 9;
            DAT_000a4748 = player->specialBlockSideOffsetPlayerIsStandingOn;

            if (((short*)(entityData + DAT_000a4748 * 2))[4] == 1) {
                SndPlaySfx(9, 0, &SVECTOR_000a2df4, 7000);
            } else {
                SndPlaySfx(100, 0, &SVECTOR_000a2df4, 7000);
            }
            Vibrate99(1, 0xff, 1);

            ptr0 = (unsigned short*)(DAT_000a4748 * 2 + (int)entityData);
            DAT_000a4754 = (short)ptr0[7];
            DAT_000a474c = ((int)(ptr0[7] << 16)) >> 20;
            DAT_000a4750 = ptr0[7] & 0xf;

            while (DAT_000a4754 != -1) {
                if (DAT_000a4750 == 6) {
                    ptr = (short*)(DAT_000a474c * 256 + (int)entityData);
                    ptr[3] = (ptr[3] + 1) % 2;
                    DAT_000a4748 = DAT_000a474c << 7;
                    DAT_000a4754 = ptr[23];
                    DAT_000a474c = ((int)(((unsigned short*)ptr)[23] << 16)) >> 20;
                    DAT_000a4750 = ((unsigned short*)ptr)[23] & 0xf;
                } else {
                    DAT_000a4748 = DAT_000a474c * 128 + DAT_000a4750 * 16;
                    ptr2 = (unsigned short*)(DAT_000a4748 * 2 + (int)entityData);
                    ptr2[4] = ((short)ptr2[4] % 2) + 1;
                    DAT_000a4754 = (short)ptr2[6];
                    DAT_000a474c = ((int)(ptr2[6] << 16)) >> 20;
                    DAT_000a4750 = ptr2[6] & 0xf;
                }
            }
        }
    }
}


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
