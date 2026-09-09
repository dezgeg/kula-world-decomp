#include "common.h"

typedef struct EntityPos {
    MATRIX matrix;
    short posX;
    short posY;
    short posZ;
    short pad;
    byte reserved[8];
} EntityPos;

typedef struct ItemState {
    int collisionDistance;
    int type;
    MATRIX matrix;
    SVECTOR pos;
} ItemState;

int IsFallingOrJumping(Player* player);
int IsPlayerInAir(Player* player);
void HandleSpecialCubeTypes(Player* player);

extern int AddLightEffect(int x, int y, int z, int whichSide);
extern void AddParticles(int type, SVECTOR* pos, int lightEffectId);
extern void CalcWhatPlayerIsStandingOn(Player* player);
extern void CreateItemDispList(MATRIX* m, int z, int specialBlockType, int dirIndexInBlock);
extern void DisableItemShadow(int entityIndex, int side, int const0);
extern void EnableLethargy(int enable);
extern void SetCubeVisited(int x, int y, int z, int visitType);
extern void SetPlayerRotation(int cubeSide, int rotation, Player* player);
extern void SetRenderScreenFade(int speed, int enableHalfFps);
extern void SetSunglassMode(int on);
extern void SndMuteVoiceByTag(int tag);
extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);
extern void UpdateSubpixelPositions(Player* player);
extern void Vibrate100(int constant, int amplitude, int arrowIncrement, int max);
extern void Vibrate101(int param_1);
extern void Vibrate98(int seqId);
extern void Vibrate99(int magnitude1, int magnitude2, int count);

extern int cameraIndex;
extern int debugDisableTimer;
extern int drawTimerPausedWidget;
extern int hourglassIsRotating;
extern int KEY_SPRITE_POSITIONS[];
extern int KEY_SPRITE_POSITIONS2[];
extern int levelEndReason;
extern int levelPlayTime[2];
extern int levelScore;
extern int levelTimeLeft;
extern int numCubesRemainingInLevel[5];
extern int numKeysInLevel;
extern int numKeysRemaining;
extern int numTimeTrialPlayers;
extern int twoPlayerWhichPlayer;
extern ItemState itemState[256];
extern MATRIX MATRIX_000a46f4;
extern MATRIX perspMatrixes[2];
extern Player thePlayer;
extern short* entityData;
extern short numEntities;
extern SVECTOR fruit1ScreenSpaceParticlesPos;
extern SVECTOR fruit2ScreenSpaceParticlesPos;
extern SVECTOR fruit3ScreenSpaceParticlesPos;
extern SVECTOR fruit4ScreenSpaceParticlesPos;
extern SVECTOR fruit5ScreenSpaceParticlesPos;
extern SVECTOR SVECTOR_000a2df4;
extern SVECTOR SVECTOR_000a2dfc;
extern SVECTOR transporterParticlesPos;
extern uint fruitsCollectedBitmask;

int gameMode;
STATIC_FOR_GP_ACCESS int levelExitEntityOffset;
STATIC_FOR_GP_ACCESS int levelHiddenExitEntityOffset;
int shouldMarkCubesVisited;
STATIC_FOR_GP_ACCESS short fireSoundTimer;

STATIC_FOR_GP_ACCESS int levelWon[2];
static SVECTOR unusedLevelUpdate3Vec;

