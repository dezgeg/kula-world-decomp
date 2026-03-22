#include "common.h"

typedef struct LocalMovingPlatformEntity {
    short tag;                // 0x0
    short movementDirection;  // 0x2
    short pad_4[2];           // 0x4, 0x6
    short startX, startY, startZ; // 0x8, 0xa, 0xc
    short endX, endY, endZ;       // 0xe, 0x10, 0x12
    short pad_14[6];              // 0x14 - 0x1f
    ushort flags;                 // 0x20
    short length;                 // 0x22
    ushort velocity;              // 0x24
    short counter;                // 0x26
    short pad_28;                 // 0x28
    short velX, velY, velZ;       // 0x2a, 0x2c, 0x2e
    byte pad_30[190];             // 0x30
    short posX, posY, posZ;       // 0xee, 0xf0, 0xf2
} LocalMovingPlatformEntity;

// gprel-used variables (defined in this file)
int D_000A4398; // mpOff
int D_000A439C; // swapMovingPlatformDir
static SVECTOR SVECTOR_000a43a0;
int DAT_000a43a8;
int DAT_000a43ac;
int D_000A43B0;
int mpLengthScaled;
int mI;

// non-gprel-used variables (extern)
extern short* entityData;
extern short numEntities;

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

extern int GetBlockAt(SVECTOR * coord);
extern int cameraIndex;
extern MATRIX perspMatrixes[];
extern int specialLevelType;
extern int gameMode;

static SVECTOR SVECTOR_000a4514;
static SVECTOR SVECTOR_000a44b8;
static SVECTOR SVECTOR_000a44c0;
static VECTOR VECTOR_000a44f8;
static VECTOR VECTOR_000a44e8;
static MATRIX MATRIX_000a44c8;
static int blockTypePlayerStandingOn;
static int r1TurnDelta;
static int DAT_000a450c;
static int DAT_000a4508;
static int DAT_000a4598;

int D_000A4430;
static VECTOR VECTOR_000a4434;
int D_000A43E8;
int D_000A43DC;
static SVECTOR SVECTOR_000a43ec;
static SVECTOR SVECTOR_000a43e0;

static SVECTOR SVECTOR_000a449c;
static VECTOR VECTOR_000a44a8;
static VECTOR VECTOR_000a451c;
static VECTOR VECTOR_000a452c;
static VECTOR VECTOR_000a453c;
static VECTOR VECTOR_000a454c;
static VECTOR initPlayerFacingDir;
static VECTOR initPlayerFacingDirCoarse;
static VECTOR initPlayerGravityDir;
static VECTOR facingGravityProd;
static int levelEntryAnimTimer;
static int levelEntryAnimTimerIncrement;

int maxDistSquared;
int xMinPlusMax;
int yMinPlusMax;
int zMinPlusMax;
int zoomInAndOutPhase;

INCLUDE_ASM("asm/nonmatchings/level_update", ScanLevelDataForMovingBlocks2);

void MoveMovingPlatforms(SVECTOR vec) {
#define EB ((LocalMovingPlatformEntity *)&entityData[D_000A4398])
    for (D_000A4398 = 0; D_000A4398 < (int)numEntities << 7; D_000A4398 += 128) {
        if (EB->tag == 5) {
            D_000A439C = 0;
            EB->velX = 0;
            EB->velY = 0;
            EB->velZ = 0;

            if (EB->counter != 0) {
                EB->counter--;
            }

            if (EB->counter == 1) {
                SVECTOR_000a43a0.vx = EB->posX - vec.vx;
                SVECTOR_000a43a0.vy = EB->posY - vec.vy;
                SVECTOR_000a43a0.vz = EB->posZ - vec.vz;
                SndPlaySfx(SFX_MOVING_PLATFORM, D_000A4398 + 1, &SVECTOR_000a43a0, 8000);
            }

            if (EB->counter == 0) {
                SVECTOR_000a43a0.vx = EB->posX - vec.vx;
                SVECTOR_000a43a0.vy = EB->posY - vec.vy;
                SVECTOR_000a43a0.vz = EB->posZ - vec.vz;
                SndUpdateVolumeBasedOnDirVec(D_000A4398 + 1, &SVECTOR_000a43a0);

                switch (EB->movementDirection) {
                case 4:
                    EB->posX -= EB->velocity;
                    EB->velX = -EB->velocity;
                    if (EB->posX <= EB->startX << 9) {
                        D_000A439C = 1;
                        EB->movementDirection = 1;
                    }
                    break;
                case 1:
                    EB->posX += EB->velocity;
                    EB->velX = EB->velocity;
                    if (EB->posX >= EB->endX << 9) {
                        D_000A439C = 1;
                        EB->movementDirection = 4;
                    }
                    break;
                case 3:
                    EB->posY -= EB->velocity;
                    EB->velY = -EB->velocity;
                    if (EB->posY <= EB->startY << 9) {
                        D_000A439C = 1;
                        EB->movementDirection = 2;
                    }
                    break;
                case 2:
                    EB->posY += EB->velocity;
                    EB->velY = EB->velocity;
                    if (EB->posY >= EB->endY << 9) {
                        D_000A439C = 1;
                        EB->movementDirection = 3;
                    }
                    break;
                case 0:
                    EB->posZ -= EB->velocity;
                    EB->velZ = -EB->velocity;
                    if (EB->posZ <= EB->startZ << 9) {
                        D_000A439C = 1;
                        EB->movementDirection = 5;
                    }
                    break;
                case 5:
                    EB->posZ += EB->velocity;
                    EB->velZ = EB->velocity;
                    if (EB->posZ >= EB->endZ << 9) {
                        D_000A439C = 1;
                        EB->movementDirection = 0;
                    }
                    break;
                }
            }

            if (D_000A439C != 0) {
                SndMuteVoiceByTag(D_000A4398 + 1);
                EB->counter = 40;
                EB->flags ^= 1;
            }
        }
    }
#undef EB
}

