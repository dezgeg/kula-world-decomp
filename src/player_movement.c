#include "common.h"

short idleSquishSinPhase;
short idleSquishMagnitude;
static SVECTOR SVECTOR_000a4358;
static SVECTOR SVECTOR_000a4360;
static SVECTOR SVECTOR_000a4368;
short *initJumpTimerPtr;
short DAT_000a4374;
short landingSquishMagnitudeIncrement;
short landingSquishMagnitude;
short landingSquishFrameCounter;
short landingSquishDamping;
extern short *ggiPart5JumpAnimData;
extern int gameMode;
extern int levelTimeLeft;

extern void EnableScreenShake(int param_1, int param_2, int param_3);
extern int GetBlockAt(SVECTOR * coord);
extern void MovePlayerDownwards(Player * player, int param_2);
extern void ResetPlayerMatrix274(Player * player);
extern void SndPlaySfx(int sfx, int tag, SVECTOR * dir, int volume);
extern void UpdateSubpixelPositions(Player * player);
extern void Vibrate99(byte magnitude1, byte magnitude2, int count);
extern SVECTOR SVECTOR_000a2dd8;
extern short* entityData;
extern short isPausedOrWaitingForRestart;
extern int GetRotationIndexFromVector(SVECTOR vec);
extern void GetVectorBasedOnTwoDirs(int dir1, int dir2, SVECTOR * param_3);

static SVECTOR cubePlayerIsOn;
static int D_000A41B0;
static short DAT_000a41b4;
static short DAT_000a41b8;
static SVECTOR SVECTOR_000a41bc;
static SVECTOR tempNewPlayerPos;
int tempNewBlock;

extern int HandleMovingPlatforms(Player * player);
extern void ClearA4374(Player *player);
extern void EnableTurningMotionBlur(void);
extern int FUN_0003382c(); // XXX: this should take Player* player
extern void JumpingOnMovingPlatform(Player *player);
extern void MovePlayerForward(Player * player, int delta);
extern void SetCubeVisited(int x, int y, int z, int visitType);
extern int shouldMarkCubesVisited;
extern int IsSubpixelZBelow257(Player * player);

void StartJumpingForward(Player *player);
void StartJumpingInplace(Player *player);
void StartRollingForward(Player *player);
void TurnLeft(Player *player);
void TurnRight(Player *player);


