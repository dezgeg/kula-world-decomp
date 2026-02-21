#include "common.h"

static SVECTOR tmpSvector;

extern int GetBlockAt(SVECTOR *coord);
extern int GetRotationIndexFromVector(SVECTOR v);
extern int FUN_000403ec(int blockType, int rotationIndex);

int FUN_0003da64(Enemy *enemy) {
    int blockType;
    int rotationIndex;
    int res;
    
    tmpSvector.vx = enemy->pos.vx + (enemy->normalVec.vx * -400) + (enemy->field1_0x8.vx * 0x200);
    tmpSvector.vy = enemy->pos.vy + (enemy->normalVec.vy * -400) + (enemy->field1_0x8.vy * 0x200);
    tmpSvector.vz = enemy->pos.vz + (enemy->normalVec.vz * -400) + (enemy->field1_0x8.vz * 0x200);

    blockType = GetBlockAt(&tmpSvector);
    rotationIndex = GetRotationIndexFromVector(enemy->normalVec);
    res = FUN_000403ec(blockType, rotationIndex);

    return res == 1;
}