int FUN_00033720(SVECTOR* vec, int itemdataOff, int param_3) {
    mpLengthScaled = (entityData[itemdataOff + 17] - 1) << 9;

    switch (entityData[itemdataOff + 2]) {
    case 1:
        DAT_000a43a8 = vec->vx + param_3 - (entityData[itemdataOff + 119] - 256);
        DAT_000a43ac = entityData[itemdataOff + 119] + mpLengthScaled + 256 - (vec->vx - param_3);
        break;
    case 2:
        DAT_000a43a8 = vec->vy + param_3 - (entityData[itemdataOff + 120] - 256);
        DAT_000a43ac = entityData[itemdataOff + 120] + mpLengthScaled + 256 - (vec->vy - param_3);
        break;
    case 5:
        DAT_000a43a8 = vec->vz + param_3 - (entityData[itemdataOff + 121] - 256);
        DAT_000a43ac = entityData[itemdataOff + 121] + mpLengthScaled + 256 - (vec->vz - param_3);
        break;
    }

    if (DAT_000a43a8 >= 0 && DAT_000a43ac >= 0) {
        return 1;
    }
    return 0;
}

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

void JumpingOnMovingPlatform(Player *player) {
    player->jumpingOnMovingPlatform = 1;
    player->onMovingPlatform = 0;

    for (mI = 2; mI <= entityData[player->movingPlatformEntityIdStandingOn + 17] + 1; mI++) {
        SHORT_ARRAY_ARRAY_ARRAY_000d4678[mI][2][2] =
        SHORT_ARRAY_ARRAY_ARRAY_000d4678[2][mI][2] =
        SHORT_ARRAY_ARRAY_ARRAY_000d4678[2][2][mI] = -1;
    }

    player->finePos.vx += entityData[player->movingPlatformEntityIdStandingOn + 119] - 512;
    player->finePos.vy += entityData[player->movingPlatformEntityIdStandingOn + 120] - 512;
    player->finePos.vz += entityData[player->movingPlatformEntityIdStandingOn + 121] - 512;

    player->svec54.vx = player->svec54.vy = player->svec54.vz = 0;
}

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

