#include "common.h"

extern SVECTOR tmpEnemyPos;

void FUN_0003dce0(Enemy *enemy) {
    tmpEnemyPos = enemy->dir;
    enemy->dir.vx = -enemy->field1_0x8.vx;
    enemy->dir.vy = -enemy->field1_0x8.vy;
    enemy->dir.vz = -enemy->field1_0x8.vz;
    enemy->field1_0x8 = tmpEnemyPos;
}
