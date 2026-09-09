#include "common.h"
#include <libgte.h>

extern int GetBlockAt(SVECTOR* coord);
extern int GetRotationIndexFromVector(SVECTOR v);
extern void CreateEnemyDispList(MATRIX* m, int screenZ, int modelId, int p4, int p5, int p6, int p7,
                                int p8, int blockX, int blockY, int blockZ, int dirIndex,
                                int otherBlockX, int otherBlockY, int otherBlockZ, int p16,
                                MATRIX* gteMatrix, int shadowColor, int p19);
extern void MatrixFromDirectionIndex(MATRIX* m, int p2, int dirIndex, int delta, SVECTOR* vec);
extern void InitEnemy(int side, int rotation, Enemy* enemy);
extern int Rand(int);
extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);
extern void SndUpdateVolumeBasedOnDirVec(int tag, SVECTOR* pan);

int EnemyIsBlockWalkable(int blockType, int rotationIndex);
int EnemyGetBlockProgress(SVECTOR* enemyPos, Enemy* enemy);

extern Enemy enemies[];
extern int cameraIndex;
extern Enemy enemies[];
extern MATRIX perspMatrixes[];
extern short* entityData;
extern short numEntities;

static int numEnemies;
static int enemiesProcessedOnce;
static SVECTOR tmpEnemyPos;
static SVECTOR unusedEnemyRotationVec;
static SVECTOR unusedEnemyVecs[4];

void InitEnemies(void) {
    int i, j;
    int rotation;

    numEnemies = 0;
    enemiesProcessedOnce = 0;

    for (i = 0; i < numEntities; i++) {
        if (entityData[i * 128] >= 5) {
            continue;
        }
        for (j = 0; j < 6; j++) {
            if (!(entityData[i * 128 + j * 16 + 1] == OBJ_SLOW_STAR ||
                    entityData[i * 128 + j * 16 + 1] == OBJ_TIRE ||
                    entityData[i * 128 + j * 16 + 1] == OBJ_CAPTURE_POD ||
                    entityData[i * 128 + j * 16 + 1] == OBJ_CAPTIVATOR ||
                    entityData[i * 128 + j * 16 + 1] == OBJ_FAST_STAR)) {
                continue;
            }
            rotation = entityData[i * 128 + j * 16 + 2];
            InitEnemy(j, rotation, &enemies[numEnemies]);

            enemies[numEnemies].pos.vx = entityData[i * 128 + 125] * 512 + enemies[numEnemies].normalVec.vx * 456;
            enemies[numEnemies].pos.vy = entityData[i * 128 + 126] * 512 + enemies[numEnemies].normalVec.vy * 456;
            enemies[numEnemies].pos.vz = entityData[i * 128 + 127] * 512 + enemies[numEnemies].normalVec.vz * 456;

            enemies[numEnemies].initPos = enemies[numEnemies].pos;

            enemies[numEnemies].enemyType = entityData[i * 128 + j * 16 + 1];
            enemies[numEnemies].rotationVec.vx = enemies[numEnemies].rotationVec.vy = enemies[numEnemies].rotationVec.vz = 0;
            enemies[numEnemies].state = enemies[numEnemies].timer = 0;

            if (enemies[numEnemies].enemyType == OBJ_CAPTIVATOR) {
                if (entityData[i * 128 + j * 16 + 3] == 2) {
                    enemies[numEnemies].timer = 341;
                }
                if (entityData[i * 128 + j * 16 + 3] == 1) {
                    enemies[numEnemies].timer = 682;
                }
                if (entityData[i * 128 + j * 16 + 3] == 0) {
                    enemies[numEnemies].timer = 1024;
                }
            }

            enemies[numEnemies].counter = -1;
            enemies[numEnemies].matrix.m[0][0] = enemies[numEnemies].matrix.m[1][1] = enemies[numEnemies].matrix.m[2][2] = 4096;
            enemies[numEnemies].matrix.m[1][0] =
            enemies[numEnemies].matrix.m[2][0] =
            enemies[numEnemies].matrix.m[0][1] =
            enemies[numEnemies].matrix.m[2][1] =
            enemies[numEnemies].matrix.m[0][2] =
            enemies[numEnemies].matrix.m[1][2] = 0;

            RotMatrixZ(1024, &enemies[numEnemies].matrix);

            MatrixFromDirectionIndex(&enemies[numEnemies].matrix2, rotation,
                                        GetRotationIndexFromVector(enemies[numEnemies].normalVec),
                                        -200, &tmpEnemyPos);

            numEnemies++;
        }
    }
}

