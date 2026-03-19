#include "common.h"

#define CUBE_TYPE_AT(x, y, z) levelData[(x) * 1156 + (y) * 34 + (z)]

typedef struct ItemState {
    int collisionDistance;
    int type;
    MATRIX matrix;
    SVECTOR pos;
} ItemState;

int ballTextureIndex;
int curController;
short numCopycatMoves;
short unusedNumCopycatRounds;

int levelExitEntityOffset;
int levelHiddenExitEntityOffset;

int getBlockX;
int getBlockY;
int getBlockZ;
int getBlockResult;

short isPausedOrWaitingForRestart;

extern int inGetReadyScreen;
extern int isPaused;
extern int levelEndReason;
extern int debugDisableTimer;
extern int drawTimerPausedWidget;
extern int levelTimeLeft;
extern uint fruitsCollectedBitmask;
extern SVECTOR SVECTOR_000a2df4;
int gameMode;
extern int numCameras;
static int levelWon[2];
static SVECTOR playerCombinedPos;
extern InvisBlockVisibility invisBlockVisibility;
extern ItemState itemState[256];

int D_000A4430;
int D_000A43E8;
int D_000A43DC;
static SVECTOR SVECTOR_000a43ec;
static SVECTOR SVECTOR_000a43e0;
static SVECTOR initPlayerFacingVec;
static SVECTOR initPlayerGravityVec;
static SVECTOR initPlayerRightVec;


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

int ballTextureIndex;
int curController;
short numCopycatMoves;
short unusedNumCopycatRounds;

int getBlockX;
int getBlockY;
int getBlockZ;
int getBlockResult;

short isPausedOrWaitingForRestart;

int hpbPrevControllerButtons;
short copycatIdleTimer;
short copycatStateVar;
short curCopycatMove;
short padCross;
short padUp;
short timerBeforeJumpOrRoll;
short turnDelayEnabled;
short turnDelayFrames;
uint prevControllerButtons;
uint controllerButtons;

extern int GetButtonsFromReplay(void);
extern void RecordButtonsToDevkit(int buttons);
extern void SndMuteAllTaggedVoices(void);
extern void WriteToDevkit(int param_1);
extern int buttonSaveReplayMode;

extern short numEntities;
extern short* levelData;
extern short copycatMoves[1024];
extern short copycatNewOrCopyMoves;
extern Player thePlayer;
extern short* entityData;

void HandlePauseModeRotationEffect(Player* player);

extern int inGetReadyScreen;
extern int isPaused;
extern int levelEndReason;
extern int debugDisableTimer;
extern int drawTimerPausedWidget;
extern int levelTimeLeft;
extern SVECTOR SVECTOR_000a2df4;
int gameMode;
extern int numCameras;
static int levelWon[2];
static SVECTOR playerCombinedPos;
extern InvisBlockVisibility invisBlockVisibility;

static SVECTOR initPlayerFacingVec;
static SVECTOR initPlayerGravityVec;
static SVECTOR initPlayerRightVec;

extern int cameraIndex;
extern int specialLevelType;
extern int curWorld;
extern int numKeysRemaining;
extern int twoPlayerWhichPlayer;

extern void CalcLevelBounds(Player* player);
extern void CreateItemsFromLevelData(void);
extern void HandlePlayerMovementStuff(Player* player);
extern void InitEnemies(void);
extern void ResetPlayerVars(Player* player);

static SVECTOR SVECTOR_000a45d8;
static SVECTOR SVECTOR_000a4638;
static SVECTOR SVECTOR_000a4640;
static SVECTOR SVECTOR_000a4738;
static SVECTOR SVECTOR_000a4740;

extern int FUN_00033720(SVECTOR * vec, int itemdataOff, int param_3);
extern void FUN_0003418c(Player * player);
extern int GetRotationIndexFromVector(SVECTOR vec);

short calcI;
short calcX2;
short calcY2;
short calcZ2;
short D_000A45F8;
short D_000A45FC;
short D_000A4600;
short calcJ;
short calcK;
static SVECTOR SVECTOR_000a4618;
static short calcBlockType;

typedef struct EntityPos {
    MATRIX matrix;
    short posX;
    short posY;
    short posZ;
    short pad;
    byte reserved[8];
} EntityPos;

short pauseForStartPress;
short fireSoundTimer;
int shouldMarkCubesVisited;
short D_000A45CC;

extern void ProcessEnemies(void);
extern void CalcPlayerMatrixesAndDrawPlayer(Player* player);
extern void CreateAllItemDispLists(void);

