#include "common.h"

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
