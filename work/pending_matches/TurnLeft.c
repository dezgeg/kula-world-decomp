#include "common.h"

extern void EnableTurningMotionBlur(void);

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