extern void CheckPlayerJumpingStuff(Player * player);
extern void HandleItemTouching(Player * player);
extern void HandleSpecialCubeTypes(Player* player);
extern void HandleViewportRotationStart(Player* player);
extern int IsCollidingWithEnemy(SVECTOR pos);
extern void MoveMovingPlatforms(SVECTOR pos);
extern void ProcessCameraAndMovement(Player* player);
extern void ProcessMovement(Player* player);
extern void SetVec184ToVec54(Player * player);
extern void StartMovementIfNeeded(Player * player);
extern void UpdateEnemies(SVECTOR pos);

void CreateItemsFromLevelData(void) {
    int i, j, k;
    int eoff;

    levelHiddenExitEntityOffset = -1;
    k = 0;

    for (i = 0; i < numEntities; i++) {
        if (entityData[i * 128] < 5) {
            for (j = 0; j < 6; j++) {
                int shouldCreate = 1;
                eoff = i * 128 + j * 16;

                itemState[k].type = 0;

                switch (entityData[eoff + 1]) {
                case 43:
                case 44:
                case 45:
                case 46:
                case 47:
                    itemState[k].collisionDistance = 32400;
                    itemState[k].type = 1;
                    entityData[eoff + 9] = 386;
                    if (!(fruitsCollectedBitmask & 0x10)) entityData[eoff + 1] = 47;
                    if (!(fruitsCollectedBitmask & 0x08)) entityData[eoff + 1] = 46;
                    if (!(fruitsCollectedBitmask & 0x04)) entityData[eoff + 1] = 45;
                    if (!(fruitsCollectedBitmask & 0x02)) entityData[eoff + 1] = 44;
                    if (!(fruitsCollectedBitmask & 0x01)) entityData[eoff + 1] = 43;
                    break;
                case 32:
                case 33:
                case 34:
                case 38:
                    itemState[k].collisionDistance = 25600;
                    itemState[k].type = 1;
                    entityData[eoff + 9] = 386;
                    break;
                case 37:
                    itemState[k].collisionDistance = 25600;
                    itemState[k].type = 1;
                    entityData[eoff + 9] = 406;
                    break;
                case 42:
                    itemState[k].collisionDistance = 25600;
                    itemState[k].type = 1;
                    entityData[eoff + 9] = 416;
                    break;
                case 35:
                    itemState[k].collisionDistance = 25600;
                    itemState[k].type = 1;
                    entityData[eoff + 9] = 436;
                    break;
                case 36:
                    itemState[k].collisionDistance = 25600;
                    itemState[k].type = 1;
                    if (entityData[eoff + 3] == 0) {
                        entityData[eoff + 9] = 386;
                    } else {
                        entityData[eoff + 9] = 436;
                    }
                    break;
                case 7:
                    levelExitEntityOffset = eoff;
                    entityData[eoff + 3] = 1;
                    entityData[eoff + 9] = 500;
                    break;
                case 26:
                    levelHiddenExitEntityOffset = eoff;
                    entityData[eoff + 4] = 0;
                    entityData[eoff + 9] = 500;
                    break;
                case 31:
                    itemState[k].collisionDistance = 22500;
                    numKeysRemaining++;
                    itemState[k].type = 1;
                    entityData[eoff + 9] = 386;
                    break;
                case 10:
                    entityData[eoff + 11] = 16;
                    entityData[eoff + 9] = 256;
                    break;
                case 5:
                case 9:
                case 11:
                case 12:
                case 28:
                    entityData[eoff + 9] = 256;
                    break;
                default:
                    shouldCreate = 0;
                    break;
                }

                if (shouldCreate) {
                    entityData[eoff + 11] = rand(4095);
                    if (entityData[eoff + 1] == 10) {
                        entityData[eoff + 11] = 16;
                    }
                    entityData[eoff + 12] = rand(4095);
                    entityData[eoff + 13] = rand(4095);
                    entityData[eoff + 5] = k;

                    itemState[k].matrix.t[0] = entityData[i * 128 + 125] << 9;
                    itemState[k].matrix.t[1] = entityData[i * 128 + 126] << 9;
                    itemState[k].matrix.t[2] = entityData[i * 128 + 127] << 9;

                    SetEntityRotation(&itemState[k].matrix, entityData[eoff + 2], j, entityData[eoff + 9]);
                    k++;
                }
            }
        }
    }

    if (levelHiddenExitEntityOffset == -1) {
        levelHiddenExitEntityOffset = levelExitEntityOffset;
    }
}

