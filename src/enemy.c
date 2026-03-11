#include "common.h"

int collI;
int enemyPlayerDistSq;
int numEnemies;
extern Enemy enemies[];

static SVECTOR tmpEnemyPos;
static SVECTOR SVECTOR_000a4810;
static SVECTOR SVECTOR_000a4818;
static SVECTOR SVECTOR_000a4820;
static SVECTOR SVECTOR_000a4828;
static SVECTOR SVECTOR_000a4830;
static SVECTOR SVECTOR_000a4838;

static SVECTOR SVECTOR_000a48fc;
static SVECTOR SVECTOR_000a48e4;
static SVECTOR SVECTOR_000a48ec;
static SVECTOR SVECTOR_000a48f4;

extern int GetBlockAt(SVECTOR* coord);
extern int GetRotationIndexFromVector(SVECTOR v);
extern short* entityData;
int FUN_000403ec(int blockType, int rotationIndex);

INCLUDE_ASM("asm/nonmatchings/enemy", InitEnemies);

INCLUDE_ASM("asm/nonmatchings/enemy", UpdateEnemies);

int FUN_0003d758(Enemy* e) {
    int blockType;
    int rotationIndex;
    int res;

    SVECTOR_000a4810.vx = e->pos.vx - e->normalVec.vx * 400 + e->dir.vx * 0x200;
    SVECTOR_000a4810.vy = e->pos.vy - e->normalVec.vy * 400 + e->dir.vy * 0x200;
    SVECTOR_000a4810.vz = e->pos.vz - e->normalVec.vz * 400 + e->dir.vz * 0x200;

    SVECTOR_000a4818.vx = e->pos.vx + e->dir.vx * 0x200;
    SVECTOR_000a4818.vy = e->pos.vy + e->dir.vy * 0x200;
    SVECTOR_000a4818.vz = e->pos.vz + e->dir.vz * 0x200;

    blockType = GetBlockAt(&SVECTOR_000a4810);
    rotationIndex = GetRotationIndexFromVector(e->normalVec);

    if (FUN_000403ec(blockType, rotationIndex) != 1) {
        return 0;
    }

    res = GetBlockAt(&SVECTOR_000a4818);

    if (res == -1) {
        res = 1;
    } else {
        res = 0;
    }
    return res;
}

int FUN_0003d8b8(Enemy* e) {
    int res;

    SVECTOR_000a4820.vx = e->pos.vx - e->normalVec.vx * 400 - e->dir.vx * 0x200;
    SVECTOR_000a4820.vy = e->pos.vy - e->normalVec.vy * 400 - e->dir.vy * 0x200;
    SVECTOR_000a4820.vz = e->pos.vz - e->normalVec.vz * 400 - e->dir.vz * 0x200;

    SVECTOR_000a4828.vx = e->pos.vx - e->dir.vx * 0x200;
    SVECTOR_000a4828.vy = e->pos.vy - e->dir.vy * 0x200;
    SVECTOR_000a4828.vz = e->pos.vz - e->dir.vz * 0x200;

    if (FUN_000403ec(GetBlockAt(&SVECTOR_000a4820), GetRotationIndexFromVector(e->normalVec)) != 1) {
        return 0;
    }

    res = GetBlockAt(&SVECTOR_000a4828);
    if (res == -1) {
        res = 1;
    } else {
        res = 0;
    }
    return res;
}

void FUN_0003da18(Enemy* enemy) {
    enemy->dir.vx = -enemy->dir.vx;
    enemy->dir.vy = -enemy->dir.vy;
    enemy->dir.vz = -enemy->dir.vz;
    enemy->field1_0x8.vx = -enemy->field1_0x8.vx;
    enemy->field1_0x8.vy = -enemy->field1_0x8.vy;
    enemy->field1_0x8.vz = -enemy->field1_0x8.vz;
}

int FUN_0003da64(Enemy* enemy) {
    SVECTOR_000a4830.vx = enemy->pos.vx + (enemy->normalVec.vx * -400) + (enemy->field1_0x8.vx * 0x200);
    SVECTOR_000a4830.vy = enemy->pos.vy + (enemy->normalVec.vy * -400) + (enemy->field1_0x8.vy * 0x200);
    SVECTOR_000a4830.vz = enemy->pos.vz + (enemy->normalVec.vz * -400) + (enemy->field1_0x8.vz * 0x200);

    return FUN_000403ec(GetBlockAt(&SVECTOR_000a4830), GetRotationIndexFromVector(enemy->normalVec)) == 1;
}

void FUN_0003db64(Enemy* enemy) {
    tmpEnemyPos = enemy->dir;
    enemy->dir = enemy->field1_0x8;
    enemy->field1_0x8.vx = -tmpEnemyPos.vx;
    enemy->field1_0x8.vy = -tmpEnemyPos.vy;
    enemy->field1_0x8.vz = -tmpEnemyPos.vz;
}

int FUN_0003dbe0(Enemy* enemy) {
    SVECTOR_000a4838.vx = enemy->pos.vx + (enemy->normalVec.vx * -400) - (enemy->field1_0x8.vx * 0x200);
    SVECTOR_000a4838.vy = enemy->pos.vy + (enemy->normalVec.vy * -400) - (enemy->field1_0x8.vy * 0x200);
    SVECTOR_000a4838.vz = enemy->pos.vz + (enemy->normalVec.vz * -400) - (enemy->field1_0x8.vz * 0x200);

    return FUN_000403ec(GetBlockAt(&SVECTOR_000a4838), GetRotationIndexFromVector(enemy->normalVec)) == 1;
}

