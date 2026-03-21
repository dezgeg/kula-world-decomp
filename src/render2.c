#include "common.h"

typedef struct P {
    ulong* p0;
    ulong* p1;
} P;

#define CUBE_INDEX_AT(x, y, z) (*(short*)(0x1af000 + (x) * 34 * 34 * 2 + (y) * 34 * 2 + (z) * 2))

extern int GteTransformAndClipQuad(SVECTOR * v1, SVECTOR * v2, SVECTOR * v3, SVECTOR * v4, int out1, int out2, int out3, int out4, int* p1, int* p2, int* p3);
extern void LoadScaledGteMatrix(MATRIX * matrix);

extern int cameraIndex;
extern int cameraIndex;
extern int cubeStates[256 * 16];
extern int* ggiPart0A;
extern int* ggiPart0A;
extern int itemsDispList[];
extern int itemsDispListIdx;
extern int playerEnemyDispListIdx;
extern int specialLevelType;
extern int specialLevelType;
extern int whichDrawDispEnv;
extern int whichDrawDispEnv;
extern POLY_FT4 shadowPrims[2][1][2][16];
extern P shadowPrimPtrs[2][1][2][16];
extern short* tgiPart1;
extern short* tgiPart1;
extern short* tgiPart3;
extern short* tgiPart3;
extern SVECTOR SHADOW_VERTEX1;
extern SVECTOR SHADOW_VERTEX2;
extern SVECTOR SHADOW_VERTEX3;
extern SVECTOR SHADOW_VERTEX4;
extern TgiFile * tgi;
extern TgiFile* tgi;
extern void* otag[2][1][1026];
extern void* otag[2][1][1026];
extern void* playerEnemyDispList[];

void Unused_FUN_0002caec(MATRIX *m, int z, int modelId, int param_4, int param_5, int param_6,int param_7, int param_8) {
    int* p;

    if (z < 400) {
        return;
    }
    z >>= 6;

    itemsDispList[itemsDispListIdx++] = &otag[whichDrawDispEnv][cameraIndex][z + 1];
    itemsDispList[itemsDispListIdx++] = param_8;
    if (specialLevelType == 1) {
        itemsDispList[itemsDispListIdx++] = tgiPart3[z];
    } else {
        itemsDispList[itemsDispListIdx++] = tgiPart1[z];
    }

    itemsDispList[itemsDispListIdx++] = param_5;
    itemsDispList[itemsDispListIdx++] = param_6;
    itemsDispList[itemsDispListIdx++] = param_7;

    modelId *= 4;
    if (z > tgi->lodDistance[5]) {
        modelId += 2;
    } else if (z > tgi->lodDistance[6]) {
        modelId += 1;
    }
    itemsDispList[itemsDispListIdx++] = &ggiPart0A[ggiPart0A[modelId] / 4];
    itemsDispList[itemsDispListIdx++] = param_4;

    p = (int *)m;
    for (z = 0; z < 8; z++) {
        itemsDispList[itemsDispListIdx++] = *p++;
    }
}