static int HandleItemTouching_distSq;
static int HandleItemTouching_dontDisableShadow;
static int HandleItemTouching_entityBlockOffset;
static int HandleItemTouching_i;
static int HandleItemTouching_iter;
static int HandleItemTouching_j;
static int HandleItemTouching_itemOffset;
static int HandleItemTouching_playerRadius;
static int HandleItemTouching_itemIdx;
static short HandleItemTouching_cubeX;
static short HandleItemTouching_cubeY;
static short HandleItemTouching_cubeZ;
static short HandleItemTouching_side;
static SVECTOR HandleItemTouching_keyParticlesPos;
static SVECTOR HandleItemTouching_playerPos;
#define distSq HandleItemTouching_distSq
#define dontDisableShadow HandleItemTouching_dontDisableShadow
#define entityBlockOffset HandleItemTouching_entityBlockOffset
#define i HandleItemTouching_i
#define iter HandleItemTouching_iter
#define j HandleItemTouching_j
#define itemOffset HandleItemTouching_itemOffset
#define playerRadius HandleItemTouching_playerRadius
#define itemIdx HandleItemTouching_itemIdx
#define cubeX HandleItemTouching_cubeX
#define cubeY HandleItemTouching_cubeY
#define cubeZ HandleItemTouching_cubeZ
#define side HandleItemTouching_side
#define keyParticlesPos HandleItemTouching_keyParticlesPos
#define playerPos HandleItemTouching_playerPos
void HandleItemTouching(Player* player) {
    int* ptr;

    playerPos = player->finePos;
    playerRadius = 10000;

    for (iter = 0; iter < 3; iter++) {
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                entityBlockOffset = (player->surroundingBlocks[iter][i][j] - 5) * 128;
                if (entityBlockOffset < 0) {
                    continue;
                }

                if (entityData[entityBlockOffset] >= 5) {
                    continue;
                }

                for (itemOffset = 0; itemOffset < 96; itemOffset += 16) {
                    itemIdx = entityData[entityBlockOffset + itemOffset + 5];

                    if (entityData[entityBlockOffset + itemOffset + 4] == 0 || itemState[itemIdx].type == 0) {
                        continue;
                    }

                    distSq = (playerPos.vx - itemState[itemIdx].pos.vx) * (playerPos.vx - itemState[itemIdx].pos.vx) +
                                   (playerPos.vy - itemState[itemIdx].pos.vy) * (playerPos.vy - itemState[itemIdx].pos.vy) +
                                   (playerPos.vz - itemState[itemIdx].pos.vz) * (playerPos.vz - itemState[itemIdx].pos.vz);

                    if (distSq >= itemState[itemIdx].collisionDistance + playerRadius) {
                        continue;
                    }
                    cubeX = entityData[entityBlockOffset + 125];
                    cubeY = entityData[entityBlockOffset + 126];
                    cubeZ = entityData[entityBlockOffset + 127];
                    side = itemOffset / 16;
                    dontDisableShadow = 0;

                    switch (entityData[entityBlockOffset + itemOffset + 1]) {
                        case OBJ_SPIKE_TRAP:
                            if (entityData[entityBlockOffset + itemOffset + 3] == 0) {
                                if (thePlayer.invulnerabilityTimer == -1 && thePlayer.movementInhibitTimer == 0) {
                                    thePlayer.delayedLevelEndReason = LEVEL_END_CAPTURED;
                                    thePlayer.dying = 1;
                                    thePlayer.movementVelocity = 0;
                                    thePlayer.rotX = 0;
                                    thePlayer.movementInhibitTimer = 10;
                                    thePlayer.ballBlinking = 1;
                                }
                                dontDisableShadow = 1;
                            } else {
                                AddParticles(7, &itemState[itemIdx].pos, AddLightEffect(cubeX, cubeY, cubeZ, side));
                                levelScore += 1550;
                            }
                            break;

                        case OBJ_LETHARGY_PILL:
                            AddParticles(9, &itemState[itemIdx].pos, AddLightEffect(cubeX, cubeY, cubeZ, side));
                            thePlayer.lethargyTimer = 300;
                            EnableLethargy(1);
                            Vibrate100(120, 120, 200, 1);
                            Vibrate101(200);
                            SndPlaySfx(SFX_LETHARGY_PILL, 0, &SVECTOR_000a2df4, 7000);
                            break;

                        case OBJ_INVINCIBILITY_PILL:
                            AddParticles(9, &itemState[itemIdx].pos, AddLightEffect(cubeX, cubeY, cubeZ, side));
                            thePlayer.invulnerabilityTimer = 700;
                            SndPlaySfx(SFX_LETHARGY_PILL, 0, &SVECTOR_000a2df4, 7000);
                            break;

                        case OBJ_BOUNCY_PILL:
                            AddParticles(9, &itemState[itemIdx].pos, AddLightEffect(cubeX, cubeY, cubeZ, side));
                            thePlayer.bounceTimer = 700;
                            SndPlaySfx(SFX_BOUNCE_PILL, 0, &SVECTOR_000a2df4, 7000);
                            break;

                        case OBJ_SUNGLASSES:
                            SndPlaySfx(SFX_SUNGLASSES_COLLECTION, 0, &SVECTOR_000a2df4, 7000);
                            AddParticles(8, &itemState[itemIdx].pos, AddLightEffect(cubeX, cubeY, cubeZ, side));
                            thePlayer.sunglassTimer = 700;
                            levelScore += 500;
                            SetSunglassMode(1);
                            break;

                        case OBJ_KEY:
                            ptr = (int*)((char*)KEY_SPRITE_POSITIONS - 0x20);
                            if (gameMode == 2 && numTimeTrialPlayers == 2) {
                                ptr = (int*)((char*)KEY_SPRITE_POSITIONS2 - 0x20);
                            }

                            {
                                int* p = (int*)(numKeysInLevel * 32 + (int)ptr);
                                keyParticlesPos.vx = *(int*)((char*)p + (numKeysInLevel - numKeysRemaining) * 8) + 6;
                                keyParticlesPos.vy = *(int*)((char*)p + (numKeysInLevel - numKeysRemaining) * 8 + 4) + 11;
                            }
                            asm volatile("");
                            keyParticlesPos.vz = -1;

                            AddParticles(0, &keyParticlesPos, 0);
                            AddParticles(6, &itemState[itemIdx].pos, AddLightEffect(cubeX, cubeY, cubeZ, side));
                            numKeysRemaining--;
                            levelScore += 1000;
                            if (numKeysRemaining == 0) {
                                entityData[levelExitEntityOffset + 3] = 0;
                                entityData[levelExitEntityOffset + 4] = 1;
                                entityData[levelHiddenExitEntityOffset + 4] = 1;
                                SndPlaySfx(SFX_LAST_KEY_COLLECTION, 0, &SVECTOR_000a2df4, 7000);
                            } else {
                                SndPlaySfx(SFX_KEY_COLLECTION, 0, &SVECTOR_000a2df4, 7000);
                            }
                            break;

                        case OBJ_GEM:
                            AddParticles(7, &itemState[itemIdx].pos, AddLightEffect(cubeX, cubeY, cubeZ, side));
                            levelScore += 2975;
                            SndPlaySfx(SFX_GEM_COLLECTION, 0, &SVECTOR_000a2df4, 7000);
                            break;

                        case OBJ_COIN:
                            AddParticles(2, &itemState[itemIdx].pos, AddLightEffect(cubeX, cubeY, cubeZ, side));
                            SndPlaySfx(SFX_COIN_COLLECTION, 0, &SVECTOR_000a2df4, 7000);
                            if (entityData[entityBlockOffset + itemOffset + 3] == 0) {
                                levelScore += 750;
                            }
                            if (entityData[entityBlockOffset + itemOffset + 3] == 1) {
                                levelScore += 500;
                            }
                            if (entityData[entityBlockOffset + itemOffset + 3] == 2) {
                                levelScore += 250;
                            }
                            break;

                        case OBJ_APPLE:
                            if (gameMode != 2) {
                                AddParticles(0, &fruit1ScreenSpaceParticlesPos, 0);
                            }
                            AddParticles(4, &itemState[itemIdx].pos, AddLightEffect(cubeX, cubeY, cubeZ, side));
                            SndPlaySfx(SFX_FRUIT_1, 0, &SVECTOR_000a2df4, 7000);
                            levelScore += 2500;
                            fruitsCollectedBitmask |= 1;
                            levelPlayTime[twoPlayerWhichPlayer] -= 200;
                            break;

                        case OBJ_WATERMELON:
                            if (gameMode != 2) {
                                AddParticles(0, &fruit2ScreenSpaceParticlesPos, 0);
                            }
                            AddParticles(4, &itemState[itemIdx].pos, AddLightEffect(cubeX, cubeY, cubeZ, side));
                            SndPlaySfx(SFX_FRUIT_2, 0, &SVECTOR_000a2df4, 7000);
                            levelScore += 2500;
                            fruitsCollectedBitmask |= 2;
                            levelPlayTime[twoPlayerWhichPlayer] -= 200;
                            break;

                        case OBJ_PUMPKIN:
                            if (gameMode != 2) {
                                AddParticles(0, &fruit3ScreenSpaceParticlesPos, 0);
                            }
                            AddParticles(4, &itemState[itemIdx].pos, AddLightEffect(cubeX, cubeY, cubeZ, side));
                            SndPlaySfx(SFX_FRUIT_3, 0, &SVECTOR_000a2df4, 7000);
                            levelScore += 2500;
                            fruitsCollectedBitmask |= 4;
                            levelPlayTime[twoPlayerWhichPlayer] -= 200;
                            break;

                        case OBJ_BANANA:
                            if (gameMode != 2) {
                                AddParticles(0, &fruit4ScreenSpaceParticlesPos, 0);
                            }
                            AddParticles(4, &itemState[itemIdx].pos, AddLightEffect(cubeX, cubeY, cubeZ, side));
                            SndPlaySfx(SFX_FRUIT_4, 0, &SVECTOR_000a2df4, 7000);
                            levelScore += 2500;
                            fruitsCollectedBitmask |= 8;
                            levelPlayTime[twoPlayerWhichPlayer] -= 200;
                            break;

                        case OBJ_STRAWBERRY:
                            if (gameMode != 2) {
                                AddParticles(0, &fruit5ScreenSpaceParticlesPos, 0);
                            }
                            AddParticles(4, &itemState[itemIdx].pos, AddLightEffect(cubeX, cubeY, cubeZ, side));
                            SndPlaySfx(SFX_FRUIT_5, 0, &SVECTOR_000a2df4, 7000);
                            levelScore += 2500;
                            fruitsCollectedBitmask |= 0x10;
                            levelPlayTime[twoPlayerWhichPlayer] -= 200;
                            break;

                        case OBJ_FRUIT_BOWL:
                            AddParticles(0, &fruit1ScreenSpaceParticlesPos, 0);
                            AddParticles(0, &fruit2ScreenSpaceParticlesPos, 0);
                            AddParticles(0, &fruit3ScreenSpaceParticlesPos, 0);
                            AddParticles(0, &fruit4ScreenSpaceParticlesPos, 0);
                            AddParticles(0, &fruit5ScreenSpaceParticlesPos, 0);
                            AddParticles(4, &itemState[itemIdx].pos, AddLightEffect(cubeX, cubeY, cubeZ, side));
                            SndPlaySfx(SFX_FRUIT_5, 0, &SVECTOR_000a2df4, 7000);
                            levelScore += 2500;
                            fruitsCollectedBitmask |= 0x1f;
                            break;

                        case OBJ_HOURGLASS:
                            SndPlaySfx(SFX_HOURGLASS, 0, &SVECTOR_000a2df4, 7000);
                            levelTimeLeft = 4999 - levelTimeLeft;
                            hourglassIsRotating = 1;
                            levelScore += (levelTimeLeft / 50) * 10;
                            AddParticles(1, &SVECTOR_000a2dfc, 0);
                            AddParticles(10, &itemState[itemIdx].pos, AddLightEffect(cubeX, cubeY, cubeZ, side));
                            break;
                    }

                    if (!dontDisableShadow) {
                        entityData[entityBlockOffset + itemOffset + 4] = 0;
                        entityData[entityBlockOffset + itemOffset + 1] = 0;
                        DisableItemShadow(entityBlockOffset / 128, itemOffset / 16, 0);
                    } else {
                        dontDisableShadow = 0;
                    }
                }
            }
        }
    }
}
#undef distSq
#undef dontDisableShadow
#undef entityBlockOffset
#undef i
#undef iter
#undef j
#undef itemOffset
#undef playerRadius
#undef itemIdx
#undef cubeX
#undef cubeY
#undef cubeZ
#undef side
#undef keyParticlesPos
#undef playerPos

