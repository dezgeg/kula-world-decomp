#include "common.h"

extern void DrawBigGuiSprite(int param_1);
extern void DrawPsxButtonBackground(void);
extern void FadeOutBigGuiSprite(void);
extern void FadePsxButtonBackground(void);
extern void PutDrawAndDispEnvs(void);
extern void SndFadeVoiceVolume(int tag);

extern int gotVsync;
extern int loadingIsComplete;
extern int numCameras;
extern int unkUnused;
extern int unusedNumHblanks;
extern int whichDrawDispEnv;
extern void* otag[2][1][1026];

void UnusedUpdateHblankCounter() {
    unkUnused++;
    if (unkUnused == 15) {
        unusedNumHblanks = VSync(1);
    }
}

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
