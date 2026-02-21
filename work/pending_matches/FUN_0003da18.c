#include "common.h"

void FUN_0003da18(Enemy *enemy) {
    short sVar1;
    
    (enemy->dir).vx = -(enemy->dir).vx;
    (enemy->dir).vy = -(enemy->dir).vy;
    sVar1 = (enemy->field1_0x8).vx;
    (enemy->dir).vz = -(enemy->dir).vz;
    (enemy->field1_0x8).vx = -sVar1;
    (enemy->field1_0x8).vy = -(enemy->field1_0x8).vy;
    (enemy->field1_0x8).vz = -(enemy->field1_0x8).vz;
}