static int CreateAllItemDispLists_entityOffset;
static int CreateAllItemDispLists_itemOffset;
static int CreateAllItemDispLists_distSq;
static int CreateAllItemDispLists_entityDir;
static int CreateAllItemDispLists_itemIdx;
static MATRIX CreateAllItemDispLists_drawMatrix;
static MATRIX CreateAllItemDispLists_rotMatrix;
static SVECTOR CreateAllItemDispLists_calcPos;
static SVECTOR CreateAllItemDispLists_rotVec;
static SVECTOR CreateAllItemDispLists_camPos;
static SVECTOR CreateAllItemDispLists_offsetVec;
#define entityOffset CreateAllItemDispLists_entityOffset
#define itemOffset CreateAllItemDispLists_itemOffset
#define distSq CreateAllItemDispLists_distSq
#define entityDir CreateAllItemDispLists_entityDir
#define itemIdx CreateAllItemDispLists_itemIdx
#define drawMatrix CreateAllItemDispLists_drawMatrix
#define rotMatrix CreateAllItemDispLists_rotMatrix
#define calcPos CreateAllItemDispLists_calcPos
#define rotVec CreateAllItemDispLists_rotVec
#define camPos CreateAllItemDispLists_camPos
#define offsetVec CreateAllItemDispLists_offsetVec
void CreateAllItemDispLists(void) {
    for (entityOffset = 0; entityOffset < numEntities * 128; entityOffset += 128) {
        if (entityData[entityOffset] >= 5) {
            continue;
        }
        for (itemOffset = 0; itemOffset < 96; itemOffset += 16) {
            if (entityData[entityOffset + itemOffset + 4] == 0) {
                continue;
            }
            entityDir = entityData[entityOffset + itemOffset + 2];
            calcPos.vx = entityData[entityOffset + 125] * 512;
            calcPos.vy = entityData[entityOffset + 126] * 512;
            calcPos.vz = entityData[entityOffset + 127] * 512;
            offsetVec.vx = offsetVec.vy = offsetVec.vz = 0;

            switch (entityData[entityOffset + itemOffset + 1]) {
                case OBJ_SPIKE_TRAP:
                    entityData[entityOffset + itemOffset + 11] = (entityData[entityOffset + itemOffset + 11] + 16) % 4096;
                    if (entityData[entityOffset + itemOffset + 3] == 0) {
                        entityData[entityOffset + itemOffset + 13] = (entityData[entityOffset + itemOffset + 13] + 100) % 4096;
                    }
                    if (entityData[entityOffset + itemOffset + 3] == 1) {
                        entityData[entityOffset + itemOffset + 13] = (entityData[entityOffset + itemOffset + 13] + 8) % 4096;
                    }

                    if (entityData[entityOffset + itemOffset + 11] > 2048) {
                        entityData[entityOffset + itemOffset + 3] = 1;
                    } else {
                        entityData[entityOffset + itemOffset + 3] = 0;
                    }

                    if (entityData[entityOffset + itemOffset + 3] == 1) {
                        entityData[entityOffset + itemOffset + 12] = (entityData[entityOffset + itemOffset + 12] + (entityData[entityOffset + itemOffset + 11] % 2048) / 2) % 4096;
                    }
                    offsetVec.vz = (rsin(entityData[entityOffset + itemOffset + 12]) * 0) / 4096; // guess
                    offsetVec.vx = (rsin(entityData[entityOffset + itemOffset + 13]) * 100) / 4096;
                    offsetVec.vy = (rcos(entityData[entityOffset + itemOffset + 13]) * 100) / 4096;
                    rotVec.vy = rotVec.vx = 0;
                    rotVec.vz = 1024 - entityData[entityOffset + itemOffset + 13];
                    RotMatrixZYX(&rotVec, &rotMatrix);
                    break;

                case OBJ_BUTTON:
                case OBJ_MOVING_SPIKE:
                case OBJ_SPIKE:
                case OBJ_ARROW:
                    rotVec.vx = rotVec.vy = rotVec.vz = 0;
                    RotMatrix(&rotVec, &rotMatrix);
                    break;

                case OBJ_TRANSPORTER:
                    if (entityData[entityOffset + itemOffset + 4] == 1 && cameraIndex == 0) {
                        entityData[entityOffset + itemOffset + 11] = (entityData[entityOffset + itemOffset + 11] + 30) % 4096;
                    }
                    rotVec.vx = rotVec.vy = 0;
                    rotVec.vz = entityData[entityOffset + itemOffset + 11];
                    RotMatrix(&rotVec, &rotMatrix);
                    break;

                case OBJ_EXIT:
                case OBJ_HIDDEN_EXIT:
                    if (entityData[entityOffset + itemOffset + 4] == 1) {
                        entityData[entityOffset + itemOffset + 11] = (entityData[entityOffset + itemOffset + 11] - 55) % 4096;
                    } else {
                        entityData[entityOffset + itemOffset + 11] = (entityData[entityOffset + itemOffset + 11] - 25) % 4096;
                    }
                    rotVec.vx = rotVec.vy = 0;
                    rotVec.vz = entityData[entityOffset + itemOffset + 11];
                    RotMatrix(&rotVec, &rotMatrix);
                    break;

                case OBJ_APPLE:
                case OBJ_WATERMELON:
                case OBJ_PUMPKIN:
                case OBJ_BANANA:
                case OBJ_STRAWBERRY:
                    entityData[entityOffset + itemOffset + 11] = (entityData[entityOffset + itemOffset + 11] + 24) % 4096;
                    entityData[entityOffset + itemOffset + 12] = (entityData[entityOffset + itemOffset + 12] - 80) % 4096;
                    entityData[entityOffset + itemOffset + 13] = (entityData[entityOffset + itemOffset + 13] - 60) % 4096;
                    offsetVec.vz = (rsin(entityData[entityOffset + itemOffset + 13]) * 20) / 4096 + 10;
                    rotVec.vy = 0;
                    rotVec.vx = (rsin(entityData[entityOffset + itemOffset + 12]) * 150) / 4096;
                    rotVec.vz = entityData[entityOffset + itemOffset + 11];
                    RotMatrixZYX(&rotVec, &rotMatrix);
                    break;

                case OBJ_KEY:
                case OBJ_COIN:
                case OBJ_SUNGLASSES:
                    entityData[entityOffset + itemOffset + 11] = (entityData[entityOffset + itemOffset + 11] - 90) % 4096;
                    entityData[entityOffset + itemOffset + 13] = (entityData[entityOffset + itemOffset + 13] - 70) % 4096;
                    offsetVec.vz = (rsin(entityData[entityOffset + itemOffset + 13]) * 20) / 4096 + 10;
                    rotVec.vy = 0;
                    rotVec.vx = 0;
                    rotVec.vz = entityData[entityOffset + itemOffset + 11];
                    RotMatrixZYX(&rotVec, &rotMatrix);
                    break;

                case OBJ_GEM:
                    entityData[entityOffset + itemOffset + 11] = (entityData[entityOffset + itemOffset + 11] + 35) % 4096;
                    rotVec.vy = 0;
                    rotVec.vx = 0;
                    rotVec.vz = entityData[entityOffset + itemOffset + 11];
                    RotMatrix(&rotVec, &rotMatrix);
                    break;

                case OBJ_HOURGLASS:
                    entityData[entityOffset + itemOffset + 11] = (entityData[entityOffset + itemOffset + 11] + 32) % 4096;
                    entityData[entityOffset + itemOffset + 12] = (entityData[entityOffset + itemOffset + 12] - 20) % 4096;
                    rotVec.vy = (rsin(entityData[entityOffset + itemOffset + 11]) / 2) + 30;
                    rotVec.vx = 0;
                    rotVec.vz = entityData[entityOffset + itemOffset + 12];
                    RotMatrixZYX(&rotVec, &rotMatrix);
                    break;

                case OBJ_LETHARGY_PILL:
                case OBJ_BOUNCY_PILL:
                case OBJ_INVINCIBILITY_PILL:
                    entityData[entityOffset + itemOffset + 11] = (entityData[entityOffset + itemOffset + 11] + 80) % 4096;
                    entityData[entityOffset + itemOffset + 12] = (entityData[entityOffset + itemOffset + 12] + 25) % 4096;
                    rotVec.vy = entityData[entityOffset + itemOffset + 11];
                    rotVec.vz = entityData[entityOffset + itemOffset + 12];
                    rotVec.vx = 0;
                    RotMatrixZYX(&rotVec, &rotMatrix);
                    break;

                case OBJ_PURPLE_PRESENT:
                case OBJ_RED_PRESENT:
                case OBJ_YELLOW_PRESENT:
                case OBJ_BLUE_PRESENT:
                case OBJ_GREEN_PRESENT:
                    entityData[entityOffset + itemOffset + 11] = (entityData[entityOffset + itemOffset + 11] - 17) % 4096;
                    rotVec.vx = rotVec.vy = 0;
                    rotVec.vz = entityData[entityOffset + itemOffset + 11];
                    RotMatrix(&rotVec, &rotMatrix);
                    break;

                case OBJ_BOUNCEPAD:
                    rotVec.vz = 0;
                    rotVec.vy = 0;
                    rotVec.vx = 0;
                    RotMatrix(&rotVec, &rotMatrix);
                    distSq = (calcPos.vx - thePlayer.finePos.vx) * (calcPos.vx - thePlayer.finePos.vx) +
                                                (calcPos.vy - thePlayer.finePos.vy) * (calcPos.vy - thePlayer.finePos.vy) +
                                                (calcPos.vz - thePlayer.finePos.vz) * (calcPos.vz - thePlayer.finePos.vz);

                    if (distSq < 490000) {
                        if (entityData[entityOffset + itemOffset + 11] > 0)
                            entityData[entityOffset + itemOffset + 11]--;
                        rotMatrix.m[2][2] = (rotMatrix.m[2][2] * entityData[entityOffset + itemOffset + 11]) / 16;
                    } else if (entityData[entityOffset + itemOffset + 11] < 16) {
                        entityData[entityOffset + itemOffset + 11] += 4;
                        rotMatrix.m[2][2] = (rotMatrix.m[2][2] * entityData[entityOffset + itemOffset + 11]) / 16;
                    }
                    break;
            }

            itemIdx = entityData[entityOffset + itemOffset + 5];
            ApplyMatrixSV(&itemState[itemIdx].matrix, &offsetVec, &offsetVec);
            calcPos.vx = itemState[itemIdx].pos.vx = offsetVec.vx + itemState[itemIdx].matrix.t[0];
            calcPos.vy = itemState[itemIdx].pos.vy = offsetVec.vy + itemState[itemIdx].matrix.t[1];
            calcPos.vz = itemState[itemIdx].pos.vz = offsetVec.vz + itemState[itemIdx].matrix.t[2];
            MulMatrix0(&itemState[itemIdx].matrix, &rotMatrix, &MATRIX_000a46f4);
            MulMatrix0(&perspMatrixes[cameraIndex], &MATRIX_000a46f4, &drawMatrix);
            ApplyMatrixSV(&perspMatrixes[cameraIndex], &calcPos, &camPos);
            drawMatrix.t[0] = camPos.vx + perspMatrixes[cameraIndex].t[0];
            drawMatrix.t[1] = camPos.vy + perspMatrixes[cameraIndex].t[1];
            drawMatrix.t[2] = camPos.vz + perspMatrixes[cameraIndex].t[2];
            CreateItemDispList(&drawMatrix, drawMatrix.t[2], entityOffset / 128, itemOffset / 16);
        }
    }
}
#undef entityOffset
#undef itemOffset
#undef distSq
#undef entityDir
#undef itemIdx
#undef drawMatrix
#undef rotMatrix
#undef calcPos
#undef rotVec
#undef camPos
#undef offsetVec

