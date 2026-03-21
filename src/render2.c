#include "common.h"

extern int cameraIndex;
extern int* ggiPart0A;
extern int itemsDispList[];
extern int itemsDispListIdx;
extern int specialLevelType;
extern TgiFile* tgi;
extern short* tgiPart1;
extern short* tgiPart3;
extern int whichDrawDispEnv;
extern void* otag[2][1][1026];

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

INCLUDE_ASM("asm/nonmatchings/render2", CreateEnemyDispList);

INCLUDE_ASM("asm/nonmatchings/render2", CreatePlayerDispList);

INCLUDE_ASM("asm/nonmatchings/render2", CreateItemDispList);
