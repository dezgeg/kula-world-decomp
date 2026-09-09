#include "common.h"

typedef struct LocalMovingPlatformEntity {
    short tag;                    // 0x0
    short movementDirection;      // 0x2
    short pad_4[2];               // 0x4, 0x6
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

extern int GetBlockAt(SVECTOR* coord);

void HandlePlayerMovementStuff(Player* player);
void MoveMovingPlatforms(SVECTOR vec);

// non-gprel-used variables (extern)
extern short* entityData;
extern short numEntities;
extern short SHORT_ARRAY_ARRAY_ARRAY_000d4678[8][8][8];
extern Player thePlayer;
extern short* levelData;
extern int cameraIndex;
extern MATRIX perspMatrixes[];
extern int specialLevelType;
extern int gameMode;

// gprel-used variables (defined in this file)
static int unusedA43B0;
static int unusedBezierInvT;
static int maxDistSquared;
static int xMinPlusMax;
static int yMinPlusMax;
static int zMinPlusMax;
static int zoomInAndOutPhase;
static VECTOR unusedFacingGravityProd;
static VECTOR initPlayerFacingDir;
static VECTOR unusedInitPlayerFacingDirCoarse;
static VECTOR initPlayerGravityDir;
static VECTOR initPlayerRightDir;
static VECTOR levelEntryBezierP0;
static VECTOR levelEntryBezierP1;
static VECTOR levelEntryBezierP2;
static VECTOR levelEntryBezierP3;
static VECTOR levelEntryCamPos;

static int cameraR2TurnAmount;
static int levelEntryAnimTimer;
static int levelEntryAnimTimerIncrement;
static int r1TurnDelta;

SVECTOR SVECTOR_000a2de4 = {};
SVECTOR SVECTOR_allMinus1 = { -1, -1, -1 };

static int ScanLevelDataForMovingBlocks2_i;
static int ScanLevelDataForMovingBlocks2_j;
static int ScanLevelDataForMovingBlocks2_k;
static int ScanLevelDataForMovingBlocks2_l;
#define i ScanLevelDataForMovingBlocks2_i
#define j ScanLevelDataForMovingBlocks2_j
#define k ScanLevelDataForMovingBlocks2_k
#define l ScanLevelDataForMovingBlocks2_l
void ScanLevelDataForMovingBlocks2(void) {
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            for (k = 0; k < 8; k++) {
                SHORT_ARRAY_ARRAY_ARRAY_000d4678[i][j][k] = -1;
            }
        }
    }

    for (i = 0; i < numEntities * 128; i += 128) {
        if (entityData[i] != OBJ_TRANSPORTER) {
            continue;
        }
        j = entityData[i + 4];
        k = entityData[i + 5];
        l = entityData[i + 6];
        switch (entityData[i + 2]) {
            case 1:
                for (j = entityData[i + 4]; j < entityData[i + 7] + entityData[i + 17]; j++) {
                    levelData[j * 1156 + k * 34 + l] = entityData[i + 20];
                }
                break;
            case 2:
                for (k = entityData[i + 5]; k < entityData[i + 8] + entityData[i + 17]; k++) {
                    levelData[j * 1156 + k * 34 + l] = entityData[i + 20];
                }
                break;
            case 5:
                for (l = entityData[i + 6]; l < entityData[i + 9] + entityData[i + 17]; l++) {
                    levelData[j * 1156 + k * 34 + l] = entityData[i + 20];
                }
                break;
        }

        entityData[i + 19] = 0;
        entityData[i + 18] = entityData[i + 18];
    }
    MoveMovingPlatforms(SVECTOR_000a2de4);
}
#undef i
#undef j
#undef k
#undef l

