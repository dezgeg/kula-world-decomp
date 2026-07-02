#include "common.h"

typedef struct P {
    ulong* p0;
    ulong* p1;
} P;

typedef struct ItemEntry {
    short id;
    short rotation;
    short variant;
    short state;
    short unk8;
    short power1;
    short power2;
    short unkE;
    short unk10;
    short unk12;
    short unk14;
    short unk16;
    short unk18;
    short unk1A;
    short counter;
    short unk1E;
} ItemEntry;

#define IE_ID 1
#define IE_VARIANT 3
#define IE_STATE 4
#define IE_UNK8 8
#define IE_COUNTER 15
#define IE_UNK16 16

typedef struct EntityBlock {
    short tag;
    ItemEntry items[6];
    short pad[28];
    short x;
    short y;
    short z;
} EntityBlock;

extern int GteTransformAndClipQuad(SVECTOR * v1, SVECTOR * v2, SVECTOR * v3, SVECTOR * v4, int out1, int out2, int out3, int out4, int* p1, int* p2, int* p3);
extern void LoadScaledGteMatrix(MATRIX * matrix);

extern int AddLightEffect(int x, int y, int z, int whichSide);
extern void SetLightEffectColor(int param_1, int param_2);
extern void SetLightEffectToBeDisabled(int param_1);

extern int cameraIndex;
extern int curWorld;
extern void* cubeStates[256 * 16];
extern short* entityData;
extern int* ggiPart0A;
extern int* ggiPart0B;
extern int itemsDispList[];
extern int itemsDispListIdx;
extern int playerEnemyDispListIdx;
extern int specialLevelType;
extern int whichDrawDispEnv;
extern POLY_FT4 shadowPrims[2][1][2][16];
extern P shadowPrimPtrs[2][1][2][16];
extern short* tgiPart1;
extern short* tgiPart3;
extern TgiFile* tgi;
extern void* otag[2][1][1026];
extern void* playerEnemyDispList[];

int BUTTON_COLORS[] = {
    0x001919, 0xCC0000, 0x001900, 0x000019,
    0x001919, 0x800000, 0x001900, 0x000019,
    0x001919, 0x800000, 0x001900, 0x000019,
    0x001919, 0x200000, 0x001900, 0x000019,
    0x003319, 0x800000, 0x003300, 0x000019,
    0x001919, 0x590000, 0x001900, 0x000019,
    0x001940, 0x190000, 0x001900, 0x000040,
    0x001919, 0x2C0000, 0x001900, 0x000019,
    0x002619, 0xC00000, 0x002600, 0x000019,
    0x003919, 0x800000, 0x003900, 0x000019,
};

SVECTOR SHADOW_VERTEX1 = { 150, 0, 150, 0 };
SVECTOR SHADOW_VERTEX2 = { -150, 0, 150, 0 };
SVECTOR SHADOW_VERTEX3 = { 150, 0, -150, 0 };
SVECTOR SHADOW_VERTEX4 = { -150, 0, -150, 0 };

int EXIT_LIGHT_COLORS_PER_WORLD[10] = {
    0x00104010,
    0x00104010,
    0x00104010,
    0x00104010,
    0x00104010,
    0x00104010,
    0x00104010,
    0x00104010,
    0x00104010,
    0x00104010,
};

int TRANSPORTER_COLORS[4][3] = {
    { 6144, 6144, 5120 },
    { 5120, 5120, 6144 },
    { 5120, 6144, 5120 },
    { 6144, 5120, 5120 },
};

#define CUBE_INDEX_AT(x, y, z) (*(short*)(0x1af000 + (x) * 34 * 34 * 2 + (y) * 34 * 2 + (z) * 2))

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

