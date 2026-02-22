#include "common.h"

short DAT_000a4374 = 0;
short landingSquishDamping;
short landingSquishFrameCounter;
short landingSquishMagnitude;
short landingSquishMagnitudeIncrement;
short *initJumpTimerPtr;
extern short *ggiPart5JumpAnimData;

void ResetPlayerVars(Player *player) {
    short* anim = ggiPart5JumpAnimData;
    short vx = player->finePos.vx;
    short vy = player->finePos.vy;
    short vz = player->finePos.vz;

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
    initJumpTimerPtr = anim;
    player->svec_184.vx = vx;
    player->svec_184.vy = vy;
    player->svec_184.vz = vz;

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

INCLUDE_ASM("asm/nonmatchings/player_movement", SetVec184ToVec54);

INCLUDE_ASM("asm/nonmatchings/player_movement", StartMovementIfNeeded);

INCLUDE_ASM("asm/nonmatchings/player_movement", StartJumpingForward);

INCLUDE_ASM("asm/nonmatchings/player_movement", StartRollingForward);

INCLUDE_ASM("asm/nonmatchings/player_movement", StartJumpingInplace);

INCLUDE_ASM("asm/nonmatchings/player_movement", TurnRight);

INCLUDE_ASM("asm/nonmatchings/player_movement", TurnLeft);

INCLUDE_ASM("asm/nonmatchings/player_movement", ProcessMovement);

INCLUDE_ASM("asm/nonmatchings/player_movement", HandleViewportRotationStart);

INCLUDE_ASM("asm/nonmatchings/player_movement", CheckPlayerJumpingStuff);

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

INCLUDE_ASM("asm/nonmatchings/player_movement", IsRollingForwardBlocked);

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

void ClearA4374(void) {
    DAT_000a4374 = 0;
}

void SetBallShapeAndRotationWhenJumping(Player *player) {
    int rot;

    rot = player->rotX * -6;
    RotMatrixX(rot, &player->matrix_254);

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
