#include "common.h"

int IsFallingOrJumping(Player *player) {
    if (player->howMoving198 == FALLING || (uint)player->howMoving198 < ROLLING) {
        return 1;
    }
    return 0;
}