static int UpdateEnemies_i;
#define i UpdateEnemies_i
static int UpdateEnemies_blockProgress;
#define blockProgress UpdateEnemies_blockProgress
static int UpdateEnemies_sumOfDeltas;
#define sumOfDeltas UpdateEnemies_sumOfDeltas
static SVECTOR UpdateEnemies_sfxDir;
#define sfxDir UpdateEnemies_sfxDir
void UpdateEnemies(SVECTOR playerPos) {
    int diff;

    for (i = 0; i < numEnemies; i++) {
        blockProgress = EnemyGetBlockProgress(&enemies[i].pos, &enemies[i]);

        diff = enemies[i].pos.vx - enemies[i].initPos.vx +
               enemies[i].pos.vy - enemies[i].initPos.vy +
               enemies[i].pos.vz - enemies[i].initPos.vz;
        if (diff < 0) {
            diff = -diff;
        }
        sumOfDeltas = diff;

        if (enemies[i].enemyType == OBJ_SLOW_STAR) {
            if (blockProgress < 256 && blockProgress + 16 >= 256) {
                if (EnemyCanTurnRight(&enemies[i])) {
                    EnemyTurnRight(&enemies[i]);
                } else if (EnemyCanTurnLeft(&enemies[i])) {
                    EnemyTurnLeft(&enemies[i]);
                } else if (!EnemyCanMoveForward(&enemies[i])) {
                    EnemyTurnAround(&enemies[i]);
                }
            }
            enemies[i].pos.vx += enemies[i].dir.vx * 16;
            enemies[i].pos.vy += enemies[i].dir.vy * 16;
            enemies[i].pos.vz += enemies[i].dir.vz * 16;
        }
        if (enemies[i].enemyType == OBJ_TIRE && enemies[i].state == 0) {
            if (!(EnemyCanMoveForward(&enemies[i]) || blockProgress >= 256 || blockProgress + 15 < 256)) {
                if (EnemyCanTurnLeft(&enemies[i])) {
                    enemies[i].state = 2;
                    enemies[i].timer = 0;
                } else if (EnemyCanTurnRight(&enemies[i])) {
                    enemies[i].state = 1;
                    enemies[i].timer = 0;
                } else {
                    enemies[i].state = 3;
                    enemies[i].timer = 0;
                }
            }
            if (enemies[i].state == 0) {
                enemies[i].pos.vx += enemies[i].dir.vx * 15;
                enemies[i].pos.vy += enemies[i].dir.vy * 15;
                enemies[i].pos.vz += enemies[i].dir.vz * 15;
            }
        }
        if (enemies[i].enemyType == OBJ_FAST_STAR) {
            enemies[i].pos.vx = enemies[i].initPos.vx + (rsin(enemies[i].timer) * 600) * enemies[i].dir.vx / 4096;
            enemies[i].pos.vy = enemies[i].initPos.vy + (rsin(enemies[i].timer) * 600) * enemies[i].dir.vy / 4096;
            enemies[i].pos.vz = enemies[i].initPos.vz + (rsin(enemies[i].timer) * 600) * enemies[i].dir.vz / 4096;

            enemies[i].timer = (enemies[i].timer + 64) % 4096;

            sfxDir.vx = enemies[i].pos.vx - playerPos.vx;
            sfxDir.vy = enemies[i].pos.vy - playerPos.vy;
            sfxDir.vz = enemies[i].pos.vz - playerPos.vz;

            if (enemies[i].timer == 1024 || enemies[i].timer == 3072) {
                SndPlaySfx(SFX_FAST_MOVING_STAR, 666 + i, &sfxDir, 2000);
            } else {
                SndUpdateVolumeBasedOnDirVec(666 + i, &sfxDir);
            }
        }
        if (enemies[i].enemyType == OBJ_CAPTIVATOR) {
            enemies[i].timer += 28;
            if (enemies[i].timer >= 2048) {
                sfxDir.vx = enemies[i].pos.vx - playerPos.vx;
                sfxDir.vy = enemies[i].pos.vy - playerPos.vy;
                sfxDir.vz = enemies[i].pos.vz - playerPos.vz;
                SndPlaySfx(SFX_CAPTIVATOR, 0, &sfxDir, 7000);
            }
            enemies[i].timer %= 2048;
            enemies[i].pos.vx = enemies[i].initPos.vx + (rsin(enemies[i].timer % 2048) * 400 * enemies[i].normalVec.vx) / 4096;
            enemies[i].pos.vy = enemies[i].initPos.vy + (rsin(enemies[i].timer % 2048) * 400 * enemies[i].normalVec.vy) / 4096;
            enemies[i].pos.vz = enemies[i].initPos.vz + (rsin(enemies[i].timer % 2048) * 400 * enemies[i].normalVec.vz) / 4096;
        }
        if (enemies[i].enemyType == OBJ_CAPTURE_POD) {
            if ((sumOfDeltas > 512 && enemies[i].counter == -1) || !enemiesProcessedOnce) {
                enemies[i].counter = 0;
                enemies[i].initPos = enemies[i].pos;
                enemies[i].timer = 0;
                if (enemies[i].dir.vx != 0) {
                    enemies[i].initPos.vx = (enemies[i].pos.vx + 128) & 0xff00;
                }
                if (enemies[i].dir.vy != 0) {
                    enemies[i].initPos.vy = (enemies[i].pos.vy + 128) & 0xff00;
                }
                if (enemies[i].dir.vz != 0) {
                    enemies[i].initPos.vz = (enemies[i].pos.vz + 128) & 0xff00;
                }
                while (enemies[i].counter == 0) {
                    switch (Rand(4)) {
                        case 0:
                            if (EnemyCanTurnRight(&enemies[i])) {
                                EnemyTurnRight(&enemies[i]);
                                enemies[i].counter = 1;
                            }
                            break;
                        case 1:
                            if (EnemyCanTurnLeft(&enemies[i])) {
                                EnemyTurnLeft(&enemies[i]);
                                enemies[i].counter = 1;
                            }
                            break;
                        case 2:
                            if (EnemyCanMoveForward(&enemies[i])) {
                                enemies[i].counter = 1;
                            }
                            break;
                        case 3:
                            if (EnemyCanMoveBackward(&enemies[i])) {
                                EnemyTurnAround(&enemies[i]);
                                enemies[i].counter = 1;
                            }
                            break;
                    }
                }
            }
            if (enemies[i].counter != -1) {
                enemies[i].timer += (enemies[i].counter * enemies[i].counter / 8) % 4096;
                enemies[i].pos.vx = enemies[i].initPos.vx + enemies[i].dir.vx * (rcos(enemies[i].timer) * -65 / 4096 + 65);
                enemies[i].pos.vy = enemies[i].initPos.vy + enemies[i].dir.vy * (rcos(enemies[i].timer) * -65 / 4096 + 65);
                enemies[i].pos.vz = enemies[i].initPos.vz + enemies[i].dir.vz * (rcos(enemies[i].timer) * -65 / 4096 + 65);
                enemies[i].counter++;
                if (enemies[i].counter > 64) {
                    enemies[i].counter = -1;
                    enemies[i].pos = enemies[i].initPos;
                    sfxDir.vx = enemies[i].pos.vx - playerPos.vx;
                    sfxDir.vy = enemies[i].pos.vy - playerPos.vy;
                    sfxDir.vz = enemies[i].pos.vz - playerPos.vz;
                    SndPlaySfx(SFX_CAPTURE_POD, 0, &sfxDir, 7000);
                }
            }
            if (enemies[i].counter == -1 && enemiesProcessedOnce) {
                enemies[i].pos.vx += enemies[i].dir.vx * 64;
                enemies[i].pos.vy += enemies[i].dir.vy * 64;
                enemies[i].pos.vz += enemies[i].dir.vz * 64;
            }
        }
        if (enemies[i].state) {
            switch (enemies[i].state) {
                case 1:
                    if (enemies[i].timer++ < 64) {
                        enemies[i].rotationVec.vz -= 16;
                    } else {
                        enemies[i].rotationVec.vz = (enemies[i].rotationVec.vz + 256) & 0xfc00;
                        EnemyTurnRight(&enemies[i]);
                        enemies[i].state = 0;
                    }
                    break;
                case 2:
                    if (enemies[i].timer++ < 64) {
                        enemies[i].rotationVec.vz += 16;
                    } else {
                        enemies[i].rotationVec.vz = (enemies[i].rotationVec.vz + 256) & 0xfc00;
                        EnemyTurnLeft(&enemies[i]);
                        enemies[i].state = 0;
                    }
                    break;
                case 3:
                    if (enemies[i].timer++ < 64) {
                        enemies[i].rotationVec.vz += 32;
                    } else {
                        enemies[i].rotationVec.vz = (enemies[i].rotationVec.vz + 256) & 0xfc00;
                        EnemyTurnAround(&enemies[i]);
                        enemies[i].state = 0;
                    }
                    break;
            }
        }
    }
    enemiesProcessedOnce = 1;
}
#undef i
#undef blockProgress
#undef sumOfDeltas
#undef sfxDir

