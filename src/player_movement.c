#include "common.h"

#ifdef FIX_HORRIBLE_BUGS
extern int IsPlayerOnMovingPlatform(Player* player);
#else
// This should take Player* player, but I guess they are calling the function without prototype,
// and $a0 just happens to contain the player pointer anyway.
extern int IsPlayerOnMovingPlatform();
#endif
extern int GetBlockAt(SVECTOR* coord);
extern int GetRotationIndexFromVector(SVECTOR vec);
extern int HandleMovingPlatforms(Player* player);
extern void ClearJumpSquish(Player* player);
extern void CreatePlayerDispList(MATRIX* m, int const0x100, int ballTextureIndex, int const0,
                                 int colorR, int colorG, int colorB, int const0_, int blockX,
                                 int blockY, int blockZ, int blockDirIndex, int otherBlockX,
                                 int otherBlockY, int otherBlockZ, int otherBlockDirIndex,
                                 MATRIX* gteMatrix, int shadowColor, int param_19, MATRIX* param_20,
                                 int param_21, int param_22, int const0_23, int const0_24,
                                 int const0xb2, SVECTOR* param_26);
extern void EnableScreenShake(int param_1, int param_2, int param_3);
extern void EnableTurningMotionBlur(void);
extern void GetVectorBasedOnTwoDirs(int dir1, int dir2, SVECTOR* param_3);
extern void JumpingOnMovingPlatform(Player* player);
extern void MatrixFromDirectionIndex(MATRIX* m, int p2, int dirIndex, int delta, SVECTOR* vec);
extern void MovePlayerDownwards(Player* player, int param_2);
extern void MovePlayerForward(Player* player, int delta);
extern void ResetPlayerMatrix274(Player* player);
extern void SetCubeVisited(int x, int y, int z, int visitType);
extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);
extern void UpdateSubpixelPositions(Player* player);
extern void Vibrate99(byte magnitude1, byte magnitude2, int count);

void StartJumpingForward(Player* player);
void StartJumpingInplace(Player* player);
void StartRollingForward(Player* player);
void TurnLeft(Player* player);
void TurnRight(Player* player);

extern int ballTextureIndex;
extern int cameraIndex;
extern int gameMode;
extern int levelTimeLeft;
extern int shouldMarkCubesVisited;
extern InvisBlockVisibility invisBlockVisibility;
extern MATRIX perspMatrixes[];
extern short* entityData;
extern short* ggiPart5JumpAnimData;
extern short isPausedOrWaitingForRestart;
extern short* levelData;

static short jumpSquishMagnitudeIncrement;
static short* initJumpTimerPtr;
static short landingSquishDamping;
static short landingSquishFrameCounter;
static short landingSquishMagnitude;
static short landingSquishMagnitudeIncrement;

static int ballColorB;
static int ballColorG;
static int ballColorR;
static SVECTOR playerFinePosWithSquish;

SVECTOR ZERO_SVECTOR_a2dd8 = {};
int unusedA2DE0 = 0;

void ResetPlayerVars(Player* player) {
    player->howMoving198 = NOT_MOVING;
    player->field119_0x1c8 = 0;
    player->viewpointRotationTimer = 0;
    player->turningTimer = 0;
    player->viewportRotationAngle = 0;
    player->turningPhase = 0;
    player->field_1d2 = 0;
    player->field_1e2 = 0;
    player->startTurningTo = 0;
    player->unusedTurningWhere = 0;
    player->field100_0x1ac = 0;
    player->howMoving0 = 0;
    player->movementVelocity = 0;
    player->gravityVelocity = 0;
    player->field_134 = 0;

    player->matrix_234.m[2][2] = 0x1000;
    player->matrix_254.m[2][2] = 0x1000;
    player->matrix_274.m[2][2] = 0x1000;
    player->matrix_234.m[1][1] = 0x1000;
    player->matrix_254.m[1][1] = 0x1000;
    player->matrix_274.m[1][1] = 0x1000;
    player->matrix_234.m[0][0] = 0x1000;
    player->matrix_254.m[0][0] = 0x1000;
    player->matrix_274.m[0][0] = 0x1000;

    player->matrix_234.m[2][1] = 0;
    initJumpTimerPtr = ggiPart5JumpAnimData;
    player->svec_184.vx = player->finePos.vx;
    player->svec_184.vy = player->finePos.vy;
    player->svec_184.vz = player->finePos.vz;

    player->matrix_254.m[2][1] = 0;
    player->matrix_274.m[2][1] = 0;
    player->matrix_234.m[2][0] = 0;
    player->matrix_254.m[2][0] = 0;
    player->matrix_274.m[2][0] = 0;
    player->matrix_234.m[1][2] = 0;
    player->matrix_254.m[1][2] = 0;
    player->matrix_274.m[1][2] = 0;
    player->matrix_234.m[1][0] = 0;
    player->matrix_254.m[1][0] = 0;
    player->matrix_274.m[1][0] = 0;
    player->matrix_234.m[0][2] = 0;
    player->matrix_254.m[0][2] = 0;
    player->matrix_274.m[0][2] = 0;
    player->matrix_234.m[0][1] = 0;
    player->matrix_254.m[0][1] = 0;
    player->matrix_274.m[0][1] = 0;
}

void SetVec184ToVec54(Player* player) {
    player->svec_184.vx = player->finePos.vx + player->svec54.vx;
    player->svec_184.vy = player->finePos.vy + player->svec54.vy;
    player->svec_184.vz = player->finePos.vz + player->svec54.vz;
}

void StartMovementIfNeeded(Player* player) {
    switch (gameMode) {
        case 0:
        case 2:
            if (player->howMoving198 == JUMPING_FORWARD)
                return;
            if (player->howMoving198 == JUMPING_INPLACE) {
                if (player->turnDirection == 1)
                    TurnLeft(player);
                if (player->turnDirection == -1)
                    TurnRight(player);
                return;
            }
            if (player->howMoving198 == ROLLING && player->jumping == 1) {
                StartJumpingForward(player);
            }
            if (player->howMoving198 == NOT_MOVING) {
                if (player->jumping == 1) {
                    if (player->rollingForward == 1) {
                        StartJumpingForward(player);
                    } else {
                        StartJumpingInplace(player);
                    }
                } else {
                    if (player->rollingForward == 1) {
                        StartRollingForward(player);
                    } else {
                        if (player->turnDirection == 1) {
                            TurnLeft(player);
                        }
                        if (player->turnDirection == -1) {
                            TurnRight(player);
                        }
                    }
                }
            }
            return;
        case 1:
            if (player->howMoving198 == NOT_MOVING) {
                if (player->jumping == 1) {
                    if (player->rollingForward == 1) {
                        StartJumpingForward(player);
                    } else {
                        StartJumpingInplace(player);
                    }
                    return;
                }
                if (player->rollingForward == 1) {
                    StartRollingForward(player);
                    return;
                }
                if (player->turnDirection == 1) {
                    TurnLeft(player);
                }
                if (player->turnDirection == -1) {
                    TurnRight(player);
                }
            }
            if (player->howMoving198 == ROLLING && player->jumping == 1) {
                StartJumpingForward(player);
            }
            return;
    }
}

