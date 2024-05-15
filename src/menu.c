#include "common.h"

extern void DrawStaticUiSprite(short id, short x, short y, short count);
extern void DrawTextFancyFont(char* str, short x, short y);
extern void DrawTitleAndCopyrightSprites(void);
extern void HandleMenus0To6(void);
extern void MainMenu(void);
extern void OptionsMenu(void);
extern void PauseMenu(void);
extern void QuitAreYouSureMenu(void);
extern void ScreenAdjustMenu(void);
extern void SetBigGuiSpriteVisible(void);
extern void SinglePlayerMenu(void);
extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);
extern void TimeTrialDifficultySelectionMenu(void);
extern void TwoPlayerLevelSelectMenu(void);
extern void TwoPlayerMenu(void);

short qualifyScreenCursorX;
short qualifyScreenCursorY;
int copycatModeStartingPlayer;
int curMenu;
int cursorPosInMenu[8];
int isPaused;
int levelScoreSummaryConfirmed;
int menuUnkAlwaysZero;
int repeatRateTimer;
int savePointMenuConfirmed;
int timeTrialAtEndOfWorld;
int twoPlayerLevelSelectionCursorPos;
int wasPausedPreviousFrame;

extern SVECTOR ZERO_SVECTOR_a3340;
extern char scoreText[128];
extern int TIME_TRIAL_PAR_TIMES[150];
extern int byteCountToReceiveFromSio;
extern int cheated;
extern int copycatPlayerScores[2];
extern int curLevel;
extern int curWorld2;
extern int curWorld;
extern int debugBonusLevels;
extern int debugDisableTimer;
extern int dispenvScreenX;
extern int dispenvScreenY;
extern int displayHeight;
extern int displayWidth;
extern int drawCopyright;
extern int drawCopyright;
extern int finalUnlocked;
extern int gameMode;
extern int gameState;
extern int gotSioData;
extern int highestLevelReached;
extern int isFinal;
extern int levelEndReason;
extern int levelHasBeenCompletedByPlayer[2];
extern int levelPlayTime[2];
extern int levelPlaytimesInThisWorld[15];
extern int levelScore;
extern int loadNewWorld;
extern int musicVolume;
extern int numCameras;
extern int numTimeTrialPlayers;
extern int screenOffsetY;
extern int screenOffsetY;
extern int sfxVolume;
extern int skipNextLoad;
extern int specialLevelType;
extern int startingPlayerForThisLevel;
extern int timeTrialDifficulty;
extern int totalPlayTime[2];
extern int totalScore;
extern int twoPlayerWhichPlayer;
extern int vibrationEnabled;
extern int whichDrawDispEnv;
extern short MENU_CURSOR_START_Y_MAIN_MENU[20];
extern short MENU_CURSOR_START_Y_PAUSE_MENU[20];
extern short numFruits;
extern short turnDelayEnabled;
extern uint curController;
extern uint controllerButtons;
extern uint prevControllerButtons;
extern LINE_F3 screenAdjustLine1[2];
extern LINE_F3 screenAdjustLine2[2];
extern DrawDisp drawdisp[2];
extern PrimList primLists[2];

static inline int TestButton(int button) {
    return (controllerButtons & (~prevControllerButtons & button)) != 0;
}

