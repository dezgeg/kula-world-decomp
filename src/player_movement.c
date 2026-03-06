#include "common.h"

short idleSquishSinPhase;
short pad_000a4352;
short idleSquishMagnitude;
short pad_000a4356;
static SVECTOR SVECTOR_000a4358;
static SVECTOR SVECTOR_000a4360;
static SVECTOR SVECTOR_000a4368;
short *initJumpTimerPtr;
short DAT_000a4374;
short pad_000a4376;
short landingSquishMagnitudeIncrement;
short pad_000a437a;
short landingSquishMagnitude;
short pad_000a437e;
short landingSquishFrameCounter;
short pad_000a4382;
unsigned short landingSquishDamping;
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
extern u8 * entityData;
extern short isPausedOrWaitingForRestart;

int tempNewBlock;
static SVECTOR tempNewPlayerPos;

extern int CheckForPlayerWallHit(Player * player);
extern int HandleMovingPlatforms(Player * player);
extern void ClearA4374(Player *player);
extern void EnableTurningMotionBlur(void);
extern void FUN_00031288(Player * player);
extern void JumpingOnMovingPlatform(Player *player);
extern void MovePlayerForward(Player * player, int delta);

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
        if (IsRollingForwardBlocked(player) != 0 || player->subpixelPositionOnCube.vz < 256) {
            player->howMoving0 = 2;
            player->howMoving198 = ROLLING;
            player->onGround = 0;
        }
    }
}

void StartJumpingInplace(Player *player) {
    player->howMoving198 = JUMPING_INPLACE;
    if (player->onMovingPlatform != 0) {
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

INCLUDE_ASM("asm/nonmatchings/player_movement", ProcessMovement);

INCLUDE_ASM("asm/nonmatchings/player_movement", HandleViewportRotationStart);

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

INCLUDE_ASM("asm/nonmatchings/player_movement", CheckForPlayerWallHit);

INCLUDE_ASM("asm/nonmatchings/player_movement", FUN_00031288);

int CheckIfPlayerLanded(Player *player) {
    int dummy[2];
    tempNewPlayerPos.vx = player->finePos.vx - (short)(player->gravityDir.vx * 100);
    tempNewPlayerPos.vy = player->finePos.vy - (short)(player->gravityDir.vy * 100);
    tempNewPlayerPos.vz = player->finePos.vz - (short)(player->gravityDir.vz * 100);
    
    tempNewBlock = GetBlockAt(&tempNewPlayerPos);
    
    if ((tempNewBlock >= 5 && *(short *)(entityData + tempNewBlock * 256 - 0x500) == 5) || tempNewBlock < 0) {
        return 0;
    }
    
    if (player->howMoving198 == 1) {
        ResetPlayerMatrix274(player);
    }
    if (player->alreadyProcessedEntityAction != 5 && player->playerHasControl == 1 && isPausedOrWaitingForRestart == 0) {
        SndPlaySfx(0x66, 0, &SVECTOR_000a2dd8, 7000);
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
    if (player->rollingForward != 0) {
        player->movementVelocity = 0x28;
    }
    landingSquishFrameCounter = 4;
    landingSquishMagnitudeIncrement = 0xbb;
    player->gravityVelocity = 0;
    player->longJump = 0;
    landingSquishMagnitude = 0;
    landingSquishDamping = 100;
    UpdateSubpixelPositions(player);
    if (player->subpixelPositionOnCube.vz < 0xb4 && player->jumping == 0) {
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
    RotMatrixX(-(player->rotX * 6), &player->matrix_254);
    if (landingSquishFrameCounter > 0) {
        landingSquishFrameCounter--;
        landingSquishDamping = 100;
        landingSquishMagnitude += landingSquishMagnitudeIncrement;
    } else if (landingSquishMagnitude > 0) {
        landingSquishDamping -= 10;
        if ((short)landingSquishDamping < 20) {
            landingSquishDamping = 20;
        }
        landingSquishMagnitude -= landingSquishDamping;
        if (landingSquishMagnitude < 0) {
            landingSquishMagnitude = 0;
        }
    } else {
        int temp = (5000 - levelTimeLeft) / 40;
        idleSquishSinPhase = (idleSquishSinPhase + temp + 40) % 4096;
        idleSquishMagnitude = (rsin(idleSquishSinPhase) * 200) / 4096;
    }
    player->ballMorphShape = (ushort)idleSquishMagnitude + (ushort)landingSquishMagnitude;
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