static int MoveMovingPlatforms_entityOffset;
static int MoveMovingPlatforms_reachedEnd;
static SVECTOR MoveMovingPlatforms_sfxDir;
#define entityOffset MoveMovingPlatforms_entityOffset
#define reachedEnd MoveMovingPlatforms_reachedEnd
#define sfxDir MoveMovingPlatforms_sfxDir
void MoveMovingPlatforms(SVECTOR vec) {
#define EB ((LocalMovingPlatformEntity*)&entityData[entityOffset])
    for (entityOffset = 0; entityOffset < (int)numEntities * 128; entityOffset += 128) {
        if (EB->tag == OBJ_TRANSPORTER) {
            reachedEnd = 0;
            EB->velX = 0;
            EB->velY = 0;
            EB->velZ = 0;

            if (EB->counter != 0) {
                EB->counter--;
            }

            if (EB->counter == 1) {
                sfxDir.vx = EB->posX - vec.vx;
                sfxDir.vy = EB->posY - vec.vy;
                sfxDir.vz = EB->posZ - vec.vz;
                SndPlaySfx(SFX_MOVING_PLATFORM, entityOffset + 1, &sfxDir, 8000);
            }

            if (EB->counter == 0) {
                sfxDir.vx = EB->posX - vec.vx;
                sfxDir.vy = EB->posY - vec.vy;
                sfxDir.vz = EB->posZ - vec.vz;
                SndUpdateVolumeBasedOnDirVec(entityOffset + 1, &sfxDir);

                switch (EB->movementDirection) {
                    case 4:
                        EB->posX -= EB->velocity;
                        EB->velX = -EB->velocity;
                        if (EB->posX <= EB->startX << 9) {
                            reachedEnd = 1;
                            EB->movementDirection = 1;
                        }
                        break;
                    case 1:
                        EB->posX += EB->velocity;
                        EB->velX = EB->velocity;
                        if (EB->posX >= EB->endX << 9) {
                            reachedEnd = 1;
                            EB->movementDirection = 4;
                        }
                        break;
                    case 3:
                        EB->posY -= EB->velocity;
                        EB->velY = -EB->velocity;
                        if (EB->posY <= EB->startY << 9) {
                            reachedEnd = 1;
                            EB->movementDirection = 2;
                        }
                        break;
                    case 2:
                        EB->posY += EB->velocity;
                        EB->velY = EB->velocity;
                        if (EB->posY >= EB->endY << 9) {
                            reachedEnd = 1;
                            EB->movementDirection = 3;
                        }
                        break;
                    case 0:
                        EB->posZ -= EB->velocity;
                        EB->velZ = -EB->velocity;
                        if (EB->posZ <= EB->startZ << 9) {
                            reachedEnd = 1;
                            EB->movementDirection = 5;
                        }
                        break;
                    case 5:
                        EB->posZ += EB->velocity;
                        EB->velZ = EB->velocity;
                        if (EB->posZ >= EB->endZ << 9) {
                            reachedEnd = 1;
                            EB->movementDirection = 0;
                        }
                        break;
                }
            }

            if (reachedEnd) {
                SndMuteVoiceByTag(entityOffset + 1);
                EB->counter = 40;
                EB->flags ^= 1;
            }
        }
    }
#undef EB
}
#undef entityOffset
#undef reachedEnd
#undef sfxDir

static int IsVecWithinPlatformBounds_diffLow;
static int IsVecWithinPlatformBounds_diffHigh;
static int IsVecWithinPlatformBounds_lengthScaled;
#define diffLow IsVecWithinPlatformBounds_diffLow
#define diffHigh IsVecWithinPlatformBounds_diffHigh
#define lengthScaled IsVecWithinPlatformBounds_lengthScaled
int IsVecWithinPlatformBounds(SVECTOR* pos, int entityOffset, int tolerance) {
    lengthScaled = (entityData[entityOffset + 17] - 1) << 9;

    switch (entityData[entityOffset + 2]) {
        case 1:
            diffLow = pos->vx + tolerance - (entityData[entityOffset + 119] - 256);
            diffHigh = entityData[entityOffset + 119] + lengthScaled + 256 - (pos->vx - tolerance);
            break;
        case 2:
            diffLow = pos->vy + tolerance - (entityData[entityOffset + 120] - 256);
            diffHigh = entityData[entityOffset + 120] + lengthScaled + 256 - (pos->vy - tolerance);
            break;
        case 5:
            diffLow = pos->vz + tolerance - (entityData[entityOffset + 121] - 256);
            diffHigh = entityData[entityOffset + 121] + lengthScaled + 256 - (pos->vz - tolerance);
            break;
    }

    if (diffLow >= 0 && diffHigh >= 0) {
        return 1;
    }
    return 0;
}
#undef diffLow
#undef diffHigh
#undef lengthScaled

static int IsPlayerOnMovingPlatform_entityOffset;
#define entityOffset IsPlayerOnMovingPlatform_entityOffset
int IsPlayerOnMovingPlatform(Player* player) {
    entityOffset = (player->surroundingBlocks[1][1][1] - 5) * 128;
    if (entityOffset < 0 || entityData[entityOffset] != OBJ_TRANSPORTER) {
        entityOffset = (player->surroundingBlocks[2][1][1] - 5) * 128;
        if (entityOffset < 0 || entityData[entityOffset] != OBJ_TRANSPORTER) {
            return 0;
        }
    }
    return IsVecWithinPlatformBounds(&player->finePos, entityOffset, 100);
}
#undef entityOffset

