#include "common.h"

int IsPlayerInAir(Player *player) {
    if (player->howMoving198 == FALLING ||
        ((uint)player->howMoving198 < ROLLING && player->jumpingOrViewportRotationTimer > 1)) {
        return 1;
    }
    return 0;
}
