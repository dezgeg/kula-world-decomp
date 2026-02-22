#include "common.h"

int collI;
int enemyPlayerDistSq;
int numEnemies;
extern Enemy enemies[];

INCLUDE_ASM("asm/nonmatchings/enemy", InitEnemies);

INCLUDE_ASM("asm/nonmatchings/enemy", UpdateEnemies);

INCLUDE_ASM("asm/nonmatchings/enemy", FUN_0003d758);

INCLUDE_ASM("asm/nonmatchings/enemy", FUN_0003d8b8);

INCLUDE_ASM("asm/nonmatchings/enemy", FUN_0003da18);

INCLUDE_ASM("asm/nonmatchings/enemy", FUN_0003da64);

INCLUDE_ASM("asm/nonmatchings/enemy", FUN_0003db64);

INCLUDE_ASM("asm/nonmatchings/enemy", FUN_0003dbe0);

INCLUDE_ASM("asm/nonmatchings/enemy", FUN_0003dce0);

int IsCollidingWithEnemy(SVECTOR pos) {
    int dx, dy, dz;

    collI = 0;
    if (numEnemies > 0) {
        do {
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
            collI++;
        } while (collI < numEnemies);
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/enemy", Noop4);

INCLUDE_ASM("asm/nonmatchings/enemy", ProcessEnemies);

INCLUDE_ASM("asm/nonmatchings/enemy", InitEnemy);

INCLUDE_ASM("asm/nonmatchings/enemy", FUN_000403ec);

INCLUDE_ASM("asm/nonmatchings/enemy", FUN_00040490);