void ProcessCameraAndMovement(Player *player) {
    player->playerHasControl = 0;
    if (levelEntryAnimTimer >= 1025 || gameMode == 1) {
        if (player->debugCameraMode != 0) {
            HandleDebugCamera(player);
        } else {
            HandlePlayerMovementStuff(player);
        }
        return;
    }

    player->copycatMoveIndex = 0;
    FUN_00034518(&VECTOR_000a451c, &VECTOR_000a452c, &VECTOR_000a453c, &VECTOR_000a454c, &VECTOR_000a4434, (short)levelEntryAnimTimer, 15);

    initPlayerFacingDirCoarse.vx = initPlayerFacingDir.vx / 1024;
    initPlayerFacingDirCoarse.vy = initPlayerFacingDir.vy / 1024;
    initPlayerFacingDirCoarse.vz = initPlayerFacingDir.vz / 1024;

    VectorNormal(&initPlayerFacingDirCoarse, &initPlayerFacingDirCoarse);
    OuterProduct12(&initPlayerFacingDirCoarse, &initPlayerGravityDir, &facingGravityProd);
    VectorNormal(&facingGravityProd, &facingGravityProd);

    perspMatrixes[cameraIndex].m[0][0] = (short)facingGravityProd.vx;
    perspMatrixes[cameraIndex].m[0][1] = (short)facingGravityProd.vy;
    perspMatrixes[cameraIndex].m[0][2] = (short)facingGravityProd.vz;
    perspMatrixes[cameraIndex].m[1][0] = -(short)initPlayerGravityDir.vx;
    perspMatrixes[cameraIndex].m[1][1] = -(short)initPlayerGravityDir.vy;
    perspMatrixes[cameraIndex].m[1][2] = -(short)initPlayerGravityDir.vz;
    perspMatrixes[cameraIndex].m[2][0] = (short)initPlayerFacingDirCoarse.vx;
    perspMatrixes[cameraIndex].m[2][1] = (short)initPlayerFacingDirCoarse.vy;
    perspMatrixes[cameraIndex].m[2][2] = (short)initPlayerFacingDirCoarse.vz;

    RotMatrixY(levelEntryAnimTimer - 1024, &perspMatrixes[cameraIndex]);
    RotMatrixX(250, &perspMatrixes[cameraIndex]);
    RotMatrixZ((1024 - levelEntryAnimTimer) * 2, &perspMatrixes[cameraIndex]);

    ApplyMatrixLV(&perspMatrixes[cameraIndex], &VECTOR_000a4434, &VECTOR_000a4434);

    perspMatrixes[cameraIndex].t[0] = -VECTOR_000a4434.vx;
    perspMatrixes[cameraIndex].t[1] = -VECTOR_000a4434.vy + 250;
    perspMatrixes[cameraIndex].t[2] = -VECTOR_000a4434.vz + 800 + (1024 - levelEntryAnimTimer) * 14000 / 1024;

    player->movementInhibitTimer = 2000;

    if (levelEntryAnimTimer > 880) {
        levelEntryAnimTimer += levelEntryAnimTimerIncrement;
        levelEntryAnimTimerIncrement--;
        if (levelEntryAnimTimerIncrement < 1) {
            levelEntryAnimTimerIncrement = 1;
        }
    } else {
        levelEntryAnimTimer += 16;
    }

    if (levelEntryAnimTimer > 1024) {
        player->movementInhibitTimer = 1;
    }

    player->debugCamY = 0;
    player->debugCamX = 0;
}

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