void StartJumpingForward(Player* player) {
    if (player->surroundingBlocks[0][1][1] >= 0) {
        player->onGround = 0;
        player->howMoving198 = JUMPING_FORWARD;
        if (player->onMovingPlatform) {
            JumpingOnMovingPlatform(player);
        }
        player->jumpingInplaceOnTopOfMovingPlatform = 0;
        player->jumpStartPos = player->finePos;

        player->jumpdataPtr = initJumpTimerPtr;
        player->jumpingOrViewportRotationTimer = initJumpTimerPtr[0];
        player->howMoving0 = 3;
        player->movementVelocity = 40;
        player->rotX = 40;
        player->jumpdataPtr += 4;
        ClearJumpSquish(player);
    }
}

void StartRollingForward(Player* player) {
    if (player->surroundingBlocks[0][1][1] >= 0) {
        if (IsRollingForwardBlocked(player) || player->subpixelPositionOnCube.vz < 256) {
            player->howMoving0 = 2;
            player->howMoving198 = ROLLING;
            player->onGround = 0;
        }
    }
}

void StartJumpingInplace(Player* player) {
    player->howMoving198 = JUMPING_INPLACE;
    if (player->onMovingPlatform) {
        JumpingOnMovingPlatform(player);
        player->jumpingInplaceOnTopOfMovingPlatform = 1;
    }
    player->jumpStartPos = player->finePos;
    player->jumpdataPtr = initJumpTimerPtr;
    player->jumpingOrViewportRotationTimer = *player->jumpdataPtr;
    player->howMoving0 = 1;
    player->onGround = 0;
    player->jumpdataPtr += 4;
}

void TurnRight(Player* player) {
    int pad[2];

    if (player->startTurningTo == 0 && player->field119_0x1c8 == 0) {
        player->unusedTurningWhere = 1;
        player->startTurningTo = 1;
        player->turningTimer = 16;
        player->turningDelta = -64;
        player->onGround = 0;
        player->howMoving0 = 1;
        if (player->turningRightRelated >= 2) {
            player->turningRightRelated = 3;
        }
        EnableTurningMotionBlur();
    }
}

void TurnLeft(Player* player) {
    int pad[2];

    if (player->startTurningTo == 0 && player->field119_0x1c8 == 0) {
        player->unusedTurningWhere = 2;
        player->startTurningTo = 2;
        player->turningTimer = 16;
        player->turningDelta = 64;
        player->onGround = 0;
        player->howMoving0 = 1;
        if (player->turningLeftRelated >= 2) {
            player->turningLeftRelated = 3;
        }
        EnableTurningMotionBlur();
    }
}