static inline void SetDispScreen(ushort x, ushort y) {
    drawdisp[0].disp.screen.x = drawdisp[1].disp.screen.x = x;
    drawdisp[0].disp.screen.y = drawdisp[1].disp.screen.y = y;
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

void QuitToMainMenu(void) {
    gameState = 4;
    curMenu = 0;
    cursorPosInMenu[0] = 0;
    isPaused = 1;
    debugDisableTimer = 0;
    debugBonusLevels = 0;
    totalScore = -1;
    levelScore = 0;
    wasPausedPreviousFrame = 0;
    curController = 0;
    twoPlayerWhichPlayer = 0;
    curWorld = 0;
    timeTrialAtEndOfWorld = 0;
    byteCountToReceiveFromSio = 0;
    gotSioData = 0;
    if (gameMode == 1 || isFinal == 1) {
        loadNewWorld = 1;
    }
}

void ScreenAdjustMenu(void) {
    DrawWidgets(10, 0);

    SetLineF3(&screenAdjustLine1[whichDrawDispEnv]);
    SetLineF3(&screenAdjustLine2[whichDrawDispEnv]);

    setRGB0(&screenAdjustLine1[whichDrawDispEnv], 255, 0, 0);
    setXY3(&screenAdjustLine1[whichDrawDispEnv],
            0, 0,
            displayWidth - 1, 0,
            displayWidth - 1, displayHeight - 1);

    setRGB0(&screenAdjustLine2[whichDrawDispEnv], 255, 0, 0);
    setXY3(&screenAdjustLine2[whichDrawDispEnv],
            displayWidth - 1, displayHeight - 1,
            0, displayHeight - 1,
            0, 0);

    addPrim(&primLists[whichDrawDispEnv].main, &screenAdjustLine1[whichDrawDispEnv]);
    addPrim(&primLists[whichDrawDispEnv].main, &screenAdjustLine2[whichDrawDispEnv]);

    if (controllerButtons & PAD_U && dispenvScreenY > 0) {
        dispenvScreenY--;
    }
    if (controllerButtons & PAD_D && dispenvScreenY < 60) {
        dispenvScreenY++;
    }
    if (controllerButtons & PAD_L && dispenvScreenX > 0) {
        dispenvScreenX--;
    }
    if (controllerButtons & PAD_R && dispenvScreenX < 60) {
        dispenvScreenX++;
    }

    if (TestButton(PAD_TRIANGLE)) {
        dispenvScreenX = 0;
        dispenvScreenY = 18;
        SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
    }

    SetDispScreen(dispenvScreenX, dispenvScreenY);

    if (TestButton(PAD_CROSS)) {
        curMenu--;
        SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
    }
}

void HandleMenus0To6(void) {
    if (isPaused == -1) {
        repeatRateTimer = 0;
        menuUnkAlwaysZero = 0;
    }
    if (controllerButtons == prevControllerButtons) {
        repeatRateTimer++;
    } else {
        repeatRateTimer = 0;
    }

    if (repeatRateTimer > 20) {
        repeatRateTimer -= 5;
        prevControllerButtons = 0;
    }
    switch (curMenu) {
        case 0:
            MainMenu();
            break;
        case 1:
            OptionsMenu();
            break;
        case 2:
            ScreenAdjustMenu();
            break;
        case 3:
            SinglePlayerMenu();
            break;
        case 4:
            TwoPlayerMenu();
            break;
        case 5:
            TimeTrialDifficultySelectionMenu();
            break;
        case 6:
            TwoPlayerLevelSelectMenu();
            break;
    }
}

void MainMenu(void) {
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
    DrawWidgets(0, cursorPosInMenu[curMenu]);
    if (TestButton(PAD_CROSS)) {
        drawCopyright = 0;
        switch (cursorPosInMenu[curMenu]) {
            case 0:
                curMenu = 3;
                cursorPosInMenu[3] = 0;
                SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
                break;
            case 1:
                curMenu = 4;
                cursorPosInMenu[4] = 0;
                SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
                break;
            case 2:
                HighScoreUi(0);
                break;
            case 3:
                curMenu = 1;
                cursorPosInMenu[1] = 0;
                SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
                break;
        }
    }
}

void SinglePlayerMenu(void) {
    int fullLevel;
    int i;

    if (finalUnlocked == 1) {
        SinglePlayerMenuWhenFinalUnlocked();
    } else {
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
        DrawWidgets(4, cursorPosInMenu[curMenu]);
        if (TestButton(PAD_TRIANGLE)) {
            cursorPosInMenu[curMenu] = 0;
            curMenu = 0;
            SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
        }
        if (TestButton(PAD_CROSS)) {
            switch (cursorPosInMenu[curMenu]) {
                case 0:
                    numFruits = 0;
                    wasPausedPreviousFrame = 0;
                    gameMode = 0;
                    totalPlayTime[0] = 0;
                    numCameras = 1;
                    isPaused = 0;
                    cursorPosInMenu[curMenu] = 0;
                    curMenu = 0;
                    cheated = 0;
                    screenOffsetY = displayHeight;
                    gameState++;
                    InitAllDigitSprites();
                    SndPlaySfx(30, 0, &ZERO_SVECTOR_a3340, 7000);
                    break;
                case 1:
                    curMenu = 5;
                    SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
                    break;
                case 2:
                    drawCopyright = 0;
                    if (LoadSaveMenu() != 1) {
                        return;
                    }
                    if (gameMode == 0) {
                        wasPausedPreviousFrame = 0;
                        gameMode = 0;
                        screenOffsetY = displayHeight;
                        numCameras = 1;
                        isPaused = 0;
                        cursorPosInMenu[curMenu] = 0;
                        curMenu = 0;
                        cheated = 0;
                        gameState++;
                        InitAllDigitSprites();
                        loadNewWorld = 1;
                        if (curWorld == curWorld2) {
                            skipNextLoad = 1;
                        }
                        return;
                    }

                    numTimeTrialPlayers = 1;
                    twoPlayerWhichPlayer = 0;
                    for (i = 0; i < 15; i++) {
                        levelPlaytimesInThisWorld[i] = 0;
                    }

                    for (i = 0; i < numTimeTrialPlayers; i++) {
                        levelPlayTime[i] = (-TIME_TRIAL_PAR_TIMES[curWorld * 15 + curLevel] - timeTrialDifficulty) * 50;
                        levelHasBeenCompletedByPlayer[i] = 0;
                    }
                    gameMode = 2;
                    screenOffsetY = displayHeight;
                    wasPausedPreviousFrame = 0;
                    numCameras = 1;
                    isPaused = 0;
                    cursorPosInMenu[curMenu] = 0;
                    curMenu = 0;
                    cheated = 0;
                    startingPlayerForThisLevel = 0;
                    gameState++;
                    InitAllDigitSprites();
                    loadNewWorld = 1;
                    if (curWorld == 0 && !isFinal) {
                        skipNextLoad = 1;
                    }
                    break;
                case 3:
                    cursorPosInMenu[curMenu] = 0;
                    curMenu = 0;
                    SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
                    break;
            }
        }
    }
}

void SinglePlayerMenuWhenFinalUnlocked(void) {
    int fullLevel;
    int i;

    if (TestButton(PAD_U)) {
        if (cursorPosInMenu[curMenu] <= 0) {
            cursorPosInMenu[curMenu] = 4;
        } else {
            cursorPosInMenu[curMenu]--;
        }
        SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    if (TestButton(PAD_D)) {
        cursorPosInMenu[curMenu] = (cursorPosInMenu[curMenu] + 1) % 5;
        SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    DrawWidgets(6, cursorPosInMenu[curMenu]);
    if (TestButton(PAD_TRIANGLE)) {
        cursorPosInMenu[curMenu] = 0;
        curMenu = 0;
        SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    if (TestButton(PAD_CROSS)) {
        switch (cursorPosInMenu[curMenu]) {
            case 0:
                isFinal = 1;
                numFruits = 0;
                wasPausedPreviousFrame = 0;
                gameMode = 0;
                totalPlayTime[0] = 0;
                screenOffsetY = displayHeight;
                numCameras = 1;
                isPaused = 0;
                cursorPosInMenu[curMenu] = 0;
                curMenu = 0;
                cheated = 0;
                gameState++;
                InitAllDigitSprites();
                loadNewWorld = 1;
                break;
            case 1:
                numFruits = 0;
                wasPausedPreviousFrame = 0;
                gameMode = 0;
                totalPlayTime[0] = 0;
                screenOffsetY = displayHeight;
                numCameras = 1;
                isPaused = 0;
                cursorPosInMenu[curMenu] = 0;
                curMenu = 0;
                cheated = 0;
                gameState++;
                InitAllDigitSprites();
                SndPlaySfx(30, 0, &ZERO_SVECTOR_a3340, 7000);
                break;
            case 2:
                curMenu = 5;
                SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
                break;
            case 3:
                if (LoadSaveMenu() != 1) {
                    return;
                }
                if (gameMode == 0) {
                    wasPausedPreviousFrame = 0;
                    gameMode = 0;
                    screenOffsetY = displayHeight;
                    numCameras = 1;
                    isPaused = 0;
                    cursorPosInMenu[curMenu] = 0;
                    curMenu = 0;
                    cheated = 0;
                    gameState++;
                    InitAllDigitSprites();
                    loadNewWorld = 1;
                    if (curWorld == curWorld2) {
                        skipNextLoad = 1;
                    }
                    return;
                }

                numTimeTrialPlayers = 1;
                twoPlayerWhichPlayer = 0;
                for (i = 0; i < 15; i++) {
                    levelPlaytimesInThisWorld[i] = 0;
                }

                for (i = 0; i < numTimeTrialPlayers; i++) {
                    levelPlayTime[i] = (-TIME_TRIAL_PAR_TIMES[curWorld * 15 + curLevel] - timeTrialDifficulty) * 50;
                    levelHasBeenCompletedByPlayer[i] = 0;
                }
                gameMode = 2;
                screenOffsetY = displayHeight;
                wasPausedPreviousFrame = 0;
                numCameras = 1;
                isPaused = 0;
                cursorPosInMenu[curMenu] = 0;
                curMenu = 0;
                cheated = 0;
                startingPlayerForThisLevel = 0;
                gameState++;
                InitAllDigitSprites();
                loadNewWorld = 1;
                if (curWorld == 0 && !isFinal) {
                    skipNextLoad = 1;
                }
                break;
            case 4:
                cursorPosInMenu[curMenu] = 0;
                curMenu = 0;
                SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
                break;
        }
    }
}

void TwoPlayerMenu(void) {
    int i;

    if (TestButton(PAD_U)) {
        if (cursorPosInMenu[curMenu] <= 0) {
            cursorPosInMenu[curMenu] = 2;
        } else {
            cursorPosInMenu[curMenu]--;
        }
        SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    if (TestButton(PAD_D)) {
        cursorPosInMenu[curMenu] = (cursorPosInMenu[curMenu] + 1) % 3;
        SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    DrawWidgets(5, cursorPosInMenu[curMenu]);
    if (TestButton(PAD_TRIANGLE)) {
        cursorPosInMenu[curMenu] = 0;
        curMenu = 0;
        SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    if (TestButton(PAD_CROSS)) {
        switch (cursorPosInMenu[curMenu]) {
            case 0:
                for (i = 0; i < 2; i++) {
                    copycatPlayerScores[i] = 0;
                }
                wasPausedPreviousFrame = 0;
                gameMode = 1;
                screenOffsetY = displayHeight;
                numCameras = 1;
                copycatModeStartingPlayer = 1;
                isPaused = 0;
                gameState++;
                cursorPosInMenu[curMenu] = 0;
                cursorPosInMenu[0] = 0;
                curMenu = 0;
                InitAllDigitSprites();
                loadNewWorld = 1;
                break;
            case 1:
                twoPlayerLevelSelectionCursorPos = 0;
                curMenu = 6;
                SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
                break;
            case 2:
                curMenu = 0;
                SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
                break;
        }
    }
}

void TimeTrialDifficultySelectionMenu(void) {
    int i;
    if (TestButton(PAD_U)) {
        if (cursorPosInMenu[curMenu] <= 0) {
            cursorPosInMenu[curMenu] = 3;
        } else {
            cursorPosInMenu[curMenu]--;
        }
        SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    if (TestButton(PAD_D)) {
        cursorPosInMenu[curMenu] = (cursorPosInMenu[curMenu] + 1) % 4;
        SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    DrawWidgets(7, cursorPosInMenu[curMenu]);
    if (TestButton(PAD_TRIANGLE)) {
        cursorPosInMenu[curMenu] = 0;
        curMenu = 3;
        SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    if (TestButton(PAD_CROSS)) {
        if (cursorPosInMenu[curMenu] < 3) {
            timeTrialDifficulty = 20 - 10 * cursorPosInMenu[curMenu];
            numTimeTrialPlayers = 1;
            twoPlayerWhichPlayer = 0;
            for (i = 0; i < numTimeTrialPlayers; i++) {
                levelPlayTime[i] = (- TIME_TRIAL_PAR_TIMES[curWorld * 15 + curLevel] -timeTrialDifficulty ) * 50;
                totalPlayTime[i] = 0;
                levelHasBeenCompletedByPlayer[i] = 0;
            }
            screenOffsetY = displayHeight;
            gameMode = 2;
            numCameras = 1;
            numFruits = 0;
            wasPausedPreviousFrame = 0;
            isPaused = 0;
            cursorPosInMenu[curMenu] = 0;
            curMenu = 0;
            cheated = 0;
            startingPlayerForThisLevel = 0;
            gameState++;
            for (i = 0; i < 15; i++) {
                levelPlaytimesInThisWorld[i] = 0;
            }
            InitAllDigitSprites();
            SndPlaySfx(30, 0, &ZERO_SVECTOR_a3340, 7000);
        } else {
            cursorPosInMenu[curMenu] = 0;
            curMenu = 3;
            SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
        }
    }
}

extern char S_FMTd_4[];
void TwoPlayerLevelSelectMenu(void) {
    int i;

    SetBigGuiSpriteVisible();
    if (TestButton(PAD_L)) {
        if (twoPlayerLevelSelectionCursorPos > 0) {
            twoPlayerLevelSelectionCursorPos--;
        } else {
            twoPlayerLevelSelectionCursorPos = highestLevelReached / 15;
        }
        SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    if (TestButton(PAD_R)) {
        twoPlayerLevelSelectionCursorPos = (twoPlayerLevelSelectionCursorPos + 1) % (highestLevelReached / 15 + 1);
        SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    DrawWidgets(8, cursorPosInMenu[curMenu]);
    sprintf(scoreText, S_FMTd_4, twoPlayerLevelSelectionCursorPos * 15 + 1);
    Update2PlayerLevelSelectionSprites(49 + (twoPlayerLevelSelectionCursorPos + 1) * 20 + (twoPlayerLevelSelectionCursorPos / 9) * 8, 187);
    for (i = 1; i < 11; i++) {
        if (highestLevelReached / 15 + 1 < i) {
            SetFancyTextColor(0, 0x80, 0x80);
        }
        sprintf(scoreText, S_FMTd_4, i);
        DrawTextFancyFont(scoreText, 54 + i * 20 + (i / 10) * 12, 178);
    }

    if (TestButton(PAD_TRIANGLE)) {
        cursorPosInMenu[curMenu] = 0;
        curMenu = 4;
        SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
    }
    if (TestButton(PAD_CROSS)) {
        numTimeTrialPlayers = 2;
        twoPlayerWhichPlayer = 0;
        for (i = 0; i < numTimeTrialPlayers; i++) {
            totalPlayTime[i] = 0;
            levelPlayTime[i] = 0;
            levelHasBeenCompletedByPlayer[i] = 0;
        }
        gameMode = 2;
        wasPausedPreviousFrame = 0;
        numCameras = 1;
        isPaused = 0;
        screenOffsetY = displayHeight;
        cursorPosInMenu[curMenu] = 0;
        cursorPosInMenu[0] = 0;
        curMenu = 0;
        gameState++;
        startingPlayerForThisLevel = 0;
        InitAllDigitSprites();
        curWorld = twoPlayerLevelSelectionCursorPos;
        levelPlayTime[twoPlayerWhichPlayer] = -TIME_TRIAL_PAR_TIMES[twoPlayerLevelSelectionCursorPos * 15 + curLevel] * 50;
        if (twoPlayerLevelSelectionCursorPos != curWorld2) {
            loadNewWorld = 1;
        } else {
            SndPlaySfx(30, 0, &ZERO_SVECTOR_a3340, 7000);
        }
    }
}

void SavePointMenu(void) {
    DrawWidgets(9, cursorPosInMenu[curMenu]);
    if (TestButton(PAD_CROSS) && MemCardUi() > 0) {
        savePointMenuConfirmed = 1;
        prevControllerButtons = 0;
        controllerButtons = controllerButtons | PAD_CROSS;
    } else {
        if (TestButton(PAD_TRIANGLE)) {
            SndPlaySfx(109, 37000, &ZERO_SVECTOR_a3340, 8000);
            savePointMenuConfirmed = 1;
        }
    }
}

int GetReadyScreen(void) {
    SetBigGuiSpriteVisible();
    DrawStaticUiSprite(7, 90, 150, 0);
    DrawStaticUiSprite(twoPlayerWhichPlayer + 8, 144, 151, 0);
    DrawStaticUiSprite(6, 162, 150, 0);
    DrawWidgets(19, 0);
    if (TestButton(PAD_CROSS)) {
        SndPlaySfx(0x1e, 0, &ZERO_SVECTOR_a3340, 7000);
        return 0;
    }
    return 1;
}

void UnusedInitFailedToQualifyScreen(void) {
    qualifyScreenCursorX = 0;
    qualifyScreenCursorY = 0;
    timeTrialAtEndOfWorld = 1;
}

extern char S_FMTd_5[];
void TimeTrialFailedToQualifyScreen(void) {
    int i;
    int j;
    char buf[16];
    int time;
    int k;

    curLevel = 0;
    time = 0;
    for (k = 0; k < 15; k++) {
        time += levelPlaytimesInThisWorld[k];
    }
    totalPlayTime[0] = time;

    if (totalPlayTime[0] < 1) {
        if (TestButton(PAD_CROSS)) {
            totalPlayTime[0] = 0;
            time = 0;
            for (k = 0; k < 15; k++) {
                levelPlaytimesInThisWorld[k] = 0;
            }
            loadNewWorld = 1;
            timeTrialAtEndOfWorld = 0;
            levelScoreSummaryConfirmed = 1;
        }
    } else {
        if (TestButton(PAD_U)) {
            if (qualifyScreenCursorY >= 1) {
                qualifyScreenCursorY = qualifyScreenCursorY - 1;
            } else {
                if (qualifyScreenCursorX == 0) {
                    qualifyScreenCursorY = 7;
                } else {
                    qualifyScreenCursorY = 6;
                }
            }
            SndPlaySfx(109, 0, &ZERO_SVECTOR_a3340, 8000);
        }
        if (TestButton(PAD_D)) {
            qualifyScreenCursorY = (qualifyScreenCursorY + 1) % 8;
            if (qualifyScreenCursorX == 1 && qualifyScreenCursorY == 7) {
                qualifyScreenCursorY = 0;
            }
            SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
        }
        if (TestButton(PAD_L) && qualifyScreenCursorY != 7) {
            qualifyScreenCursorX = qualifyScreenCursorX ^ 1;
            SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
        }
        if ((TestButton(PAD_R) != 0) && (qualifyScreenCursorY != 7)) {
            qualifyScreenCursorX = qualifyScreenCursorX ^ 1;
            SndPlaySfx(0x6d, 0, &ZERO_SVECTOR_a3340, 8000);
        }
        if (TestButton(PAD_CROSS)) {
            levelScoreSummaryConfirmed = 1;
            loadNewWorld = 0;
            curLevel = qualifyScreenCursorX * 8 + qualifyScreenCursorY;
        }
        DrawMenuCursorSprite(116 + qualifyScreenCursorX * 150, 85 + qualifyScreenCursorY * 18);
    }

    i = 0;

    for (; i < 2; i++) {
        DrawStaticUiSprite(3, 25 + 150 * i, 67, 0);
        DrawStaticUiSprite(1, 92 + 150 * i, 67, 0);
        for (j = 0; j < 8 && i * j < 7; j++) {
            sprintf(buf, S_FMTd_5, j + i * 8 + curWorld2 * 15 + 1);
            DrawTextFancyFont(buf, 57 + 150 * i, 85 + 18 * j);
            FormatTime(levelPlaytimesInThisWorld[i * 8 + j], buf, 1);
            DrawTextFancyFont(buf, 140 + 150 * i, 85 + 18 * j);
        }
    }

    FormatTime(time, buf, 1);
    DrawStaticUiSprite(0, 170, 210, 7);
    DrawStaticUiSprite(16, 175, 220, 0);
    DrawTextFancyFont(buf, 290, 220);
}