static SVECTOR SetEntityRotation_tmpVec;
#define tmpVec SetEntityRotation_tmpVec
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
        tmpVec.vx = pos->matrix.m[0][0];
        tmpVec.vy = pos->matrix.m[1][0];
        tmpVec.vz = pos->matrix.m[2][0];

        pos->matrix.m[0][0] = -pos->matrix.m[0][1];
        pos->matrix.m[1][0] = -pos->matrix.m[1][1];
        pos->matrix.m[2][0] = -pos->matrix.m[2][1];

        pos->matrix.m[0][1] = tmpVec.vx;
        pos->matrix.m[1][1] = tmpVec.vy;
        pos->matrix.m[2][1] = tmpVec.vz;
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
        tmpVec.vx = pos->matrix.m[0][0];
        tmpVec.vy = pos->matrix.m[1][0];
        tmpVec.vz = pos->matrix.m[2][0];

        pos->matrix.m[0][0] = pos->matrix.m[0][1];
        pos->matrix.m[1][0] = pos->matrix.m[1][1];
        pos->matrix.m[2][0] = pos->matrix.m[2][1];

        pos->matrix.m[0][1] = -tmpVec.vx;
        pos->matrix.m[1][1] = -tmpVec.vy;
        pos->matrix.m[2][1] = -tmpVec.vz;
    }
}
#undef tmpVec

