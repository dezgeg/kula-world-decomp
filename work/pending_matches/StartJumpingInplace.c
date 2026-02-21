#include "common.h"

extern void JumpingOnMovingPlatform(Player *player);
short *initJumpTimerPtr;

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
