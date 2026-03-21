#include "common.h"
#include <libgte.h>

int collI;
int enemyPlayerDistSq;
int numEnemies;
int enemiesProcessedOnce;
int enemyI;
int DAT_000a4850;
extern Enemy enemies[];

static SVECTOR tmpEnemyPos;
static SVECTOR tmpEnemyScreenPos;
static SVECTOR tmpEnemyPixelPos;
static SVECTOR tmpEnemyPixelPos2;
static SVECTOR SVECTOR_000a4874;
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

static MATRIX rotationMatrix;
MATRIX MATRIX_000a48a4;
static MATRIX MATRIX_000a48c4;

extern int cameraIndex;
extern Enemy enemies[];
extern MATRIX perspMatrixes[];

extern int GetBlockAt(SVECTOR* coord);
extern int GetRotationIndexFromVector(SVECTOR v);
extern void CreateEnemyDispList(MATRIX* m, int screenZ, int modelId, int p4, int p5, int p6, int p7, int p8, int blockX, int blockY, int blockZ, int dirIndex, int otherBlockX, int otherBlockY, int otherBlockZ, int p16, MATRIX* gteMatrix, int shadowColor, int p19);
extern void MatrixFromDirectionIndex(MATRIX* m, int p2, int dirIndex, int delta, SVECTOR* vec);
extern short* entityData;
extern short numEntities;
extern void InitEnemy(int side, int rotation, Enemy* enemy);
int FUN_000403ec(int blockType, int rotationIndex);

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
            if (entityData[i * 128 + j * 16 + 1] == OBJ_SLOW_STAR ||
                    entityData[i * 128 + j * 16 + 1] == OBJ_TIRE ||
                    entityData[i * 128 + j * 16 + 1] == OBJ_CAPTURE_POD ||
                    entityData[i * 128 + j * 16 + 1] == OBJ_CAPTIVATOR ||
                    entityData[i * 128 + j * 16 + 1] == OBJ_FAST_STAR) {
                rotation = entityData[i * 128 + j * 16 + 2];
                InitEnemy(j, rotation, &enemies[numEnemies]);

                enemies[numEnemies].pos.vx = entityData[i * 128 + 125] * 512 + enemies[numEnemies].normalVec.vx * 456;
                enemies[numEnemies].pos.vy = entityData[i * 128 + 126] * 512 + enemies[numEnemies].normalVec.vy * 456;
                enemies[numEnemies].pos.vz = entityData[i * 128 + 127] * 512 + enemies[numEnemies].normalVec.vz * 456;

                enemies[numEnemies].initPos = enemies[numEnemies].pos;

                enemies[numEnemies].enemyType = entityData[i * 128 + j * 16 + 1];
                enemies[numEnemies].rotationVec.vx = enemies[numEnemies].rotationVec.vy = enemies[numEnemies].rotationVec.vz = 0;
                enemies[numEnemies].state = enemies[numEnemies].timer = 0;

                if (enemies[numEnemies].enemyType == 56) {
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

                enemies[numEnemies].field_b0 = -1;
                enemies[numEnemies].matrix.m[0][0] = enemies[numEnemies].matrix.m[1][1] = enemies[numEnemies].matrix.m[2][2] = 4096;
                enemies[numEnemies].matrix.m[1][0] =
                enemies[numEnemies].matrix.m[2][0] =
                enemies[numEnemies].matrix.m[0][1] =
                enemies[numEnemies].matrix.m[2][1] =
                enemies[numEnemies].matrix.m[0][2] =
                enemies[numEnemies].matrix.m[1][2] = 0;

                RotMatrixZ(1024, &enemies[numEnemies].matrix);

                MatrixFromDirectionIndex(&enemies[numEnemies].matrix2, rotation, GetRotationIndexFromVector(enemies[numEnemies].normalVec), -200, &tmpEnemyPos);

                numEnemies++;
            }
        }
    }
}


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
        dx = pos.vx - enemies[collI].pos.vx;
        dy = pos.vy - enemies[collI].pos.vy;
        dz = pos.vz - enemies[collI].pos.vz;

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