static int HandleMovingPlatforms_platformDir;
static int HandleMovingPlatforms_entityId;
static int HandleMovingPlatforms_counter;
#define platformDir HandleMovingPlatforms_platformDir
#define entityId HandleMovingPlatforms_entityId
#define counter HandleMovingPlatforms_counter
int HandleMovingPlatforms(Player* player) {
    if (player->onMovingPlatform) {
        return 0;
    }

    entityId = GetMovingPlatformAt(player, &SVECTOR_allMinus1);
    if (entityId != -1) {
        SndPlaySfx(SFX_BALL_BOUNCE, 0, &SVECTOR_000a2de4, 7000);
        player->onMovingPlatform = 1;
        player->howMoving0 = 0;
        player->howMoving198 = NOT_MOVING;

        platformDir = entityData[entityId + 2];

        for (counter = 2; counter <= entityData[entityId + 17] + 1; counter++) {
            if (platformDir == 1) {
                SHORT_ARRAY_ARRAY_ARRAY_000d4678[counter][2][2] = 0;
            }
            if (platformDir == 2) {
                SHORT_ARRAY_ARRAY_ARRAY_000d4678[2][counter][2] = 0;
            }
            if (platformDir == 5) {
                SHORT_ARRAY_ARRAY_ARRAY_000d4678[2][2][counter] = 0;
            }
        }

        player->finePos.vx -= entityData[entityId + 119] - 512;
        player->finePos.vy -= entityData[entityId + 120] - 512;
        player->finePos.vz -= entityData[entityId + 121] - 512;

        player->svec54.vx = entityData[entityId + 119] - 512;
        player->svec54.vy = entityData[entityId + 120] - 512;
        player->svec54.vz = entityData[entityId + 121] - 512;

        player->movingPlatformEntityIdStandingOn = entityId;
        player->longJump = 0;

        SetLandingSquishVars();
        UpdatePlayerSurroundingBlocks(player);
        UpdateSubpixelPositions(player);
        return 1;
    }

    entityId = GetAlternateMovingPlatform(player, &SVECTOR_allMinus1);
    if (entityId != -1) {
        SndPlaySfx(SFX_BALL_BOUNCE, 0, &SVECTOR_000a2de4, 7000);
        player->onMovingPlatform = 1;
        player->howMoving0 = 0;
        player->howMoving198 = NOT_MOVING;

        platformDir = entityData[entityId + 2];

        for (counter = 2; counter <= entityData[entityId + 17] + 1; counter++) {
            if (platformDir == 1) {
                SHORT_ARRAY_ARRAY_ARRAY_000d4678[counter][2][2] = 0;
            }
            if (platformDir == 2) {
                SHORT_ARRAY_ARRAY_ARRAY_000d4678[2][counter][2] = 0;
            }
            if (platformDir == 5) {
                SHORT_ARRAY_ARRAY_ARRAY_000d4678[2][2][counter] = 0;
            }
        }

        player->finePos.vx -= entityData[entityId + 119] - 512;
        player->finePos.vy -= entityData[entityId + 120] - 512;
        player->finePos.vz -= entityData[entityId + 121] - 512;

        switch (GetRotationIndexFromVector(player->gravityDir)) {
            case 4:
                player->finePos.vx = 156;
                break;
            case 1:
                player->finePos.vx = entityData[entityId + 17] * 512 + 356;
                break;
            case 3:
                player->finePos.vy = 156;
                break;
            case 2:
                player->finePos.vy = entityData[entityId + 17] * 512 + 356;
                break;
            case 0:
                player->finePos.vz = 156;
                break;
            case 5:
                player->finePos.vz = entityData[entityId + 17] * 512 + 356;
                break;
        }

        player->svec54.vx = entityData[entityId + 119] - 512;
        player->svec54.vy = entityData[entityId + 120] - 512;
        player->svec54.vz = entityData[entityId + 121] - 512;

        player->movingPlatformEntityIdStandingOn = entityId;
        player->longJump = 0;

        SetLandingSquishVars();
        UpdatePlayerSurroundingBlocks(player);
        UpdateSubpixelPositions(player);
        return 1;
    }
    return 0;
}
#undef platformDir
#undef entityId
#undef counter

static int GetMovingPlatformAt_entityOffset;
static SVECTOR GetMovingPlatformAt_targetPos;
#define entityOffset GetMovingPlatformAt_entityOffset
#define targetPos GetMovingPlatformAt_targetPos
int GetMovingPlatformAt(Player* player, SVECTOR* checkPos) {
    if (checkPos->vx == -1) {
        targetPos.vx = player->finePos.vx - player->gravityDir.vx * 512;
        targetPos.vy = player->finePos.vy - player->gravityDir.vy * 512;
        targetPos.vz = player->finePos.vz - player->gravityDir.vz * 512;
    } else {
        targetPos = *checkPos;
    }

    entityOffset = (GetBlockAt(&targetPos) - 5) * 128;

    if (entityOffset >= 0) {
        if (entityData[entityOffset] != OBJ_TRANSPORTER) {
            return -1;
        }

        if (AreDirectionsOnSameAxis(entityData[entityOffset + 2], GetRotationIndexFromVector(player->gravityDir))) {
            return -1;
        }

        if (player->subpixelPositionOnCube.vy < 101) {
            if (IsVecWithinPlatformBounds(&player->finePos, entityOffset, 0)) {
                goto ret_D;
            }
            return -1;
        }
    }

    return -1;
ret_D:
    return entityOffset;
}
#undef entityOffset
#undef targetPos

static int GetAlternateMovingPlatform_entityOffset;
static SVECTOR GetAlternateMovingPlatform_targetPos;
#define entityOffset GetAlternateMovingPlatform_entityOffset
#define targetPos GetAlternateMovingPlatform_targetPos
int GetAlternateMovingPlatform(Player* player, SVECTOR* checkPos) {
    if (checkPos->vx == -1) {
        targetPos = player->finePos;
    } else {
        targetPos = *checkPos;
    }

    entityOffset = (GetBlockAt(&targetPos) - 5) * 128;

    if (entityOffset < 0 || entityData[entityOffset] != OBJ_TRANSPORTER) {
        targetPos.vx -= player->gravityDir.vx * 512;
        targetPos.vy -= player->gravityDir.vy * 512;
        targetPos.vz -= player->gravityDir.vz * 512;

        entityOffset = (GetBlockAt(&targetPos) - 5) * 128;
        if (entityOffset < 0) {
            return -1;
        }
        if (entityData[entityOffset] != 5) {
            return -1;
        }
    }

    if (!AreDirectionsOnSameAxis(entityData[entityOffset + 2], GetRotationIndexFromVector(player->gravityDir))) {
        return -1;
    }
    if (IsVecWithinPlatformBounds(&player->finePos, entityOffset, 100)) {
        return entityOffset;
    }

    return -1;
}
#undef entityOffset
#undef targetPos

