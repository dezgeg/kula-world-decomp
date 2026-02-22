#include "common.h"

short DAT_000a4374;
short landingSquishDamping;
short landingSquishFrameCounter;
short landingSquishMagnitude;
short landingSquishMagnitudeIncrement;
short *initJumpTimerPtr;
extern short *ggiPart5JumpAnimData;

extern int CheckForPlayerWallHit(Player * player);
extern int CheckIfPlayerLanded(Player * player);
extern int HandleMovingPlatforms(Player * player);
extern void ClearA4374(Player *player);
extern void EnableTurningMotionBlur(void);
extern void FUN_00031288(Player * player);
extern void JumpingOnMovingPlatform(Player *player);
extern void MovePlayerForward(Player * player, int delta);

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

INCLUDE_ASM("asm/nonmatchings/player_movement", StartMovementIfNeeded);

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

INCLUDE_ASM("asm/nonmatchings/player_movement", CheckIfPlayerLanded);

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

INCLUDE_ASM("asm/nonmatchings/player_movement", SetBallShapeAndRotationWhenRollingOrIdle);

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