void ProcessEnemies(void) {
    for (enemyI = 0; enemyI < numEnemies; enemyI++) {
        if (enemies[enemyI].enemyType == 50) {
            enemies[enemyI].rotationVec.vx = (enemies[enemyI].rotationVec.vx + 14) % 4096;
            enemies[enemyI].rotationVec.vy = (enemies[enemyI].rotationVec.vy - 120) % 4096;
            enemies[enemyI].rotationVec.vz = (enemies[enemyI].rotationVec.vz + 52) % 4096;

            RotMatrix(&enemies[enemyI].rotationVec, &rotationMatrix);
            MulMatrix0(&perspMatrixes[cameraIndex], &rotationMatrix, &rotationMatrix);

            tmpEnemyPos = enemies[enemyI].pos;
            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &tmpEnemyScreenPos);

            rotationMatrix.t[0] = tmpEnemyScreenPos.vx + perspMatrixes[cameraIndex].t[0];
            rotationMatrix.t[1] = tmpEnemyScreenPos.vy + perspMatrixes[cameraIndex].t[1];
            rotationMatrix.t[2] = tmpEnemyScreenPos.vz + perspMatrixes[cameraIndex].t[2];

            tmpEnemyPixelPos.vx = (enemies[enemyI].pos.vx + 256 + enemies[enemyI].dir.vx * 150 - enemies[enemyI].normalVec.vx * 300) >> 9;
            tmpEnemyPixelPos.vy = (enemies[enemyI].pos.vy + 256 + enemies[enemyI].dir.vy * 150 - enemies[enemyI].normalVec.vy * 300) >> 9;
            tmpEnemyPixelPos.vz = (enemies[enemyI].pos.vz + 256 + enemies[enemyI].dir.vz * 150 - enemies[enemyI].normalVec.vz * 300) >> 9;

            tmpEnemyPixelPos2.vx = (enemies[enemyI].pos.vx + 256 - (enemies[enemyI].dir.vx * 150) - enemies[enemyI].normalVec.vx * 300) >> 9;
            tmpEnemyPixelPos2.vy = (enemies[enemyI].pos.vy + 256 - (enemies[enemyI].dir.vy * 150) - enemies[enemyI].normalVec.vy * 300) >> 9;
            tmpEnemyPixelPos2.vz = (enemies[enemyI].pos.vz + 256 - (enemies[enemyI].dir.vz * 150) - enemies[enemyI].normalVec.vz * 300) >> 9;

            MatrixFromDirectionIndex(&MATRIX_000a48c4, 0, GetRotationIndexFromVector(enemies[enemyI].normalVec), -200, &tmpEnemyPos);

            SVECTOR_000a4874.vx = 1024;
            SVECTOR_000a4874.vz = 0;
            SVECTOR_000a4874.vy = 0;
            RotMatrix(&SVECTOR_000a4874, &MATRIX_000a48a4);
            MulMatrix0(&MATRIX_000a48c4, &MATRIX_000a48a4, &MATRIX_000a48c4);
            MulMatrix0(&perspMatrixes[cameraIndex], &MATRIX_000a48c4, &MATRIX_000a48c4);

            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &tmpEnemyScreenPos);

            MATRIX_000a48c4.t[0] = tmpEnemyScreenPos.vx + perspMatrixes[cameraIndex].t[0];
            MATRIX_000a48c4.t[1] = tmpEnemyScreenPos.vy + perspMatrixes[cameraIndex].t[1];
            MATRIX_000a48c4.t[2] = tmpEnemyScreenPos.vz + perspMatrixes[cameraIndex].t[2];

            CreateEnemyDispList(&rotationMatrix, rotationMatrix.t[2], 20, 0, 4096, 4096, 4096, 0,
                                tmpEnemyPixelPos.vx, tmpEnemyPixelPos.vy, tmpEnemyPixelPos.vz,
                                GetRotationIndexFromVector(enemies[enemyI].normalVec),
                                tmpEnemyPixelPos2.vx, tmpEnemyPixelPos2.vy, tmpEnemyPixelPos2.vz,
                                GetRotationIndexFromVector(enemies[enemyI].normalVec), &MATRIX_000a48c4, 128, 0);
        }

        if (enemies[enemyI].enemyType == 53) {
            enemies[enemyI].rotationVec.vx = (enemies[enemyI].rotationVec.vx + 14) % 4096;
            enemies[enemyI].rotationVec.vy = (enemies[enemyI].rotationVec.vy - 120) % 4096;
            enemies[enemyI].rotationVec.vz = (enemies[enemyI].rotationVec.vz + 52) % 4096;

            RotMatrix(&enemies[enemyI].rotationVec, &rotationMatrix);
            MulMatrix0(&perspMatrixes[cameraIndex], &rotationMatrix, &rotationMatrix);

            tmpEnemyPos = enemies[enemyI].pos;
            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &tmpEnemyScreenPos);

            rotationMatrix.t[0] = tmpEnemyScreenPos.vx + perspMatrixes[cameraIndex].t[0];
            rotationMatrix.t[1] = tmpEnemyScreenPos.vy + perspMatrixes[cameraIndex].t[1];
            rotationMatrix.t[2] = tmpEnemyScreenPos.vz + perspMatrixes[cameraIndex].t[2];

            tmpEnemyPixelPos.vx = (enemies[enemyI].pos.vx + 256 + enemies[enemyI].dir.vx * 150 - enemies[enemyI].normalVec.vx * 300) >> 9;
            tmpEnemyPixelPos.vy = (enemies[enemyI].pos.vy + 256 + enemies[enemyI].dir.vy * 150 - enemies[enemyI].normalVec.vy * 300) >> 9;
            tmpEnemyPixelPos.vz = (enemies[enemyI].pos.vz + 256 + enemies[enemyI].dir.vz * 150 - enemies[enemyI].normalVec.vz * 300) >> 9;

            tmpEnemyPixelPos2.vx = (enemies[enemyI].pos.vx + 256 - (enemies[enemyI].dir.vx * 150) - enemies[enemyI].normalVec.vx * 300) >> 9;
            tmpEnemyPixelPos2.vy = (enemies[enemyI].pos.vy + 256 - (enemies[enemyI].dir.vy * 150) - enemies[enemyI].normalVec.vy * 300) >> 9;
            tmpEnemyPixelPos2.vz = (enemies[enemyI].pos.vz + 256 - (enemies[enemyI].dir.vz * 150) - enemies[enemyI].normalVec.vz * 300) >> 9;

            MatrixFromDirectionIndex(&MATRIX_000a48c4, 0, GetRotationIndexFromVector(enemies[enemyI].normalVec), -200, &tmpEnemyPos);

            SVECTOR_000a4874.vx = 1024;
            SVECTOR_000a4874.vz = 0;
            SVECTOR_000a4874.vy = 0;
            RotMatrix(&SVECTOR_000a4874, &MATRIX_000a48a4);
            MulMatrix0(&MATRIX_000a48c4, &MATRIX_000a48a4, &MATRIX_000a48c4);
            MulMatrix0(&perspMatrixes[cameraIndex], &MATRIX_000a48c4, &MATRIX_000a48c4);

            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &tmpEnemyScreenPos);

            MATRIX_000a48c4.t[0] = tmpEnemyScreenPos.vx + perspMatrixes[cameraIndex].t[0];
            MATRIX_000a48c4.t[1] = tmpEnemyScreenPos.vy + perspMatrixes[cameraIndex].t[1];
            MATRIX_000a48c4.t[2] = tmpEnemyScreenPos.vz + perspMatrixes[cameraIndex].t[2];

            CreateEnemyDispList(&rotationMatrix, rotationMatrix.t[2], 23, 0, 4096, 4096, 4096, 0,
                                tmpEnemyPixelPos.vx, tmpEnemyPixelPos.vy, tmpEnemyPixelPos.vz,
                                GetRotationIndexFromVector(enemies[enemyI].normalVec),
                                tmpEnemyPixelPos2.vx, tmpEnemyPixelPos2.vy, tmpEnemyPixelPos2.vz,
                                GetRotationIndexFromVector(enemies[enemyI].normalVec), &MATRIX_000a48c4, 128, 0);
        }

        if (enemies[enemyI].enemyType == 56) {
            enemies[enemyI].rotationVec.vx = 0;
            enemies[enemyI].rotationVec.vy = 0;
            enemies[enemyI].rotationVec.vz = (enemies[enemyI].rotationVec.vz - (rcos(enemies[enemyI].timer % 2048) * 400) / 4096) & 0xfff;

            RotMatrixZYX(&enemies[enemyI].rotationVec, &rotationMatrix);
            MulMatrix0(&rotationMatrix, &enemies[enemyI].matrix, &rotationMatrix);
            MulMatrix0(&enemies[enemyI].matrix2, &rotationMatrix, &rotationMatrix);
            MulMatrix0(&perspMatrixes[cameraIndex], &rotationMatrix, &rotationMatrix);

            tmpEnemyPos = enemies[enemyI].pos;
            tmpEnemyPos.vx = tmpEnemyPos.vx - (enemies[enemyI].normalVec.vx * 150);
            tmpEnemyPos.vy = tmpEnemyPos.vy - (enemies[enemyI].normalVec.vy * 150);
            tmpEnemyPos.vz = tmpEnemyPos.vz - (enemies[enemyI].normalVec.vz * 150);

            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &tmpEnemyScreenPos);

            rotationMatrix.t[0] = tmpEnemyScreenPos.vx + perspMatrixes[cameraIndex].t[0];
            rotationMatrix.t[1] = tmpEnemyScreenPos.vy + perspMatrixes[cameraIndex].t[1];
            rotationMatrix.t[2] = tmpEnemyScreenPos.vz + perspMatrixes[cameraIndex].t[2];

            tmpEnemyPixelPos.vx = tmpEnemyPixelPos2.vx = (enemies[enemyI].initPos.vx + 256 - (enemies[enemyI].normalVec.vx * 300)) >> 9;
            tmpEnemyPixelPos.vy = tmpEnemyPixelPos2.vy = (enemies[enemyI].initPos.vy + 256 - (enemies[enemyI].normalVec.vy * 300)) >> 9;
            tmpEnemyPixelPos.vz = tmpEnemyPixelPos2.vz = (enemies[enemyI].initPos.vz + 256 - (enemies[enemyI].normalVec.vz * 300)) >> 9;

            MatrixFromDirectionIndex(&MATRIX_000a48c4, 0, GetRotationIndexFromVector(enemies[enemyI].normalVec), 0, &tmpEnemyPos);

            SVECTOR_000a4874.vx = 1024;
            SVECTOR_000a4874.vz = 0;
            SVECTOR_000a4874.vy = 0;
            RotMatrixZYX(&SVECTOR_000a4874, &MATRIX_000a48a4);
            MulMatrix0(&MATRIX_000a48c4, &MATRIX_000a48a4, &MATRIX_000a48c4);
            MulMatrix0(&perspMatrixes[cameraIndex], &MATRIX_000a48c4, &MATRIX_000a48c4);

            tmpEnemyPos = enemies[enemyI].initPos;
            tmpEnemyPos.vx = tmpEnemyPos.vx - (enemies[enemyI].normalVec.vx * 170);
            tmpEnemyPos.vy = tmpEnemyPos.vy - (enemies[enemyI].normalVec.vy * 170);
            tmpEnemyPos.vz = tmpEnemyPos.vz - (enemies[enemyI].normalVec.vz * 170);

            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &tmpEnemyScreenPos);
            MATRIX_000a48c4.t[0] = tmpEnemyScreenPos.vx + perspMatrixes[cameraIndex].t[0];
            MATRIX_000a48c4.t[1] = tmpEnemyScreenPos.vy + perspMatrixes[cameraIndex].t[1];
            MATRIX_000a48c4.t[2] = tmpEnemyScreenPos.vz + perspMatrixes[cameraIndex].t[2];

            DAT_000a4850 = 128 - (rsin(enemies[enemyI].timer % 2048) * 48) / 4096;
            CreateEnemyDispList(&rotationMatrix, rotationMatrix.t[2], 24, 0, 4096, 4096, 4096, 0,
                                tmpEnemyPixelPos.vx, tmpEnemyPixelPos.vy, tmpEnemyPixelPos.vz,
                                GetRotationIndexFromVector(enemies[enemyI].normalVec),
                                tmpEnemyPixelPos2.vx, tmpEnemyPixelPos2.vy, tmpEnemyPixelPos2.vz,
                                GetRotationIndexFromVector(enemies[enemyI].normalVec), &MATRIX_000a48c4, DAT_000a4850, 0);
        }

        if (enemies[enemyI].enemyType == 51) {
            if (enemies[enemyI].state == 0) {
                enemies[enemyI].rotationVec.vx -= 64;
            }
            enemies[enemyI].rotationVec.vx %= 4096;
            enemies[enemyI].rotationVec.vy %= 4096;
            enemies[enemyI].rotationVec.vz %= 4096;

            RotMatrixZYX(&enemies[enemyI].rotationVec, &rotationMatrix);
            MulMatrix0(&rotationMatrix, &enemies[enemyI].matrix, &rotationMatrix);
            MulMatrix0(&enemies[enemyI].matrix2, &rotationMatrix, &rotationMatrix);
            MulMatrix0(&perspMatrixes[cameraIndex], &rotationMatrix, &rotationMatrix);

            tmpEnemyPos = enemies[enemyI].pos;
            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &tmpEnemyScreenPos);

            rotationMatrix.t[0] = tmpEnemyScreenPos.vx + perspMatrixes[cameraIndex].t[0];
            rotationMatrix.t[1] = tmpEnemyScreenPos.vy + perspMatrixes[cameraIndex].t[1];
            rotationMatrix.t[2] = tmpEnemyScreenPos.vz + perspMatrixes[cameraIndex].t[2];

            tmpEnemyPixelPos.vx = (enemies[enemyI].pos.vx + 256 + enemies[enemyI].dir.vx * 150 - enemies[enemyI].normalVec.vx * 300) >> 9;
            tmpEnemyPixelPos.vy = (enemies[enemyI].pos.vy + 256 + enemies[enemyI].dir.vy * 150 - enemies[enemyI].normalVec.vy * 300) >> 9;
            tmpEnemyPixelPos.vz = (enemies[enemyI].pos.vz + 256 + enemies[enemyI].dir.vz * 150 - enemies[enemyI].normalVec.vz * 300) >> 9;

            tmpEnemyPixelPos2.vx = (enemies[enemyI].pos.vx + 256 - (enemies[enemyI].dir.vx * 150) - enemies[enemyI].normalVec.vx * 300) >> 9;
            tmpEnemyPixelPos2.vy = (enemies[enemyI].pos.vy + 256 - (enemies[enemyI].dir.vy * 150) - enemies[enemyI].normalVec.vy * 300) >> 9;
            tmpEnemyPixelPos2.vz = (enemies[enemyI].pos.vz + 256 - (enemies[enemyI].dir.vz * 150) - enemies[enemyI].normalVec.vz * 300) >> 9;

            MatrixFromDirectionIndex(&MATRIX_000a48c4, 0, GetRotationIndexFromVector(enemies[enemyI].normalVec), -200, &tmpEnemyPos);

            SVECTOR_000a4874.vx = 1024;
            SVECTOR_000a4874.vy = 0;
            SVECTOR_000a4874.vz = enemies[enemyI].rotationVec.vz;
            RotMatrixZYX(&SVECTOR_000a4874, &MATRIX_000a48a4);
            MulMatrix0(&MATRIX_000a48c4, &MATRIX_000a48a4, &MATRIX_000a48c4);
            MulMatrix0(&perspMatrixes[cameraIndex], &MATRIX_000a48c4, &MATRIX_000a48c4);

            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &tmpEnemyScreenPos);
            MATRIX_000a48c4.t[0] = tmpEnemyScreenPos.vx + perspMatrixes[cameraIndex].t[0];
            MATRIX_000a48c4.t[1] = tmpEnemyScreenPos.vy + perspMatrixes[cameraIndex].t[1];
            MATRIX_000a48c4.t[2] = tmpEnemyScreenPos.vz + perspMatrixes[cameraIndex].t[2];

            CreateEnemyDispList(&rotationMatrix, rotationMatrix.t[2], 21, 0, 4096, 4096, 4096, 0,
                                tmpEnemyPixelPos.vx, tmpEnemyPixelPos.vy, tmpEnemyPixelPos.vz,
                                GetRotationIndexFromVector(enemies[enemyI].normalVec),
                                tmpEnemyPixelPos2.vx, tmpEnemyPixelPos2.vy, tmpEnemyPixelPos2.vz,
                                GetRotationIndexFromVector(enemies[enemyI].normalVec), &MATRIX_000a48c4, 128, 1);
        }

        if (enemies[enemyI].enemyType == 52) {
            enemies[enemyI].rotationVec.vx = (enemies[enemyI].rotationVec.vx + 64) % 4096;
            enemies[enemyI].rotationVec.vy = (enemies[enemyI].rotationVec.vy + 32) % 4096;
            enemies[enemyI].rotationVec.vz %= 4096;

            RotMatrix(&enemies[enemyI].rotationVec, &MATRIX_000a48a4);
            MulMatrix0(&perspMatrixes[cameraIndex], &MATRIX_000a48a4, &rotationMatrix);

            tmpEnemyPos = enemies[enemyI].pos;
            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &tmpEnemyScreenPos);

            rotationMatrix.t[0] = tmpEnemyScreenPos.vx + perspMatrixes[cameraIndex].t[0];
            rotationMatrix.t[1] = tmpEnemyScreenPos.vy + perspMatrixes[cameraIndex].t[1];
            rotationMatrix.t[2] = tmpEnemyScreenPos.vz + perspMatrixes[cameraIndex].t[2];

            tmpEnemyPixelPos.vx = (enemies[enemyI].pos.vx + 256 + enemies[enemyI].dir.vx * 150 - enemies[enemyI].normalVec.vx * 300) >> 9;
            tmpEnemyPixelPos.vy = (enemies[enemyI].pos.vy + 256 + enemies[enemyI].dir.vy * 150 - enemies[enemyI].normalVec.vy * 300) >> 9;
            tmpEnemyPixelPos.vz = (enemies[enemyI].pos.vz + 256 + enemies[enemyI].dir.vz * 150 - enemies[enemyI].normalVec.vz * 300) >> 9;

            tmpEnemyPixelPos2.vx = (enemies[enemyI].pos.vx + 256 - (enemies[enemyI].dir.vx * 150) - enemies[enemyI].normalVec.vx * 300) >> 9;
            tmpEnemyPixelPos2.vy = (enemies[enemyI].pos.vy + 256 - (enemies[enemyI].dir.vy * 150) - enemies[enemyI].normalVec.vy * 300) >> 9;
            tmpEnemyPixelPos2.vz = (enemies[enemyI].pos.vz + 256 - (enemies[enemyI].dir.vz * 150) - enemies[enemyI].normalVec.vz * 300) >> 9;

            MatrixFromDirectionIndex(&MATRIX_000a48c4, 0, GetRotationIndexFromVector(enemies[enemyI].normalVec), -200, &tmpEnemyPos);

            SVECTOR_000a4874.vx = 1024;
            SVECTOR_000a4874.vz = 0;
            SVECTOR_000a4874.vy = 0;
            RotMatrix(&SVECTOR_000a4874, &MATRIX_000a48a4);
            MulMatrix0(&MATRIX_000a48c4, &MATRIX_000a48a4, &MATRIX_000a48c4);
            MulMatrix0(&perspMatrixes[cameraIndex], &MATRIX_000a48c4, &MATRIX_000a48c4);

            ApplyMatrixSV(&perspMatrixes[cameraIndex], &tmpEnemyPos, &tmpEnemyScreenPos);
            MATRIX_000a48c4.t[0] = tmpEnemyScreenPos.vx + perspMatrixes[cameraIndex].t[0];
            MATRIX_000a48c4.t[1] = tmpEnemyScreenPos.vy + perspMatrixes[cameraIndex].t[1];
            MATRIX_000a48c4.t[2] = tmpEnemyScreenPos.vz + perspMatrixes[cameraIndex].t[2];

            CreateEnemyDispList(&rotationMatrix, rotationMatrix.t[2], 22, 0, 4096, 4096, 4096, 0,
                                tmpEnemyPixelPos.vx, tmpEnemyPixelPos.vy, tmpEnemyPixelPos.vz,
                                GetRotationIndexFromVector(enemies[enemyI].normalVec),
                                tmpEnemyPixelPos2.vx, tmpEnemyPixelPos2.vy, tmpEnemyPixelPos2.vz,
                                GetRotationIndexFromVector(enemies[enemyI].normalVec), &MATRIX_000a48c4, 128, 0);
        }
    }
}

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
