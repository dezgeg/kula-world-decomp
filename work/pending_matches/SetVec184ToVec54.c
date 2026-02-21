#include "common.h"

void SetVec184ToVec54(Player *player) {
    player->svec_184.vx = player->finePos.vx + player->svec54.vx;
    player->svec_184.vy = player->finePos.vy + player->svec54.vy;
    player->svec_184.vz = player->finePos.vz + player->svec54.vz;
}