void InitLasers(void) {
    int i;
    int x;
    int y;
    int z;

    for (i = 0; i < numEntities * 128; i += 128) {
        if (entityData[i + 0] != 8)
            continue;
        x = entityData[i + 4];
        y = entityData[i + 5];
        z = entityData[i + 6];
        switch (entityData[i + 2]) {
            case 1:
                levelData[x * 1156 + y * 34 + z] = 0;
                for (x = entityData[i + 4] + 1; x < entityData[i + 7]; x++) {
                    levelData[x * 1156 + y * 34 + z] = -2;
                }
                levelData[x * 1156 + y * 34 + z] = 0;
                break;
            case 2:
                levelData[x * 1156 + y * 34 + z] = 0;
                for (y = entityData[i + 5] + 1; y < entityData[i + 8]; y++) {
                    levelData[x * 1156 + y * 34 + z] = -2;
                }
                levelData[x * 1156 + y * 34 + z] = 0;
                break;
            case 5:
                levelData[x * 1156 + y * 34 + z] = 0;
                for (z = entityData[i + 6] + 1; z < entityData[i + 9]; z++) {
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

void LevelInit(void) {
    int i, j;

    if (gameMode == 1) {
        thePlayer.movementInhibitTimer = 10;
        copycatNewOrCopyMoves = 0;
        copycatStateVar = 0;
        timerBeforeJumpOrRoll = 0;
        copycatIdleTimer = 0;
        thePlayer.copycatMoveIndex = 0;
        numCopycatMoves += 2;
        unusedNumCopycatRounds++;
    }

    fireSoundTimer = 0;
    numKeysRemaining = 0;
    levelEndReason = 0;
    levelWon[0] = 0;
    levelWon[1] = 0;

    ScanLevelDataForMovingBlocks2();
    MoveMovingPlatforms(thePlayer.finePos);
    InitEnemies();

    thePlayer.sunglassTimer = -1;
    thePlayer.lethargyTimer = -1;
    thePlayer.invulnerabilityTimer = -1;
    thePlayer.bounceTimer = -1;

    thePlayer.debugCameraMode = 0;
    thePlayer.turnDelayTimer = 0;
    thePlayer.ballBlinking = 0;
    thePlayer.isRotatingViewport = 0;
    thePlayer.dying = 0;
    thePlayer.field_2bc = 0;
    thePlayer.ballMorphShape = 0;
    thePlayer.flatteningTimer = 0;
    thePlayer.startedIceSfx = 0;
    thePlayer.delayedLevelEndReason = 0;
    thePlayer.jumpingOnMovingPlatform = 0;
    thePlayer.onMovingPlatform = 0;
    thePlayer.alreadyProcessedEntityAction = 0;
    thePlayer.movementInhibitTimer = 20;
    thePlayer.forcedRollForwardTimer = 0;
    thePlayer.turnDirection = 0;
    thePlayer.rollingForward = 0;
    thePlayer.jumpingInplaceOnTopOfMovingPlatform = 0;
    thePlayer.jumping = 0;
    thePlayer.longJump = 0;
    thePlayer.onGround = 0;
    thePlayer.fireTimer = 0;
    thePlayer.acidTimer = 0;
    thePlayer.iceColorChangeTimer = 0;

    thePlayer.cameraR1R2TurnDirection = SVECTOR_000a2df4;
    thePlayer.cameraR1TurnAmount = SVECTOR_000a2df4;
    thePlayer.svec54 = SVECTOR_000a2df4;

    pauseForStartPress = 0;
    if (entityData[numEntities * 128] == 666) {
        levelTimeLeft = entityData[numEntities * 128 + 6] * 50;
    } else {
        levelTimeLeft = 4950;
    }

    CreateItemsFromLevelData();

    if (numKeysRemaining == 0 && gameMode != 1) {
        shouldMarkCubesVisited = 1;
    } else {
        shouldMarkCubesVisited = 0;
    }

    ballTextureIndex = curWorld;
    if (shouldMarkCubesVisited != 0) {
        ballTextureIndex = D_000A45CC + 10;
    }

    D_000A45CC = (D_000A45CC + 1) % 3;

    if (specialLevelType == 2) {
        ballTextureIndex = 13;
    }

    if (gameMode == 1) {
        ballTextureIndex = curController;
    }

    if (gameMode == 2) {
        ballTextureIndex = twoPlayerWhichPlayer;
    }

    for (i = 0; i < numEntities; i++) {
        if (entityData[i * 128] < 5) {
            for (j = 0; j < 6; j++) {
                if ((ushort)(entityData[i * 128 + j * 16 + 1] - 29) < 2) {
                    SetPlayerRotation(j, entityData[i * 128 + j * 16 + 2], &thePlayer);

                    thePlayer.perspVec1 = thePlayer.rightVec;
                    thePlayer.perspVec3 = thePlayer.facingDir;
                    thePlayer.perspVec2 = thePlayer.gravityDir;

                    SetPlayerMatrix6(&thePlayer);

                    thePlayer.jumpStartPos.vx = entityData[i * 128 + 125] * 512 + thePlayer.gravityDir.vx * 256;
                    thePlayer.jumpStartPos.vy = entityData[i * 128 + 126] * 512 + thePlayer.gravityDir.vy * 256;
                    thePlayer.jumpStartPos.vz = entityData[i * 128 + 127] * 512 + thePlayer.gravityDir.vz * 256;

                    thePlayer.finePos = thePlayer.jumpStartPos;

                    CalcWhatPlayerIsStandingOn(&thePlayer);
                    ResetPlayerVars(&thePlayer);
                    HandlePlayerMovementStuff(&thePlayer);

                    if (entityData[i * 128 + j * 16 + 1] == 29) {
                        entityData[i * 128 + j * 16 + 1] = 8;
                    }
                }
            }
        }
    }
    CalcLevelBounds(&thePlayer);
}

void ProcessPlayer(void) {
    int fireVibration;

    SubtractLevelTimer(1);
    if (isPausedOrWaitingForRestart != 0) {
        isPausedOrWaitingForRestart++;
    }
    if (isPausedOrWaitingForRestart == 20) {
        isPausedOrWaitingForRestart = 0;
    }

    if (copycatStateVar == 1 && thePlayer.startTurningTo == 0 && thePlayer.howMoving198 == NOT_MOVING) {
        thePlayer.delayedLevelEndReason = 1;
        thePlayer.movementInhibitTimer = 25;
        copycatStateVar = 0;
        curController = (curController + 1) % 2;
    }

    if (copycatStateVar == 2 && thePlayer.startTurningTo == 0 && thePlayer.howMoving198 == NOT_MOVING) {
        thePlayer.delayedLevelEndReason = -8;
        thePlayer.movementInhibitTimer = 25;
        copycatStateVar = 0;
    }

    if (thePlayer.movementInhibitTimer != 0) {
        short timer = thePlayer.movementInhibitTimer - 1;
        thePlayer.movementInhibitTimer = timer;
        thePlayer.rollingForward = 0;
        thePlayer.turnDirection = 0;
        thePlayer.jumping = 0;

        if (thePlayer.delayedLevelEndReason == -4) {
            thePlayer.flatteningTimer += 210;
        }
        if (thePlayer.delayedLevelEndReason == -1 && timer == 1) {
            levelEndReason = -1;
        }
        if (thePlayer.delayedLevelEndReason == -6) {
            thePlayer.acidTimer += 300;
        }
        if (thePlayer.movementInhibitTimer == 0) {
            levelWon[cameraIndex] = thePlayer.delayedLevelEndReason;
        }
    }

    if (thePlayer.forcedRollForwardTimer != 0) {
        thePlayer.forcedRollForwardTimer--;
        thePlayer.rollingForward = 1;
        thePlayer.turnDirection = 0;
        thePlayer.jumping = 0;
    }

    if (thePlayer.movementInhibitTimer < 1 && thePlayer.forcedRollForwardTimer < 1 && thePlayer.faceTypePlayerStandingOn != 10) {
        HandlePlayerButtons(&thePlayer);
    }

    if (levelWon[cameraIndex] == 1) {
        thePlayer.rollingForward = 0;
        thePlayer.turnDirection = 0;
        thePlayer.jumping = 0;
    }

    if (thePlayer.bounceTimer > 698 && thePlayer.howMoving198 == ROLLING) {
        thePlayer.jumping = 0;
        thePlayer.rollingForward = 0;
        thePlayer.bounceTimer++;
    }

    if (levelWon[cameraIndex] == 1) {
        if (numCameras == 1 || (levelWon[0] == 1 && levelWon[1] == 1)) {
            levelEndReason = levelWon[cameraIndex];
        }
    } else if (levelWon[cameraIndex] != 0) {
        levelEndReason = levelWon[cameraIndex];
    }

    if (thePlayer.movementInhibitTimer < 1000) {
        playerCombinedPos.vx = thePlayer.finePos.vx + thePlayer.svec54.vx;
        playerCombinedPos.vy = thePlayer.finePos.vy + thePlayer.svec54.vy;
        playerCombinedPos.vz = thePlayer.finePos.vz + thePlayer.svec54.vz;

        MoveMovingPlatforms(playerCombinedPos);
        if (thePlayer.delayedLevelEndReason != -5) {
            UpdateEnemies(playerCombinedPos);
        }
        if (isPausedOrWaitingForRestart == 0) {
            HandleSpecialCubeTypes(&thePlayer);
        }
    }

    if (thePlayer.fireTimer > 0) {
        fireVibration = thePlayer.fireTimer / 25 + 40;
        if (fireVibration > 125) {
            Vibrate99(1, fireVibration, 1);
        } else {
            Vibrate99(0, fireVibration, 1);
        }
    }

    if (thePlayer.invulnerabilityTimer == -1) {
        if (IsCollidingWithEnemy(thePlayer.finePos) && thePlayer.delayedLevelEndReason != -5) {
            SndPlaySfx(0x71, 0, &SVECTOR_000a2df4, 7000);
            Vibrate99(1, 255, 10);
            thePlayer.dying = 1;
            thePlayer.movementInhibitTimer = 10;
            thePlayer.movementVelocity = 0;
            thePlayer.rotX = 0;
            thePlayer.delayedLevelEndReason = -5;
            thePlayer.ballBlinking = 1;
        }
    }

    thePlayer.onGround = 0;
    StartMovementIfNeeded(&thePlayer);
    ProcessMovement(&thePlayer);
    CalcWhatPlayerIsStandingOn(&thePlayer);
    UpdateSubpixelPositions(&thePlayer);

    if ((uint)((ushort)thePlayer.subpixelPositionOnCube.vz - 156) > 200 || (uint)((ushort)thePlayer.subpixelPositionOnCube.vx - 156) > 200) {
        thePlayer.alreadyProcessedEntityAction = 0;
    }

    HandleViewportRotationStart(&thePlayer);
    CheckPlayerJumpingStuff(&thePlayer);
    SetVec184ToVec54(&thePlayer);

    if (thePlayer.playerHasControl == 1 && isPausedOrWaitingForRestart == 0) {
        HandleItemTouching(&thePlayer);
    }

    ProcessCameraAndMovement(&thePlayer);
    SetPlayerMatrix6(&thePlayer);

    invisBlockVisibility.pos[0] = (int)thePlayer.finePos.vx + (int)thePlayer.svec54.vx;
    invisBlockVisibility.pos[1] = (int)thePlayer.finePos.vy + (int)thePlayer.svec54.vy;
    invisBlockVisibility.pos[2] = (int)thePlayer.finePos.vz + (int)thePlayer.svec54.vz;
}

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

void RenderItems_() {
    CreateAllItemDispLists();
}

void HandlePlayerButtons(Player *player) {
    if (turnDelayEnabled) {
        turnDelayFrames = 6;
    } else {
        turnDelayFrames = 0;
    }

    switch (buttonSaveReplayMode) {
        case 1:
            if (controllerButtons & PAD_CIRCLE) {
                WriteToDevkit(0);
            } else {
                RecordButtonsToDevkit((short)controllerButtons);
            }
            break;
        case 2:
            controllerButtons |= GetButtonsFromReplay();
            break;
    }
    player->jumping = 0;


    player->rollingForward = 0;
    player->turnDirection = 0;
    player->cameraR1R2TurnDirection.vy = 0;
    player->cameraR1R2TurnDirection.vx = 0;

    if (thePlayer.bounceTimer >= 0) {
        player->jumping = 1;
    }

    if ((controllerButtons & PAD_CROSS) && isPausedOrWaitingForRestart) {
        return;
    }

    hpbPrevControllerButtons = prevControllerButtons;
    isPausedOrWaitingForRestart = 0;

    switch (gameMode) {
        case 0:
        case 2:
            if (player->playerHasControl) {
                if (player->startTurningTo == 0) {
                    if (++player->turnDelayTimer > turnDelayFrames) {
                        hpbPrevControllerButtons = 0;
                    }
                    if (player->startTurningLeftNextFrame == 1) {
                        player->startTurningLeftNextFrame = 0;
                        player->turnDelayTimer = 0;
                        player->turnDirection = 1;
                    }
                    if (player->startTurningRightNextFrame == 1) {
                        player->startTurningRightNextFrame = 0;
                        player->turnDelayTimer = 0;
                        player->turnDirection = -1;
                    }
                    if ((controllerButtons & PAD_L) & ~hpbPrevControllerButtons) {
                        player->startTurningLeftNextFrame = 0;
                        player->turnDelayTimer = 0;
                        player->turnDirection = 1;
                    }
                    if ((controllerButtons & PAD_R) & ~hpbPrevControllerButtons) {
                        player->startTurningRightNextFrame = 0;
                        player->turnDelayTimer = 0;
                        player->turnDirection = -1;
                    }
                } else {
                    player->turnDelayTimer = 0;
                    if ((controllerButtons & PAD_L) & ~hpbPrevControllerButtons) {
                        player->startTurningLeftNextFrame = 1;
                    }
                    if ((controllerButtons & PAD_R) & ~hpbPrevControllerButtons) {
                        player->startTurningRightNextFrame = 1;
                    }
                }

                if (thePlayer.bounceTimer >= 0) {
                    if (controllerButtons & PAD_U) {
                        padUp = 1;
                    }
                    if (controllerButtons & PAD_CROSS) {
                        padCross = 1;
                    }
                    if (padUp == 1 || padCross == 1) {
                        timerBeforeJumpOrRoll++;
                    } else {
                        timerBeforeJumpOrRoll = 0;
                    }
                    if (timerBeforeJumpOrRoll > 3) {
                        player->rollingForward = padUp;
                        if (padCross == 1 && padUp == 1 && player->longJump == 0 && player->howMoving198 != JUMPING_FORWARD) {
                            player->longJump = 1;
                        }
                        padCross = 0;
                        padUp = 0;
                    }
                } else {
                    if (controllerButtons & PAD_U) {
                        player->rollingForward = 1;
                    }
                    if (controllerButtons & PAD_CROSS) {
                        player->jumping = 1;
                    }
                }
            }
            break;

        case 1:
            if (player->playerHasControl == 0) break;
            if (copycatStateVar == 1) break;
            if (copycatStateVar == 2) break;
            {
                if (player->howMoving198 == NOT_MOVING && player->startTurningTo == 0) {
                    copycatIdleTimer++;
                } else {
                    copycatIdleTimer = 0;
                }

                if (controllerButtons & PAD_U) {
                    padUp = 1;
                }
                if (controllerButtons & PAD_CROSS) {
                    padCross = 1;
                }
                if (padUp == 1 || padCross == 1) {
                    timerBeforeJumpOrRoll++;
                }

                if (timerBeforeJumpOrRoll > 4) {
                    player->jumping = padCross;
                    player->rollingForward = padUp;
                    timerBeforeJumpOrRoll = 0;
                    padCross = 0;
                    padUp = 0;
                }

                if (copycatIdleTimer < 3) {
                    timerBeforeJumpOrRoll = 0;
                    padCross = 0;
                    padUp = 0;
                    if (player->howMoving198 == NOT_MOVING && player->startTurningTo == 0) {
                        if (copycatMoves[player->copycatMoveIndex] == -1) {
                            copycatNewOrCopyMoves = 1;
                        } else {
                            copycatNewOrCopyMoves = 0;
                        }
                    }
                }

                if (copycatIdleTimer < 6) {
                    player->jumping = 0;
                    player->turnDirection = 0;
                    player->rollingForward = 0;
                } else {
                    if (controllerButtons & PAD_L) {
                        player->turnDirection = 1;
                        copycatIdleTimer = 0;
                    }
                    if (controllerButtons & PAD_R) {
                        player->turnDirection = -1;
                        copycatIdleTimer = 0;
                    }
                }

                if (player->rollingForward == 1 && player->jumping == 0 && player->surroundingBlocks[0][2][1] < 0 &&
                    (player->surroundingBlocks[0][1][0] > -1 || player->surroundingBlocks[0][1][2] > -1)) {
                    player->rollingForward = 0;
                }

                if (player->turnDirection != 0 || player->rollingForward != 0 || player->jumping != 0) {
                    curCopycatMove = 0;
                    if (player->turnDirection == -1) curCopycatMove = 1;
                    if (player->turnDirection == 1) curCopycatMove |= 2;
                    if (player->rollingForward == 1) curCopycatMove |= 4;
                    if (player->jumping == 1) curCopycatMove |= 8;

                    if (copycatMoves[player->copycatMoveIndex] == -1) {
                        copycatMoves[player->copycatMoveIndex] = curCopycatMove;
                        if (player->copycatMoveIndex == numCopycatMoves) {
                            copycatStateVar = 1;
                        }
                    } else {
                        if (copycatMoves[player->copycatMoveIndex] != curCopycatMove) {
                            copycatStateVar = 2;
                        }
                    }
                    player->copycatMoveIndex++;
                }
            }
            break;
    }

    if (controllerButtons & PAD_R1) {
        player->cameraR1R2TurnDirection.vx = -1;
    }
    if (controllerButtons & PAD_R2) {
        player->cameraR1R2TurnDirection.vx = 1;
    }
    if (player->rollingForward == 1) {
        player->cameraR1R2TurnDirection.vy = 0;
    }

    if ((controllerButtons & PAD_START) & ~prevControllerButtons) {
        pauseForStartPress = 1;
        SndMuteAllTaggedVoices();
    }

    if (player->debugCameraMode) {
        if ((controllerButtons & PAD_CIRCLE) & ~prevControllerButtons) {
            player->debugCameraMode = (player->debugCameraMode + 1) % 2;
        }
        player->debugCamY = 0;
        player->debugCamX = 0;
        if (controllerButtons & PAD_D) player->debugCamY = -25;
        if (controllerButtons & PAD_U) player->debugCamY = 25;
        if (controllerButtons & PAD_L) player->debugCamX = 32;
        if (controllerButtons & PAD_R) player->debugCamX = -32;

        if ((controllerButtons & PAD_L1) & ~prevControllerButtons) player->debugCameraParam -= 4;
        if ((controllerButtons & PAD_L2) & ~prevControllerButtons) player->debugCameraParam += 4;

        if (player->debugCameraParam > 19) player->debugCameraParam = 20;
        if (player->debugCameraParam < 5) player->debugCameraParam = 4;

        player->debugCamY %= 4096;
        player->debugCamX %= 4096;

        player->jumping = 0;
        player->rollingForward = 0;
        player->turnDirection = 0;
    }

    if (player->surroundingBlocks[0][2][1] < 0 && player->surroundingBlocks[0][1][0] < 0 &&
        player->surroundingBlocks[0][1][2] < 0 && player->faceTypePlayerStandingOn == 96 &&
        player->jumping == 0 && player->movementVelocity > 0 && player->subpixelPositionOnCube.vz > 300) {

        player->rollingForward = 0;
        if (player->subpixelPositionOnCube.vz > 400) {
            player->finePos.vx += player->facingDir.vx * -40;
            player->finePos.vy += player->facingDir.vy * -40;
            player->finePos.vz += player->facingDir.vz * -40;
        }
        player->movementVelocity = 0;
    }
}


void CalcWhatPlayerIsStandingOn(Player *player) {
    if (player->onMovingPlatform != 0) {
        FUN_0003418c(player);
    } else {
        SVECTOR_000a4618.vx = (player->finePos.vx + 256) >> 9;
        SVECTOR_000a4618.vy = (player->finePos.vy + 256) >> 9;
        SVECTOR_000a4618.vz = (player->finePos.vz + 256) >> 9;

        calcX2 = player->rightVec.vx + (SVECTOR_000a4618.vx - player->gravityDir.vx) - player->facingDir.vx;
        calcY2 = player->rightVec.vy + (SVECTOR_000a4618.vy - player->gravityDir.vy) - player->facingDir.vy;
        calcZ2 = player->rightVec.vz + (SVECTOR_000a4618.vz - player->gravityDir.vz) - player->facingDir.vz;

        for (calcI = 0; calcI < 3; calcI++) {
            for (calcJ = 0; calcJ < 3; calcJ++) {
                for (calcK = 0; calcK < 3; calcK++) {
                    D_000A45F8 = calcX2 + calcI * player->gravityDir.vx + calcJ * player->facingDir.vx - calcK * player->rightVec.vx;
                    D_000A45FC = calcY2 + calcI * player->gravityDir.vy + calcJ * player->facingDir.vy - calcK * player->rightVec.vy;
                    D_000A4600 = calcZ2 + calcI * player->gravityDir.vz + calcJ * player->facingDir.vz - calcK * player->rightVec.vz;

                    if (!(D_000A45F8 >= 1 && D_000A45FC >= 1 && D_000A4600 >= 1 && D_000A45F8 <= 32 && D_000A45FC <= 32 && D_000A4600 <= 32)) {
                        player->surroundingBlocks[calcI][calcJ][calcK] = -1;
                    } else {
                        calcBlockType = player->surroundingBlocks[calcI][calcJ][calcK] = levelData[D_000A45F8 * 1156 + D_000A45FC * 34 + D_000A4600];

                        if (calcBlockType >= 5 && entityData[(calcBlockType - 5) * 128] == 5) {
                            if (calcI == 2 || calcJ == 2) {
                                if (FUN_00033720(&player->finePos, (calcBlockType - 5) * 128, 100) == 0) {
                                    player->surroundingBlocks[calcI][calcJ][calcK] = -1;
                                }
                            } else {
                                player->surroundingBlocks[calcI][calcJ][calcK] = -1;
                            }
                        }
                    }
                }
            }
        }

        player->specialBlockIndexPlayerIsStandingOn = player->surroundingBlocks[0][1][1];

        if (player->specialBlockIndexPlayerIsStandingOn >= 5) {
            player->specialBlockIndexPlayerIsStandingOn = (player->specialBlockIndexPlayerIsStandingOn - 5) * 128;
            player->specialBlockSideOffsetPlayerIsStandingOn = player->specialBlockIndexPlayerIsStandingOn + GetRotationIndexFromVector(player->gravityDir) * 16;

            if (entityData[player->specialBlockIndexPlayerIsStandingOn] == 5) player->faceTypePlayerStandingOn = 95;
            if (entityData[player->specialBlockIndexPlayerIsStandingOn] == 6) player->faceTypePlayerStandingOn = 96;
            if (entityData[player->specialBlockIndexPlayerIsStandingOn] == 7) player->faceTypePlayerStandingOn = 97;
            if (entityData[player->specialBlockIndexPlayerIsStandingOn] == 1) player->faceTypePlayerStandingOn = 1;
            if (entityData[player->specialBlockIndexPlayerIsStandingOn] == 2) player->faceTypePlayerStandingOn = 2;
            if (entityData[player->specialBlockIndexPlayerIsStandingOn] == 3) player->faceTypePlayerStandingOn = 93;
            if (entityData[player->specialBlockIndexPlayerIsStandingOn] == 4) player->faceTypePlayerStandingOn = 4;
            if (entityData[player->specialBlockIndexPlayerIsStandingOn] == 0) {
                 player->faceTypePlayerStandingOn = entityData[player->specialBlockSideOffsetPlayerIsStandingOn + 1];
            }
        } else {
            if (player->specialBlockIndexPlayerIsStandingOn == 1) player->faceTypePlayerStandingOn = 1;
            if (player->specialBlockIndexPlayerIsStandingOn == 2) player->faceTypePlayerStandingOn = 2;
            if (player->specialBlockIndexPlayerIsStandingOn == 3) player->faceTypePlayerStandingOn = 93;
            if (player->specialBlockIndexPlayerIsStandingOn == 4) player->faceTypePlayerStandingOn = 4;
            if (player->specialBlockIndexPlayerIsStandingOn == 0) {
                player->faceTypePlayerStandingOn = -1;
            }
            if (player->specialBlockIndexPlayerIsStandingOn == -1) {
                player->faceTypePlayerStandingOn = -1;
            }
            player->specialBlockIndexPlayerIsStandingOn = -1;
            player->specialBlockSideOffsetPlayerIsStandingOn = -1;
        }
    }
}


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


INCLUDE_ASM("asm/nonmatchings/level_update2", MovePlayerForward);

INCLUDE_ASM("asm/nonmatchings/level_update2", MovePlayerDownwards);

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

INCLUDE_ASM("asm/nonmatchings/level_update2", HandleItemTouching);

INCLUDE_ASM("asm/nonmatchings/level_update2", CreateAllItemDispLists);

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

extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);
extern void Vibrate99(int magnitude1, int magnitude2, int count);
int IsFallingOrJumping(Player* player);

int DAT_000a4748;
int DAT_000a474c;
int DAT_000a4750;
int DAT_000a4754;

void CheckForButtonEntity(Player* player) {
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

            if (entityData[DAT_000a4748 + 4] == 1) {
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


INCLUDE_ASM("asm/nonmatchings/level_update2", HandleTransporter);

INCLUDE_ASM("asm/nonmatchings/level_update2", HandleSpecialCubeTypes);

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

INCLUDE_ASM("asm/nonmatchings/level_update2", Unused_FUN_0003bdec);