void HandlePlayerMovementStuff(Player *player) {
    short turningTimer;

    player->playerHasControl = 1;

    if (player->startTurningTo == 1) {
        SVECTOR_000a4514 = player->rightVec;
        player->facingDirBeforeStartingTurning = player->facingDir;
        player->rightVec.vx = -player->facingDir.vx;
        player->rightVec.vy = -player->facingDir.vy;
        player->rightVec.vz = -player->facingDir.vz;
        player->facingDir = SVECTOR_000a4514;
        player->turningWhere = 3;
        player->startTurningTo = -1;
    }

    if (player->startTurningTo == 2) {
        player->facingDirBeforeStartingTurning = player->facingDir;
        SVECTOR_000a4514.vx = -player->rightVec.vx;
        SVECTOR_000a4514.vy = -player->rightVec.vy;
        SVECTOR_000a4514.vz = -player->rightVec.vz;
        player->rightVec = player->facingDir;
        player->facingDir = SVECTOR_000a4514;
        player->turningWhere = 4;
        player->startTurningTo = -1;
    }

    turningTimer = player->turningTimer;
    if (turningTimer > 0) {
        player->turningPhase += player->turningDelta;
        player->turningTimer--;
        if (player->movementVelocity > 10) {
            player->movementVelocity = 10;
        }
    }

    if (player->turningTimer == 0) {
        player->facingDirBeforeStartingTurning = player->facingDir;
        if (player->turningTimer == 0) {
            player->turningTimer--;
            player->turningPhase = 0;
            player->perspVec1 = player->rightVec;
            player->perspVec3 = player->facingDir;
            player->turnDelayTimer = 0;
            if (player->turningLeftRelated != 2) {
                player->turningLeftRelated = 0;
            }
            if (player->turningRightRelated != 2) {
                player->turningRightRelated = 0;
            }
            player->startTurningTo = 0;
            player->field119_0x1c8 = 0;
        }
    }

    if (player->field100_0x1ac == -1 && player->field119_0x1c8 != 2) {
        SVECTOR_000a4514 = player->perspVec2;
        player->gravityDir.vx = -player->facingDir.vx;
        player->gravityDir.vy = -player->facingDir.vy;
        player->gravityDir.vz = -player->facingDir.vz;
        player->facingDir = SVECTOR_000a4514;
        player->field100_0x1ac = 0;
        player->field119_0x1c8 = 1;
    }

    if (player->field100_0x1ac == 1 && player->field119_0x1c8 != 2) {
        player->field100_0x1ac = 0;
        player->field119_0x1c8 = 1;
    }

    if (player->viewpointRotationTimer > 0 && player->field119_0x1c8 != 2) {
        player->viewportRotationAngle += player->viewpointRotationAngleIncrement;
        player->viewpointRotationTimer--;
    }

    if (player->viewpointRotationTimer == 0 && player->field119_0x1c8 != 2) {
        player->viewpointRotationTimer--;
        player->viewportRotationAngle = 0;
        player->perspVec2 = player->gravityDir;
        player->perspVec3 = player->facingDir;
        player->field119_0x1c8 = 0;
    }

    perspMatrixes[cameraIndex].m[0][0] = (u16)player->perspVec1.vx << 12;
    perspMatrixes[cameraIndex].m[0][1] = (u16)player->perspVec1.vy << 12;
    perspMatrixes[cameraIndex].m[0][2] = (u16)player->perspVec1.vz << 12;
    perspMatrixes[cameraIndex].m[1][0] = (-(int)(u16)player->perspVec2.vx) << 12;
    perspMatrixes[cameraIndex].m[1][1] = (-(int)(u16)player->perspVec2.vy) << 12;
    perspMatrixes[cameraIndex].m[1][2] = (-(int)(u16)player->perspVec2.vz) << 12;
    perspMatrixes[cameraIndex].m[2][0] = (u16)player->perspVec3.vx << 12;
    perspMatrixes[cameraIndex].m[2][1] = (u16)player->perspVec3.vy << 12;
    perspMatrixes[cameraIndex].m[2][2] = (u16)player->perspVec3.vz << 12;

    VECTOR_000a44e8.vx = player->svec_184.vx;
    VECTOR_000a44e8.vy = player->svec_184.vy;
    VECTOR_000a44e8.vz = player->svec_184.vz;

    DAT_000a450c = 0;
    DAT_000a4508 = 0;
    if (player->cameraR1R2TurnDirection.vx == -1) {
        if (player->howMoving198 != FALLING && DAT_000a4598 == 0) {
            DAT_000a450c = 1;
            player->cameraR1TurnAmount.vx += 26;
            if (player->cameraR1TurnAmount.vx > 650)
                player->cameraR1TurnAmount.vx = 650;
        }
    }

    if (player->cameraR1R2TurnDirection.vx == 1 && player->cameraR1TurnAmount.vx == 0) {
        DAT_000a4508 = 1;
        DAT_000a4598 += 26;
        if (DAT_000a4598 > 650)
            DAT_000a4598 = 650;
    }

    SVECTOR_000a44b8.vx = player->finePos.vx - (player->gravityDir.vx * 512);
    SVECTOR_000a44b8.vy = player->finePos.vy - (player->gravityDir.vy * 512);
    SVECTOR_000a44b8.vz = player->finePos.vz - (player->gravityDir.vz * 512);

    blockTypePlayerStandingOn = GetBlockAt(&SVECTOR_000a44b8);
    blockTypePlayerStandingOn = player->surroundingBlocks[0][1][1];

    if (player->howMoving198 == FALLING || (player->howMoving198 == JUMPING_FORWARD && player->jumpingOrViewportRotationTimer < 10)) {
        if (blockTypePlayerStandingOn == -1) {
            if (player->cameraR1TurnAmount.vx < 550) {
                if (DAT_000a4598 == 0) {
                    DAT_000a450c = 1;
                    r1TurnDelta++;
                    if (r1TurnDelta > 32)
                        r1TurnDelta = 32;
                    player->cameraR1TurnAmount.vx += r1TurnDelta;
                }
            } else {
                DAT_000a450c = 1;
                r1TurnDelta--;
                if (r1TurnDelta < 10)
                    r1TurnDelta = 10;
                player->cameraR1TurnAmount.vx += r1TurnDelta;
                if (player->cameraR1TurnAmount.vx > 650)
                    player->cameraR1TurnAmount.vx = 650;
            }
        }
    } else {
        r1TurnDelta = 3;
    }

    if (DAT_000a4508 == 0 && DAT_000a4598 > 0) {
        DAT_000a4598 -= 18;
        if (DAT_000a4598 < 0)
            DAT_000a4598 = 0;
    }

    if (DAT_000a450c == 0) {
        if (player->cameraR1TurnAmount.vx > 0) {
            player->cameraR1TurnAmount.vx -= 18;
            if (player->cameraR1TurnAmount.vx < 0)
                player->cameraR1TurnAmount.vx = 0;
        }
        if (player->cameraR1TurnAmount.vx < 0) {
            player->cameraR1TurnAmount.vx += 18;
            if (player->cameraR1TurnAmount.vx > 0)
                player->cameraR1TurnAmount.vx = 0;
        }
    }

    if (player->cameraR1R2TurnDirection.vy == 0) {
        if (player->cameraR1TurnAmount.vy > 0) {
            player->cameraR1TurnAmount.vy -= 40;
            if (player->cameraR1TurnAmount.vy < 0)
                player->cameraR1TurnAmount.vy = 0;
        }
        if (player->cameraR1TurnAmount.vy < 0) {
            player->cameraR1TurnAmount.vy += 40;
            if (player->cameraR1TurnAmount.vy > 0)
                player->cameraR1TurnAmount.vy = 0;
        }
    }

    RotMatrixY(player->turningPhase + player->cameraR1TurnAmount.vy, &perspMatrixes[cameraIndex]);
    RotMatrixX(player->viewportRotationAngle, &perspMatrixes[cameraIndex]);

    player->perspMatrix.m[0][0] = perspMatrixes[cameraIndex].m[0][0];
    player->perspMatrix.m[0][1] = perspMatrixes[cameraIndex].m[0][1];
    player->perspMatrix.m[0][2] = perspMatrixes[cameraIndex].m[0][2];
    player->perspMatrix.m[1][0] = perspMatrixes[cameraIndex].m[1][0];
    player->perspMatrix.m[1][1] = perspMatrixes[cameraIndex].m[1][1];
    player->perspMatrix.m[1][2] = perspMatrixes[cameraIndex].m[1][2];
    player->perspMatrix.m[2][0] = perspMatrixes[cameraIndex].m[2][0];
    player->perspMatrix.m[2][1] = perspMatrixes[cameraIndex].m[2][1];
    player->perspMatrix.m[2][2] = perspMatrixes[cameraIndex].m[2][2];

    if (player->lethargyTimer > 0) {
        RotMatrixZ(rsin((player->lethargyTimer * 64) % 4096) / 32, &perspMatrixes[cameraIndex]);
    }

    if (DAT_000a4598 != 0) {
        RotMatrixX(250, &perspMatrixes[cameraIndex]);
        RotMatrixX(-DAT_000a4598, &perspMatrixes[cameraIndex]);
        ApplyMatrixLV(&perspMatrixes[cameraIndex], &VECTOR_000a44e8, &VECTOR_000a44e8);
        VECTOR_000a44f8.vx = 0;
        SVECTOR_000a44c0.vy = 0;
        SVECTOR_000a44c0.vz = 0;
        VECTOR_000a44f8.vz = 800 - DAT_000a4598 / 2;
        SVECTOR_000a44c0.vx = -DAT_000a4598;
        VECTOR_000a44f8.vy = 250 - DAT_000a4598 / 3;
        RotMatrix(&SVECTOR_000a44c0, &MATRIX_000a44c8);
        ApplyMatrixLV(&MATRIX_000a44c8, &VECTOR_000a44f8, &VECTOR_000a44f8);
        perspMatrixes[cameraIndex].t[0] = -VECTOR_000a44e8.vx + VECTOR_000a44f8.vx;
        perspMatrixes[cameraIndex].t[1] = -VECTOR_000a44e8.vy + VECTOR_000a44f8.vy;
        perspMatrixes[cameraIndex].t[2] = -VECTOR_000a44e8.vz + VECTOR_000a44f8.vz;
    } else {
        RotMatrixX(player->cameraR1TurnAmount.vx + 250, &perspMatrixes[cameraIndex]);
        ApplyMatrixLV(&perspMatrixes[cameraIndex], &VECTOR_000a44e8, &VECTOR_000a44e8);
        perspMatrixes[cameraIndex].t[0] = -VECTOR_000a44e8.vx;
        perspMatrixes[cameraIndex].t[1] = -VECTOR_000a44e8.vy + 250;
        perspMatrixes[cameraIndex].t[2] = -VECTOR_000a44e8.vz + 800;
    }
}

void SetInvulnerable(void) {
    thePlayer.invulnerabilityTimer = 3000;
}
