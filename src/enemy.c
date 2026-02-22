#include "common.h"

int collI;
int enemyPlayerDistSq;
int numEnemies;
extern Enemy enemies[];

SVECTOR tmpEnemyPos;
SVECTOR SVECTOR_000a4830 = {0};
SVECTOR SVECTOR_000a4838 = {0};

extern int GetBlockAt(SVECTOR *coord);
extern int GetRotationIndexFromVector(SVECTOR v);
int FUN_000403ec(int blockType, int rotationIndex);

INCLUDE_ASM("asm/nonmatchings/enemy", InitEnemies);

INCLUDE_ASM("asm/nonmatchings/enemy", UpdateEnemies);

INCLUDE_ASM("asm/nonmatchings/enemy", FUN_0003d758);

INCLUDE_ASM("asm/nonmatchings/enemy", FUN_0003d8b8);

void FUN_0003da18(Enemy *enemy) {
    enemy->dir.vx = -enemy->dir.vx;
    enemy->dir.vy = -enemy->dir.vy;
    enemy->dir.vz = -enemy->dir.vz;
    enemy->field1_0x8.vx = -enemy->field1_0x8.vx;
    enemy->field1_0x8.vy = -enemy->field1_0x8.vy;
    enemy->field1_0x8.vz = -enemy->field1_0x8.vz;
}

int FUN_0003da64(Enemy *enemy) {
    int blockType;
    int rotationIndex;
    int res;

    SVECTOR_000a4830.vx = enemy->pos.vx + (enemy->normalVec.vx * -400) + (enemy->field1_0x8.vx * 0x200);
    SVECTOR_000a4830.vy = enemy->pos.vy + (enemy->normalVec.vy * -400) + (enemy->field1_0x8.vy * 0x200);
    SVECTOR_000a4830.vz = enemy->pos.vz + (enemy->normalVec.vz * -400) + (enemy->field1_0x8.vz * 0x200);

    blockType = GetBlockAt(&SVECTOR_000a4830);
    rotationIndex = GetRotationIndexFromVector(enemy->normalVec);
    res = FUN_000403ec(blockType, rotationIndex);

    return res == 1;
}

INCLUDE_ASM("asm/nonmatchings/enemy", FUN_0003db64);

int FUN_0003dbe0(Enemy *e) {
    int blockType;
    int rotationIndex;
    int res;

    SVECTOR_000a4838.vx = e->pos.vx + (e->normalVec.vx * -400) - (e->field1_0x8.vx * 0x200);
    SVECTOR_000a4838.vy = e->pos.vy + (e->normalVec.vy * -400) - (e->field1_0x8.vy * 0x200);
    SVECTOR_000a4838.vz = e->pos.vz + (e->normalVec.vz * -400) - (e->field1_0x8.vz * 0x200);

    blockType = GetBlockAt(&SVECTOR_000a4838);
    rotationIndex = GetRotationIndexFromVector(e->normalVec);
    res = FUN_000403ec(blockType, rotationIndex);

    return res == 1;
}

void FUN_0003dce0(Enemy *enemy) {
    tmpEnemyPos = enemy->dir;
    enemy->dir.vx = -enemy->field1_0x8.vx;
    enemy->dir.vy = -enemy->field1_0x8.vy;
    enemy->dir.vz = -enemy->field1_0x8.vz;
    enemy->field1_0x8 = tmpEnemyPos;
}

int IsCollidingWithEnemy(SVECTOR pos) {
    int dx, dy, dz;

    for (collI = 0; collI < numEnemies; collI++) {
        dx = (int)pos.vx - (int)enemies[collI].pos.vx;
        dy = (int)pos.vy - (int)enemies[collI].pos.vy;
        dz = (int)pos.vz - (int)enemies[collI].pos.vz;

        enemyPlayerDistSq = dx * dx + dy * dy + dz * dz;

        if (enemies[collI].enemyType == 53) {
            if (enemyPlayerDistSq < 16200) {
                return 1;
            }
        } else if (enemyPlayerDistSq < 45000) {
            return 1;
        }
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/enemy", Noop4);

INCLUDE_ASM("asm/nonmatchings/enemy", ProcessEnemies);

INCLUDE_ASM("asm/nonmatchings/enemy", InitEnemy);

INCLUDE_ASM("asm/nonmatchings/enemy", FUN_000403ec);

INCLUDE_ASM("asm/nonmatchings/enemy", FUN_00040490);