static SVECTOR MatrixFromDirectionIndex_tmpVec;
#define tmpVec MatrixFromDirectionIndex_tmpVec
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
        tmpVec.vx = m->m[0][0];
        tmpVec.vy = m->m[1][0];
        tmpVec.vz = m->m[2][0];

        m->m[0][0] = -m->m[0][1];
        m->m[1][0] = -m->m[1][1];
        m->m[2][0] = -m->m[2][1];

        m->m[0][1] = tmpVec.vx;
        m->m[1][1] = tmpVec.vy;
        m->m[2][1] = tmpVec.vz;
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
        tmpVec.vx = m->m[0][0];
        tmpVec.vy = m->m[1][0];
        tmpVec.vz = m->m[2][0];

        m->m[0][0] = m->m[0][1];
        m->m[1][0] = m->m[1][1];
        m->m[2][0] = m->m[2][1];

        m->m[0][1] = -tmpVec.vx;
        m->m[1][1] = -tmpVec.vy;
        m->m[2][1] = -tmpVec.vz;
    }
}
#undef tmpVec

static int CheckForButtonEntity_entityOffset;
static int CheckForButtonEntity_targetEntity;
static int CheckForButtonEntity_targetSide;
static int CheckForButtonEntity_link;
#define entityOffset CheckForButtonEntity_entityOffset
#define targetEntity CheckForButtonEntity_targetEntity
#define targetSide CheckForButtonEntity_targetSide
#define link CheckForButtonEntity_link
void CheckForButtonEntity(Player* player) {
    short* ptr;
    unsigned short* ptr0;
    unsigned short* ptr2;
    if (IsFallingOrJumping(player)) {
        player->alreadyProcessedEntityAction = 0;
        return;
    }

    if (player->alreadyProcessedEntityAction != OBJ_BUTTON && player->faceTypePlayerStandingOn == OBJ_BUTTON &&
        (u16)player->subpixelPositionOnCube.vz - 197U < 119U &&
        (u16)player->subpixelPositionOnCube.vx - 197U < 119U) {
        player->alreadyProcessedEntityAction = OBJ_BUTTON;
        entityOffset = player->specialBlockSideOffsetPlayerIsStandingOn;

        if (entityData[entityOffset + 4] == 1) {
            SndPlaySfx(SFX_BUTTON_PRESS, 0, &SVECTOR_000a2df4, 7000);
        } else {
            SndPlaySfx(SFX_BUTTON_DEPRESS, 0, &SVECTOR_000a2df4, 7000);
        }
        Vibrate99(1, 0xff, 1);

        ptr0 = (unsigned short*)(entityOffset * 2 + (int)entityData);
        link = (short)ptr0[7];
        targetEntity = ((int)(ptr0[7] << 16)) >> 20;
        targetSide = ptr0[7] & 0xf;

        while (link != -1) {
            if (targetSide == 6) {
                ptr = (short*)(targetEntity * 256 + (int)entityData);
                ptr[3] = (ptr[3] + 1) % 2;
                entityOffset = targetEntity << 7;
                link = ptr[23];
                targetEntity = ((int)(((unsigned short*)ptr)[23] << 16)) >> 20;
                targetSide = ((unsigned short*)ptr)[23] & 0xf;
            } else {
                entityOffset = targetEntity * 128 + targetSide * 16;
                ptr2 = (unsigned short*)(entityOffset * 2 + (int)entityData);
                ptr2[4] = ((short)ptr2[4] % 2) + 1;
                link = (short)ptr2[6];
                targetEntity = ((int)(ptr2[6] << 16)) >> 20;
                targetSide = ptr2[6] & 0xf;
            }
        }
    }
}
#undef entityOffset
#undef targetEntity
#undef targetSide
#undef link

