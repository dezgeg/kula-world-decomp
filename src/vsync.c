#include "common.h"

#include <libapi.h>

extern void DrawBigGuiSprite(int param_1);
#if !VER_EU
extern void DrawPsxButtonBackground(int);
#define DrawPsxButtonBackground() DrawPsxButtonBackground(1)
#else
extern void DrawPsxButtonBackground(void);
#endif
extern void FadeOutBigGuiSprite(void);
extern void FadePsxButtonBackground(void);
extern void PutDrawAndDispEnvs(void);
extern void SndFadeVoiceVolume(int tag);

extern int unusedNumHblanks[];
extern OT_TYPE otag[2][1][1026];
#if VER_JP
// XXX probably this file needs to be compiled with -G0
extern int gotVsync[];
extern int loadingIsComplete[];
extern int numCameras[];
extern uint unkUnused[];
extern int whichDrawDispEnv[];
#define gotVsync (gotVsync[0])
#define loadingIsComplete (loadingIsComplete[0])
#define numCameras (numCameras[0])
#define unkUnused (unkUnused[0])
#define whichDrawDispEnv (whichDrawDispEnv[0])
#else
extern int gotVsync;
extern int loadingIsComplete;
extern int numCameras;
extern uint unkUnused;
extern int whichDrawDispEnv;
#endif

extern uint* savedSp[];
extern uint* unusedInitStackParamEnd[];
extern uint* unusedInitStackParamStart[];

#ifndef SKIP_UNUSED_CODE
// This is used as DrawSyncCallback in some of the demos
void UnusedUpdateHblankCounter() {
    unkUnused++;
#if !VER_EU
    if (unkUnused < 8) {
        unusedNumHblanks[unkUnused] = VSync(1);
    }
#else
    if (unkUnused == 15) {
        unusedNumHblanks[0] = VSync(1);
    }
#endif
}
#endif

void MainGameVSyncCallback() {
    gotVsync = 1;
}

void LoadingScreenVSyncCallback(void) {
    int i;

    whichDrawDispEnv = !whichDrawDispEnv;
    PutDrawAndDispEnvs();
    for (i = 0; i < numCameras; i++) {
        ClearOTagR(&otag[whichDrawDispEnv][i][0], 1026);
    }
    for (i = 0; i < numCameras; i++) {
        DrawOTag(&otag[!whichDrawDispEnv][i][1024] - 1);
    }

    DrawBigGuiSprite(1);
    DrawPsxButtonBackground();
    if (loadingIsComplete > 0) {
        SndFadeVoiceVolume(888);
        FadePsxButtonBackground();
        FadeOutBigGuiSprite();
    }
}

#ifndef SKIP_UNUSED_CODE
void UnusedInitStack(uint end, uint start) {
    uint* endMasked;
    uint* p;
    int val;

    unusedInitStackParamEnd[0] = endMasked = (uint*)(end & 0xfffffff);
    unusedInitStackParamStart[0] = (uint*)(start & 0xfffffff);

    p = (int*)(GetSp() & 0xfffffff);
    val = 0x12345678;
    savedSp[0] = p;

    while (endMasked <= --p) {
        *p = val++;
    }
}

int UnusedValidateStack(void) {
    uint* p = unusedInitStackParamEnd[0];
    uint val = (savedSp[0] - unusedInitStackParamEnd[0]) + 0x12345678;

    while (p <= unusedInitStackParamStart[0]) {
        if (*p++ != --val) {
            break;
        }
    }

    return unusedInitStackParamStart[0] - p;
}
#endif