void ProcessMovement(Player* player) {
    int dummy[2];

    player->turningWhereNextFrame = 0;
    if (player->howMoving198 != JUMPING_FORWARD && player->longJump > 1) {
        player->longJump = 0;
    }

    if (player->dying) {
        return;
    }

    switch (player->howMoving198) {
        case NOT_MOVING:
            MovePlayerDownwards(player, 100);
            if (player->surroundingBlocks[0][1][1] < 0) {
                if (player->onMovingPlatform == 0) {
                    player->howMoving198 = FALLING;
                    player->howMoving0 = 3;
                    player->movementVelocity = 0;
                    player->gravityVelocity = -10;
                }
            }
            player->rotX = 0;
            break;

        case ROLLING:
            AutoCenterSubpixelPosition(player, 8);
            if (player->surroundingBlocks[0][1][1] < 0) {
                if ((ushort)(player->subpixelPositionOnCube.vz - 101) < 327) {
                    if (player->onMovingPlatform == 0) {
                        player->howMoving198 = FALLING;
                        player->howMoving0 = 3;
                        player->movementVelocity = 0;
                        player->gravityVelocity = -10;
                    }
                }
            }

            if (player->subpixelPositionOnCube.vz > 100) {
                player->isRotatingViewport = 0;
            }

            player->movementVelocity += 7;
            if (player->movementVelocity > 39) {
                player->movementVelocity = 40;
            }

            player->rotX = player->movementVelocity;

            player->finePos.vx += player->movementVelocity * player->facingDir.vx;
            player->finePos.vy += player->movementVelocity * player->facingDir.vy;
            player->finePos.vz += player->movementVelocity * player->facingDir.vz;
            break;

        case ROTATING:
            player->jumpingOrViewportRotationTimer++;
            player->isRotatingViewport = 1;
            player->turningWhereNextFrame = 1;
            if (player->jumpingOrViewportRotationTimer == 1) {
                player->finePos.vx += player->gravityDir.vx * 38 + player->facingDir.vx * 8;
                player->finePos.vy += player->gravityDir.vy * 38 + player->facingDir.vy * 8;
                player->finePos.vz += player->gravityDir.vz * 38 + player->facingDir.vz * 8;
            }

            if (player->jumpingOrViewportRotationTimer == 2) {
                player->finePos.vx += player->gravityDir.vx * 32 + player->facingDir.vx * 22;
                player->finePos.vy += player->gravityDir.vy * 32 + player->facingDir.vy * 22;
                player->finePos.vz += player->gravityDir.vz * 32 + player->facingDir.vz * 22;
            }

            if (player->jumpingOrViewportRotationTimer == 3) {
                player->finePos.vx += player->gravityDir.vx * 22 + player->facingDir.vx * 32;
                player->finePos.vy += player->gravityDir.vy * 22 + player->facingDir.vy * 32;
                player->finePos.vz += player->gravityDir.vz * 22 + player->facingDir.vz * 32;
            }

            if (player->jumpingOrViewportRotationTimer == 4) {
                player->howMoving198 = ROLLING;
                player->finePos.vx += player->gravityDir.vx * 8 + player->facingDir.vx * 39;
                player->finePos.vy += player->gravityDir.vy * 8 + player->facingDir.vy * 39;
                player->howMoving0 = 0;
                player->finePos.vz += player->gravityDir.vz * 8 + player->facingDir.vz * 39;
                MovePlayerDownwards(player, 100);
            }
            break;

        case JUMPING_FORWARD:
            player->jumpingOrViewportRotationTimer--;

            if (player->jumpingOrViewportRotationTimer > 0) {
                if (!player->dying) {
                    player->jumpVec.vz = player->jumpdataPtr[0];
                    player->jumpdataPtr++;
                    player->jumpVec.vx = player->jumpdataPtr[0];
                    player->jumpdataPtr++;
                    player->jumpVec.vy = player->jumpdataPtr[0];
                    player->jumpdataPtr++;

                    if (player->longJump) {
                        player->jumpVec.vx = (player->jumpVec.vx * 3) / 2;
                        player->jumpVec.vy = (player->jumpVec.vy * 3) / 2;
                        player->jumpVec.vz = (player->jumpVec.vz * 3) / 2;
                        player->longJump++;
                    }

                    ApplyMatrixSV(&player->matrix_d4, &player->jumpVec, &player->jumpVec);
                    AutoAlignJumpStartPos(player, 8);

                    player->finePos.vx = player->jumpStartPos.vx + player->jumpVec.vx;
                    player->finePos.vy = player->jumpStartPos.vy + player->jumpVec.vy;
                    player->finePos.vz = player->jumpStartPos.vz + player->jumpVec.vz;
                }
            } else {
                if (player->longJump >= 3) {
                    player->finePos.vx = player->jumpStartPos.vx + player->facingDir.vx * 1536;
                    player->finePos.vy = player->jumpStartPos.vy + player->facingDir.vy * 1536;
                    player->finePos.vz = player->jumpStartPos.vz + player->facingDir.vz * 1536;
                } else {
                    player->finePos.vx = player->jumpStartPos.vx + player->facingDir.vx * 1024;
                    player->finePos.vy = player->jumpStartPos.vy + player->facingDir.vy * 1024;
                    player->finePos.vz = player->jumpStartPos.vz + player->facingDir.vz * 1024;
                }

                if (CheckIfPlayerLanded(player)) {
                    player->jumpingOnMovingPlatform = 0;
                } else {
                    player->howMoving198 = FALLING;
                    player->howMoving0 = 3;
                    player->movementVelocity = 0;
                    player->gravityVelocity = -0x28;
                    player->gravityVelocity = player->svec_144.vy;
                    if (player->gravityVelocity >= 0) {
                        player->gravityVelocity = -player->svec_144.vy;
                    } else {
                        player->gravityVelocity = player->svec_144.vy;
                    }
                    player->longJump = 0;
                }
            }
            break;

        case FALLING:
            if (player->rollingForward) {
                player->rotX += 6;
                if (player->rotX > 40) {
                    player->rotX = 40;
                }
            } else {
                player->rotX--;
                if (player->rotX < 1) {
                    player->rotX = 0;
                }
            }

            if (player->movementVelocity > 0) {
                player->movementVelocity = 0;
            }

            if (player->movementVelocity < 0) {
                player->movementVelocity += 7;
                if (player->movementVelocity > 0) {
                    player->movementVelocity = 0;
                }
            }

            player->gravityVelocity -= 6;
            if (player->gravityVelocity < -79) {
                player->gravityVelocity = -80;
            }

            player->finePos.vx += (ushort)player->gravityVelocity * (ushort)player->gravityDir.vx + (ushort)player->movementVelocity * (ushort)player->facingDir.vx;
            player->finePos.vy += (ushort)player->gravityVelocity * (ushort)player->gravityDir.vy + (ushort)player->movementVelocity * (ushort)player->facingDir.vy;
            player->finePos.vz += (ushort)player->gravityVelocity * (ushort)player->gravityDir.vz + (ushort)player->movementVelocity * (ushort)player->facingDir.vz;
            break;

        case JUMPING_INPLACE:
            player->jumpingOrViewportRotationTimer--;

            if (player->jumpingOrViewportRotationTimer > 0) {
                player->rotX = 0;
                player->jumpVec.vz = player->jumpdataPtr[3] - player->jumpdataPtr[0];
                player->jumpdataPtr++;
                player->jumpVec.vx = player->jumpdataPtr[3] - player->jumpdataPtr[0];
                player->jumpdataPtr++;
                player->jumpVec.vy = player->jumpdataPtr[3] - player->jumpdataPtr[0];
                player->jumpdataPtr++;

                player->jumpVec.vx = 0;
                player->jumpVec.vy = 0;

                ApplyMatrixSV(&player->matrix_d4, &player->jumpVec, &player->jumpVec);

                player->finePos.vx += player->jumpVec.vx;
                player->finePos.vy += player->jumpVec.vy;
                player->finePos.vz += player->jumpVec.vz;

                if (player->jumpingInplaceOnTopOfMovingPlatform) {
                    player->finePos.vx += entityData[player->movingPlatformEntityIdStandingOn + 21];
                    player->finePos.vy += entityData[player->movingPlatformEntityIdStandingOn + 22];
                    player->finePos.vz += entityData[player->movingPlatformEntityIdStandingOn + 23];
                }
                break;
            }
            player->onGround = 1;
            player->jumpingInplaceOnTopOfMovingPlatform = 0;
            player->howMoving0 = 0;
            player->howMoving198 = NOT_MOVING;

            if (player->surroundingBlocks[0][1][1] < 0) {
                player->howMoving198 = FALLING;
                player->onGround = 0;
                player->howMoving0 = 3;
                player->movementVelocity = 0;
                player->gravityVelocity = player->svec_144.vy;
            } else {
                SndPlaySfx(102, 0, &ZERO_SVECTOR_a2dd8, 7000);
                landingSquishFrameCounter = 4;
                landingSquishMagnitudeIncrement = 187;
                landingSquishMagnitude = 0;
                landingSquishDamping = 100;
            }
            break;
    }
}

static int HandleViewportRotationStart_blockType;
#define blockType HandleViewportRotationStart_blockType
static SVECTOR HandleViewportRotationStart_cubePos;
#define cubePos HandleViewportRotationStart_cubePos
void HandleViewportRotationStart(Player* player) {
    int pad[3];
    if (player->dying)
        return;
    if (player->howMoving198 != ROLLING)
        return;

    if (IsSubpixelZBelow257(player)) {
        if (player->rollingForward == 0 || player->turnDirection != 0) {
            player->howMoving0 = 0;
            player->movementVelocity = 0;
            player->howMoving198 = -1;
            MovePlayerForward(player, 256);
            return;
        }
    }

    if (!IsRollingForwardBlocked(player) && player->subpixelPositionOnCube.vz > 255) {
        if (player->faceTypePlayerStandingOn == OBJ_ICE_PATCH)
            return;

        if (IsSubpixelZBelow257(player)) {
            MovePlayerForward(player, 256);
        } else {
            player->finePos.vx -= player->movementVelocity * player->facingDir.vx;
            player->finePos.vy -= player->movementVelocity * player->facingDir.vy;
            player->finePos.vz -= player->movementVelocity * player->facingDir.vz;
        }
        player->howMoving0 = 0;
        player->movementVelocity = 0;
        player->howMoving198 = NOT_MOVING;
        return;
    }

    if (player->surroundingBlocks[1][2][1] > -1 && player->subpixelPositionOnCube.vz > 412) {
        cubePos.vx = (player->finePos.vx - player->gravityDir.vx * 356 + (player->facingDir.vx << 9) + 256) >> 9;
        cubePos.vy = (player->finePos.vy - player->gravityDir.vy * 356 + (player->facingDir.vy << 9) + 256) >> 9;
        cubePos.vz = (player->finePos.vz - player->gravityDir.vz * 356 + (player->facingDir.vz << 9) + 256) >> 9;

        if (shouldMarkCubesVisited) {
            SetCubeVisited(cubePos.vx, cubePos.vy, cubePos.vz, 1);
        }

        player->field100_0x1ac = -1;
        player->viewpointRotationTimer = 14;
        player->viewpointRotationAngleIncrement = -73;
        MovePlayerForward(player, 412);
        player->turningWhereNextFrame = 2;
    }

    blockType = player->surroundingBlocks[0][0][1];
    if (blockType != 0) {
        blockType = entityData[(blockType - 5) * 128];
    } else {
        blockType = 0;
    }

    if (player->surroundingBlocks[0][1][1] < 0 && player->surroundingBlocks[0][0][0] < 0 &&
        player->surroundingBlocks[0][0][2] < 0 &&
        (player->svec_154.vz + player->movementVelocity > 511) &&
        player->isRotatingViewport == 0) {

        player->howMoving198 = ROTATING;
        player->howMoving0 = 3;
        player->field100_0x1ac = 1;
        player->viewpointRotationTimer = 11;
        player->gravityVelocity = 0;
        player->jumpingOrViewportRotationTimer = 0;
        player->viewpointRotationAngleIncrement = 93;
        MovePlayerForward(player, 0);

        playerFinePosWithSquish.vx = -player->gravityDir.vx;
        playerFinePosWithSquish.vy = -player->gravityDir.vy;
        playerFinePosWithSquish.vz = -player->gravityDir.vz;

        player->gravityDir = player->facingDir;
        player->facingDir = playerFinePosWithSquish;
    }
}
#undef blockType
#undef cubePos