static SVECTOR EnemyCanMoveForward_groundPos;
#define groundPos EnemyCanMoveForward_groundPos
static SVECTOR EnemyCanMoveForward_fwdPos;
#define fwdPos EnemyCanMoveForward_fwdPos
int EnemyCanMoveForward(Enemy* e) {
    int blockType;
    int rotationIndex;
    int res;

    groundPos.vx = e->pos.vx - e->normalVec.vx * 400 + e->dir.vx * 0x200;
    groundPos.vy = e->pos.vy - e->normalVec.vy * 400 + e->dir.vy * 0x200;
    groundPos.vz = e->pos.vz - e->normalVec.vz * 400 + e->dir.vz * 0x200;

    fwdPos.vx = e->pos.vx + e->dir.vx * 0x200;
    fwdPos.vy = e->pos.vy + e->dir.vy * 0x200;
    fwdPos.vz = e->pos.vz + e->dir.vz * 0x200;

    blockType = GetBlockAt(&groundPos);
    rotationIndex = GetRotationIndexFromVector(e->normalVec);

    if (EnemyIsBlockWalkable(blockType, rotationIndex) != 1) {
        return 0;
    }

    res = GetBlockAt(&fwdPos);

    if (res == -1) {
        res = 1;
    } else {
        res = 0;
    }
    return res;
}
#undef groundPos
#undef fwdPos

static SVECTOR EnemyCanMoveBackward_groundPos;
#define groundPos EnemyCanMoveBackward_groundPos
static SVECTOR EnemyCanMoveBackward_backPos;
#define backPos EnemyCanMoveBackward_backPos
int EnemyCanMoveBackward(Enemy* e) {
    int res;

    groundPos.vx = e->pos.vx - e->normalVec.vx * 400 - e->dir.vx * 0x200;
    groundPos.vy = e->pos.vy - e->normalVec.vy * 400 - e->dir.vy * 0x200;
    groundPos.vz = e->pos.vz - e->normalVec.vz * 400 - e->dir.vz * 0x200;

    backPos.vx = e->pos.vx - e->dir.vx * 0x200;
    backPos.vy = e->pos.vy - e->dir.vy * 0x200;
    backPos.vz = e->pos.vz - e->dir.vz * 0x200;

    if (EnemyIsBlockWalkable(GetBlockAt(&groundPos), GetRotationIndexFromVector(e->normalVec)) != 1) {
        return 0;
    }

    res = GetBlockAt(&backPos);
    if (res == -1) {
        res = 1;
    } else {
        res = 0;
    }
    return res;
}
#undef groundPos
#undef backPos

