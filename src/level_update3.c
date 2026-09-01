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

int DAT_000a4748;
int DAT_000a474c;
int DAT_000a4750;
int DAT_000a4754;
int gameMode;
int levelExitEntityOffset;
int levelHiddenExitEntityOffset;
int shouldMarkCubesVisited;
int tmpOff;
int tmpOff2;
int transportDestCubeSide;
int transportDestRotation;
int transporterDestEntityIdx;
int transporterTimer;
short fireSoundTimer;

static int calcEntityPositionsDistSq;
static int D_000A46A8;
static int D_000A4734;
static int distSquared2;
static int dontDisableItemShadow;
static int entityBlockOffset;
static int handleItemTouchI;
static int handleItemTouchIter;
static int handleItemTouchJ;
static int itemOffset;
STATIC_FOR_GP_ACCESS int levelWon[2];
static int playerRadius;
static int touchItemIdx;
static MATRIX MATRIX_000a46d4;
static MATRIX MATRIX_000a4714;
static short DAT_000a4788;
static short touchCubeX;
static short touchCubeY;
static short touchCubeZ;
static short touchSide;
static SVECTOR calcEntityPositionsVec;
static SVECTOR cubeBelowPlayerPos;
static SVECTOR keyScreenSpaceParticlesPos;
static SVECTOR SVECTOR_000a46ac;
static SVECTOR SVECTOR_000a46b4;
static SVECTOR SVECTOR_000a46bc;
static SVECTOR SVECTOR_000a4738;
static SVECTOR SVECTOR_000a4740;
static SVECTOR SVECTOR_000a4778;
static SVECTOR tempPlayerPos;

