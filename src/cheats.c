#include "common.h"

typedef struct CheatState {
    int activated;
    int toggled;
    int preventActivation;
    int unused;
} CheatState;

extern void EnableLethargy(int enable);
extern void SetInvulnerable(void);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering,
                         u_char use24Bit);
extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);
extern void TweakSkyboxColors(uint param_1);

uint buttonBuf[8];
uint* buttonsPtr;
int cheated;
int sioCheatActivated;

extern CheatState cheatState[9];
extern uint CHEAT_BUTTONS[9][8];
extern uint controllerButtons;
extern SVECTOR SVECTOR_000a3334;
extern int finalUnlocked;
extern int gameState;
extern int isPaused;
extern int levelEndReason;
extern int levelPlayTime[2];
extern int levelScore;
extern int specialLevelType;
extern uint prevControllerButtons;

void ResetCheats(void) {
    int i, j;

    for (i = 0; i < 9; i++) {
        int* p = &cheatState[i];
        for (j = 0; j < 4; j++) {
            p[j] = 0;
        }
    }
    for (i = 0; i < 8; i++) {
        buttonBuf[i] = 0;
    }
    buttonsPtr = buttonBuf;
}

void HandleCheats(void) {
    uint* p;
    int buts;
    int ch1;
    int ref;
    int i;
    int j;
    int k;

    i = controllerButtons & ~prevControllerButtons; // XXX: permuter mess
    buts = i;
    if (buts != 0) {

        *buttonsPtr = buts;
        buttonsPtr++;
        if (buttonsPtr >= buttonBuf + 8) {
            buttonsPtr -= 8;
        }
        for (i = 0; i < 9; i++) {
            p = buttonsPtr;
            for (j = 7; j >= 0; j--) {
                p--;
                if (p < buttonBuf) {
                    p += 8;
                }
                if (CHEAT_BUTTONS[i][j] != *p) {
                    break;
                }
            }
            if (j < 0) {
                if (isPaused == 0 && gameState != 0) {
                    cheatState[i].activated = 1;
                    cheatState[i].toggled ^= 1;
                    SndPlaySfx(0x6d, 0, &SVECTOR_000a3334, 0x1f40);
                }
                for (j = 0; j < 8; j++) {
                    buttonBuf[j] = 0;
                }
                buttonsPtr = buttonBuf;
                break;
            }
        }
    }
    if (cheatState[0].activated == 1) {
        cheatState[0].activated = 0;
        if (cheatState[0].toggled == 1) {
            TweakSkyboxColors(0x787878);
        } else {
            TweakSkyboxColors(0x808080);
        }
    }

    if (cheatState[1].activated == 1) {
        cheatState[1].activated = 0;
        if (specialLevelType == 2) {
            if (cheatState[1].toggled == 1) {
                SetupDisplay(0, 0, 0, 0, 0, 0);
            } else {
                SetupDisplay(1, 0, 0, 0, 0, 0);
            }
        }
    }
    if (cheatState[2].activated == 1) {
        cheatState[2].activated = 0;
        if (cheatState[2].toggled == 1) {
                EnableLethargy(1);
            } else {
                EnableLethargy(0);
            }
    }
    if (cheatState[3].activated == 1) {
        cheatState[3].activated = 0;
        if (sioCheatActivated == 0) {
            sioCheatActivated = 1;
            AddSIO(9600);
        }
    }
    if (cheatState[4].activated == 1) {
        cheatState[4].activated = 0;
        cheated = 1;
        levelEndReason = 3;
    }
    if (cheatState[5].activated == 1) {
        cheatState[5].activated = 0;
        if (cheatState[5].preventActivation == 0) {
            cheatState[5].preventActivation = 1;
            cheated = 1;
            levelScore = levelScore + 30000;
        }
    }
    if (cheatState[6].activated == 1) {
        cheatState[6].activated = 0;
        cheated = 1;
        finalUnlocked = 1;
    }
    if (cheatState[7].activated == 1) {
        cheatState[7].activated = 0;
        if (cheatState[7].preventActivation == 0) {
            cheatState[7].preventActivation = 1;
            cheated = 1;
            levelPlayTime[0] = levelPlayTime[0] + -1500;
        }
    }
            if (cheatState[8].activated == 1) {
            cheatState[8].activated = 0;
            cheated = 1;
            SetInvulnerable();
        }
}