void EnemyTurnAround(Enemy* enemy) {
    enemy->dir.vx = -enemy->dir.vx;
    enemy->dir.vy = -enemy->dir.vy;
    enemy->dir.vz = -enemy->dir.vz;
    enemy->rightVec.vx = -enemy->rightVec.vx;
    enemy->rightVec.vy = -enemy->rightVec.vy;
    enemy->rightVec.vz = -enemy->rightVec.vz;
}

static SVECTOR EnemyCanTurnRight_turnPos;
#define turnPos EnemyCanTurnRight_turnPos
int EnemyCanTurnRight(Enemy* enemy) {
    turnPos.vx = enemy->pos.vx + (enemy->normalVec.vx * -400) + (enemy->rightVec.vx * 0x200);
    turnPos.vy = enemy->pos.vy + (enemy->normalVec.vy * -400) + (enemy->rightVec.vy * 0x200);
    turnPos.vz = enemy->pos.vz + (enemy->normalVec.vz * -400) + (enemy->rightVec.vz * 0x200);

    return EnemyIsBlockWalkable(GetBlockAt(&turnPos), GetRotationIndexFromVector(enemy->normalVec)) == 1;
}
#undef turnPos

void EnemyTurnRight(Enemy* enemy) {
    tmpEnemyPos = enemy->dir;
    enemy->dir = enemy->rightVec;
    enemy->rightVec.vx = -tmpEnemyPos.vx;
    enemy->rightVec.vy = -tmpEnemyPos.vy;
    enemy->rightVec.vz = -tmpEnemyPos.vz;
}

static SVECTOR EnemyCanTurnLeft_turnPos;
#define turnPos EnemyCanTurnLeft_turnPos
int EnemyCanTurnLeft(Enemy* enemy) {
    turnPos.vx = enemy->pos.vx + (enemy->normalVec.vx * -400) - (enemy->rightVec.vx * 0x200);
    turnPos.vy = enemy->pos.vy + (enemy->normalVec.vy * -400) - (enemy->rightVec.vy * 0x200);
    turnPos.vz = enemy->pos.vz + (enemy->normalVec.vz * -400) - (enemy->rightVec.vz * 0x200);

    return EnemyIsBlockWalkable(GetBlockAt(&turnPos), GetRotationIndexFromVector(enemy->normalVec)) == 1;
}
#undef turnPos

void EnemyTurnLeft(Enemy* enemy) {
    tmpEnemyPos = enemy->dir;
    enemy->dir.vx = -enemy->rightVec.vx;
    enemy->dir.vy = -enemy->rightVec.vy;
    enemy->dir.vz = -enemy->rightVec.vz;
    enemy->rightVec = tmpEnemyPos;
}

static int IsCollidingWithEnemy_i;
#define i IsCollidingWithEnemy_i
static int IsCollidingWithEnemy_distSq;
#define distSq IsCollidingWithEnemy_distSq
int IsCollidingWithEnemy(SVECTOR pos) {
    int dx, dy, dz;

    for (i = 0; i < numEnemies; i++) {
        dx = pos.vx - enemies[i].pos.vx;
        dy = pos.vy - enemies[i].pos.vy;
        dz = pos.vz - enemies[i].pos.vz;

        distSq = dx * dx + dy * dy + dz * dz;

        if (enemies[i].enemyType == OBJ_CAPTURE_POD) {
            if (distSq < 16200) {
                return 1;
            }
        } else if (distSq < 45000) {
            return 1;
        }
    }

    return 0;
}
#undef i
#undef distSq

#ifndef SKIP_UNUSED_CODE
void Noop4() {
}
#endif

