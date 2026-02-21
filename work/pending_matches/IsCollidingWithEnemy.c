#include "common.h"

// gprel-used variables
int collI;
int enemyPlayerDistSq;
int numEnemies;

// Externs
extern Enemy enemies[];

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