void CreateEnemyDispList(MATRIX *matrix, int z, int modelId, void* const0_arg4, void* colorR, void* colorG, void* colorB, void* const0_arg8,
                        uint blockX, uint blockY, uint blockZ, int dir,
                        uint otherBlockX, uint otherBlockY, uint otherBlockZ, int otherDir,
                        MATRIX *gteMatrix, int shadowColor, int shadowFlag) {
    int nextVertexId;
    POLY_FT4 *shadowPrim;
    int screenCoords[3];
    int i;
    int shadowDepth;
    int result2;
    int off;
    Quad* q;

    if (z < 400) return;
    z >>= 6;

    if (blockX < 34 && blockY < 34 && blockZ < 34 &&
            CUBE_INDEX_AT(blockX, blockY, blockZ) != -1 &&
            (q = (Quad*)cubeStates[CUBE_INDEX_AT(blockX, blockY, blockZ) * 16 + dir]) != -1) {
        shadowDepth = q->otagIndex;
    } else {
        shadowDepth = -1;
    }

    if (otherBlockX < 34 && otherBlockY < 34 && otherBlockZ < 34) {
        int cubeIndex = CUBE_INDEX_AT(otherBlockX, otherBlockY, otherBlockZ);
        if (cubeIndex != -1) {
            Quad* q = (Quad*)cubeStates[cubeIndex * 16 + otherDir];
            if (q != (Quad*)-1) {
                result2 = q->otagIndex;
                goto LAB_shadow;
            }
        }
    }

    result2 = -1;
LAB_shadow:
    shadowDepth = (ushort)shadowDepth;
    result2 &= 0xffff;
    if (result2 < shadowDepth) {
        shadowDepth = result2;
    }

    if (shadowDepth == 0xffff) {
        shadowDepth = z;
    } else {
        shadowDepth -= 2;
    }

    if (shadowDepth < 0) return;

    playerEnemyDispList[playerEnemyDispListIdx++] = &otag[whichDrawDispEnv][cameraIndex][shadowDepth + 1];
    playerEnemyDispList[playerEnemyDispListIdx++] = const0_arg8;

    if (specialLevelType == 1) {
        playerEnemyDispList[playerEnemyDispListIdx++] = (void*)(int)tgiPart3[shadowDepth];
    } else {
        playerEnemyDispList[playerEnemyDispListIdx++] = (void*)(int)tgiPart1[shadowDepth];
    }

    playerEnemyDispList[playerEnemyDispListIdx++] = colorR;
    playerEnemyDispList[playerEnemyDispListIdx++] = colorG;
    playerEnemyDispList[playerEnemyDispListIdx++] = colorB;

    modelId *= 4;
    if (z > tgi->lodDistance[5]) {
        modelId += 2;
    } else if (z > tgi->lodDistance[6]) {
        modelId += 1;
    }

    playerEnemyDispList[playerEnemyDispListIdx++] = &ggiPart0A[ggiPart0A[modelId] / 4];
    playerEnemyDispList[playerEnemyDispListIdx++] = const0_arg4;

    {
        int* mPtr = (int*)matrix;
        for (z = 0; z < 8; z++) {
            playerEnemyDispList[playerEnemyDispListIdx++] = (void*)*mPtr++;
        }
    }

    for (nextVertexId = 0; nextVertexId < 16; nextVertexId++) {
        if (shadowPrimPtrs[whichDrawDispEnv][cameraIndex][shadowFlag][nextVertexId].p0 == (ulong*)-1) break;
    }

    LoadScaledGteMatrix(gteMatrix);
    shadowPrim = &shadowPrims[whichDrawDispEnv][cameraIndex][shadowFlag][nextVertexId];

    if (GteTransformAndClipQuad(&SHADOW_VERTEX1, &SHADOW_VERTEX2, &SHADOW_VERTEX3, &SHADOW_VERTEX4,
                            (int)&shadowPrim->x0, (int)&shadowPrim->x1, (int)&shadowPrim->x2, (int)&shadowPrim->x3,
                            &screenCoords[0], &screenCoords[1], &screenCoords[2]) > 0) {
        shadowPrim->r0 = shadowColor;
        shadowPrim->g0 = shadowColor;
        shadowPrim->b0 = shadowColor;
        shadowPrimPtrs[whichDrawDispEnv][cameraIndex][shadowFlag][nextVertexId].p0 = (ulong*)&otag[whichDrawDispEnv][cameraIndex][shadowDepth + 1];
        shadowPrimPtrs[whichDrawDispEnv][cameraIndex][shadowFlag][nextVertexId].p1 = (ulong*)shadowPrim;
    }
}

INCLUDE_ASM("asm/nonmatchings/render2", CreatePlayerDispList);

INCLUDE_ASM("asm/nonmatchings/render2", CreateItemDispList);