void CheckPlayerJumpingStuff(Player* player) {
    if (player->dying) {
        return;
    }
    switch (player->howMoving198) {
        case JUMPING_FORWARD:
            if (player->jumpingOrViewportRotationTimer < 12) {
                if (HandleMovingPlatforms(player)) {
                    return;
                }
                if (CheckIfPlayerLanded(player)) {
                    return;
                }
            }
            if (player->jumpingOrViewportRotationTimer <= 0) {
                return;
            }
            if (CheckForPlayerWallHit(player)) {
                if (!player->jumpingOnMovingPlatform) {
                    return;
                }
                if (player->jumpingOrViewportRotationTimer < 12) {
                    return;
                }
            }
            if (player->jumpingOnMovingPlatform) {
                return;
            }
            break;

        case FALLING:
            if (HandleMovingPlatforms(player)) {
                return;
            }
            if (CheckIfPlayerLanded(player)) {
                return;
            }
            if (player->subpixelPositionOnCube.vz > 410 && player->surroundingBlocks[1][2][1] > -1) {
                MovePlayerForward(player, 409);
            }
            if (player->subpixelPositionOnCube.vz > 101) {
                return;
            }
            if (player->surroundingBlocks[1][0][1] < 0) {
                return;
            }
            MovePlayerForward(player, 103);
            return;

        case JUMPING_INPLACE:
            if (player->jumpingOrViewportRotationTimer < 12 && HandleMovingPlatforms(player) != 0) {
                return;
            }
            if (player->jumpingOrViewportRotationTimer < 1) {
                return;
            }
            break;

        default:
            return;
    }
    CheckPlayerHitCeiling(player);
}

static short CheckForPlayerWallHit_blockIndex;
#define blockIndex CheckForPlayerWallHit_blockIndex
static short CheckForPlayerWallHit_sideOffset;
#define sideOffset CheckForPlayerWallHit_sideOffset
static SVECTOR CheckForPlayerWallHit_arrowVec;
#define arrowVec CheckForPlayerWallHit_arrowVec
int CheckForPlayerWallHit(Player* player) {
    if (player->subpixelPositionOnCube.vz >= 412 && player->svec_144.vz >= 0) {
        if (player->surroundingBlocks[1][2][1] < 0 && (player->subpixelPositionOnCube.vy < 412 || player->surroundingBlocks[2][2][1] < 0) && (player->subpixelPositionOnCube.vy >= 100 || player->surroundingBlocks[0][2][1] < 0)) {
            blockIndex = player->surroundingBlocks[0][1][1];
            sideOffset = (blockIndex - 5) * 128 + GetRotationIndexFromVector(player->gravityDir) * 16;

            if (blockIndex < 5)
                return 0;

            if (entityData[(blockIndex - 5) * 128] != 0)
                return 0;
            if (entityData[sideOffset + 1] != OBJ_ARROW)
                return 0;

            GetVectorBasedOnTwoDirs(GetRotationIndexFromVector(player->gravityDir), entityData[sideOffset + 2], &arrowVec);

            if (player->facingDir.vx == arrowVec.vx &&
                player->facingDir.vy == arrowVec.vy &&
                player->facingDir.vz == arrowVec.vz) {
                return 0;
            }
        }
        SndPlaySfx(SFX_BALL_BOUNCE, 0, &ZERO_SVECTOR_a2dd8, 7000);
        Vibrate99(0, 200, 3);

        player->howMoving198 = FALLING;
        player->howMoving0 = 3;

        player->finePos.vx -= player->svec_144.vz * 2 * player->facingDir.vx;
        player->finePos.vy -= player->svec_144.vz * 2 * player->facingDir.vy;
        player->finePos.vz -= player->svec_144.vz * 2 * player->facingDir.vz;

        if (player->svec_144.vy > 0) {
            player->gravityVelocity = -10;
        }

        player->rotX = 0;
        player->longJump = 0;
        player->movementVelocity = -player->svec_144.vz;

        return 1;
    }
    return 0;
}
#undef blockIndex
#undef sideOffset
#undef arrowVec

