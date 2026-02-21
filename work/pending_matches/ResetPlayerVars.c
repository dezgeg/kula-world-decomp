#include "common.h"

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
