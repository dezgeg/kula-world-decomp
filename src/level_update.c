#include "common.h"

int DAT_000a43c4;
short DAT_000a43fc;
short DAT_000a4400;
short DAT_000a4404;
short DAT_000a4408;
short DAT_000a440c;
short DAT_000a4410;
static SVECTOR SVECTOR_000a4428;
short mpVelSum;
short tempI;
short tempJ;
short tempK;

extern short SHORT_ARRAY_ARRAY_ARRAY_000d4678[8][8][8];
extern Player thePlayer;
extern short* entityData;

int FUN_00033720(SVECTOR* vec, int itemdataOff, int param_3);

int D_000A4430;
int D_000A43E8;
int D_000A43DC;
static SVECTOR SVECTOR_000a43ec;
static SVECTOR SVECTOR_000a43e0;

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

INCLUDE_ASM("asm/nonmatchings/level_update", ScanLevelDataForMovingBlocks2);

INCLUDE_ASM("asm/nonmatchings/level_update", MoveMovingPlatforms);

INCLUDE_ASM("asm/nonmatchings/level_update", FUN_00033720);

int FUN_0003382c(Player* player) {
    DAT_000a43c4 = (player->surroundingBlocks[1][1][1] - 5) * 128;
    if (DAT_000a43c4 < 0 || entityData[DAT_000a43c4] != 5) {
        DAT_000a43c4 = (player->surroundingBlocks[2][1][1] - 5) * 128;
        if (DAT_000a43c4 < 0 || entityData[DAT_000a43c4 ] != 5) {
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
        if (entityData[D_000A43DC] != 5) {
            return -1;
        }

        if (FUN_000344b0(entityData[D_000A43DC + 2], GetRotationIndexFromVector(player->gravityDir)) != 0) {
            return -1;
        }

        if (player->subpixelPositionOnCube.vy < 101) {
            if (FUN_00033720(&player->finePos, D_000A43DC, 0) != 0) {
                goto ret_D;
            }
            return -1;
        }
    }

    return -1;
ret_D:
    return D_000A43DC;
}

int FUN_00033eb0(Player* player, SVECTOR* param_2) {
    if (param_2->vx == -1) {
        SVECTOR_000a43ec = player->finePos;
    } else {
        SVECTOR_000a43ec = *param_2;
    }

    D_000A43E8 = (GetBlockAt(&SVECTOR_000a43ec) - 5) * 128;

    if (D_000A43E8 < 0 || entityData[D_000A43E8] != 5) {
        SVECTOR_000a43ec.vx -= player->gravityDir.vx * 512;
        SVECTOR_000a43ec.vy -= player->gravityDir.vy * 512;
        SVECTOR_000a43ec.vz -= player->gravityDir.vz * 512;

        D_000A43E8 = (GetBlockAt(&SVECTOR_000a43ec) - 5) * 128;
        if (D_000A43E8 < 0) {
            return -1;
        }
        if (entityData[D_000A43E8] != 5) {
            return -1;
        }
    }

    if (FUN_000344b0(entityData[D_000A43E8 + 2], GetRotationIndexFromVector(player->gravityDir)) == 0) {
        return -1;
    }
    if (FUN_00033720(&player->finePos, D_000A43E8, 100) != 0) {
        return D_000A43E8;
    }

    return -1;
}

INCLUDE_ASM("asm/nonmatchings/level_update", JumpingOnMovingPlatform);

void FUN_0003418c(Player *player) {
    short (*grid)[8][8] = SHORT_ARRAY_ARRAY_ARRAY_000d4678;
    short gx, gy, gz;
    short rx, ry, rz;
    short vx, vy, vz;

    vx = (player->finePos.vx + 256) >> 9;
    SVECTOR_000a4428.vx = vx;
    vy = (player->finePos.vy + 256) >> 9;
    SVECTOR_000a4428.vy = vy;
    vz = (player->finePos.vz + 256) >> 9;
    SVECTOR_000a4428.vz = vz;

    gx = player->gravityDir.vx;
    rx = player->rightVec.vx;
    gx--;
    vx -= gx;
    DAT_000a43fc = (rx + vx) - player->facingDir.vx;

    gy = player->gravityDir.vy;
    ry = player->rightVec.vy;
    gy--;
    vy -= gy;
    DAT_000a4400 = (ry + vy) - player->facingDir.vy;

    gz = player->gravityDir.vz;
    rz = player->rightVec.vz;
    gz--;
    vz -= gz;
    DAT_000a4404 = (rz + vz) - player->facingDir.vz;

    for (tempI = 0; tempI < 3; tempI++) {
        for (tempJ = 0; tempJ < 3; tempJ++) {
            for (tempK = 0; tempK < 3; tempK++) {
                DAT_000a4408 = (DAT_000a43fc + tempI * player->gravityDir.vx + tempJ * player->facingDir.vx) - tempK * player->rightVec.vx;
                DAT_000a440c = (DAT_000a4400 + tempI * player->gravityDir.vy + tempJ * player->facingDir.vy) - tempK * player->rightVec.vy;
                DAT_000a4410 = (DAT_000a4404 + tempI * player->gravityDir.vz + tempJ * player->facingDir.vz) - tempK * player->rightVec.vz;
                player->surroundingBlocks[tempI][tempJ][tempK] = grid[DAT_000a4408][DAT_000a440c][DAT_000a4410];
                mpVelSum = (short)player->surroundingBlocks[tempI][tempJ][tempK];
            }
        }
    }

    SVECTOR_000a4428.vx = player->finePos.vx + player->svec54.vx;
    SVECTOR_000a4428.vy = player->finePos.vy + player->svec54.vy;
    SVECTOR_000a4428.vz = player->finePos.vz + player->svec54.vz;

    mpVelSum = GetBlockAt(&SVECTOR_000a4428);
    if (mpVelSum == -2) {
        player->surroundingBlocks[1][1][1] = -2;
    }

    {
        MovingPlatformEntity *mpe = (MovingPlatformEntity *)(player->movingPlatformEntityIdStandingOn * 2 + (int)entityData);
        mpVelSum = mpe->velX + mpe->velY + mpe->velZ;
    }

    if (mpVelSum != 0) {
        Vibrate99(0, 70, 2);
    }
}

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
