#include "common.h"

static SVECTOR tmpSvector;

extern int GetBlockAt(SVECTOR *coord);
extern int GetRotationIndexFromVector(SVECTOR v);
extern int FUN_000403ec(int blockType, int rotationIndex);

int FUN_0003dbe0(Enemy *e) {
    int blockType;
    int rotationIndex;
    int res;
    
    tmpSvector.vx = e->pos.vx + (e->normalVec.vx * -400) - (e->field1_0x8.vx * 0x200);
    tmpSvector.vy = e->pos.vy + (e->normalVec.vy * -400) - (e->field1_0x8.vy * 0x200);
    tmpSvector.vz = e->pos.vz + (e->normalVec.vz * -400) - (e->field1_0x8.vz * 0x200);

    blockType = GetBlockAt(&tmpSvector);
    rotationIndex = GetRotationIndexFromVector(e->normalVec);
    res = FUN_000403ec(blockType, rotationIndex);

    return res == 1;
}
