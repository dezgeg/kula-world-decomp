#include "common.h"

// Prototypes
extern void DrawPsxButtonBackground(void);
extern void DrawTextCrappyFont(char * str);
extern int GetControllerButtons(int slot);
extern int GetControllerStatus(int slot);
extern void MusicCheckForLoop(void);
extern void PutDrawAndDispEnvs(void);
extern void ResetTextRenderState(void);
extern void ResetTextVars(void);
extern void SetBigGuiSpriteVisible(void);
extern void SetTextParams(int posX, int posY, int align, int colorR, int colorG, int colorB);
extern void SndPlaySfx(int sfx, int tag, SVECTOR * dir, int volume);
extern void SndProcessSpuVoices(void);

// non-gprel-used variables (extern)
extern SVECTOR SVECTOR_000a2fac;
extern int controllerButtons;
extern int curController;
extern int displayHeight;
extern int displayWidth;
extern int musicShouldLoop;
extern int numCameras;
extern uint prevControllerButtons;
extern PrimList primLists[2];
extern int whichDrawDispEnv;
extern void* otag[2][1][1026];

typedef struct TgiBuf {
    char pad[0x10c];
    int otagLen;
} TgiBuf;
extern TgiBuf* tgi;

static inline int GetButtonsFromAnyController() {
    if (GetControllerStatus(curController) != 0) {
        controllerButtons = GetControllerButtons(curController);
    } else {
        controllerButtons = GetControllerButtons((curController + 1) % 2);
    }
    return controllerButtons;
}

static inline int TestButton(int button) {
    return controllerButtons & (button & ~prevControllerButtons);
}

extern char S_THIS_WILL_OVERWRITE_ANOTHER_SAVED_GAME_CONTINUE_g_YES_e_NO[];
int AskSaveOverwrite(void) {
    int i;

    while (1) {
        DrawSync(0);
        VSync(0);
        whichDrawDispEnv = whichDrawDispEnv == 0;
        PutDrawAndDispEnvs();
        for (i = 0; i < numCameras; i++) {
            ClearOTagR(&otag[whichDrawDispEnv][i][0], 1026);
        }
        ClearOTagR(&primLists[whichDrawDispEnv].main,4);
        ResetTextRenderState();
        for (i = 0; i < numCameras; i++) {
            DrawOTag(&otag[!whichDrawDispEnv][i][tgi->otagLen]);
        }
        DrawOTag(&primLists[!whichDrawDispEnv].gui3);
        SndProcessSpuVoices();
        prevControllerButtons = controllerButtons;
        GetButtonsFromAnyController();
        if (musicShouldLoop == 1) {
            MusicCheckForLoop();
        }
        SetBigGuiSpriteVisible();
        ResetTextVars();
        DrawPsxButtonBackground();
        SetTextParams(displayWidth / 2,displayHeight / 2 - 50,1,0x80,0x80,0x80);
        DrawTextCrappyFont(S_THIS_WILL_OVERWRITE_ANOTHER_SAVED_GAME_CONTINUE_g_YES_e_NO);
        if (TestButton(PAD_CROSS)) {
            SndPlaySfx(109,0,&SVECTOR_000a2fac,8000);
            prevControllerButtons = 0xffffffff;
            return 1;
        }
        if (TestButton(PAD_TRIANGLE)) {
            SndPlaySfx(109,0,&SVECTOR_000a2fac,8000);
            prevControllerButtons = 0xffffffff;
            return 0;
        }
    }

}

void ShowMemCardFullScreenText(char *str) {
    int i;
    int j;
    int iter;
    int l;

    for (iter = 0; iter < 3; iter++) {
        SetTextParams(displayWidth / 2,displayHeight / 2 - 25,1,0x80,0x80,0x80);
        DrawTextCrappyFont(str);
        DrawSync(0);
        VSync(0);
        whichDrawDispEnv = whichDrawDispEnv == 0;
        PutDrawAndDispEnvs();
        for (i = 0; i < numCameras; i++) {
            ClearOTagR(&otag[whichDrawDispEnv][i][0], 1026);
        }
        ClearOTagR(&primLists[whichDrawDispEnv].main,4);
        ResetTextRenderState();
        for (i = 0; i < numCameras; i++) {
            DrawOTag(&otag[!whichDrawDispEnv][i][tgi->otagLen]);
        }
        DrawOTag(&primLists[!whichDrawDispEnv].gui3);
        SndProcessSpuVoices();
        DrawPsxButtonBackground();
    }
}
