#include "common.h"

INCLUDE_ASM("asm/nonmatchings/player_movement", ResetPlayerVars);

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

INCLUDE_ASM("asm/nonmatchings/player_movement", SetLandingSquishVars);

INCLUDE_ASM("asm/nonmatchings/player_movement", IsRollingForwardBlocked);

INCLUDE_ASM("asm/nonmatchings/player_movement", IsSubpixelZBelow257);

INCLUDE_ASM("asm/nonmatchings/player_movement", AutoCenterSubpixelPosition);

INCLUDE_ASM("asm/nonmatchings/player_movement", AutoAlignJumpStartPos);

INCLUDE_ASM("asm/nonmatchings/player_movement", CalcPlayerMatrixesAndDrawPlayer);

INCLUDE_ASM("asm/nonmatchings/player_movement", ClearA4374);

INCLUDE_ASM("asm/nonmatchings/player_movement", SetBallShapeAndRotationWhenJumping);

INCLUDE_ASM("asm/nonmatchings/player_movement", SetBallShapeAndRotationWhenRollingOrIdle);

INCLUDE_ASM("asm/nonmatchings/player_movement", ResetPlayerMatrix274);