void HandleItemTouching(Player* player) {
    int* ptr;

    tempPlayerPos = player->finePos;
    playerRadius = 10000;

    for (handleItemTouchIter = 0; handleItemTouchIter < 3; handleItemTouchIter++) {
        for (handleItemTouchI = 0; handleItemTouchI < 3; handleItemTouchI++) {
            for (handleItemTouchJ = 0; handleItemTouchJ < 3; handleItemTouchJ++) {
                entityBlockOffset = (player->surroundingBlocks[handleItemTouchIter][handleItemTouchI][handleItemTouchJ] - 5) * 128;
                if (entityBlockOffset < 0) {
                    continue;
                }

                if (entityData[entityBlockOffset] >= 5) {
                    continue;
                }

                for (itemOffset = 0; itemOffset < 96; itemOffset += 16) {
                    touchItemIdx = entityData[entityBlockOffset + itemOffset + 5];

                    if (entityData[entityBlockOffset + itemOffset + 4] == 0 || itemState[touchItemIdx].type == 0) {
                        continue;
                    }

                    distSquared2 = (tempPlayerPos.vx - itemState[touchItemIdx].pos.vx) * (tempPlayerPos.vx - itemState[touchItemIdx].pos.vx) +
                                   (tempPlayerPos.vy - itemState[touchItemIdx].pos.vy) * (tempPlayerPos.vy - itemState[touchItemIdx].pos.vy) +
                                   (tempPlayerPos.vz - itemState[touchItemIdx].pos.vz) * (tempPlayerPos.vz - itemState[touchItemIdx].pos.vz);

                    if (distSquared2 >= itemState[touchItemIdx].collisionDistance + playerRadius) {
                        continue;
                    }
                    touchCubeX = entityData[entityBlockOffset + 125];
                    touchCubeY = entityData[entityBlockOffset + 126];
                    touchCubeZ = entityData[entityBlockOffset + 127];
                    touchSide = itemOffset / 16;
                    dontDisableItemShadow = 0;

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
                                dontDisableItemShadow = 1;
                            } else {
                                AddParticles(7, &itemState[touchItemIdx].pos, AddLightEffect(touchCubeX, touchCubeY, touchCubeZ, touchSide));
                                levelScore += 1550;
                            }
                            break;

                        case OBJ_LETHARGY_PILL:
                            AddParticles(9, &itemState[touchItemIdx].pos, AddLightEffect(touchCubeX, touchCubeY, touchCubeZ, touchSide));
                            thePlayer.lethargyTimer = 300;
                            EnableLethargy(1);
                            Vibrate100(120, 120, 200, 1);
                            Vibrate101(200);
                            SndPlaySfx(SFX_LETHARGY_PILL, 0, &SVECTOR_000a2df4, 7000);
                            break;

                        case OBJ_INVINCIBILITY_PILL:
                            AddParticles(9, &itemState[touchItemIdx].pos, AddLightEffect(touchCubeX, touchCubeY, touchCubeZ, touchSide));
                            thePlayer.invulnerabilityTimer = 700;
                            SndPlaySfx(SFX_LETHARGY_PILL, 0, &SVECTOR_000a2df4, 7000);
                            break;

                        case OBJ_BOUNCY_PILL:
                            AddParticles(9, &itemState[touchItemIdx].pos, AddLightEffect(touchCubeX, touchCubeY, touchCubeZ, touchSide));
                            thePlayer.bounceTimer = 700;
                            SndPlaySfx(SFX_BOUNCE_PILL, 0, &SVECTOR_000a2df4, 7000);
                            break;

                        case OBJ_SUNGLASSES:
                            SndPlaySfx(SFX_SUNGLASSES_COLLECTION, 0, &SVECTOR_000a2df4, 7000);
                            AddParticles(8, &itemState[touchItemIdx].pos, AddLightEffect(touchCubeX, touchCubeY, touchCubeZ, touchSide));
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
                                keyScreenSpaceParticlesPos.vx = *(int*)((char*)p + (numKeysInLevel - numKeysRemaining) * 8) + 6;
                                keyScreenSpaceParticlesPos.vy = *(int*)((char*)p + (numKeysInLevel - numKeysRemaining) * 8 + 4) + 11;
                            }
                            asm volatile("");
                            keyScreenSpaceParticlesPos.vz = -1;

                            AddParticles(0, &keyScreenSpaceParticlesPos, 0);
                            AddParticles(6, &itemState[touchItemIdx].pos, AddLightEffect(touchCubeX, touchCubeY, touchCubeZ, touchSide));
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
                            AddParticles(7, &itemState[touchItemIdx].pos, AddLightEffect(touchCubeX, touchCubeY, touchCubeZ, touchSide));
                            levelScore += 2975;
                            SndPlaySfx(SFX_GEM_COLLECTION, 0, &SVECTOR_000a2df4, 7000);
                            break;

                        case OBJ_COIN:
                            AddParticles(2, &itemState[touchItemIdx].pos, AddLightEffect(touchCubeX, touchCubeY, touchCubeZ, touchSide));
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
                            AddParticles(4, &itemState[touchItemIdx].pos, AddLightEffect(touchCubeX, touchCubeY, touchCubeZ, touchSide));
                            SndPlaySfx(SFX_FRUIT_1, 0, &SVECTOR_000a2df4, 7000);
                            levelScore += 2500;
                            fruitsCollectedBitmask |= 1;
                            levelPlayTime[twoPlayerWhichPlayer] -= 200;
                            break;

                        case OBJ_WATERMELON:
                            if (gameMode != 2) {
                                AddParticles(0, &fruit2ScreenSpaceParticlesPos, 0);
                            }
                            AddParticles(4, &itemState[touchItemIdx].pos, AddLightEffect(touchCubeX, touchCubeY, touchCubeZ, touchSide));
                            SndPlaySfx(SFX_FRUIT_2, 0, &SVECTOR_000a2df4, 7000);
                            levelScore += 2500;
                            fruitsCollectedBitmask |= 2;
                            levelPlayTime[twoPlayerWhichPlayer] -= 200;
                            break;

                        case OBJ_PUMPKIN:
                            if (gameMode != 2) {
                                AddParticles(0, &fruit3ScreenSpaceParticlesPos, 0);
                            }
                            AddParticles(4, &itemState[touchItemIdx].pos, AddLightEffect(touchCubeX, touchCubeY, touchCubeZ, touchSide));
                            SndPlaySfx(SFX_FRUIT_3, 0, &SVECTOR_000a2df4, 7000);
                            levelScore += 2500;
                            fruitsCollectedBitmask |= 4;
                            levelPlayTime[twoPlayerWhichPlayer] -= 200;
                            break;

                        case OBJ_BANANA:
                            if (gameMode != 2) {
                                AddParticles(0, &fruit4ScreenSpaceParticlesPos, 0);
                            }
                            AddParticles(4, &itemState[touchItemIdx].pos, AddLightEffect(touchCubeX, touchCubeY, touchCubeZ, touchSide));
                            SndPlaySfx(SFX_FRUIT_4, 0, &SVECTOR_000a2df4, 7000);
                            levelScore += 2500;
                            fruitsCollectedBitmask |= 8;
                            levelPlayTime[twoPlayerWhichPlayer] -= 200;
                            break;

                        case OBJ_STRAWBERRY:
                            if (gameMode != 2) {
                                AddParticles(0, &fruit5ScreenSpaceParticlesPos, 0);
                            }
                            AddParticles(4, &itemState[touchItemIdx].pos, AddLightEffect(touchCubeX, touchCubeY, touchCubeZ, touchSide));
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
                            AddParticles(4, &itemState[touchItemIdx].pos, AddLightEffect(touchCubeX, touchCubeY, touchCubeZ, touchSide));
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
                            AddParticles(10, &itemState[touchItemIdx].pos, AddLightEffect(touchCubeX, touchCubeY, touchCubeZ, touchSide));
                            break;
                    }

                    if (!dontDisableItemShadow) {
                        entityData[entityBlockOffset + itemOffset + 4] = 0;
                        entityData[entityBlockOffset + itemOffset + 1] = 0;
                        DisableItemShadow(entityBlockOffset / 128, itemOffset / 16, 0);
                    } else {
                        dontDisableItemShadow = 0;
                    }
                }
            }
        }
    }
}