void CreateItemDispList(MATRIX *m,int z,int entityIndex,int dirIndexInBlock) {
    int* colorPtr;
    EntityBlock *ent;
    int blinkState;
    int lightEffectId;
    int depth;
    int depthOffset;
    int colorR;
    int colorG;
    int colorB;
    Quad *quad;
    int* p;
    short* e;
    int two = 2;

    short *entBase = entityData + entityIndex * 128;
    quad = cubeStates[16 * CUBE_INDEX_AT(*(entBase + 125), *(entBase + 126), *(entBase + 127)) + dirIndexInBlock];
    e = entBase + dirIndexInBlock * 16;
    if (specialLevelType == 1) {
        colorPtr = (int *)((char *)tgi + (tgi->unk12c[dirIndexInBlock] * 12 + 156));
    } else {
        colorPtr = (int *)((char *)tgi + (tgi->unk12c[dirIndexInBlock] * 12 + 24));
    }
    colorR = colorPtr[0];
    colorG = colorPtr[1];
    colorB = colorPtr[2];
    depthOffset = 0;
    switch(e[IE_ID]) {
    case 5:
        // transporter
        lightEffectId = e[IE_UNK16] & 0xff;
        blinkState = e[IE_UNK16] >> 8;
        depthOffset = 1;
        if (e[IE_STATE] == 1) {
            e[IE_COUNTER]--;
            if (e[IE_COUNTER] < 1) {
                e[IE_COUNTER] = 16;
                if (blinkState == 0) {
                    ent = (EntityBlock *)(entityData + entityIndex * 128);
                    lightEffectId = AddLightEffect(ent->x,ent->y,ent->z,dirIndexInBlock);
                    blinkState = 1;
                    e[IE_UNK16] = lightEffectId | 0x100;
                } else {
                    blinkState = 0;
                    SetLightEffectToBeDisabled(lightEffectId);
                    e[IE_UNK16] = 0xff;
                }
            }
            if (blinkState == 1) {
                colorR = colorR * TRANSPORTER_COLORS[e[IE_VARIANT]][0] >> 12;
                colorG = colorG * TRANSPORTER_COLORS[e[IE_VARIANT]][1] >> 12;
                colorB = colorB * TRANSPORTER_COLORS[e[IE_VARIANT]][2] >> 12;
                SetLightEffectColor(lightEffectId, BUTTON_COLORS[curWorld * 4 + e[IE_VARIANT]]);
            }
            break;
        }
        depthOffset = 1;
        goto disable_light;
    default:
        break;
    case 7:
        // exit
        depthOffset = 1;
        lightEffectId = e[IE_UNK16] & 0xff;
        blinkState = e[IE_UNK16] >> 8;
        if (e[IE_STATE] == 1) {
            e[IE_COUNTER]--;
            if (e[IE_COUNTER] < 1) {
                e[IE_COUNTER] = 16;
                if (blinkState == 0) {
                    ent = (EntityBlock *)(entityData + entityIndex * 128);
                    lightEffectId = AddLightEffect(ent->x,ent->y,ent->z,dirIndexInBlock);
                    blinkState = 1;
                    e[IE_UNK16] = lightEffectId | 0x100;
                } else {
                    blinkState = 0;
                    SetLightEffectToBeDisabled(lightEffectId);
                    e[IE_UNK16] = 0xff;
                }
            }
            if (blinkState == 1) {
                colorPtr = EXIT_LIGHT_COLORS_PER_WORLD[curWorld];
                colorR = (colorR << 13) >> 12;
                colorG = (colorG << 13) >> 12;
                colorB = (colorB << 13) >> 12;
                SetLightEffectColor(lightEffectId,colorPtr);
            }
            break;
        }
        depthOffset = 1;
        goto disable_light;
    case 9:
        // button
        depthOffset = 1;
        lightEffectId = e[IE_UNK16] & 0xff;
        blinkState = e[IE_UNK16] >> 8;
        if (e[IE_STATE] == 1) {
            e[IE_COUNTER]--;
            if (e[IE_COUNTER] < 1) {
                e[IE_COUNTER] = 0x10;
                if (blinkState == 0) {
                    ent = (EntityBlock *)(entityData + entityIndex * 128);
                    lightEffectId = AddLightEffect(ent->x,ent->y,ent->z,dirIndexInBlock);
                    blinkState = 1;
                    e[IE_UNK16] = lightEffectId | 0x100;
                } else {
                    blinkState = 0;
                    SetLightEffectToBeDisabled(lightEffectId);
                    e[IE_UNK16] = 0xff;
                }
            }
            if (blinkState == 1) {
                colorPtr = BUTTON_COLORS[curWorld * 4 + e[IE_VARIANT]];
                colorR = (colorR << 13) >> 12;
                colorG = (colorG << 13) >> 12;
                colorB = (colorB << 13) >> 12;
                SetLightEffectColor(lightEffectId,colorPtr);
            }
            break;
        }
        depthOffset = 1;
disable_light:
        if (lightEffectId != 0xff) {
            SetLightEffectToBeDisabled(lightEffectId);
            e[IE_UNK16] = 0xff;
        }
        break;
    case 10:
    case 11:
    case 12:
    case 26:
    case 28:
        depthOffset = 1;
        break;
    }

    if (z < 400) {
        return;
    }
    
    z >>= 6;
    depth = quad->otagIndex;
    if (depth == -1) {
        depth = z;
    } else {
        depth += depthOffset - two;
    }
    if (depth < 0) {
        return;
    }
    // 0: otag pointer
    itemsDispList[itemsDispListIdx++] = &otag[whichDrawDispEnv][cameraIndex][depth+1];
    // 1: always zero
    itemsDispList[itemsDispListIdx++] = 0;
    // 2: depth value
    if (specialLevelType == 1) {
        itemsDispList[itemsDispListIdx++] = tgiPart3[depth];
    } else {
        itemsDispList[itemsDispListIdx++] = tgiPart1[depth];
    }
    // 3: color R
    // 4: color G
    // 5: color B
    itemsDispList[itemsDispListIdx++] = colorR;
    itemsDispList[itemsDispListIdx++] = colorG;
    itemsDispList[itemsDispListIdx++] = colorB;
    z = e[IE_ID] * 16;
    if (depth > tgi->lodDistance[5]) {
        z += 8;
    } else if (depth > tgi->lodDistance[6]) {
        z += 4;
    }
    z += e[IE_VARIANT];
    // 6: model ptr
    itemsDispList[itemsDispListIdx++] = &ggiPart0B[ggiPart0B[z] / 4];
    // 7: unknown
    itemsDispList[itemsDispListIdx++] = e[IE_UNK8];
    // 8+: matrix
    p = m;
    for (z = 0; z < 8; z++) {
        itemsDispList[itemsDispListIdx++] = *p++;
    }
}