void FUN_0003dce0(Enemy* enemy) {
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

void Noop4() {
}

INCLUDE_ASM("asm/nonmatchings/enemy", ProcessEnemies);

void InitEnemy(int side, int rotation, Enemy* enemy) {
    SVECTOR_000a48f4.vz = 0;
    SVECTOR_000a48f4.vy = 0;
    SVECTOR_000a48f4.vx = 0;
    SVECTOR_000a48ec.vz = 0;
    SVECTOR_000a48ec.vy = 0;
    SVECTOR_000a48ec.vx = 0;
    SVECTOR_000a48e4.vz = 0;
    SVECTOR_000a48e4.vy = 0;
    SVECTOR_000a48e4.vx = 0;

    if (side == 5) {
        SVECTOR_000a48f4.vz = 1;
        SVECTOR_000a48ec.vy = 1;
        SVECTOR_000a48e4.vx = 1;
    }
    if (side == 0) {
        SVECTOR_000a48e4.vx = -1;
        SVECTOR_000a48ec.vy = 1;
        SVECTOR_000a48f4.vz = -1;
    }
    if (side == 4) {
        SVECTOR_000a48ec.vy = 1;
        SVECTOR_000a48e4.vz = 1;
        SVECTOR_000a48f4.vx = -1;
    }
    if (side == 1) {
        SVECTOR_000a48e4.vz = -1;
        SVECTOR_000a48f4.vx = 1;
        SVECTOR_000a48ec.vy = 1;
    }
    if (side == 2) {
        SVECTOR_000a48e4.vx = 1;
        SVECTOR_000a48ec.vz = -1;
        SVECTOR_000a48f4.vy = 1;
    }
    if (side == 3) {
        SVECTOR_000a48ec.vz = 1;
        SVECTOR_000a48e4.vx = 1;
        SVECTOR_000a48f4.vy = -1;
    }

    enemy->field1_0x8 = SVECTOR_000a48e4;
    enemy->dir = SVECTOR_000a48ec;
    enemy->normalVec = SVECTOR_000a48f4;

    if (rotation == 2) {
        enemy->field1_0x8.vx = -SVECTOR_000a48ec.vx;
        enemy->field1_0x8.vy = -SVECTOR_000a48ec.vy;
        enemy->field1_0x8.vz = -SVECTOR_000a48ec.vz;
        enemy->dir = SVECTOR_000a48e4;
    }
    if (rotation == 3) {
        enemy->field1_0x8.vx = -SVECTOR_000a48e4.vx;
        enemy->field1_0x8.vy = -SVECTOR_000a48e4.vy;
        enemy->field1_0x8.vz = -SVECTOR_000a48e4.vz;
        enemy->dir.vx = -SVECTOR_000a48ec.vx;
        enemy->dir.vy = -SVECTOR_000a48ec.vy;
        enemy->dir.vz = -SVECTOR_000a48ec.vz;
    }
    if (rotation == 4) {
        enemy->field1_0x8 = SVECTOR_000a48ec;
        enemy->dir.vx = -SVECTOR_000a48e4.vx;
        enemy->dir.vy = -SVECTOR_000a48e4.vy;
        enemy->dir.vz = -SVECTOR_000a48e4.vz;
    }
}

int FUN_000403ec(int blockType, int rotationIndex) {
    short type;
    int index;

    if (blockType == -1) return 0;
    if (blockType == -2) return 0;

    if (blockType < 5) return 1;

    type = entityData[(blockType - 5) * 128];

    if (type == 5) return 0;
    if (type == 6) return 1;

    type = entityData[(blockType - 5) * 128 + rotationIndex * 16 + 1];
    if (type == 0) return 1;
    if (type >= 50) return 1; // enemy
    if (type == 30) return 1; // spawn

    if (type == 2) return 1;
    if (type == 1) return 1;
    if (type == 4) return 1;
    return 0;
}

int FUN_00040490(SVECTOR* enemyPos, Enemy* enemy) {
    SVECTOR_000a48fc.vx = (enemyPos->vx + 0x100) & 0x1ff;
    SVECTOR_000a48fc.vy = (enemyPos->vy + 0x100) & 0x1ff;
    SVECTOR_000a48fc.vz = (enemyPos->vz + 0x100) & 0x1ff;

    if (enemy->dir.vx == 1) {
        return SVECTOR_000a48fc.vx;
    }
    if (enemy->dir.vx == -1) {
        return 0x200 - SVECTOR_000a48fc.vx;
    }
    if (enemy->dir.vy == 1) {
        return SVECTOR_000a48fc.vy;
    }
    if (enemy->dir.vy == -1) {
        return 0x200 - SVECTOR_000a48fc.vy;
    }
    if (enemy->dir.vz == 1) {
        return SVECTOR_000a48fc.vz;
    }
    if (enemy->dir.vz == -1) {
        return 0x200 - SVECTOR_000a48fc.vz;
    }

    return -10000;
}