void CreateAllItemDispLists(void) {
    for (tmpOff = 0; tmpOff < numEntities * 128; tmpOff += 128) {
        if (entityData[tmpOff] >= 5) {
            continue;
        }
        for (tmpOff2 = 0; tmpOff2 < 96; tmpOff2 += 16) {
            if (entityData[tmpOff + tmpOff2 + 4] == 0) {
                continue;
            }
            D_000A46A8 = entityData[tmpOff + tmpOff2 + 2];
            calcEntityPositionsVec.vx = entityData[tmpOff + 125] * 512;
            calcEntityPositionsVec.vy = entityData[tmpOff + 126] * 512;
            calcEntityPositionsVec.vz = entityData[tmpOff + 127] * 512;
            SVECTOR_000a46bc.vx = SVECTOR_000a46bc.vy = SVECTOR_000a46bc.vz = 0;

            switch (entityData[tmpOff + tmpOff2 + 1]) {
                case OBJ_SPIKE_TRAP:
                    entityData[tmpOff + tmpOff2 + 11] = (entityData[tmpOff + tmpOff2 + 11] + 16) % 4096;
                    if (entityData[tmpOff + tmpOff2 + 3] == 0) {
                        entityData[tmpOff + tmpOff2 + 13] = (entityData[tmpOff + tmpOff2 + 13] + 100) % 4096;
                    }
                    if (entityData[tmpOff + tmpOff2 + 3] == 1) {
                        entityData[tmpOff + tmpOff2 + 13] = (entityData[tmpOff + tmpOff2 + 13] + 8) % 4096;
                    }

                    if (entityData[tmpOff + tmpOff2 + 11] > 2048) {
                        entityData[tmpOff + tmpOff2 + 3] = 1;
                    } else {
                        entityData[tmpOff + tmpOff2 + 3] = 0;
                    }

                    if (entityData[tmpOff + tmpOff2 + 3] == 1) {
                        entityData[tmpOff + tmpOff2 + 12] = (entityData[tmpOff + tmpOff2 + 12] + (entityData[tmpOff + tmpOff2 + 11] % 2048) / 2) % 4096;
                    }
                    SVECTOR_000a46bc.vz = (rsin(entityData[tmpOff + tmpOff2 + 12]) * 0) / 4096; // guess
                    SVECTOR_000a46bc.vx = (rsin(entityData[tmpOff + tmpOff2 + 13]) * 100) / 4096;
                    SVECTOR_000a46bc.vy = (rcos(entityData[tmpOff + tmpOff2 + 13]) * 100) / 4096;
                    SVECTOR_000a46ac.vy = SVECTOR_000a46ac.vx = 0;
                    SVECTOR_000a46ac.vz = 1024 - entityData[tmpOff + tmpOff2 + 13];
                    RotMatrixZYX(&SVECTOR_000a46ac, &MATRIX_000a4714);
                    break;

                case OBJ_BUTTON:
                case OBJ_MOVING_SPIKE:
                case OBJ_SPIKE:
                case OBJ_ARROW:
                    SVECTOR_000a46ac.vx = SVECTOR_000a46ac.vy = SVECTOR_000a46ac.vz = 0;
                    RotMatrix(&SVECTOR_000a46ac, &MATRIX_000a4714);
                    break;

                case OBJ_TRANSPORTER:
                    if (entityData[tmpOff + tmpOff2 + 4] == 1 && cameraIndex == 0) {
                        entityData[tmpOff + tmpOff2 + 11] = (entityData[tmpOff + tmpOff2 + 11] + 30) % 4096;
                    }
                    SVECTOR_000a46ac.vx = SVECTOR_000a46ac.vy = 0;
                    SVECTOR_000a46ac.vz = entityData[tmpOff + tmpOff2 + 11];
                    RotMatrix(&SVECTOR_000a46ac, &MATRIX_000a4714);
                    break;

                case OBJ_EXIT:
                case OBJ_HIDDEN_EXIT:
                    if (entityData[tmpOff + tmpOff2 + 4] == 1) {
                        entityData[tmpOff + tmpOff2 + 11] = (entityData[tmpOff + tmpOff2 + 11] - 55) % 4096;
                    } else {
                        entityData[tmpOff + tmpOff2 + 11] = (entityData[tmpOff + tmpOff2 + 11] - 25) % 4096;
                    }
                    SVECTOR_000a46ac.vx = SVECTOR_000a46ac.vy = 0;
                    SVECTOR_000a46ac.vz = entityData[tmpOff + tmpOff2 + 11];
                    RotMatrix(&SVECTOR_000a46ac, &MATRIX_000a4714);
                    break;

                case OBJ_APPLE:
                case OBJ_WATERMELON:
                case OBJ_PUMPKIN:
                case OBJ_BANANA:
                case OBJ_STRAWBERRY:
                    entityData[tmpOff + tmpOff2 + 11] = (entityData[tmpOff + tmpOff2 + 11] + 24) % 4096;
                    entityData[tmpOff + tmpOff2 + 12] = (entityData[tmpOff + tmpOff2 + 12] - 80) % 4096;
                    entityData[tmpOff + tmpOff2 + 13] = (entityData[tmpOff + tmpOff2 + 13] - 60) % 4096;
                    SVECTOR_000a46bc.vz = (rsin(entityData[tmpOff + tmpOff2 + 13]) * 20) / 4096 + 10;
                    SVECTOR_000a46ac.vy = 0;
                    SVECTOR_000a46ac.vx = (rsin(entityData[tmpOff + tmpOff2 + 12]) * 150) / 4096;
                    SVECTOR_000a46ac.vz = entityData[tmpOff + tmpOff2 + 11];
                    RotMatrixZYX(&SVECTOR_000a46ac, &MATRIX_000a4714);
                    break;

                case OBJ_KEY:
                case OBJ_COIN:
                case OBJ_SUNGLASSES:
                    entityData[tmpOff + tmpOff2 + 11] = (entityData[tmpOff + tmpOff2 + 11] - 90) % 4096;
                    entityData[tmpOff + tmpOff2 + 13] = (entityData[tmpOff + tmpOff2 + 13] - 70) % 4096;
                    SVECTOR_000a46bc.vz = (rsin(entityData[tmpOff + tmpOff2 + 13]) * 20) / 4096 + 10;
                    SVECTOR_000a46ac.vy = 0;
                    SVECTOR_000a46ac.vx = 0;
                    SVECTOR_000a46ac.vz = entityData[tmpOff + tmpOff2 + 11];
                    RotMatrixZYX(&SVECTOR_000a46ac, &MATRIX_000a4714);
                    break;

                case OBJ_GEM:
                    entityData[tmpOff + tmpOff2 + 11] = (entityData[tmpOff + tmpOff2 + 11] + 35) % 4096;
                    SVECTOR_000a46ac.vy = 0;
                    SVECTOR_000a46ac.vx = 0;
                    SVECTOR_000a46ac.vz = entityData[tmpOff + tmpOff2 + 11];
                    RotMatrix(&SVECTOR_000a46ac, &MATRIX_000a4714);
                    break;

                case OBJ_HOURGLASS:
                    entityData[tmpOff + tmpOff2 + 11] = (entityData[tmpOff + tmpOff2 + 11] + 32) % 4096;
                    entityData[tmpOff + tmpOff2 + 12] = (entityData[tmpOff + tmpOff2 + 12] - 20) % 4096;
                    SVECTOR_000a46ac.vy = (rsin(entityData[tmpOff + tmpOff2 + 11]) / 2) + 30;
                    SVECTOR_000a46ac.vx = 0;
                    SVECTOR_000a46ac.vz = entityData[tmpOff + tmpOff2 + 12];
                    RotMatrixZYX(&SVECTOR_000a46ac, &MATRIX_000a4714);
                    break;

                case OBJ_LETHARGY_PILL:
                case OBJ_BOUNCY_PILL:
                case OBJ_INVINCIBILITY_PILL:
                    entityData[tmpOff + tmpOff2 + 11] = (entityData[tmpOff + tmpOff2 + 11] + 80) % 4096;
                    entityData[tmpOff + tmpOff2 + 12] = (entityData[tmpOff + tmpOff2 + 12] + 25) % 4096;
                    SVECTOR_000a46ac.vy = entityData[tmpOff + tmpOff2 + 11];
                    SVECTOR_000a46ac.vz = entityData[tmpOff + tmpOff2 + 12];
                    SVECTOR_000a46ac.vx = 0;
                    RotMatrixZYX(&SVECTOR_000a46ac, &MATRIX_000a4714);
                    break;

                case OBJ_PURPLE_PRESENT:
                case OBJ_RED_PRESENT:
                case OBJ_YELLOW_PRESENT:
                case OBJ_BLUE_PRESENT:
                case OBJ_GREEN_PRESENT:
                    entityData[tmpOff + tmpOff2 + 11] = (entityData[tmpOff + tmpOff2 + 11] - 17) % 4096;
                    SVECTOR_000a46ac.vx = SVECTOR_000a46ac.vy = 0;
                    SVECTOR_000a46ac.vz = entityData[tmpOff + tmpOff2 + 11];
                    RotMatrix(&SVECTOR_000a46ac, &MATRIX_000a4714);
                    break;

                case OBJ_BOUNCEPAD:
                    SVECTOR_000a46ac.vz = 0;
                    SVECTOR_000a46ac.vy = 0;
                    SVECTOR_000a46ac.vx = 0;
                    RotMatrix(&SVECTOR_000a46ac, &MATRIX_000a4714);
                    calcEntityPositionsDistSq = (calcEntityPositionsVec.vx - thePlayer.finePos.vx) * (calcEntityPositionsVec.vx - thePlayer.finePos.vx) +
                                                (calcEntityPositionsVec.vy - thePlayer.finePos.vy) * (calcEntityPositionsVec.vy - thePlayer.finePos.vy) +
                                                (calcEntityPositionsVec.vz - thePlayer.finePos.vz) * (calcEntityPositionsVec.vz - thePlayer.finePos.vz);

                    if (calcEntityPositionsDistSq < 490000) {
                        if (entityData[tmpOff + tmpOff2 + 11] > 0)
                            entityData[tmpOff + tmpOff2 + 11]--;
                        MATRIX_000a4714.m[2][2] = (MATRIX_000a4714.m[2][2] * entityData[tmpOff + tmpOff2 + 11]) / 16;
                    } else if (entityData[tmpOff + tmpOff2 + 11] < 16) {
                        entityData[tmpOff + tmpOff2 + 11] += 4;
                        MATRIX_000a4714.m[2][2] = (MATRIX_000a4714.m[2][2] * entityData[tmpOff + tmpOff2 + 11]) / 16;
                    }
                    break;
            }

            D_000A4734 = entityData[tmpOff + tmpOff2 + 5];
            ApplyMatrixSV(&itemState[D_000A4734].matrix, &SVECTOR_000a46bc, &SVECTOR_000a46bc);
            calcEntityPositionsVec.vx = itemState[D_000A4734].pos.vx = SVECTOR_000a46bc.vx + itemState[D_000A4734].matrix.t[0];
            calcEntityPositionsVec.vy = itemState[D_000A4734].pos.vy = SVECTOR_000a46bc.vy + itemState[D_000A4734].matrix.t[1];
            calcEntityPositionsVec.vz = itemState[D_000A4734].pos.vz = SVECTOR_000a46bc.vz + itemState[D_000A4734].matrix.t[2];
            MulMatrix0(&itemState[D_000A4734].matrix, &MATRIX_000a4714, &MATRIX_000a46f4);
            MulMatrix0(&perspMatrixes[cameraIndex], &MATRIX_000a46f4, &MATRIX_000a46d4);
            ApplyMatrixSV(&perspMatrixes[cameraIndex], &calcEntityPositionsVec, &SVECTOR_000a46b4);
            MATRIX_000a46d4.t[0] = SVECTOR_000a46b4.vx + perspMatrixes[cameraIndex].t[0];
            MATRIX_000a46d4.t[1] = SVECTOR_000a46b4.vy + perspMatrixes[cameraIndex].t[1];
            MATRIX_000a46d4.t[2] = SVECTOR_000a46b4.vz + perspMatrixes[cameraIndex].t[2];
            CreateItemDispList(&MATRIX_000a46d4, MATRIX_000a46d4.t[2], tmpOff / 128, tmpOff2 / 16);
        }
    }
}

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
        DAT_000a4748 = player->specialBlockSideOffsetPlayerIsStandingOn;

        if (entityData[DAT_000a4748 + 4] == 1) {
            SndPlaySfx(SFX_BUTTON_PRESS, 0, &SVECTOR_000a2df4, 7000);
        } else {
            SndPlaySfx(SFX_BUTTON_DEPRESS, 0, &SVECTOR_000a2df4, 7000);
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

int HandleTransporter(Player* player) {
    if (IsPlayerInAir(player)) {
        player->alreadyProcessedEntityAction = 0;
        transporterTimer = -1;
        return 0;
    }
    if (player->faceTypePlayerStandingOn != OBJ_TRANSPORTER || entityData[player->specialBlockSideOffsetPlayerIsStandingOn + 4] != 1) {
        transporterTimer = -1;
        return 0;
    }
    if (player->alreadyProcessedEntityAction != OBJ_TRANSPORTER && player->subpixelPositionOnCube.vz >= 167 && player->subpixelPositionOnCube.vz <= 345) {
        if (transporterTimer == -1) {
            transporterTimer = 15;
            AddParticles(3, &transporterParticlesPos, 0);
        }

        if (transporterTimer > -1) {
            transporterTimer--;
            player->movementInhibitTimer = 15;
            player->rollingForward = 0;
            player->turnDirection = 0;
            player->jumping = 0;
        }

        if (transporterTimer == -1) {
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

            transporterDestEntityIdx = entityData[player->specialBlockSideOffsetPlayerIsStandingOn + 7] >> 4;
            transportDestCubeSide = entityData[player->specialBlockSideOffsetPlayerIsStandingOn + 7] & 15;

            player->specialBlockSideOffsetPlayerIsStandingOn = transporterDestEntityIdx * 128 + transportDestCubeSide * 16;
            player->specialBlockIndexPlayerIsStandingOn = transporterDestEntityIdx * 128;

            transportDestRotation = entityData[player->specialBlockSideOffsetPlayerIsStandingOn + 2];

            SetPlayerRotation(transportDestCubeSide, transportDestRotation, player);

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

void HandleSpecialCubeTypes(Player* player) {
    int whichSide;
    SVECTOR gravityDir;
    SVECTOR dummy;

    DAT_000a4788 = player->surroundingBlocks[1][1][1];
    if (DAT_000a4788 == -2) {
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
        GetVectorBasedOnTwoDirs(whichSide, entityData[player->specialBlockSideOffsetPlayerIsStandingOn + 2], &SVECTOR_000a4778);
        if (player->facingDir.vx != SVECTOR_000a4778.vx ||
                player->facingDir.vy != SVECTOR_000a4778.vy ||
                player->facingDir.vz != SVECTOR_000a4778.vz) {
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
            cubeBelowPlayerPos.vx = ((player->finePos.vx - (player->gravityDir.vx * 356)) + 256) >> 9;
            cubeBelowPlayerPos.vy = ((player->finePos.vy - (player->gravityDir.vy * 356)) + 256) >> 9;
            cubeBelowPlayerPos.vz = ((player->finePos.vz - (player->gravityDir.vz * 356)) + 256) >> 9;
            if (cubeBelowPlayerPos.vx < 34 && cubeBelowPlayerPos.vy < 34 && cubeBelowPlayerPos.vz < 34 && cubeBelowPlayerPos.vx > 0 && cubeBelowPlayerPos.vy > 0 && cubeBelowPlayerPos.vz > 0) {
                SetCubeVisited(cubeBelowPlayerPos.vx, cubeBelowPlayerPos.vy, cubeBelowPlayerPos.vz, 1);
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