int CheckPlayerHitCeiling(Player* player) {
    if (player->subpixelPositionOnCube.vy < 412)
        return 0;
    if (player->svec_144.vy < 0)
        return 0;

#ifdef FIX_HORRIBLE_BUGS
    if (IsPlayerOnMovingPlatform(player) ||
#else
    if (IsPlayerOnMovingPlatform() ||
#endif
        player->surroundingBlocks[2][1][1] >= 0 ||
        (player->subpixelPositionOnCube.vz >= 412 && player->surroundingBlocks[2][2][1] >= 0) ||
        (player->subpixelPositionOnCube.vz < 101 && player->surroundingBlocks[2][0][1] >= 0))
    {
        SndPlaySfx(SFX_BALL_BOUNCE, 0, &ZERO_SVECTOR_a2dd8, 7000);
        Vibrate99(0, 200, 3);

        player->finePos.vx -= player->svec_144.vy * (player->gravityDir.vx + player->gravityDir.vx);
        player->finePos.vy -= player->svec_144.vy * (player->gravityDir.vy + player->gravityDir.vy);
        player->finePos.vz -= player->svec_144.vy * (player->gravityDir.vz + player->gravityDir.vz);

        player->movementVelocity = 0;
        player->rotX = 0;
        player->jumpingInplaceOnTopOfMovingPlatform = 0;
        player->howMoving198 = FALLING;
        player->howMoving0 = 3;
        player->longJump = 0;
        player->gravityVelocity = -player->svec_144.vy;

        return 1;
    }

    return 0;
}

static int CheckIfPlayerLanded_newBlock;
#define newBlock CheckIfPlayerLanded_newBlock
static SVECTOR CheckIfPlayerLanded_newPlayerPos;
#define newPlayerPos CheckIfPlayerLanded_newPlayerPos
int CheckIfPlayerLanded(Player* player) {
    int dummy[2];
    newPlayerPos.vx = player->finePos.vx - (short)(player->gravityDir.vx * 100);
    newPlayerPos.vy = player->finePos.vy - (short)(player->gravityDir.vy * 100);
    newPlayerPos.vz = player->finePos.vz - (short)(player->gravityDir.vz * 100);

    newBlock = GetBlockAt(&newPlayerPos);

    if ((newBlock >= 5 && entityData[(newBlock - 5) * 128] == 5) || newBlock < 0) {
        return 0;
    }

    if (player->howMoving198 == JUMPING_FORWARD) {
        ResetPlayerMatrix274(player);
    }
    if (player->alreadyProcessedEntityAction != OBJ_TRANSPORTER && player->playerHasControl == 1 && !isPausedOrWaitingForRestart) {
        SndPlaySfx(SFX_BALL_BOUNCE, 0, &ZERO_SVECTOR_a2dd8, 7000);
    }
    if (player->gravityVelocity == -80) {
        Vibrate99(0, 200, 3);
        EnableScreenShake(3, 20, 2);
    }

    player->finePos.vx += player->gravityDir.vx * 100;
    player->finePos.vy += player->gravityDir.vy * 100;
    player->finePos.vz += player->gravityDir.vz * 100;

    MovePlayerDownwards(player, 100);
    player->howMoving198 = -1;
    player->onGround = 1;
    player->howMoving0 = 0;
    if (player->rollingForward) {
        player->movementVelocity = 0x28;
    }
    landingSquishFrameCounter = 4;
    landingSquishMagnitudeIncrement = 0xbb;
    player->gravityVelocity = 0;
    player->longJump = 0;
    landingSquishMagnitude = 0;
    landingSquishDamping = 100;
    UpdateSubpixelPositions(player);
    if (player->subpixelPositionOnCube.vz < 0xb4 && !player->jumping) {
        player->rollingForward = 1;
        player->forcedRollForwardTimer = 1;
        player->turnDirection = 0;
        player->movementVelocity = 0x28;
    }
    return 1;
}
#undef newBlock
#undef newPlayerPos

void SetLandingSquishVars(void) {
    int pad[2];

    landingSquishFrameCounter = 4;
    landingSquishMagnitudeIncrement = 187;
    landingSquishMagnitude = 0;
    landingSquishDamping = 100;
}

int IsRollingForwardBlocked(Player* player) {
    if (player->surroundingBlocks[0][2][1] >= 0)
        return 1;
    if (player->surroundingBlocks[0][1][0] >= 0)
        return player->faceTypePlayerStandingOn == OBJ_ICE_PATCH;
    if (player->surroundingBlocks[0][1][2] >= 0)
        return player->faceTypePlayerStandingOn == OBJ_ICE_PATCH;
    return 1;
}

int IsSubpixelZBelow257(Player* player) {
    if (player->subpixelPositionOnCube.vz >= 0x100) {
        if (player->svec_154.vz <= 0x100) {
            return 1;
        }
    }
    return 0;
}

void AutoCenterSubpixelPosition(Player* player, int amount) {
    if (player->subpixelPositionOnCube.vx < 0x100 - amount) {
        player->finePos.vx += amount * player->rightVec.vx;
        player->finePos.vy += amount * player->rightVec.vy;
        player->finePos.vz += amount * player->rightVec.vz;
    } else if (player->subpixelPositionOnCube.vx > 0x100 + amount) {
        player->finePos.vx -= amount * player->rightVec.vx;
        player->finePos.vy -= amount * player->rightVec.vy;
        player->finePos.vz -= amount * player->rightVec.vz;
    }
}

void AutoAlignJumpStartPos(Player* player, int amount) {
    if (player->subpixelPositionOnCube.vx < 0x100 - amount) {
        player->jumpStartPos.vx += amount * player->rightVec.vx;
        player->jumpStartPos.vy += amount * player->rightVec.vy;
        player->jumpStartPos.vz += amount * player->rightVec.vz;
    } else if (player->subpixelPositionOnCube.vx > 0x100 + amount) {
        player->jumpStartPos.vx -= amount * player->rightVec.vx;
        player->jumpStartPos.vy -= amount * player->rightVec.vy;
        player->jumpStartPos.vz -= amount * player->rightVec.vz;
    }
}

static int CalcPlayerMatrixesAndDrawPlayer_flattenFactor;
#define flattenFactor CalcPlayerMatrixesAndDrawPlayer_flattenFactor
static int CalcPlayerMatrixesAndDrawPlayer_subpixelY;
#define subpixelY CalcPlayerMatrixesAndDrawPlayer_subpixelY
static int CalcPlayerMatrixesAndDrawPlayer_blockOffset;
#define blockOffset CalcPlayerMatrixesAndDrawPlayer_blockOffset
static MATRIX CalcPlayerMatrixesAndDrawPlayer_animMatrix;
#define animMatrix CalcPlayerMatrixesAndDrawPlayer_animMatrix
static MATRIX CalcPlayerMatrixesAndDrawPlayer_rotMatrix;
#define rotMatrix CalcPlayerMatrixesAndDrawPlayer_rotMatrix
static MATRIX CalcPlayerMatrixesAndDrawPlayer_invRotMatrix;
#define invRotMatrix CalcPlayerMatrixesAndDrawPlayer_invRotMatrix
static MATRIX CalcPlayerMatrixesAndDrawPlayer_drawMatrix;
#define drawMatrix CalcPlayerMatrixesAndDrawPlayer_drawMatrix
static MATRIX CalcPlayerMatrixesAndDrawPlayer_dirMatrix;
#define dirMatrix CalcPlayerMatrixesAndDrawPlayer_dirMatrix
static MATRIX CalcPlayerMatrixesAndDrawPlayer_shadowMatrix;
#define shadowMatrix CalcPlayerMatrixesAndDrawPlayer_shadowMatrix
static MATRIX CalcPlayerMatrixesAndDrawPlayer_animPerspMatrix;
#define animPerspMatrix CalcPlayerMatrixesAndDrawPlayer_animPerspMatrix
static MATRIX CalcPlayerMatrixesAndDrawPlayer_ballMorphMatrix;
#define ballMorphMatrix CalcPlayerMatrixesAndDrawPlayer_ballMorphMatrix
static short CalcPlayerMatrixesAndDrawPlayer_rotIdx1;
#define rotIdx1 CalcPlayerMatrixesAndDrawPlayer_rotIdx1
static short CalcPlayerMatrixesAndDrawPlayer_rotIdx2;
#define rotIdx2 CalcPlayerMatrixesAndDrawPlayer_rotIdx2
static short CalcPlayerMatrixesAndDrawPlayer_invulCounter1;
#define invulCounter1 CalcPlayerMatrixesAndDrawPlayer_invulCounter1
static short CalcPlayerMatrixesAndDrawPlayer_invulCounter2;
#define invulCounter2 CalcPlayerMatrixesAndDrawPlayer_invulCounter2
static SVECTOR CalcPlayerMatrixesAndDrawPlayer_otherBlockPos;
#define otherBlockPos CalcPlayerMatrixesAndDrawPlayer_otherBlockPos
static SVECTOR CalcPlayerMatrixesAndDrawPlayer_screenPos;
#define screenPos CalcPlayerMatrixesAndDrawPlayer_screenPos
static SVECTOR CalcPlayerMatrixesAndDrawPlayer_transVec;
#define transVec CalcPlayerMatrixesAndDrawPlayer_transVec
static SVECTOR CalcPlayerMatrixesAndDrawPlayer_groundPos;
#define groundPos CalcPlayerMatrixesAndDrawPlayer_groundPos
static SVECTOR CalcPlayerMatrixesAndDrawPlayer_shadowVec;
#define shadowVec CalcPlayerMatrixesAndDrawPlayer_shadowVec
static SVECTOR CalcPlayerMatrixesAndDrawPlayer_dirVec;
#define dirVec CalcPlayerMatrixesAndDrawPlayer_dirVec
static SVECTOR CalcPlayerMatrixesAndDrawPlayer_scaledShadowVec;
#define scaledShadowVec CalcPlayerMatrixesAndDrawPlayer_scaledShadowVec
static SVECTOR CalcPlayerMatrixesAndDrawPlayer_camTargetPos;
#define camTargetPos CalcPlayerMatrixesAndDrawPlayer_camTargetPos
static SVECTOR CalcPlayerMatrixesAndDrawPlayer_blockPos;
#define blockPos CalcPlayerMatrixesAndDrawPlayer_blockPos
void CalcPlayerMatrixesAndDrawPlayer(Player* player) {
    int turningWhere;
    int iVar7;
    MATRIX* pMatrix1;
    MATRIX* pMatrix2;
    int xy, xy_00;
    int dirIndex;
    int uVar4, uVar5, uVar6, uVar8;
    int turningWhereTmp;
    int next;
    int val;
    int s;
    int v3, v4, v5;
    SVECTOR* pVec;

    if (player->turningWhere == 0) {
        if (player->howMoving198 == JUMPING_FORWARD) {
            SetBallShapeAndRotationWhenJumping(player);
        } else {
            SetBallShapeAndRotationWhenRollingOrIdle(player);
        }

        MatrixNormal(&player->matrix_254, &player->matrix_254);
        MatrixNormal_1(&player->matrix_254, &player->matrix_254);
        MatrixNormal_2(&player->matrix_254, &player->matrix_254);

        MatrixNormal(&player->matrix_234, &player->matrix_234);
        MatrixNormal_1(&player->matrix_234, &player->matrix_234);
        MatrixNormal_2(&player->matrix_234, &player->matrix_234);

        if (player->field119_0x1c8 == 0) {
            player->matrix_234.m[0][0] = player->rightVec.vx << 12;
            player->matrix_234.m[1][0] = player->rightVec.vy << 12;
            player->matrix_234.m[2][0] = player->rightVec.vz << 12;

            player->matrix_234.m[0][1] = -player->facingDir.vx << 12;
            player->matrix_234.m[1][1] = -player->facingDir.vy << 12;
            player->matrix_234.m[2][1] = -player->facingDir.vz << 12;

            player->matrix_234.m[0][2] = -player->gravityDir.vx << 12;
            player->matrix_234.m[1][2] = -player->gravityDir.vy << 12;
            player->matrix_234.m[2][2] = -player->gravityDir.vz << 12;
        }
    }

    if (player->turningWhere == 1) {
        transVec.vx = -256;
        transVec.vy = 0;
        transVec.vz = 0;
        RotMatrix(&transVec, &rotMatrix);
        MulMatrix0(&player->matrix_234, &rotMatrix, &player->matrix_234);
    }

    if (player->turningWhere == 3) {
        transVec.vx = 0;
        transVec.vy = 0;
        transVec.vz = -1024;
        MulMatrix0(&player->matrix_234, &player->matrix_254, &drawMatrix);
        RotMatrix(&transVec, &rotMatrix);
        MulMatrix0(&rotMatrix, &player->matrix_254, &player->matrix_254);
        TransposeMatrix(&rotMatrix, &invRotMatrix);
        MulMatrix0(&player->matrix_234, &invRotMatrix, &player->matrix_234);
        MulMatrix0(&player->matrix_234, &player->matrix_254, &drawMatrix);
    }

    if (player->turningWhere == 4) {
        transVec.vx = 0;
        transVec.vy = 0;
        transVec.vz = 1024;
        RotMatrix(&transVec, &rotMatrix);
        MulMatrix0(&rotMatrix, &player->matrix_254, &player->matrix_254);
        TransposeMatrix(&rotMatrix, &invRotMatrix);
        MulMatrix0(&player->matrix_234, &invRotMatrix, &player->matrix_234);
    }

    if (player->turningWhere == 2) {
        transVec.vx = -1024;
        transVec.vy = 0;
        transVec.vz = 0;
        RotMatrix(&transVec, &rotMatrix);
        MulMatrix0(&rotMatrix, &player->matrix_254, &player->matrix_254);
        TransposeMatrix(&rotMatrix, &invRotMatrix);
        MulMatrix0(&player->matrix_234, &invRotMatrix, &player->matrix_234);
    }

    ballMorphMatrix.m[0][0] = player->ballMorphShape + 4096;
    ballMorphMatrix.m[1][1] = player->ballMorphShape + 4096;
    ballMorphMatrix.m[2][2] = 4096 - player->ballMorphShape * 2;
    ballMorphMatrix.m[2][1] = 0;
    ballMorphMatrix.m[2][0] = 0;
    ballMorphMatrix.m[1][2] = 0;
    ballMorphMatrix.m[1][0] = 0;
    ballMorphMatrix.m[0][2] = 0;
    ballMorphMatrix.m[0][1] = 0;

    flattenFactor = player->field_2bc * 2 + player->flatteningTimer;
    if (flattenFactor > 4096) {
        flattenFactor = 4096;
        player->flatteningTimer = 4096;
    }

    animMatrix.m[2][1] = 0;
    animMatrix.m[2][0] = 0;
    animMatrix.m[1][2] = 0;
    animMatrix.m[1][0] = 0;
    animMatrix.m[0][2] = 0;
    animMatrix.m[0][1] = 0;
    animMatrix.m[1][1] = (short)flattenFactor + 4096;
    animMatrix.m[0][0] = (short)flattenFactor + 4096;
    animMatrix.m[2][2] = 4096 - (short)flattenFactor;

    MulMatrix0(&player->matrix_234, &ballMorphMatrix, &drawMatrix);
    MulMatrix0(&drawMatrix, &animMatrix, &drawMatrix);
    MulMatrix0(&drawMatrix, &player->matrix_254, &drawMatrix);

    player->matrix_294.m[0][0] = drawMatrix.m[0][0];
    player->matrix_294.m[0][1] = drawMatrix.m[0][1];
    player->matrix_294.m[0][2] = drawMatrix.m[0][2];
    player->matrix_294.m[1][0] = drawMatrix.m[1][0];
    player->matrix_294.m[1][1] = drawMatrix.m[1][1];
    player->matrix_294.m[1][2] = drawMatrix.m[1][2];
    player->matrix_294.m[2][0] = drawMatrix.m[2][0];
    player->matrix_294.m[2][1] = drawMatrix.m[2][1];
    player->matrix_294.m[2][2] = drawMatrix.m[2][2];

    TransposeMatrix(&drawMatrix, &invRotMatrix);
    MulMatrix0(&perspMatrixes[cameraIndex], &drawMatrix, &drawMatrix);

    playerFinePosWithSquish.vx = player->finePos.vx + player->svec54.vx + (((4096 - flattenFactor - player->ballMorphShape * 2) * 100) / 4096 - 100) * player->gravityDir.vx;
    playerFinePosWithSquish.vy = player->finePos.vy + player->svec54.vy + (((4096 - flattenFactor - player->ballMorphShape * 2) * 100) / 4096 - 100) * player->gravityDir.vy;
    playerFinePosWithSquish.vz = player->finePos.vz + player->svec54.vz + (((4096 - flattenFactor - player->ballMorphShape * 2) * 100) / 4096 - 100) * player->gravityDir.vz;

    ApplyMatrixSV(&perspMatrixes[cameraIndex], &playerFinePosWithSquish, &screenPos);

    shadowVec.vx = (-4 * player->perspMatrix.m[1][0] - 2 * player->perspMatrix.m[2][0]) - player->perspMatrix.m[0][0];
    shadowVec.vy = (-4 * player->perspMatrix.m[1][1] - 2 * player->perspMatrix.m[2][1]) - player->perspMatrix.m[0][1];
    shadowVec.vz = (-4 * player->perspMatrix.m[1][2] - 2 * player->perspMatrix.m[2][2]) - player->perspMatrix.m[0][2];

    VectorNormalSS(&shadowVec, &shadowVec);

    camTargetPos = player->finePos;
    camTargetPos = playerFinePosWithSquish;

    scaledShadowVec.vx = (shadowVec.vx * 100) / 4096;
    scaledShadowVec.vy = (shadowVec.vy * 100) / 4096;
    scaledShadowVec.vz = (shadowVec.vz * 100) / 4096;

    MulMatrix0(&ballMorphMatrix, &animMatrix, &animPerspMatrix);
    MulMatrix0(&animPerspMatrix, &player->perspMatrix, &animPerspMatrix);

    drawMatrix.t[0] = screenPos.vx + perspMatrixes[cameraIndex].t[0];
    drawMatrix.t[1] = screenPos.vy + perspMatrixes[cameraIndex].t[1];
    drawMatrix.t[2] = screenPos.vz + perspMatrixes[cameraIndex].t[2];

    if (player->fireTimer > 2200)
        player->fireTimer = 2200;

    if (player->startTurningTo == 0) {
        player->facingDirBeforeStartingTurning = player->facingDir;
    }

    if ((ushort)(player->subpixelPositionOnCube.vz - 100) >= 313 && player->startTurningTo != 0 && player->turningTimer <= 13) {
        blockPos.vx = (player->finePos.vx + 256 + player->facingDir.vx * 150 - player->gravityDir.vx * 400) >> 9;
        blockPos.vy = (player->finePos.vy + 256 + player->facingDir.vy * 150 - player->gravityDir.vy * 400) >> 9;
        blockPos.vz = (player->finePos.vz + 256 + player->facingDir.vz * 150 - player->gravityDir.vz * 400) >> 9;

        otherBlockPos.vx = (player->finePos.vx + 256 - player->facingDir.vx * 150 - player->gravityDir.vx * 400) >> 9;
        otherBlockPos.vy = (player->finePos.vy + 256 - player->facingDir.vy * 150 - player->gravityDir.vy * 400) >> 9;
        otherBlockPos.vz = (player->finePos.vz + 256 - player->facingDir.vz * 150 - player->gravityDir.vz * 400) >> 9;

        dirVec.vx = -player->facingDir.vx;
        dirVec.vy = -player->facingDir.vy;
        dirVec.vz = -player->facingDir.vz;
    } else {
        blockPos.vx = (player->finePos.vx + 256 + player->facingDirBeforeStartingTurning.vx * 150 - player->gravityDir.vx * 400) >> 9;
        blockPos.vy = (player->finePos.vy + 256 + player->facingDirBeforeStartingTurning.vy * 150 - player->gravityDir.vy * 400) >> 9;
        blockPos.vz = (player->finePos.vz + 256 + player->facingDirBeforeStartingTurning.vz * 150 - player->gravityDir.vz * 400) >> 9;

        otherBlockPos.vx = (player->finePos.vx + 256 - player->facingDirBeforeStartingTurning.vx * 150 - player->gravityDir.vx * 400) >> 9;
        otherBlockPos.vy = (player->finePos.vy + 256 - player->facingDirBeforeStartingTurning.vy * 150 - player->gravityDir.vy * 400) >> 9;
        otherBlockPos.vz = (player->finePos.vz + 256 - player->facingDirBeforeStartingTurning.vz * 150 - player->gravityDir.vz * 400) >> 9;

        dirVec.vx = -player->facingDirBeforeStartingTurning.vx;
        dirVec.vy = -player->facingDirBeforeStartingTurning.vy;
        dirVec.vz = -player->facingDirBeforeStartingTurning.vz;
    }

    if (levelData[blockPos.vx * 1156 + blockPos.vy * 34 + blockPos.vz] < 0 ||
        (levelData[blockPos.vx * 1156 + blockPos.vy * 34 + blockPos.vz] == 3 && invisBlockVisibility.flip != 0)) {
        rotIdx1 = GetRotationIndexFromVector(dirVec);
        blockPos = otherBlockPos;
    } else {
        rotIdx1 = GetRotationIndexFromVector(player->gravityDir);
    }

    if (levelData[otherBlockPos.vx * 1156 + otherBlockPos.vy * 34 + otherBlockPos.vz] < 0 ||
        (levelData[otherBlockPos.vx * 1156 + otherBlockPos.vy * 34 + otherBlockPos.vz] == 3 && invisBlockVisibility.flip != 0)) {
        rotIdx2 = GetRotationIndexFromVector(dirVec);
        otherBlockPos = blockPos;
    } else {
        rotIdx2 = GetRotationIndexFromVector(player->gravityDir);
    }

    groundPos = player->finePos;
    subpixelY = player->subpixelPositionOnCube.vy;
    groundPos.vx -= player->gravityDir.vx * 512;
    groundPos.vy -= player->gravityDir.vy * 512;
    groundPos.vz -= player->gravityDir.vz * 512;

    turningWhereTmp = GetBlockAt(&groundPos);
    blockOffset = (turningWhereTmp - 5) * 128;

    if (blockOffset >= 0 && entityData[blockOffset] == 5) {
        subpixelY = 512;
    } else if (GetBlockAt(&groundPos) < 0 && player->onMovingPlatform == 0) {
        subpixelY = 512;
    }

    dirIndex = GetRotationIndexFromVector(player->gravityDir);

    MatrixFromDirectionIndex(&dirMatrix, 0, dirIndex, -subpixelY, &playerFinePosWithSquish);

    ballMorphMatrix.m[2][2] = 4096;
    subpixelY = 512 - subpixelY;

    MulMatrix0(&dirMatrix, &ballMorphMatrix, &dirMatrix);
    MulMatrix0(&dirMatrix, &animMatrix, &dirMatrix);

    transVec.vx = 1024;
    transVec.vz = 0;
    transVec.vy = 0;
    RotMatrix(&transVec, &rotMatrix);
    MulMatrix0(&dirMatrix, &rotMatrix, &dirMatrix);
    MulMatrix0(&perspMatrixes[cameraIndex], &dirMatrix, &dirMatrix);

    shadowMatrix.m[0][0] = shadowVec.vx;
    shadowMatrix.m[0][1] = shadowVec.vy;
    shadowMatrix.m[0][2] = shadowVec.vz;

    TransposeMatrix(&invRotMatrix, &rotMatrix);
    MulMatrix0(&shadowMatrix, &rotMatrix, &shadowMatrix);

    ApplyMatrixSV(&perspMatrixes[cameraIndex], &playerFinePosWithSquish, &screenPos);

    dirMatrix.t[0] = screenPos.vx + perspMatrixes[cameraIndex].t[0];
    dirMatrix.t[1] = screenPos.vy + perspMatrixes[cameraIndex].t[1];
    dirMatrix.t[2] = screenPos.vz + perspMatrixes[cameraIndex].t[2];

    camTargetPos.vx += scaledShadowVec.vx;
    camTargetPos.vy += scaledShadowVec.vy;
    camTargetPos.vz += scaledShadowVec.vz;

    ballColorR = ((player->fireTimer << 12) / 400 + 4096 - ((player->acidTimer << 12) / 2000)) - (player->iceColorChangeTimer << 12) / 2000;
    ballColorG = ((player->acidTimer << 12) / 400 + 4096 - ((player->fireTimer << 12) / 2000)) - (player->iceColorChangeTimer << 12) / 2000;
    ballColorB = ((player->iceColorChangeTimer << 12) / 400 + 4096 - ((player->acidTimer << 12) / 2000)) - (player->fireTimer << 12) / 2000;

    if (player->invulnerabilityTimer != -1) {
        invulCounter1 = (invulCounter1 + 100) % 8192;

        if (invulCounter1 < 4096) {
            ballColorR += rsin(invulCounter1 % 4096) * 3;
        }
        if (invulCounter1 > 2048 && invulCounter1 < 2048 + 4096) {
            ballColorG += rsin((invulCounter1 - 2048) % 4096) * 3;
        }
        if (invulCounter1 > 4096) {
            ballColorB += rsin((invulCounter1 - 4096) % 4096) * 3;
        }
    }

    if (player->ballBlinking) {
        invulCounter2 = (invulCounter2 + 1024) % 4096;
        ballColorR = ballColorG = ballColorB = rsin(invulCounter2) + 4096;
    }

    if (player->faceTypePlayerStandingOn == 96) {
        subpixelY = (subpixelY * entityData[player->specialBlockIndexPlayerIsStandingOn + 2]) / 512;
    }

    CreatePlayerDispList(&drawMatrix, 256, ballTextureIndex, 0, ballColorR, ballColorG, ballColorB, 0,
                         blockPos.vx, blockPos.vy, blockPos.vz, rotIdx1,
                         otherBlockPos.vx, otherBlockPos.vy, otherBlockPos.vz, rotIdx2,
                         &dirMatrix, (subpixelY * 128) / 412, 0,
                         &shadowMatrix, 2048, 4000, 0, 0, 178, &camTargetPos);

    player->turningWhere = player->turningWhereNextFrame;
}
#undef flattenFactor
#undef subpixelY
#undef blockOffset
#undef animMatrix
#undef rotMatrix
#undef invRotMatrix
#undef drawMatrix
#undef dirMatrix
#undef shadowMatrix
#undef animPerspMatrix
#undef ballMorphMatrix
#undef rotIdx1
#undef rotIdx2
#undef invulCounter1
#undef invulCounter2
#undef otherBlockPos
#undef screenPos
#undef transVec
#undef groundPos
#undef shadowVec
#undef dirVec
#undef scaledShadowVec
#undef camTargetPos
#undef blockPos

void ClearJumpSquish(Player* player) {
    jumpSquishMagnitudeIncrement = 0;
}

void SetBallShapeAndRotationWhenJumping(Player* player) {
    RotMatrixX(player->rotX * -6, &player->matrix_254);

    if (player->field_2bc > -750 && player->jumpingOrViewportRotationTimer > 12) {
        player->field_2bc += jumpSquishMagnitudeIncrement;
    }

    if (player->jumpingOrViewportRotationTimer < -4) {
        landingSquishFrameCounter--;
        player->field_2bc += landingSquishMagnitudeIncrement;
        if (player->field_2bc > 0) {
            player->field_2bc = 0;
            landingSquishMagnitude += landingSquishMagnitudeIncrement;
            player->ballMorphShape = landingSquishMagnitude;
        }
    }
}

static short SetBallShapeAndRotationWhenRollingOrIdle_squishMagnitude;
#define squishMagnitude SetBallShapeAndRotationWhenRollingOrIdle_squishMagnitude
static short SetBallShapeAndRotationWhenRollingOrIdle_squishPhase;
#define squishPhase SetBallShapeAndRotationWhenRollingOrIdle_squishPhase
void SetBallShapeAndRotationWhenRollingOrIdle(Player* player) {
    RotMatrixX(player->rotX * -6, &player->matrix_254);
    if (landingSquishFrameCounter > 0) {
        landingSquishFrameCounter--;
        landingSquishDamping = 100;
        landingSquishMagnitude += landingSquishMagnitudeIncrement;
    } else if (landingSquishMagnitude > 0) {
        landingSquishDamping -= 10;
        if (landingSquishDamping < 20) {
            landingSquishDamping = 20;
        }
        landingSquishMagnitude -= landingSquishDamping;
        if (landingSquishMagnitude < 0) {
            landingSquishMagnitude = 0;
        }
    } else {
        squishPhase = (squishPhase + (5000 - levelTimeLeft) / 40 + 40) % 4096;
        squishMagnitude = (rsin(squishPhase) * 200) / 4096;
    }
    player->ballMorphShape = squishMagnitude + landingSquishMagnitude;
}
#undef squishMagnitude
#undef squishPhase

void ResetPlayerMatrix274(Player* player) {
    MulMatrix0(&player->matrix_274, &player->matrix_254, &player->matrix_254);
    player->matrix_274.m[2][1] = 0;
    player->matrix_274.m[2][0] = 0;
    player->matrix_274.m[1][2] = 0;
    player->matrix_274.m[1][0] = 0;
    player->matrix_274.m[0][2] = 0;
    player->matrix_274.m[0][1] = 0;
    player->matrix_274.m[2][2] = 0x1000;
    player->matrix_274.m[1][1] = 0x1000;
    player->matrix_274.m[0][0] = 0x1000;
}