static int RenderEnemies_i;
#define i RenderEnemies_i
static int RenderEnemies_scale;
#define scale RenderEnemies_scale
static SVECTOR RenderEnemies_screenPos;
#define screenPos RenderEnemies_screenPos
static SVECTOR RenderEnemies_pixelPos;
#define pixelPos RenderEnemies_pixelPos
static SVECTOR RenderEnemies_pixelPos2;
#define pixelPos2 RenderEnemies_pixelPos2
static SVECTOR RenderEnemies_rotVec;
#define rotVec RenderEnemies_rotVec
static MATRIX RenderEnemies_rotMatrix;
#define rotMatrix RenderEnemies_rotMatrix
static MATRIX RenderEnemies_tmpMatrix;
#define tmpMatrix RenderEnemies_tmpMatrix
static MATRIX RenderEnemies_dirMatrix;
#define dirMatrix RenderEnemies_dirMatrix
void RenderEnemies(void) {
    for (i = 0; i < numEnemies; i++) {
        if (enemies[i].enemyType == OBJ_SLOW_STAR) {
            enemies[i].rotationVec.vx = (enemies[i].rotationVec.vx + 14) % 4096;
            enemies[i].rotationVec.vy = (enemies[i].rotationVec.vy - 120) % 4096;
            enemies[i].rotationVec.vz = (enemies[i].rotationVec.vz + 52) % 4096;

            RotMatrix(&enemies[i].rotationVec, &rotMatrix);
            MulMatrix0(&perspMatrixes[cameraIndex], &rotMatrix, &rotMatrix);

            tmpEnemyPos = enemies[i].pos;
            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &screenPos);

            rotMatrix.t[0] = screenPos.vx + perspMatrixes[cameraIndex].t[0];
            rotMatrix.t[1] = screenPos.vy + perspMatrixes[cameraIndex].t[1];
            rotMatrix.t[2] = screenPos.vz + perspMatrixes[cameraIndex].t[2];

            pixelPos.vx = (enemies[i].pos.vx + 256 + enemies[i].dir.vx * 150 - enemies[i].normalVec.vx * 300) >> 9;
            pixelPos.vy = (enemies[i].pos.vy + 256 + enemies[i].dir.vy * 150 - enemies[i].normalVec.vy * 300) >> 9;
            pixelPos.vz = (enemies[i].pos.vz + 256 + enemies[i].dir.vz * 150 - enemies[i].normalVec.vz * 300) >> 9;

            pixelPos2.vx = (enemies[i].pos.vx + 256 - (enemies[i].dir.vx * 150) - enemies[i].normalVec.vx * 300) >> 9;
            pixelPos2.vy = (enemies[i].pos.vy + 256 - (enemies[i].dir.vy * 150) - enemies[i].normalVec.vy * 300) >> 9;
            pixelPos2.vz = (enemies[i].pos.vz + 256 - (enemies[i].dir.vz * 150) - enemies[i].normalVec.vz * 300) >> 9;

            MatrixFromDirectionIndex(&dirMatrix, 0, GetRotationIndexFromVector(enemies[i].normalVec), -200, &tmpEnemyPos);

            rotVec.vx = 1024;
            rotVec.vz = 0;
            rotVec.vy = 0;
            RotMatrix(&rotVec, &tmpMatrix);
            MulMatrix0(&dirMatrix, &tmpMatrix, &dirMatrix);
            MulMatrix0(&perspMatrixes[cameraIndex], &dirMatrix, &dirMatrix);

            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &screenPos);

            dirMatrix.t[0] = screenPos.vx + perspMatrixes[cameraIndex].t[0];
            dirMatrix.t[1] = screenPos.vy + perspMatrixes[cameraIndex].t[1];
            dirMatrix.t[2] = screenPos.vz + perspMatrixes[cameraIndex].t[2];

            CreateEnemyDispList(&rotMatrix, rotMatrix.t[2], 20, 0, 4096, 4096, 4096, 0,
                                pixelPos.vx, pixelPos.vy, pixelPos.vz,
                                GetRotationIndexFromVector(enemies[i].normalVec),
                                pixelPos2.vx, pixelPos2.vy, pixelPos2.vz,
                                GetRotationIndexFromVector(enemies[i].normalVec),
                                &dirMatrix, 128, 0);
        }

        if (enemies[i].enemyType == OBJ_CAPTURE_POD) {
            enemies[i].rotationVec.vx = (enemies[i].rotationVec.vx + 14) % 4096;
            enemies[i].rotationVec.vy = (enemies[i].rotationVec.vy - 120) % 4096;
            enemies[i].rotationVec.vz = (enemies[i].rotationVec.vz + 52) % 4096;

            RotMatrix(&enemies[i].rotationVec, &rotMatrix);
            MulMatrix0(&perspMatrixes[cameraIndex], &rotMatrix, &rotMatrix);

            tmpEnemyPos = enemies[i].pos;
            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &screenPos);

            rotMatrix.t[0] = screenPos.vx + perspMatrixes[cameraIndex].t[0];
            rotMatrix.t[1] = screenPos.vy + perspMatrixes[cameraIndex].t[1];
            rotMatrix.t[2] = screenPos.vz + perspMatrixes[cameraIndex].t[2];

            pixelPos.vx = (enemies[i].pos.vx + 256 + enemies[i].dir.vx * 150 - enemies[i].normalVec.vx * 300) >> 9;
            pixelPos.vy = (enemies[i].pos.vy + 256 + enemies[i].dir.vy * 150 - enemies[i].normalVec.vy * 300) >> 9;
            pixelPos.vz = (enemies[i].pos.vz + 256 + enemies[i].dir.vz * 150 - enemies[i].normalVec.vz * 300) >> 9;

            pixelPos2.vx = (enemies[i].pos.vx + 256 - (enemies[i].dir.vx * 150) - enemies[i].normalVec.vx * 300) >> 9;
            pixelPos2.vy = (enemies[i].pos.vy + 256 - (enemies[i].dir.vy * 150) - enemies[i].normalVec.vy * 300) >> 9;
            pixelPos2.vz = (enemies[i].pos.vz + 256 - (enemies[i].dir.vz * 150) - enemies[i].normalVec.vz * 300) >> 9;

            MatrixFromDirectionIndex(&dirMatrix, 0, GetRotationIndexFromVector(enemies[i].normalVec), -200, &tmpEnemyPos);

            rotVec.vx = 1024;
            rotVec.vz = 0;
            rotVec.vy = 0;
            RotMatrix(&rotVec, &tmpMatrix);
            MulMatrix0(&dirMatrix, &tmpMatrix, &dirMatrix);
            MulMatrix0(&perspMatrixes[cameraIndex], &dirMatrix, &dirMatrix);

            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &screenPos);

            dirMatrix.t[0] = screenPos.vx + perspMatrixes[cameraIndex].t[0];
            dirMatrix.t[1] = screenPos.vy + perspMatrixes[cameraIndex].t[1];
            dirMatrix.t[2] = screenPos.vz + perspMatrixes[cameraIndex].t[2];

            CreateEnemyDispList(&rotMatrix, rotMatrix.t[2], 23, 0, 4096, 4096, 4096, 0,
                                pixelPos.vx, pixelPos.vy, pixelPos.vz,
                                GetRotationIndexFromVector(enemies[i].normalVec),
                                pixelPos2.vx, pixelPos2.vy, pixelPos2.vz,
                                GetRotationIndexFromVector(enemies[i].normalVec),
                                &dirMatrix, 128, 0);
        }

        if (enemies[i].enemyType == OBJ_CAPTIVATOR) {
            enemies[i].rotationVec.vx = 0;
            enemies[i].rotationVec.vy = 0;
            enemies[i].rotationVec.vz = (enemies[i].rotationVec.vz - (rcos(enemies[i].timer % 2048) * 400) / 4096) & 0xfff;

            RotMatrixZYX(&enemies[i].rotationVec, &rotMatrix);
            MulMatrix0(&rotMatrix, &enemies[i].matrix, &rotMatrix);
            MulMatrix0(&enemies[i].matrix2, &rotMatrix, &rotMatrix);
            MulMatrix0(&perspMatrixes[cameraIndex], &rotMatrix, &rotMatrix);

            tmpEnemyPos = enemies[i].pos;
            tmpEnemyPos.vx = tmpEnemyPos.vx - (enemies[i].normalVec.vx * 150);
            tmpEnemyPos.vy = tmpEnemyPos.vy - (enemies[i].normalVec.vy * 150);
            tmpEnemyPos.vz = tmpEnemyPos.vz - (enemies[i].normalVec.vz * 150);

            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &screenPos);

            rotMatrix.t[0] = screenPos.vx + perspMatrixes[cameraIndex].t[0];
            rotMatrix.t[1] = screenPos.vy + perspMatrixes[cameraIndex].t[1];
            rotMatrix.t[2] = screenPos.vz + perspMatrixes[cameraIndex].t[2];

            pixelPos.vx = pixelPos2.vx = (enemies[i].initPos.vx + 256 - (enemies[i].normalVec.vx * 300)) >> 9;
            pixelPos.vy = pixelPos2.vy = (enemies[i].initPos.vy + 256 - (enemies[i].normalVec.vy * 300)) >> 9;
            pixelPos.vz = pixelPos2.vz = (enemies[i].initPos.vz + 256 - (enemies[i].normalVec.vz * 300)) >> 9;

            MatrixFromDirectionIndex(&dirMatrix, 0, GetRotationIndexFromVector(enemies[i].normalVec), 0, &tmpEnemyPos);

            rotVec.vx = 1024;
            rotVec.vz = 0;
            rotVec.vy = 0;
            RotMatrixZYX(&rotVec, &tmpMatrix);
            MulMatrix0(&dirMatrix, &tmpMatrix, &dirMatrix);
            MulMatrix0(&perspMatrixes[cameraIndex], &dirMatrix, &dirMatrix);

            tmpEnemyPos = enemies[i].initPos;
            tmpEnemyPos.vx = tmpEnemyPos.vx - (enemies[i].normalVec.vx * 170);
            tmpEnemyPos.vy = tmpEnemyPos.vy - (enemies[i].normalVec.vy * 170);
            tmpEnemyPos.vz = tmpEnemyPos.vz - (enemies[i].normalVec.vz * 170);

            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &screenPos);
            dirMatrix.t[0] = screenPos.vx + perspMatrixes[cameraIndex].t[0];
            dirMatrix.t[1] = screenPos.vy + perspMatrixes[cameraIndex].t[1];
            dirMatrix.t[2] = screenPos.vz + perspMatrixes[cameraIndex].t[2];

            scale = 128 - (rsin(enemies[i].timer % 2048) * 48) / 4096;
            CreateEnemyDispList(&rotMatrix, rotMatrix.t[2], 24, 0, 4096, 4096, 4096, 0,
                                pixelPos.vx, pixelPos.vy, pixelPos.vz,
                                GetRotationIndexFromVector(enemies[i].normalVec),
                                pixelPos2.vx, pixelPos2.vy, pixelPos2.vz,
                                GetRotationIndexFromVector(enemies[i].normalVec),
                                &dirMatrix, scale, 0);
        }

        if (enemies[i].enemyType == OBJ_TIRE) {
            if (enemies[i].state == 0) {
                enemies[i].rotationVec.vx -= 64;
            }
            enemies[i].rotationVec.vx %= 4096;
            enemies[i].rotationVec.vy %= 4096;
            enemies[i].rotationVec.vz %= 4096;

            RotMatrixZYX(&enemies[i].rotationVec, &rotMatrix);
            MulMatrix0(&rotMatrix, &enemies[i].matrix, &rotMatrix);
            MulMatrix0(&enemies[i].matrix2, &rotMatrix, &rotMatrix);
            MulMatrix0(&perspMatrixes[cameraIndex], &rotMatrix, &rotMatrix);

            tmpEnemyPos = enemies[i].pos;
            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &screenPos);

            rotMatrix.t[0] = screenPos.vx + perspMatrixes[cameraIndex].t[0];
            rotMatrix.t[1] = screenPos.vy + perspMatrixes[cameraIndex].t[1];
            rotMatrix.t[2] = screenPos.vz + perspMatrixes[cameraIndex].t[2];

            pixelPos.vx = (enemies[i].pos.vx + 256 + enemies[i].dir.vx * 150 - enemies[i].normalVec.vx * 300) >> 9;
            pixelPos.vy = (enemies[i].pos.vy + 256 + enemies[i].dir.vy * 150 - enemies[i].normalVec.vy * 300) >> 9;
            pixelPos.vz = (enemies[i].pos.vz + 256 + enemies[i].dir.vz * 150 - enemies[i].normalVec.vz * 300) >> 9;

            pixelPos2.vx = (enemies[i].pos.vx + 256 - (enemies[i].dir.vx * 150) - enemies[i].normalVec.vx * 300) >> 9;
            pixelPos2.vy = (enemies[i].pos.vy + 256 - (enemies[i].dir.vy * 150) - enemies[i].normalVec.vy * 300) >> 9;
            pixelPos2.vz = (enemies[i].pos.vz + 256 - (enemies[i].dir.vz * 150) - enemies[i].normalVec.vz * 300) >> 9;

            MatrixFromDirectionIndex(&dirMatrix, 0, GetRotationIndexFromVector(enemies[i].normalVec), -200, &tmpEnemyPos);

            rotVec.vx = 1024;
            rotVec.vy = 0;
            rotVec.vz = enemies[i].rotationVec.vz;
            RotMatrixZYX(&rotVec, &tmpMatrix);
            MulMatrix0(&dirMatrix, &tmpMatrix, &dirMatrix);
            MulMatrix0(&perspMatrixes[cameraIndex], &dirMatrix, &dirMatrix);

            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &screenPos);
            dirMatrix.t[0] = screenPos.vx + perspMatrixes[cameraIndex].t[0];
            dirMatrix.t[1] = screenPos.vy + perspMatrixes[cameraIndex].t[1];
            dirMatrix.t[2] = screenPos.vz + perspMatrixes[cameraIndex].t[2];

            CreateEnemyDispList(&rotMatrix, rotMatrix.t[2], 21, 0, 4096, 4096, 4096, 0,
                                pixelPos.vx, pixelPos.vy, pixelPos.vz,
                                GetRotationIndexFromVector(enemies[i].normalVec),
                                pixelPos2.vx, pixelPos2.vy, pixelPos2.vz,
                                GetRotationIndexFromVector(enemies[i].normalVec),
                                &dirMatrix, 128, 1);
        }

        if (enemies[i].enemyType == OBJ_FAST_STAR) {
            enemies[i].rotationVec.vx = (enemies[i].rotationVec.vx + 64) % 4096;
            enemies[i].rotationVec.vy = (enemies[i].rotationVec.vy + 32) % 4096;
            enemies[i].rotationVec.vz %= 4096;

            RotMatrix(&enemies[i].rotationVec, &tmpMatrix);
            MulMatrix0(&perspMatrixes[cameraIndex], &tmpMatrix, &rotMatrix);

            tmpEnemyPos = enemies[i].pos;
            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &screenPos);

            rotMatrix.t[0] = screenPos.vx + perspMatrixes[cameraIndex].t[0];
            rotMatrix.t[1] = screenPos.vy + perspMatrixes[cameraIndex].t[1];
            rotMatrix.t[2] = screenPos.vz + perspMatrixes[cameraIndex].t[2];

            pixelPos.vx = (enemies[i].pos.vx + 256 + enemies[i].dir.vx * 150 - enemies[i].normalVec.vx * 300) >> 9;
            pixelPos.vy = (enemies[i].pos.vy + 256 + enemies[i].dir.vy * 150 - enemies[i].normalVec.vy * 300) >> 9;
            pixelPos.vz = (enemies[i].pos.vz + 256 + enemies[i].dir.vz * 150 - enemies[i].normalVec.vz * 300) >> 9;

            pixelPos2.vx = (enemies[i].pos.vx + 256 - (enemies[i].dir.vx * 150) - enemies[i].normalVec.vx * 300) >> 9;
            pixelPos2.vy = (enemies[i].pos.vy + 256 - (enemies[i].dir.vy * 150) - enemies[i].normalVec.vy * 300) >> 9;
            pixelPos2.vz = (enemies[i].pos.vz + 256 - (enemies[i].dir.vz * 150) - enemies[i].normalVec.vz * 300) >> 9;

            MatrixFromDirectionIndex(&dirMatrix, 0, GetRotationIndexFromVector(enemies[i].normalVec), -200, &tmpEnemyPos);

            rotVec.vx = 1024;
            rotVec.vz = 0;
            rotVec.vy = 0;
            RotMatrix(&rotVec, &tmpMatrix);
            MulMatrix0(&dirMatrix, &tmpMatrix, &dirMatrix);
            MulMatrix0(&perspMatrixes[cameraIndex], &dirMatrix, &dirMatrix);

            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &screenPos);
            dirMatrix.t[0] = screenPos.vx + perspMatrixes[cameraIndex].t[0];
            dirMatrix.t[1] = screenPos.vy + perspMatrixes[cameraIndex].t[1];
            dirMatrix.t[2] = screenPos.vz + perspMatrixes[cameraIndex].t[2];

            CreateEnemyDispList(&rotMatrix, rotMatrix.t[2], 22, 0, 4096, 4096, 4096, 0,
                                pixelPos.vx, pixelPos.vy, pixelPos.vz,
                                GetRotationIndexFromVector(enemies[i].normalVec),
                                pixelPos2.vx, pixelPos2.vy, pixelPos2.vz,
                                GetRotationIndexFromVector(enemies[i].normalVec),
                                &dirMatrix, 128, 0);
        }
    }
}
#undef i
#undef scale
#undef screenPos
#undef pixelPos
#undef pixelPos2
#undef rotVec
#undef rotMatrix
#undef tmpMatrix
#undef dirMatrix

