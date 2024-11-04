#include "common.h"

typedef struct P {
    ulong* p0;
    ulong* p1;
} P;

extern int cameraIndex;
extern int toBeDisabledLightEffects[64];
extern int whichDrawDispEnv;
extern POLY_FT4 playerShadow[2][1][2][16];
extern POLY_FT4 specularPrims[2][1][16];
extern P shadowPrimPtrs[2][1][2][16];
extern P specularPrimPtrs[2][1][16];

void SetLightEffectToBeDisabled(int param_1) {
    int i;

    for (i = 0; i < 64; i++) {
        if (toBeDisabledLightEffects[i] == -1)
            break;
    }
    toBeDisabledLightEffects[i] = param_1;
}

void DrawShadowSprites(void) {
    int i;
    int j;
    ulong* p0;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 16; j++) {
            p0 = shadowPrimPtrs[whichDrawDispEnv][cameraIndex][i][j].p0;
            if (p0 != 0xffffffff) {
                addPrim(p0, shadowPrimPtrs[whichDrawDispEnv][cameraIndex][i][j].p1);
                shadowPrimPtrs[whichDrawDispEnv][cameraIndex][i][j].p0 = 0xffffffff;
            }
        }
    }
}

void DrawPlayerSpecularSprites(void) {
    int i;
    ulong* p0;

    for (i = 0; i < 16; i++) {
        p0 = specularPrimPtrs[whichDrawDispEnv][cameraIndex][i].p0;
        if (p0 != 0xffffffff) {
            addPrim(p0, specularPrimPtrs[whichDrawDispEnv][cameraIndex][i].p1);
            specularPrimPtrs[whichDrawDispEnv][cameraIndex][i].p0 = 0xffffffff;
        }
    }
}
