#include "common.h"

extern void DrawStaticUiSprite(short id, short x, short y, short count);
extern void DrawTextFancyFont(char* str, int x, int y);
extern void DrawTitleAndCopyrightSprites(void);
extern void HandleMenus0To6(void);
extern void OptionsMenu(void);
extern void PauseMenu(void);
extern void QuitAreYouSureMenu(void);
extern void ScreenAdjustMenu(void);
extern void SetBigGuiSpriteVisible(void);
extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);

int copycatModeStartingPlayer;
int curMenu;
int cursorPosInMenu[8];
int isPaused;
int repeatRateTimer;
int wasPausedPreviousFrame;

extern SVECTOR ZERO_SVECTOR_a3340;
extern int curLevel;
extern int curWorld2;
extern int gameMode;
extern int gameState;
extern int isFinal;
extern int levelEndReason;
extern int numTimeTrialPlayers;
extern int specialLevelType;
extern u32 controllerButtons;
extern uint prevControllerButtons;

void PauseOrMainMenu(void) {
    extern char S_FMTd_3[];
    char buf[64];

    if (gameState == 0) {
        DrawTitleAndCopyrightSprites();
        HandleMenus0To6();
        return;
    }

    SetBigGuiSpriteVisible();
    if (specialLevelType == 0) {
        short levelTextX = 112;
        int dummy = 1;
        int fullLevel = 0;
        if (gameMode == 0 || (gameMode == 2 && numTimeTrialPlayers == 1)) {
            DrawStaticUiSprite(3, 20, 20, 0);
            if (isFinal == 0) {
                fullLevel = curLevel + curWorld2 * 15 + 1;
            } else {
                fullLevel = curLevel + curWorld2 * 2 + +151;
            }
        }
        if (gameMode == 1) {
            DrawStaticUiSprite(15, 20, 20, 0);
            levelTextX += 8;
            fullLevel = curLevel * 2 + 1 + copycatModeStartingPlayer;
        }
        if (gameMode == 2 && numTimeTrialPlayers == 2) {
            DrawStaticUiSprite(15, 20, 20, 0);
            levelTextX += 8;
            fullLevel = curLevel + 1;
        }
        if (fullLevel < 100) {
            levelTextX -= 12;
        }
        if (fullLevel < 10) {
            levelTextX -= 12;
        }
        sprintf(buf, S_FMTd_3, fullLevel);
        DrawTextFancyFont(buf, levelTextX, 20);
    }
    if (controllerButtons == prevControllerButtons) {
        repeatRateTimer = repeatRateTimer + 1;
    } else {
        repeatRateTimer = 0;
    }
    if (repeatRateTimer > 20) {
        repeatRateTimer -= 5;
        prevControllerButtons = 0;
    }
    switch (curMenu) {
        case 3:
            QuitAreYouSureMenu();
            break;
        case 2:
            ScreenAdjustMenu();
            break;
        case 1:
            OptionsMenu();
            break;
        case 0:
            PauseMenu();
            break;
    }

    if (((controllerButtons & PAD_START & ~prevControllerButtons) && wasPausedPreviousFrame == 1) || !isPaused) {
        repeatRateTimer = 0;
        curMenu = 0;
        cursorPosInMenu[0] = 0;
        isPaused = 0;
        wasPausedPreviousFrame = 0;
        SndPlaySfx(109, 37000, &ZERO_SVECTOR_a3340, 8000);
    } else {
        wasPausedPreviousFrame = isPaused;
    }
}

void PauseMenu(void) {
    if ((controllerButtons & PAD_U & ~prevControllerButtons) != 0) {
        if (cursorPosInMenu[curMenu] <= 0) {
            cursorPosInMenu[curMenu] = 3;
        } else {
            cursorPosInMenu[curMenu]--;
        }
        SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    if ((controllerButtons & PAD_D & ~prevControllerButtons) != 0) {
        cursorPosInMenu[curMenu] = (cursorPosInMenu[curMenu] + 1) % 4;
        SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
    }

    DrawWidgets(1, cursorPosInMenu[curMenu]);

    if ((controllerButtons & PAD_CROSS & ~prevControllerButtons) != 0) {
        switch (cursorPosInMenu[curMenu]) {
            case 1:
                levelEndReason = -10;
                /* fall thru */
            case 0:
                isPaused = 0;
                break;
            case 2:
                curMenu = 1;
                cursorPosInMenu[1] = 0;
                SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
                return;
            case 3:
                curMenu = 3;
                return;
        }
    }
}