static SVECTOR InitEnemy_initRight;
#define initRight InitEnemy_initRight
static SVECTOR InitEnemy_initDir;
#define initDir InitEnemy_initDir
static SVECTOR InitEnemy_initNormal;
#define initNormal InitEnemy_initNormal
void InitEnemy(int side, int rotation, Enemy* enemy) {
    initNormal.vx = initNormal.vy = initNormal.vz = 0;
    initDir.vx = initDir.vy = initDir.vz = 0;
    initRight.vx = initRight.vy = initRight.vz = 0;

    if (side == 5) {
        initNormal.vz = 1;
        initDir.vy = 1;
        initRight.vx = 1;
    }
    if (side == 0) {
        initRight.vx = -1;
        initDir.vy = 1;
        initNormal.vz = -1;
    }
    if (side == 4) {
        initDir.vy = 1;
        initRight.vz = 1;
        initNormal.vx = -1;
    }
    if (side == 1) {
        initRight.vz = -1;
        initNormal.vx = 1;
        initDir.vy = 1;
    }
    if (side == 2) {
        initRight.vx = 1;
        initDir.vz = -1;
        initNormal.vy = 1;
    }
    if (side == 3) {
        initDir.vz = 1;
        initRight.vx = 1;
        initNormal.vy = -1;
    }

    enemy->rightVec = initRight;
    enemy->dir = initDir;
    enemy->normalVec = initNormal;

    if (rotation == 2) {
        enemy->rightVec.vx = -initDir.vx;
        enemy->rightVec.vy = -initDir.vy;
        enemy->rightVec.vz = -initDir.vz;
        enemy->dir = initRight;
    }
    if (rotation == 3) {
        enemy->rightVec.vx = -initRight.vx;
        enemy->rightVec.vy = -initRight.vy;
        enemy->rightVec.vz = -initRight.vz;
        enemy->dir.vx = -initDir.vx;
        enemy->dir.vy = -initDir.vy;
        enemy->dir.vz = -initDir.vz;
    }
    if (rotation == 4) {
        enemy->rightVec = initDir;
        enemy->dir.vx = -initRight.vx;
        enemy->dir.vy = -initRight.vy;
        enemy->dir.vz = -initRight.vz;
    }
}
#undef initRight
#undef initDir
#undef initNormal