static int JumpingOnMovingPlatform_i;
#define i JumpingOnMovingPlatform_i
void JumpingOnMovingPlatform(Player* player) {
    player->jumpingOnMovingPlatform = 1;
    player->onMovingPlatform = 0;

    for (i = 2; i <= entityData[player->movingPlatformEntityIdStandingOn + 17] + 1; i++) {
        SHORT_ARRAY_ARRAY_ARRAY_000d4678[i][2][2] =
        SHORT_ARRAY_ARRAY_ARRAY_000d4678[2][i][2] =
        SHORT_ARRAY_ARRAY_ARRAY_000d4678[2][2][i] = -1;
    }

    player->finePos.vx += entityData[player->movingPlatformEntityIdStandingOn + 119] - 512;
    player->finePos.vy += entityData[player->movingPlatformEntityIdStandingOn + 120] - 512;
    player->finePos.vz += entityData[player->movingPlatformEntityIdStandingOn + 121] - 512;

    player->svec54.vx = player->svec54.vy = player->svec54.vz = 0;
}
#undef i

static short UpdatePlayerSurroundingBlocks_startX;
static short UpdatePlayerSurroundingBlocks_startY;
static short UpdatePlayerSurroundingBlocks_startZ;
static short UpdatePlayerSurroundingBlocks_gridX;
static short UpdatePlayerSurroundingBlocks_gridY;
static short UpdatePlayerSurroundingBlocks_gridZ;
static short UpdatePlayerSurroundingBlocks_velSum;
static short UpdatePlayerSurroundingBlocks_i;
static short UpdatePlayerSurroundingBlocks_j;
static short UpdatePlayerSurroundingBlocks_k;
static SVECTOR UpdatePlayerSurroundingBlocks_blockCheckPos;
#define startX UpdatePlayerSurroundingBlocks_startX
#define startY UpdatePlayerSurroundingBlocks_startY
#define startZ UpdatePlayerSurroundingBlocks_startZ
#define gridX UpdatePlayerSurroundingBlocks_gridX
#define gridY UpdatePlayerSurroundingBlocks_gridY
#define gridZ UpdatePlayerSurroundingBlocks_gridZ
#define velSum UpdatePlayerSurroundingBlocks_velSum
#define i UpdatePlayerSurroundingBlocks_i
#define j UpdatePlayerSurroundingBlocks_j
#define k UpdatePlayerSurroundingBlocks_k
#define blockCheckPos UpdatePlayerSurroundingBlocks_blockCheckPos
void UpdatePlayerSurroundingBlocks(Player* player) {
    short (*grid)[8][8] = SHORT_ARRAY_ARRAY_ARRAY_000d4678;
    short gx, gy, gz;
    short rx, ry, rz;
    short vx, vy, vz;

    vx = (player->finePos.vx + 256) >> 9;
    blockCheckPos.vx = vx;
    vy = (player->finePos.vy + 256) >> 9;
    blockCheckPos.vy = vy;
    vz = (player->finePos.vz + 256) >> 9;
    blockCheckPos.vz = vz;

    gx = player->gravityDir.vx;
    rx = player->rightVec.vx;
    gx--;
    vx -= gx;
    startX = (rx + vx) - player->facingDir.vx;

    gy = player->gravityDir.vy;
    ry = player->rightVec.vy;
    gy--;
    vy -= gy;
    startY = (ry + vy) - player->facingDir.vy;

    gz = player->gravityDir.vz;
    rz = player->rightVec.vz;
    gz--;
    vz -= gz;
    startZ = (rz + vz) - player->facingDir.vz;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) {
                gridX = (startX + i * player->gravityDir.vx + j * player->facingDir.vx) - k * player->rightVec.vx;
                gridY = (startY + i * player->gravityDir.vy + j * player->facingDir.vy) - k * player->rightVec.vy;
                gridZ = (startZ + i * player->gravityDir.vz + j * player->facingDir.vz) - k * player->rightVec.vz;
                player->surroundingBlocks[i][j][k] = grid[gridX][gridY][gridZ];
                velSum = (short)player->surroundingBlocks[i][j][k];
            }
        }
    }

    blockCheckPos.vx = player->finePos.vx + player->svec54.vx;
    blockCheckPos.vy = player->finePos.vy + player->svec54.vy;
    blockCheckPos.vz = player->finePos.vz + player->svec54.vz;

    velSum = GetBlockAt(&blockCheckPos);
    if (velSum == -2) {
        player->surroundingBlocks[1][1][1] = -2;
    }

    {
        MovingPlatformEntity* mpe = (MovingPlatformEntity*)(player->movingPlatformEntityIdStandingOn * 2 + (int)entityData);
        velSum = mpe->velX + mpe->velY + mpe->velZ;
    }

    if (velSum != 0) {
        Vibrate99(0, 70, 2);
    }
}
#undef startX
#undef startY
#undef startZ
#undef gridX
#undef gridY
#undef gridZ
#undef velSum
#undef i
#undef j
#undef k
#undef blockCheckPos