static int HandleTransporter_destSide;
static int HandleTransporter_destRot;
static int HandleTransporter_destEntity;
static int HandleTransporter_timer;
#define destSide HandleTransporter_destSide
#define destRot HandleTransporter_destRot
#define destEntity HandleTransporter_destEntity
#define timer HandleTransporter_timer
int HandleTransporter(Player* player) {
    if (IsPlayerInAir(player)) {
        player->alreadyProcessedEntityAction = 0;
        timer = -1;
        return 0;
    }
    if (player->faceTypePlayerStandingOn != OBJ_TRANSPORTER || entityData[player->specialBlockSideOffsetPlayerIsStandingOn + 4] != 1) {
        timer = -1;
        return 0;
    }
    if (player->alreadyProcessedEntityAction != OBJ_TRANSPORTER && player->subpixelPositionOnCube.vz >= 167 && player->subpixelPositionOnCube.vz <= 345) {
        if (timer == -1) {
            timer = 15;
            AddParticles(3, &transporterParticlesPos, 0);
        }

        if (timer > -1) {
            timer--;
            player->movementInhibitTimer = 15;
            player->rollingForward = 0;
            player->turnDirection = 0;
            player->jumping = 0;
        }

        if (timer == -1) {
            int destEntityIdx;
            int destCubeSide;

            SetRenderScreenFade(0, 1);
            Vibrate98(0);
            player->alreadyProcessedEntityAction = OBJ_TRANSPORTER;
            SndPlaySfx(5, 0, &SVECTOR_000a2df4, 7000);
            player->movementInhibitTimer = 15;
            player->howMoving0 = 0;
            player->rollingForward = 0;
            player->turnDirection = 0;
            player->jumping = 0;

            destEntity = entityData[player->specialBlockSideOffsetPlayerIsStandingOn + 7] >> 4;
            destSide = entityData[player->specialBlockSideOffsetPlayerIsStandingOn + 7] & 15;

            player->specialBlockSideOffsetPlayerIsStandingOn = destEntity * 128 + destSide * 16;
            player->specialBlockIndexPlayerIsStandingOn = destEntity * 128;

            destRot = entityData[player->specialBlockSideOffsetPlayerIsStandingOn + 2];

            SetPlayerRotation(destSide, destRot, player);

            player->perspVec1 = player->rightVec;
            player->perspVec3 = player->facingDir;
            player->perspVec2 = player->gravityDir;

            thePlayer.matrix_d4.m[0][0] = thePlayer.rightVec.vx << 12;
            thePlayer.matrix_d4.m[1][0] = thePlayer.rightVec.vy << 12;
            thePlayer.matrix_d4.m[2][0] = thePlayer.rightVec.vz << 12;
            thePlayer.matrix_d4.m[0][2] = thePlayer.gravityDir.vx << 12;
            thePlayer.matrix_d4.m[1][2] = thePlayer.gravityDir.vy << 12;
            thePlayer.matrix_d4.m[2][2] = thePlayer.gravityDir.vz << 12;
            thePlayer.matrix_d4.m[0][1] = -thePlayer.facingDir.vx << 12;
            thePlayer.matrix_d4.m[1][1] = -thePlayer.facingDir.vy << 12;
            thePlayer.matrix_d4.m[2][1] = -thePlayer.facingDir.vz << 12;

            player->howMoving198 = NOT_MOVING;

            player->jumpStartPos.vx = entityData[player->specialBlockIndexPlayerIsStandingOn + 125] * 512 + player->gravityDir.vx * 256;
            player->jumpStartPos.vy = entityData[player->specialBlockIndexPlayerIsStandingOn + 126] * 512 + player->gravityDir.vy * 256;
            player->jumpStartPos.vz = entityData[player->specialBlockIndexPlayerIsStandingOn + 127] * 512 + player->gravityDir.vz * 256;

            player->finePos = player->jumpStartPos;
            CalcWhatPlayerIsStandingOn(player);
            return 1;
        }
    }
    return 0;
}
#undef destSide
#undef destRot
#undef destEntity
#undef timer