void ResetPlayerVars(Player *player) {
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

void SetVec184ToVec54(Player *player) {
    player->svec_184.vx = player->finePos.vx + player->svec54.vx;
    player->svec_184.vy = player->finePos.vy + player->svec54.vy;
    player->svec_184.vz = player->finePos.vz + player->svec54.vz;
}

void StartMovementIfNeeded(Player *player) {
    if (gameMode != 1) {
        if (gameMode < 2) {
            if (gameMode != 0) return;
        } else if (gameMode != 2) {
            return;
        }

        if (player->howMoving198 == JUMPING_FORWARD) return;
        if (player->howMoving198 == JUMPING_INPLACE) {
            if (player->turnDirection == 1) TurnLeft(player);
            if (player->turnDirection == -1) TurnRight(player);
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
    } else {
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
    }
}

void StartJumpingForward(Player *player) {
    if (player->surroundingBlocks[0][1][1] >= 0) {
        player->onGround = 0;
        player->howMoving198 = JUMPING_FORWARD;
        if (player->onMovingPlatform != 0) {
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
        ClearA4374(player);
    }
}

void StartRollingForward(Player *player) {
    if (player->surroundingBlocks[0][1][1] >= 0) {
        if (IsRollingForwardBlocked(player) || player->subpixelPositionOnCube.vz < 256) {
            player->howMoving0 = 2;
            player->howMoving198 = ROLLING;
            player->onGround = 0;
        }
    }
}

void StartJumpingInplace(Player *player) {
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

void TurnRight(Player *player) {
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

void TurnLeft(Player *player) {
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

void ProcessMovement(Player *player) {
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
                    player->gravityVelocity = -player->svec_144.vy ;
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
            SndPlaySfx(102, 0, &SVECTOR_000a2dd8, 7000);
            landingSquishFrameCounter = 4;
            landingSquishMagnitudeIncrement = 187;
            landingSquishMagnitude = 0;
            landingSquishDamping = 100;
        }
        break;

    }
}

void HandleViewportRotationStart(Player *player) {
    int pad[3];
    if (player->dying != 0) return;
    if (player->howMoving198 != ROLLING) return;

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
        if (player->faceTypePlayerStandingOn == 2) return;

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
        cubePlayerIsOn.vx = (player->finePos.vx - player->gravityDir.vx * 356 + (player->facingDir.vx << 9) + 256) >> 9;
        cubePlayerIsOn.vy = (player->finePos.vy - player->gravityDir.vy * 356 + (player->facingDir.vy << 9) + 256) >> 9;
        cubePlayerIsOn.vz = (player->finePos.vz - player->gravityDir.vz * 356 + (player->facingDir.vz << 9) + 256) >> 9;

        if (shouldMarkCubesVisited) {
            SetCubeVisited(cubePlayerIsOn.vx, cubePlayerIsOn.vy, cubePlayerIsOn.vz, 1);
        }

        player->field100_0x1ac = -1;
        player->viewpointRotationTimer = 14;
        player->viewpointRotationAngleIncrement = -73;
        MovePlayerForward(player, 412);
        player->turningWhereNextFrame = 2;
    }

    D_000A41B0 = player->surroundingBlocks[0][0][1];
    if (D_000A41B0 != 0) {
        D_000A41B0 = entityData[(D_000A41B0 - 5) * 128];
    } else {
        D_000A41B0 = 0;
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

        SVECTOR_000a4358.vx = -player->gravityDir.vx;
        SVECTOR_000a4358.vy = -player->gravityDir.vy;
        SVECTOR_000a4358.vz = -player->gravityDir.vz;

        player->gravityDir = player->facingDir;
        player->facingDir = SVECTOR_000a4358;
    }
}

void CheckPlayerJumpingStuff(Player *player) {
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
            if (player->jumpingOrViewportRotationTimer < 1) {
                return;
            }
            if (CheckForPlayerWallHit(player)) {
                if (player->jumpingOnMovingPlatform == 0) {
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
            if (player->subpixelPositionOnCube.vz > 410 && player->surroundingBlocks[1][2][1] > -1)
            {
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
    FUN_00031288(player);
}

int CheckForPlayerWallHit(Player *player) {
    if (player->subpixelPositionOnCube.vz >= 412 && player->svec_144.vz >= 0) {
        if (player->surroundingBlocks[1][2][1] < 0 && (player->subpixelPositionOnCube.vy < 412 || player->surroundingBlocks[2][2][1] < 0) && (player->subpixelPositionOnCube.vy >= 100 || player->surroundingBlocks[0][2][1] < 0)) {
            DAT_000a41b4 = player->surroundingBlocks[0][1][1];
            DAT_000a41b8 = (DAT_000a41b4 - 5) * 128 + GetRotationIndexFromVector(player->gravityDir) * 16;

            if (DAT_000a41b4 < 5) return 0;

            if (entityData[(DAT_000a41b4 - 5) * 128] != 0) return 0;
            if (entityData[DAT_000a41b8 + 1] != OBJ_ARROW) return 0;

            GetVectorBasedOnTwoDirs(GetRotationIndexFromVector(player->gravityDir), entityData[DAT_000a41b8 + 2], &SVECTOR_000a41bc);

            if (player->facingDir.vx == SVECTOR_000a41bc.vx &&
                player->facingDir.vy == SVECTOR_000a41bc.vy &&
                player->facingDir.vz == SVECTOR_000a41bc.vz) {
                return 0;
            }
        }
        SndPlaySfx(SFX_BALL_BOUNCE, 0, &SVECTOR_000a2dd8, 7000);
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


int FUN_00031288(Player *player) {
    if (player->subpixelPositionOnCube.vy < 412) return 0;
    if (player->svec_144.vy < 0) return 0;

    if (FUN_0003382c() ||
        player->surroundingBlocks[2][1][1] >= 0 ||
        (player->subpixelPositionOnCube.vz >= 412 && player->surroundingBlocks[2][2][1] >= 0) ||
        (player->subpixelPositionOnCube.vz < 101 && player->surroundingBlocks[2][0][1] >= 0))
    {
        SndPlaySfx(SFX_BALL_BOUNCE, 0, &SVECTOR_000a2dd8, 7000);
        Vibrate99(0, 200, 3);

        player->finePos.vx -= player->svec_144.vy * (player->gravityDir.vx + player->gravityDir.vx);
        player->finePos.vy -= player->svec_144.vy * (player->gravityDir.vy + player->gravityDir.vy);

        player->movementVelocity = 0;
        player->rotX = 0;
        player->jumpingInplaceOnTopOfMovingPlatform = 0;
        player->howMoving198 = FALLING;
        player->howMoving0 = 3;
        player->longJump = 0;

        player->finePos.vz -= player->svec_144.vy * (player->gravityDir.vz + player->gravityDir.vz);
        player->gravityVelocity = -player->svec_144.vy;

        return 1;
    }

    return 0;
}

int CheckIfPlayerLanded(Player *player) {
    int dummy[2];
    tempNewPlayerPos.vx = player->finePos.vx - (short)(player->gravityDir.vx * 100);
    tempNewPlayerPos.vy = player->finePos.vy - (short)(player->gravityDir.vy * 100);
    tempNewPlayerPos.vz = player->finePos.vz - (short)(player->gravityDir.vz * 100);

    tempNewBlock = GetBlockAt(&tempNewPlayerPos);

    if ((tempNewBlock >= 5 && entityData[(tempNewBlock - 5) * 128] == 5) || tempNewBlock < 0) {
        return 0;
    }

    if (player->howMoving198 == 1) {
        ResetPlayerMatrix274(player);
    }
    if (player->alreadyProcessedEntityAction != 5 && player->playerHasControl == 1 && !isPausedOrWaitingForRestart) {
        SndPlaySfx(SFX_BALL_BOUNCE, 0, &SVECTOR_000a2dd8, 7000);
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

void SetLandingSquishVars(void) {
    int pad[2];

    landingSquishFrameCounter = 4;
    landingSquishMagnitudeIncrement = 187;
    landingSquishMagnitude = 0;
    landingSquishDamping = 100;
}

int IsRollingForwardBlocked(Player *player) {
    if (player->surroundingBlocks[0][2][1] >= 0)
        return 1;
    if (player->surroundingBlocks[0][1][0] >= 0)
        return player->faceTypePlayerStandingOn == 2;
    if (player->surroundingBlocks[0][1][2] >= 0)
        return player->faceTypePlayerStandingOn == 2;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/player_movement", IsSubpixelZBelow257);

void AutoCenterSubpixelPosition(Player *player, int amount) {
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

void AutoAlignJumpStartPos(Player *player, int amount) {
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

INCLUDE_ASM("asm/nonmatchings/player_movement", CalcPlayerMatrixesAndDrawPlayer);

void ClearA4374(Player *player) {
    DAT_000a4374 = 0;
}

void SetBallShapeAndRotationWhenJumping(Player *player) {
    RotMatrixX(player->rotX * -6, &player->matrix_254);

    if (player->field_2bc > -750 && player->jumpingOrViewportRotationTimer > 12) {
        player->field_2bc += DAT_000a4374;
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

void SetBallShapeAndRotationWhenRollingOrIdle(Player *player) {
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
        idleSquishSinPhase = (idleSquishSinPhase + (5000 - levelTimeLeft) / 40 + 40) % 4096;
        idleSquishMagnitude = (rsin(idleSquishSinPhase) * 200) / 4096;
    }
    player->ballMorphShape = idleSquishMagnitude + landingSquishMagnitude;
}

void ResetPlayerMatrix274(Player *player) {
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
