#include "common.h"

int IsRollingForwardBlocked(Player *player) {
    if (player->surroundingBlocks[0][2][1] >= 0) return 1;
    if (player->surroundingBlocks[0][1][0] >= 0) return player->faceTypePlayerStandingOn == 2;
    if (player->surroundingBlocks[0][1][2] >= 0) return player->faceTypePlayerStandingOn == 2;
    return 1;
}