static short HandleSpecialCubeTypes_blockType;
static SVECTOR HandleSpecialCubeTypes_cubePos;
static SVECTOR HandleSpecialCubeTypes_arrowDir;
#define blockType HandleSpecialCubeTypes_blockType
#define cubePos HandleSpecialCubeTypes_cubePos
#define arrowDir HandleSpecialCubeTypes_arrowDir
void HandleSpecialCubeTypes(Player* player) {
    int whichSide;
    SVECTOR gravityDir;
    SVECTOR dummy;

    blockType = player->surroundingBlocks[1][1][1];
    if (blockType == -2) {
        if (thePlayer.invulnerabilityTimer == -1) {
            if (player->movementInhibitTimer == 0) {
                player->delayedLevelEndReason = LEVEL_END_LASER;
                player->movementInhibitTimer = 4;
                player->movementVelocity = 0;
                player->rotX = 0;
                player->fireTimer = 1900;
            }
        }
    }
    if (player->faceTypePlayerStandingOn == OBJ_ARROW) {
        gravityDir = player->gravityDir;
        if (gravityDir.vx == 1) {
            whichSide = 1;
        } else if (gravityDir.vx == -1) {
            whichSide = 4;
        } else if (gravityDir.vy == 1) {
            whichSide = 2;
        } else if (gravityDir.vy == -1) {
            whichSide = 3;
        } else if (gravityDir.vz == 1) {
            whichSide = 5;
        } else if (gravityDir.vz == -1) {
            whichSide = 0;
        } else {
            whichSide = -1;
        }
        GetVectorBasedOnTwoDirs(whichSide, entityData[player->specialBlockSideOffsetPlayerIsStandingOn + 2], &arrowDir);
        if (player->facingDir.vx != arrowDir.vx ||
                player->facingDir.vy != arrowDir.vy ||
                player->facingDir.vz != arrowDir.vz) {
            thePlayer.rollingForward = 0;
        }
    }
    if (player->faceTypePlayerStandingOn == OBJ_ICE_PATCH) {
        player->rollingForward = 1;
        player->turnDirection = 0;
    } else if (thePlayer.startedIceSfx) {
        thePlayer.startedIceSfx = 0;
        SndMuteVoiceByTag(2);
    }
    if (player->faceTypePlayerStandingOn == OBJ_FAST_STAR) {
        if (player->rollingForward == 0) {
            if (player->jumping == 1 && player->howMoving198 != JUMPING_INPLACE) {
                levelScore += 5;
            }
        }
    }
    if (player->faceTypePlayerStandingOn == OBJ_FIRE_PATCH) {
        if (!IsPlayerInAir(player)) {
            if (thePlayer.invulnerabilityTimer == -1) {
                player->fireTimer += 40;
            }
            if (fireSoundTimer == 0) {
                SndPlaySfx(1, 1, &SVECTOR_000a2df4, 7000);
            }
            fireSoundTimer = 7;
        }
        if (player->onGround == 1) {
            if (thePlayer.invulnerabilityTimer == -1) {
                player->fireTimer += 600;
            }
        }
        if (player->fireTimer > 2000) {
            if (player->movementInhibitTimer == 0) {
                player->dying = 1;
                player->delayedLevelEndReason = LEVEL_END_BURNED;
                player->movementVelocity = 0;
                player->rotX = 0;
                player->movementInhibitTimer = 25;
            }
        }
    } else {
        if (fireSoundTimer != 0) {
            if (--fireSoundTimer == 0) {
                SndMuteVoiceByTag(1);
            }
        }
        player->fireTimer -= 15;
        if (player->fireTimer < 0) {
            player->fireTimer = 0;
        }
    }
    if (!IsPlayerInAir(player)) {
        if (player->acidTimer > 2000) {
            player->acidTimer = 2000;
        }
        if (player->faceTypePlayerStandingOn == OBJ_ACID_PATCH) {
            if (player->movementInhibitTimer == 0) {
                if (thePlayer.invulnerabilityTimer == -1) {
                    player->delayedLevelEndReason = LEVEL_END_MELTED;
                    player->movementInhibitTimer = 6;
                    player->acidTimer += 300;
                }
            }
        }
        if (player->faceTypePlayerStandingOn == OBJ_ICE_PATCH) {
            if (thePlayer.startedIceSfx == 0) {
                thePlayer.startedIceSfx = 1;
                SndPlaySfx(2, 2, &SVECTOR_000a2df4, 7000);
            }
            player->fireTimer -= 150;
            if (player->fireTimer < 0) {
                player->fireTimer = 0;
            }
            player->iceColorChangeTimer += 40;
            if (player->iceColorChangeTimer > 500) {
                player->iceColorChangeTimer = 400;
            }
        } else {
            player->iceColorChangeTimer -= 10;
            if (player->iceColorChangeTimer < 0) {
                player->iceColorChangeTimer = 0;
            }
        }
        if (shouldMarkCubesVisited != 0) {
            cubePos.vx = ((player->finePos.vx - (player->gravityDir.vx * 356)) + 256) >> 9;
            cubePos.vy = ((player->finePos.vy - (player->gravityDir.vy * 356)) + 256) >> 9;
            cubePos.vz = ((player->finePos.vz - (player->gravityDir.vz * 356)) + 256) >> 9;
            if (cubePos.vx < 34 && cubePos.vy < 34 && cubePos.vz < 34 && cubePos.vx > 0 && cubePos.vy > 0 && cubePos.vz > 0) {
                SetCubeVisited(cubePos.vx, cubePos.vy, cubePos.vz, 1);
            }
            if (numCubesRemainingInLevel[0] == 0) {
                if (player->onGround) {
                    levelEndReason = LEVEL_END_ALL_CUBES;
                } else if (player->movementInhibitTimer == 0) {
                    player->delayedLevelEndReason = LEVEL_END_ALL_CUBES;
                    player->movementInhibitTimer = 5;
                }
            }
        } else {
            if (player->faceTypePlayerStandingOn == OBJ_EXIT && numKeysRemaining == 0) {
                if (player->onGround) {
                    levelWon[cameraIndex] = LEVEL_END_EXIT;
                } else if (player->movementInhibitTimer == 0) {
                    player->delayedLevelEndReason = LEVEL_END_EXIT;
                    player->movementInhibitTimer = 5;
                }
            }
            if (player->faceTypePlayerStandingOn == OBJ_HIDDEN_EXIT && numKeysRemaining == 0) {
                if (player->onGround) {
                    levelWon[cameraIndex] = LEVEL_END_HIDDEN_EXIT;
                } else if (player->movementInhibitTimer == 0) {
                    player->delayedLevelEndReason = LEVEL_END_HIDDEN_EXIT;
                    player->movementInhibitTimer = 5;
                }
            }
        }
        if (player->faceTypePlayerStandingOn == OBJ_CRUMBLING_BLOCK_FACE) {
            if (entityData[player->specialBlockIndexPlayerIsStandingOn + 1] == 1) {
                SndPlaySfx(101, 0, &SVECTOR_000a2df4, 7000);
                levelScore += 50;
            }
            entityData[player->specialBlockIndexPlayerIsStandingOn + 1] = 2;
        }
        if (player->faceTypePlayerStandingOn == OBJ_SPIKE && player->movementInhibitTimer == 0 && thePlayer.invulnerabilityTimer == -1) {
            SndPlaySfx(11, 0, &SVECTOR_000a2df4, 7000);
            Vibrate99(1, 255, 5);
            if (thePlayer.movementInhibitTimer == 0) {
                thePlayer.dying = 1;
                thePlayer.movementVelocity = 0;
                thePlayer.rotX = 0;
                thePlayer.delayedLevelEndReason = LEVEL_END_SPIKED;
                thePlayer.movementInhibitTimer = 10;
                thePlayer.ballBlinking = 1;
            }
        }
        if (player->faceTypePlayerStandingOn == OBJ_MOVING_SPIKE) {
            if (entityData[player->specialBlockSideOffsetPlayerIsStandingOn + 4] == 1 && thePlayer.invulnerabilityTimer == -1) {
                if (player->onGround == 1) {
                    Vibrate99(1, 255, 5);
                    SndPlaySfx(11, 0, &SVECTOR_000a2df4, 7000);
                    if (thePlayer.movementInhibitTimer == 0) {
                        thePlayer.dying = 1;
                        thePlayer.movementVelocity = 0;
                        thePlayer.rotX = 0;
                        thePlayer.delayedLevelEndReason = LEVEL_END_SPIKED;
                        thePlayer.movementInhibitTimer = 10;
                        thePlayer.ballBlinking = 1;
                    }
                } else if (player->movementInhibitTimer == 0) {
                    SndPlaySfx(11, 0, &SVECTOR_000a2df4, 7000);
                    Vibrate99(1, 255, 5);
                    if (thePlayer.movementInhibitTimer == 0) {
                        thePlayer.dying = 1;
                        thePlayer.movementVelocity = 0;
                        thePlayer.rotX = 0;
                        thePlayer.delayedLevelEndReason = LEVEL_END_SPIKED;
                        thePlayer.movementInhibitTimer = 10;
                        thePlayer.ballBlinking = 1;
                    }
                    player->movementInhibitTimer = 9;
                }
            }
        }
        if (player->faceTypePlayerStandingOn == OBJ_BOUNCEPAD) {
            if (player->subpixelPositionOnCube.vz > 150) {
                player->forcedRollForwardTimer = 10;
            }
            if (player->faceTypePlayerStandingOn == OBJ_BOUNCEPAD && (player->subpixelPositionOnCube.vz > 230 || player->onGround == 1)) {
                SndPlaySfx(10, 0, &SVECTOR_000a2df4, 7000);
                Vibrate98(1);
                player->forcedRollForwardTimer = 0;
                player->jumping = 1;
                if (player->longJump == 0) {
                    player->longJump = 1;
                }
                player->rollingForward = 1;
                player->turnDirection = 0;
            }
        }
    }
    if (thePlayer.sunglassTimer == 0) {
        SetSunglassMode(0);
    }
    if (thePlayer.sunglassTimer >= 0) {
        thePlayer.sunglassTimer--;
    }
    if (thePlayer.invulnerabilityTimer > -1) {
        thePlayer.invulnerabilityTimer--;
    }
    if (thePlayer.invulnerabilityTimer >= 0) {
        if (thePlayer.bounceTimer >= 0) {
            thePlayer.bounceTimer = -1;
        }
        if (thePlayer.lethargyTimer >= 0) {
            thePlayer.lethargyTimer = 0;
        } else {
            goto skip_lethargy;
        }
    }
    if (thePlayer.lethargyTimer == 0) {
        EnableLethargy(0);
    }
    if (thePlayer.lethargyTimer >= 0) {
        thePlayer.lethargyTimer -= 2;
        SubtractLevelTimer(4);
    }
skip_lethargy:
    CheckForButtonEntity(player);
    if (HandleTransporter(player)) {
        CalcWhatPlayerIsStandingOn(player);
        UpdateSubpixelPositions(player);
        if (player->subpixelPositionOnCube.vz > 356) {
            player->alreadyProcessedEntityAction = 0;
        }
    }
    if (player->onMovingPlatform) {
        player->svec54.vx = entityData[player->movingPlatformEntityIdStandingOn + 119] - 512;
        player->svec54.vy = entityData[player->movingPlatformEntityIdStandingOn + 120] - 512;
        player->svec54.vz = entityData[player->movingPlatformEntityIdStandingOn + 121] - 512;
    } else {
        player->svec54.vz = 0;
        player->svec54.vy = 0;
        player->svec54.vx = 0;
    }
}
#undef blockType
#undef cubePos
#undef arrowDir