int EnemyIsBlockWalkable(int blockType, int rotationIndex) {
    short type;
    int index;

    if (blockType == -1)
        return 0;
    if (blockType == -2)
        return 0;

    if (blockType < 5)
        return 1;

    type = entityData[(blockType - 5) * 128];

    if (type == OBJ_TRANSPORTER)
        return 0;
    if (type == OBJ_CRUMBLING_BLOCK)
        return 1;

    type = entityData[(blockType - 5) * 128 + rotationIndex * 16 + 1];
    if (type == 0)
        return 1;
    if (type >= OBJ_SLOW_STAR)
        return 1;
    if (type == OBJ_PLAYER_SPAWN)
        return 1;

    if (type == OBJ_ICE_PATCH)
        return 1;
    if (type == OBJ_FIRE_PATCH)
        return 1;
    if (type == OBJ_ACID_PATCH)
        return 1;
    return 0;
}

static SVECTOR EnemyGetBlockProgress_blockPos;
#define blockPos EnemyGetBlockProgress_blockPos
int EnemyGetBlockProgress(SVECTOR* enemyPos, Enemy* enemy) {
    blockPos.vx = (enemyPos->vx + 0x100) & 0x1ff;
    blockPos.vy = (enemyPos->vy + 0x100) & 0x1ff;
    blockPos.vz = (enemyPos->vz + 0x100) & 0x1ff;

    if (enemy->dir.vx == 1) {
        return blockPos.vx;
    }
    if (enemy->dir.vx == -1) {
        return 0x200 - blockPos.vx;
    }
    if (enemy->dir.vy == 1) {
        return blockPos.vy;
    }
    if (enemy->dir.vy == -1) {
        return 0x200 - blockPos.vy;
    }
    if (enemy->dir.vz == 1) {
        return blockPos.vz;
    }
    if (enemy->dir.vz == -1) {
        return 0x200 - blockPos.vz;
    }

    return -10000;
}
#undef blockPos