int AreDirectionsOnSameAxis(int dir1, int dir2) {
    if ((dir1 == 1 || dir1 == 4) && (dir2 == 1 || dir2 == 4))
        return 1;
    if ((dir1 == 2 || dir1 == 3) && (dir2 == 2 || dir2 == 3))
        return 1;
    if ((dir1 == 0 || dir1 == 5) && (dir2 == 0 || dir2 == 5))
        return 1;
    return 0;
}

void EvaluateCubicBezier(VECTOR* p0, VECTOR* p1, VECTOR* p2, VECTOR* p3, VECTOR* out, short t, short shift) {
    int inv_t = (1 << (shift * 2 / 3)) - t;

    int inv_t2 = inv_t * inv_t;
    int inv_t3 = inv_t2 * inv_t;
    int t2 = t * t;
    int t3 = t2 * t;

    int inv_t_3 = inv_t * 3;
    int t_3 = t * 3;

    out->vx = (inv_t3 >> shift) * p0->vx;
    out->vx += ((t * inv_t_3 * inv_t) >> shift) * p1->vx;
    out->vx += ((t * t_3 * inv_t) >> shift) * p2->vx;
    out->vx += (t3 >> shift) * p3->vx;
    out->vx >>= shift;

    out->vy = (inv_t3 >> shift) * p0->vy;
    out->vy += ((t * inv_t_3 * inv_t) >> shift) * p1->vy;
    out->vy += ((t * t_3 * inv_t) >> shift) * p2->vy;
    out->vy += (t3 >> shift) * p3->vy;
    out->vy >>= shift;

    out->vz = (inv_t3 >> shift) * p0->vz;
    out->vz += ((t * inv_t_3 * inv_t) >> shift) * p1->vz;
    out->vz += ((t * t_3 * inv_t) >> shift) * p2->vz;
    out->vz += (t3 >> shift) * p3->vz;
    out->vz >>= shift;

    unusedBezierInvT = inv_t;
}

static int CalcLevelBounds_distSq;
static int CalcLevelBounds_xMin, CalcLevelBounds_xMax;
static int CalcLevelBounds_yMin, CalcLevelBounds_yMax;
static int CalcLevelBounds_zMin, CalcLevelBounds_zMax;
#define distSq CalcLevelBounds_distSq
#define xMin CalcLevelBounds_xMin
#define xMax CalcLevelBounds_xMax
#define yMin CalcLevelBounds_yMin
#define yMax CalcLevelBounds_yMax
#define zMin CalcLevelBounds_zMin
#define zMax CalcLevelBounds_zMax
void CalcLevelBounds(Player* player) {
    int x, y, z;

    player->playerHasControl = 0;
    HandlePlayerMovementStuff(player);

    initPlayerGravityDir.vx = player->gravityDir.vx << 12;
    initPlayerGravityDir.vy = player->gravityDir.vy << 12;
    initPlayerGravityDir.vz = player->gravityDir.vz << 12;

    initPlayerFacingDir.vx = player->facingDir.vx << 12;
    initPlayerFacingDir.vy = player->facingDir.vy << 12;
    initPlayerFacingDir.vz = player->facingDir.vz << 12;

    initPlayerRightDir.vx = player->rightVec.vx << 12;
    initPlayerRightDir.vy = player->rightVec.vy << 12;
    initPlayerRightDir.vz = player->rightVec.vz << 12;

    cameraR2TurnAmount = 0;
    levelEntryAnimTimer = 0;
    levelEntryAnimTimerIncrement = 16;

    xMin = yMin = zMin = 34;
    xMax = yMax = zMax = 0;

    for (x = 0; x < 34; x++) {
        for (y = 0; y < 34; y++) {
            for (z = 0; z < 34; z++) {
                if (levelData[x * 1156 + y * 34 + z] != -1) {
                    if (x < xMin)
                        xMin = x;
                    if (y < yMin)
                        yMin = y;
                    if (z < zMin)
                        zMin = z;
                    if (x > xMax)
                        xMax = x;
                    if (y > yMax)
                        yMax = y;
                    if (z > zMax)
                        zMax = z;
                }
            }
        }
    }

    xMinPlusMax = xMin - (-xMax);
    yMinPlusMax = yMin - (-yMax);
    zMinPlusMax = zMin - (-zMax);
    maxDistSquared = 0;

    for (x = 0; x < 34; x++) {
        for (y = 0; y < 34; y++) {
            for (z = 0; z < 34; z++) {
                if (levelData[x * 1156 + y * 34 + z] != -1) {
                    distSq = (x * 2 - xMinPlusMax) * (x * 2 - xMinPlusMax) +
                             (y * 2 - yMinPlusMax) * (y * 2 - yMinPlusMax) +
                             (z * 2 - zMinPlusMax) * (z * 2 - zMinPlusMax);
                    if (distSq > maxDistSquared) {
                        maxDistSquared = distSq;
                    }
                }
            }
        }
    }

    xMinPlusMax <<= 8;
    yMinPlusMax <<= 8;
    zMinPlusMax <<= 8;

    maxDistSquared = (SquareRoot0(maxDistSquared) << 8) + 1200;

    levelEntryBezierP3.vx = player->svec_184.vx + player->gravityDir.vx * 100;
    levelEntryBezierP3.vy = player->svec_184.vy + player->gravityDir.vy * 100;
    levelEntryBezierP3.vz = player->svec_184.vz + player->gravityDir.vz * 100;

    levelEntryBezierP0.vx = levelEntryBezierP3.vx - initPlayerFacingDir.vx - initPlayerRightDir.vx;
    levelEntryBezierP0.vy = levelEntryBezierP3.vy - initPlayerFacingDir.vy - initPlayerRightDir.vy;
    levelEntryBezierP0.vz = levelEntryBezierP3.vz - initPlayerFacingDir.vz - initPlayerRightDir.vz;

    levelEntryBezierP1.vx = levelEntryBezierP0.vx + (levelEntryBezierP3.vx - levelEntryBezierP0.vx) / 4 + initPlayerRightDir.vx / 3;
    levelEntryBezierP1.vy = levelEntryBezierP0.vy + (levelEntryBezierP3.vy - levelEntryBezierP0.vy) / 4 + initPlayerRightDir.vy / 3;
    levelEntryBezierP1.vz = levelEntryBezierP0.vz + (levelEntryBezierP3.vz - levelEntryBezierP0.vz) / 4 + initPlayerRightDir.vz / 3;

    levelEntryBezierP2.vx = levelEntryBezierP3.vx - initPlayerFacingDir.vx / 2;
    levelEntryBezierP2.vy = levelEntryBezierP3.vy - initPlayerFacingDir.vy / 2;
    levelEntryBezierP2.vz = levelEntryBezierP3.vz - initPlayerFacingDir.vz / 2;
}
#undef distSq
#undef xMin
#undef xMax
#undef yMin
#undef yMax
#undef zMin
#undef zMax