void SubtractLevelTimer(int param_1) {
    if (thePlayer.faceTypePlayerStandingOn != OBJ_TIMER_PAUSE && debugDisableTimer == 0 && gameMode != 1) {
        levelTimeLeft -= param_1;
        if (levelTimeLeft <= 0) {
            levelEndReason = LEVEL_END_TIME_OUT;
        }
    }

    if (thePlayer.faceTypePlayerStandingOn == OBJ_TIMER_PAUSE || debugDisableTimer == 1) {
        drawTimerPausedWidget = 1;
    } else {
        drawTimerPausedWidget = 0;
    }
}

int IsPlayerInAir(Player* player) {
    if (player->howMoving198 == FALLING || (player->howMoving198 == JUMPING_INPLACE || player->howMoving198 == JUMPING_FORWARD) && player->jumpingOrViewportRotationTimer > 1) {
        return 1;
    }
    return 0;
}

int IsFallingOrJumping(Player* player) {
    if (player->howMoving198 == FALLING || player->howMoving198 == JUMPING_INPLACE || player->howMoving198 == JUMPING_FORWARD) {
        return 1;
    }
    return 0;
}

#ifndef SKIP_UNUSED_CODE
void Unused_FUN_0003bdec(Player* player) {
    SVECTOR v;
    v.vx = player->finePos.vx + (player->facingDir.vx * 1024);
    v.vy = player->finePos.vy + (player->facingDir.vy * 1024);
    v.vz = player->finePos.vz + (player->facingDir.vz * 1024);
}
#endif
