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
extern int musicVolume;
extern int numTimeTrialPlayers;
extern int sfxVolume;
extern int specialLevelType;
extern int vibrationEnabled;
extern short MENU_CURSOR_START_Y_MAIN_MENU[20];
extern short MENU_CURSOR_START_Y_PAUSE_MENU[20];
extern short turnDelayEnabled;
extern uint controllerButtons;
extern uint prevControllerButtons;

static inline int TestButton(int button) {
    return (controllerButtons & (~prevControllerButtons & button)) != 0;
}

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

    if ((TestButton(PAD_START) && wasPausedPreviousFrame == 1) || !isPaused) {
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
    if (TestButton(PAD_U)) {
        if (cursorPosInMenu[curMenu] <= 0) {
            cursorPosInMenu[curMenu] = 3;
        } else {
            cursorPosInMenu[curMenu]--;
        }
        SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    if (TestButton(PAD_D)) {
        cursorPosInMenu[curMenu] = (cursorPosInMenu[curMenu] + 1) % 4;
        SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
    }

    DrawWidgets(1, cursorPosInMenu[curMenu]);

    if (TestButton(PAD_CROSS)) {
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

void OptionsMenu(void) {
    int x;
    int y;
    int onOffSprite;

    int decrement;
    int increment;
    int cross;

    if (TestButton(PAD_U)) {
        if (cursorPosInMenu[curMenu] < 1) {
            cursorPosInMenu[curMenu] = 5;
        } else {
            cursorPosInMenu[curMenu]--;
        }
        SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    if (TestButton(PAD_D)) {
        cursorPosInMenu[curMenu] = (cursorPosInMenu[curMenu] + 1) % 6;
        SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    DrawWidgets(2, cursorPosInMenu[curMenu]);
    SetBigGuiSpriteVisible();
    if (gameState == 0) {
        y = MENU_CURSOR_START_Y_MAIN_MENU[2];
    } else {
        y = MENU_CURSOR_START_Y_PAUSE_MENU[2];
    }
    onOffSprite = 18;
    if (turnDelayEnabled == 1) {
        onOffSprite = 17;
        x = 192;
    } else {
        x = 189;
    }

    DrawStaticUiSprite(onOffSprite, x, y + 59, 0);
    onOffSprite = 18;
    if (vibrationEnabled == 1) {
        onOffSprite = 17;
        x = 187;
    } else {
        x = 184;
    }
    DrawStaticUiSprite(onOffSprite, x, y + 79, 0);

    cross = TestButton(PAD_CROSS);
    decrement = TestButton(PAD_L);
    increment = TestButton(PAD_R);

    if (TestButton(PAD_TRIANGLE)) {
        curMenu--;
        SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    switch (cursorPosInMenu[curMenu]) {
        case 0:
            if (decrement && sfxVolume > 0) {
                sfxVolume--;
                SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
            }
            if (increment && sfxVolume < 12) {
                sfxVolume++;
                SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
            }
            break;
        case 1:
            if (decrement && musicVolume > 0) {
                musicVolume--;
                SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
            }
            if (increment && musicVolume < 12) {
                musicVolume++;
                SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
            }
            SndSetMusicVolume();
            break;
        case 2:
            if (cross) {
                /* adjust screen */
                curMenu = 2;
                SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
            }

            break;
        case 3:
            if (increment && turnDelayEnabled == 0) {
                turnDelayEnabled = 1;
                SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
            }
            if (decrement && turnDelayEnabled == 1) {
                turnDelayEnabled = 0;
                SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
            }
            if (cross) {
                turnDelayEnabled = turnDelayEnabled ^ 1;
                SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
            }
            break;
        case 4:
            if (increment && vibrationEnabled == 0) {
                vibrationEnabled = 1;
                SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
            }
            if (decrement && vibrationEnabled == 1) {
                vibrationEnabled = 0;
                SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
            }
            if (cross) {
                vibrationEnabled = vibrationEnabled ^ 1;
                SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
            }
            break;
        case 5:
            if (cross) {
                curMenu--;
                SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
            }

            break;
    }
}

void QuitAreYouSureMenu(void) {
    DrawWidgets(18, 0);
    if (TestButton(PAD_TRIANGLE)) {
        curMenu = 0;
    }
    if (TestButton(PAD_CIRCLE)) {
        QuitToMainMenu();
    }
}