static VECTOR ProcessCameraAndMovement_facingGravityProd;
static VECTOR ProcessCameraAndMovement_coarseFacingDir;
#define facingGravityProd ProcessCameraAndMovement_facingGravityProd
#define coarseFacingDir ProcessCameraAndMovement_coarseFacingDir
void ProcessCameraAndMovement(Player* player) {
    player->playerHasControl = 0;
    if (levelEntryAnimTimer > 1024 || gameMode == 1) {
        if (player->debugCameraMode != 0) {
            HandleDebugCamera(player);
        } else {
            HandlePlayerMovementStuff(player);
        }
        return;
    }

    player->copycatMoveIndex = 0;
    EvaluateCubicBezier(&levelEntryBezierP0, &levelEntryBezierP1, &levelEntryBezierP2, &levelEntryBezierP3, &levelEntryCamPos, (short)levelEntryAnimTimer, 15);

    coarseFacingDir.vx = initPlayerFacingDir.vx / 1024;
    coarseFacingDir.vy = initPlayerFacingDir.vy / 1024;
    coarseFacingDir.vz = initPlayerFacingDir.vz / 1024;

    VectorNormal(&coarseFacingDir, &coarseFacingDir);
    OuterProduct12(&coarseFacingDir, &initPlayerGravityDir, &facingGravityProd);
    VectorNormal(&facingGravityProd, &facingGravityProd);

    perspMatrixes[cameraIndex].m[0][0] = (short)facingGravityProd.vx;
    perspMatrixes[cameraIndex].m[0][1] = (short)facingGravityProd.vy;
    perspMatrixes[cameraIndex].m[0][2] = (short)facingGravityProd.vz;
    perspMatrixes[cameraIndex].m[1][0] = -(short)initPlayerGravityDir.vx;
    perspMatrixes[cameraIndex].m[1][1] = -(short)initPlayerGravityDir.vy;
    perspMatrixes[cameraIndex].m[1][2] = -(short)initPlayerGravityDir.vz;
    perspMatrixes[cameraIndex].m[2][0] = (short)coarseFacingDir.vx;
    perspMatrixes[cameraIndex].m[2][1] = (short)coarseFacingDir.vy;
    perspMatrixes[cameraIndex].m[2][2] = (short)coarseFacingDir.vz;

    RotMatrixY(levelEntryAnimTimer - 1024, &perspMatrixes[cameraIndex]);
    RotMatrixX(250, &perspMatrixes[cameraIndex]);
    RotMatrixZ((1024 - levelEntryAnimTimer) * 2, &perspMatrixes[cameraIndex]);

    ApplyMatrixLV(&perspMatrixes[cameraIndex], &levelEntryCamPos, &levelEntryCamPos);

    perspMatrixes[cameraIndex].t[0] = -levelEntryCamPos.vx;
    perspMatrixes[cameraIndex].t[1] = -levelEntryCamPos.vy + 250;
    perspMatrixes[cameraIndex].t[2] = -levelEntryCamPos.vz + 800 + (1024 - levelEntryAnimTimer) * 14000 / 1024;

    player->movementInhibitTimer = 2000;

    if (levelEntryAnimTimer > 880) {
        levelEntryAnimTimer += levelEntryAnimTimerIncrement;
        levelEntryAnimTimerIncrement--;
        if (levelEntryAnimTimerIncrement <= 0) {
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
#undef facingGravityProd
#undef coarseFacingDir

static VECTOR HandleDebugCamera_transVec;
static short HandleDebugCamera_camX;
static short HandleDebugCamera_camY;
#define transVec HandleDebugCamera_transVec
#define camX HandleDebugCamera_camX
#define camY HandleDebugCamera_camY
void HandleDebugCamera(Player* player) {
    player->playerHasControl = 1;

    camY = player->debugCamY % 4096;
    do {} while(0); // HACK
    camX = player->debugCamX % 4096;

    RotMatrixZ(camX, &perspMatrixes[cameraIndex]);
    RotMatrixX(camY, &perspMatrixes[cameraIndex]);

    transVec.vx = -xMinPlusMax;
    transVec.vy = -yMinPlusMax;
    transVec.vz = -zMinPlusMax;

    ApplyMatrixLV(&perspMatrixes[cameraIndex], &transVec, &transVec);

    perspMatrixes[cameraIndex].t[0] = transVec.vx;
    perspMatrixes[cameraIndex].t[1] = transVec.vy;
    perspMatrixes[cameraIndex].t[2] = transVec.vz + maxDistSquared;
}
#undef transVec
#undef camX
#undef camY

static SVECTOR HandlePauseModeRotationEffect_rotVec;
static VECTOR HandlePauseModeRotationEffect_transVec;
#define rotVec HandlePauseModeRotationEffect_rotVec
#define transVec HandlePauseModeRotationEffect_transVec
void HandlePauseModeRotationEffect(Player* player) {
    player->playerHasControl = 0;

    rotVec.vx = (rotVec.vx - 10) % 4096;
    rotVec.vy = (rotVec.vy + 4) % 4096;
    rotVec.vz = (rotVec.vz + 13) % 4096;

    if (specialLevelType == 0) {
        zoomInAndOutPhase = (zoomInAndOutPhase + 15) % 4096;
    }
    if (specialLevelType == 1) {
        zoomInAndOutPhase = (zoomInAndOutPhase + 70) % 4096;
    }

    RotMatrix(&rotVec, &perspMatrixes[cameraIndex]);

    transVec.vx = -xMinPlusMax;
    transVec.vy = -yMinPlusMax;
    transVec.vz = -zMinPlusMax;

    ApplyMatrixLV(&perspMatrixes[cameraIndex], &transVec, &transVec);

    perspMatrixes[cameraIndex].t[0] = transVec.vx;
    perspMatrixes[cameraIndex].t[1] = transVec.vy;
    perspMatrixes[cameraIndex].t[2] = transVec.vz + maxDistSquared + 400;

    if (specialLevelType == 0) {
        perspMatrixes[cameraIndex].t[2] = transVec.vz + maxDistSquared + 1400 + ((rsin(zoomInAndOutPhase) * 1000) / 4096);
    }
    if (specialLevelType == 1) {
        perspMatrixes[cameraIndex].t[2] = transVec.vz + maxDistSquared + 3400 + ((rsin(zoomInAndOutPhase) * 3000) / 4096);
    }
}
#undef rotVec
#undef transVec

static MATRIX HandlePlayerMovementStuff_camRotMatrix;
static SVECTOR HandlePlayerMovementStuff_groundCheckPos;
static SVECTOR HandlePlayerMovementStuff_camRotVec;
static SVECTOR HandlePlayerMovementStuff_tempTurnVec;
static VECTOR HandlePlayerMovementStuff_camOffsetVec;
static VECTOR HandlePlayerMovementStuff_camTransVec;
static int HandlePlayerMovementStuff_blockType;
static int HandlePlayerMovementStuff_r1TurnFlag;
static int HandlePlayerMovementStuff_r2TurnFlag;
#define camRotMatrix HandlePlayerMovementStuff_camRotMatrix
#define groundCheckPos HandlePlayerMovementStuff_groundCheckPos
#define camRotVec HandlePlayerMovementStuff_camRotVec
#define tempTurnVec HandlePlayerMovementStuff_tempTurnVec
#define camOffsetVec HandlePlayerMovementStuff_camOffsetVec
#define camTransVec HandlePlayerMovementStuff_camTransVec
#define blockType HandlePlayerMovementStuff_blockType
#define r1TurnFlag HandlePlayerMovementStuff_r1TurnFlag
#define r2TurnFlag HandlePlayerMovementStuff_r2TurnFlag
void HandlePlayerMovementStuff(Player* player) {
    short turningTimer;

    player->playerHasControl = 1;

    if (player->startTurningTo == 1) {
        tempTurnVec = player->rightVec;
        player->facingDirBeforeStartingTurning = player->facingDir;
        player->rightVec.vx = -player->facingDir.vx;
        player->rightVec.vy = -player->facingDir.vy;
        player->rightVec.vz = -player->facingDir.vz;
        player->facingDir = tempTurnVec;
        player->turningWhere = 3;
        player->startTurningTo = -1;
    }

    if (player->startTurningTo == 2) {
        player->facingDirBeforeStartingTurning = player->facingDir;
        tempTurnVec.vx = -player->rightVec.vx;
        tempTurnVec.vy = -player->rightVec.vy;
        tempTurnVec.vz = -player->rightVec.vz;
        player->rightVec = player->facingDir;
        player->facingDir = tempTurnVec;
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
        tempTurnVec = player->perspVec2;
        player->gravityDir.vx = -player->facingDir.vx;
        player->gravityDir.vy = -player->facingDir.vy;
        player->gravityDir.vz = -player->facingDir.vz;
        player->facingDir = tempTurnVec;
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

    camOffsetVec.vx = player->svec_184.vx;
    camOffsetVec.vy = player->svec_184.vy;
    camOffsetVec.vz = player->svec_184.vz;

    r2TurnFlag = 0;
    r1TurnFlag = 0;
    if (player->cameraR1R2TurnDirection.vx == -1) {
        if (player->howMoving198 != FALLING && cameraR2TurnAmount == 0) {
            r2TurnFlag = 1;
            player->cameraR1TurnAmount.vx += 26;
            if (player->cameraR1TurnAmount.vx > 650)
                player->cameraR1TurnAmount.vx = 650;
        }
    }

    if (player->cameraR1R2TurnDirection.vx == 1 && player->cameraR1TurnAmount.vx == 0) {
        r1TurnFlag = 1;
        cameraR2TurnAmount += 26;
        if (cameraR2TurnAmount > 650)
            cameraR2TurnAmount = 650;
    }

    groundCheckPos.vx = player->finePos.vx - (player->gravityDir.vx * 512);
    groundCheckPos.vy = player->finePos.vy - (player->gravityDir.vy * 512);
    groundCheckPos.vz = player->finePos.vz - (player->gravityDir.vz * 512);

    blockType = GetBlockAt(&groundCheckPos);
    blockType = player->surroundingBlocks[0][1][1];

    if (player->howMoving198 == FALLING || (player->howMoving198 == JUMPING_FORWARD && player->jumpingOrViewportRotationTimer < 10)) {
        if (blockType == -1) {
            if (player->cameraR1TurnAmount.vx < 550) {
                if (cameraR2TurnAmount == 0) {
                    r2TurnFlag = 1;
                    r1TurnDelta++;
                    if (r1TurnDelta > 32)
                        r1TurnDelta = 32;
                    player->cameraR1TurnAmount.vx += r1TurnDelta;
                }
            } else {
                r2TurnFlag = 1;
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

    if (r1TurnFlag == 0 && cameraR2TurnAmount > 0) {
        cameraR2TurnAmount -= 18;
        if (cameraR2TurnAmount < 0)
            cameraR2TurnAmount = 0;
    }

    if (r2TurnFlag == 0) {
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

    if (cameraR2TurnAmount != 0) {
        RotMatrixX(250, &perspMatrixes[cameraIndex]);
        RotMatrixX(-cameraR2TurnAmount, &perspMatrixes[cameraIndex]);
        ApplyMatrixLV(&perspMatrixes[cameraIndex], &camOffsetVec, &camOffsetVec);
        camTransVec.vx = 0;
        camRotVec.vy = 0;
        camRotVec.vz = 0;
        camTransVec.vz = 800 - cameraR2TurnAmount / 2;
        camRotVec.vx = -cameraR2TurnAmount;
        camTransVec.vy = 250 - cameraR2TurnAmount / 3;
        RotMatrix(&camRotVec, &camRotMatrix);
        ApplyMatrixLV(&camRotMatrix, &camTransVec, &camTransVec);
        perspMatrixes[cameraIndex].t[0] = -camOffsetVec.vx + camTransVec.vx;
        perspMatrixes[cameraIndex].t[1] = -camOffsetVec.vy + camTransVec.vy;
        perspMatrixes[cameraIndex].t[2] = -camOffsetVec.vz + camTransVec.vz;
    } else {
        RotMatrixX(player->cameraR1TurnAmount.vx + 250, &perspMatrixes[cameraIndex]);
        ApplyMatrixLV(&perspMatrixes[cameraIndex], &camOffsetVec, &camOffsetVec);
        perspMatrixes[cameraIndex].t[0] = -camOffsetVec.vx;
        perspMatrixes[cameraIndex].t[1] = -camOffsetVec.vy + 250;
        perspMatrixes[cameraIndex].t[2] = -camOffsetVec.vz + 800;
    }
}
#undef camRotMatrix
#undef groundCheckPos
#undef camRotVec
#undef tempTurnVec
#undef camOffsetVec
#undef camTransVec
#undef blockType
#undef r1TurnFlag
#undef r2TurnFlag

void SetInvulnerable(void) {
    thePlayer.invulnerabilityTimer = 3000;
}
