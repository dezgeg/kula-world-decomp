#include "common.h"

#include <string.h>

// Prototypes
extern void DrawHighScoreCubes(void);
extern void DrawPsxButtonBackground(void);
extern void DrawScoreGraph(void);
extern void DrawTextCrappyFont(char *str);
extern void DrawWidgets(int menuId, int cursorPos);
extern void FormatTime(int time, char *s, int showPlus);
extern int GetControllerButtons(int slot);
extern int GetControllerStatus(int slot);
extern void InitHighscoreCubes(void);
extern void InitPsxButtonBackgroundSprites(int);
extern void InitScoreGraph(void *pBuf, int *levelScores, int *unusedScorePtr, int numLevels, int maxScore, int x, int y, int graphWidth, int graphHeight, int isHighscore);
extern void MusicCheckForLoop(void);
extern void PutDrawAndDispEnvs(void);
extern void ResetTextRenderState(void);
extern void ResetTextVars(void);
extern void SetTextParams(int posX, int posY, int align, int colorR, int colorG, int colorB);
extern void SndMuteAllVoices(void);
extern void SndPlaySfx(int sfx, int tag, SVECTOR *dir, int volume);
extern void SndProcessSpuVoices(void);
extern uint Rand(int param_1);

// gprel-used variables (defined in this file)
short highscoreEntryCursorX;
short highscoreEntryCursorY;
int idleTimer;
unsigned char rainbowColorB;
unsigned char rainbowColorDeltaB;
unsigned char rainbowColorDeltaG;
unsigned char rainbowColorDeltaR;
unsigned char rainbowColorG;
unsigned char rainbowColorR;
int repeatRateTimer;
Highscore highscores[6];

// non-gprel-used variables (extern)
extern SVECTOR ZERO_SVECTOR_a3340;
extern int curController;
extern int curLevel;
extern int curWorld;
extern int curWorld2;
extern int displayWidth;
extern int levelScores[150];
extern int musicShouldLoop;
extern int numCameras;
extern short numFruits;
extern uint controllerButtons;
extern uint prevControllerButtons;
extern PrimList primLists[2];
extern int totalPlayTime[2];
extern int whichDrawDispEnv;
extern uint otag[2][1][1026];
extern int highscoreLevelScores[6][150];

extern char S_ABCDEFGHIJKLMNOPQRSTUVWXYZ[3][10];
extern char S__4[];
extern char S__5[];
extern char S_FMTs_3[];
extern char S_SCORE[];
extern char S_TIMETRIAL[];
extern char S_FMTd_4[];
extern char S_LEVEL[];
extern char S_FRUITS[];
extern char S_TIME[];
extern char S_CONGRATULATIONS_PLEASE_WRITE_YOUR_SIGNATURE[];
extern char S_FMTc[];

static inline int TestButton(int button) {
    return controllerButtons & (button & ~prevControllerButtons);
}

void InitHighscores(void) {
    extern char S_LEIF[];
    extern char S_SAVE_1[];
    extern char S_SAVE_2[];
    extern char S_SAVE_3[];
    extern char S_SAVE_4[];
    extern char S_PLAYER[];
    int levelScore;
    int i;
    int j;

    highscores[0].score = 50000;
    highscores[0].fruits = 25;
    highscores[0].time = 2110;
    highscores[0].level = 50;
    levelScore = 0;
    for (j = 0; j < 150; j++) {
        levelScore += Rand(1000) - (j * 500) / 30;
        if (levelScore > 50000) {
            levelScore = 50000;
        }
        if (levelScore < 0) {
            break;
        }
        highscoreLevelScores[0][j] = levelScore;
    }

    for (i = 1; i < 6; i++) {
        highscores[i].score = 0;
        highscores[i].fruits = 0;
        highscores[i].time = 0;
        highscores[i].level = 0;
        for (j = 0; j < 150; j++) {
            highscoreLevelScores[i][j] = 0;
        }
    }

    sprintf(highscores[0].name, S_LEIF);
    sprintf(highscores[1].name, S_SAVE_1);
    sprintf(highscores[2].name, S_SAVE_2);
    sprintf(highscores[3].name, S_SAVE_3);
    sprintf(highscores[4].name, S_SAVE_4);
    sprintf(highscores[5].name, S_PLAYER);
}

void HighScoreUi(int param_1) {
    int maxScore;
    int placement;
    int cursorIndex;
    int highscoreEntryCharIndex;
    int local_50;
    int i, j;
    int v, c, waveX;
    char buf[64];

    placement = 5;
    highscoreEntryCharIndex = 0;
    maxScore = 0;
    local_50 = 4296;
    cursorIndex = 5;

    SndMuteAllVoices();
    DrawWidgets(11, 0);
    InitPsxButtonBackgroundSprites(1);
    InitHighscoreCubes();
    VSync(0);
    DrawSync(0);

    for (i = 0; i < 1; i++) {
        ClearOTagR(otag[0][i], 1026);
        ClearOTagR(otag[1][i], 1026);
    }
    ClearOTagR(&primLists[whichDrawDispEnv].main, 4);
    DrawSync(0);

    if (param_1 == 1) {
        for (i = 0; i < 150; i++) {
            if (levelScores[i] > maxScore) {
                maxScore = levelScores[i];
            }
        }
        highscores[5].fruits = numFruits;
        highscores[5].time = totalPlayTime[0];
        highscores[5].level = curWorld2 * 15 + curLevel + 1;
        highscores[5].score = maxScore;
        if (curWorld2 == 9 && curWorld == 10) {
            highscores[5].level = 151;
        }
        for (i = 0; i < 150; i++) {
            highscoreLevelScores[5][i] = levelScores[i];
        }

        if (maxScore > highscores[0].score) {
            placement = 0;
            highscores[0].fruits = numFruits;
            highscores[0].time = totalPlayTime[0];
            highscores[0].score = maxScore;
            highscores[0].level = curLevel + curWorld2 * 15  + 1;
            if (curWorld2 == 9 && curWorld == 10) {
                highscores[0].level = 151;
            }
            for (i = 0; i < 8; i++) {
                highscores[0].name[i] = ' ';
            }
            for (i = 0; i < 150; i++) {
                highscoreLevelScores[0][i] = levelScores[i];
            }
        }
    }

    for (i = 0, maxScore = 0; i < 6; i++) {
        for (j = 0; j < 150; j++) {
            if (highscoreLevelScores[i][j] > maxScore) {
                maxScore = highscoreLevelScores[i][j];
            }
            if (highscoreLevelScores[i][j] > highscores[i].score) {
                highscores[i].score = highscoreLevelScores[i][j];
            }
        }
    }

    InitScoreGraph((void*)0x179000, highscoreLevelScores[cursorIndex], (int *)-1, 150, maxScore, 16, 140, 280, 100, 1);
    highscoreEntryCursorX = 0;
    highscoreEntryCursorY = 0;

    while (1) {
        DrawSync(0);
        VSync(0);
        whichDrawDispEnv = !whichDrawDispEnv;
        PutDrawAndDispEnvs();

        for (i = 0; i < numCameras; i++) {
            ClearOTagR(&otag[whichDrawDispEnv][i][0], 1026);
        }
        ClearOTagR(&primLists[whichDrawDispEnv].main, 4);

        ResetTextRenderState();
        ResetTextVars();

        for (i = 0; i < numCameras; i++) {
            DrawOTag(&otag[!whichDrawDispEnv][i][1024] - 1);
        }
        DrawOTag(&primLists[!whichDrawDispEnv].gui3);
        SndProcessSpuVoices();

        prevControllerButtons = controllerButtons;
        if (GetControllerStatus(curController)) {
            controllerButtons = GetControllerButtons(curController);
        } else {
            controllerButtons = GetControllerButtons((curController + 1) % 2);
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

        if (musicShouldLoop == 1) {
            MusicCheckForLoop();
        }

        if (placement == 0) {
            if (TestButton(PAD_U)) {
                if (highscoreEntryCursorY > 0) {
                    highscoreEntryCursorY--;
                } else {
                    highscoreEntryCursorY = 2;
                }
                SndPlaySfx(SFX_MENU_SELECTION_2, 0, &ZERO_SVECTOR_a3340, 8000);
            }

            if (TestButton(PAD_D)) {
                highscoreEntryCursorY = (highscoreEntryCursorY + 1) % 3;
                SndPlaySfx(SFX_MENU_SELECTION_2, 0, &ZERO_SVECTOR_a3340, 8000);
            }

            if (TestButton(PAD_L)) {
                if (highscoreEntryCursorX > 0) {
                    highscoreEntryCursorX--;
                } else {
                    highscoreEntryCursorX = 9;
                }
                SndPlaySfx(SFX_MENU_SELECTION_2, 0, &ZERO_SVECTOR_a3340, 8000);
            }

            if (TestButton(PAD_R)) {
                highscoreEntryCursorX = (highscoreEntryCursorX + 1) % 10;
                SndPlaySfx(SFX_MENU_SELECTION_2, 0, &ZERO_SVECTOR_a3340, 8000);
            }

            if (TestButton(PAD_CROSS) || TestButton(PAD_TRIANGLE) || TestButton(PAD_SQUARE) || TestButton(PAD_CIRCLE)) {
                if (highscoreEntryCursorY == 2) {
                    if (highscoreEntryCursorX == 9) {
                        placement = 6;
                    }
                    if (highscoreEntryCursorX == 8) {
                        if (highscoreEntryCharIndex > 0) {
                            highscoreEntryCharIndex--;
                        }
                        SndPlaySfx(SFX_MENU_SELECTION_2, 0, &ZERO_SVECTOR_a3340, 8000);
                        goto LABEL_SKIP_ENTRY;
                    }
                }
                SndPlaySfx(SFX_MENU_SELECTION_2, 0, &ZERO_SVECTOR_a3340, 8000);
                if (highscoreEntryCharIndex < 6) {
                    highscoreEntryCharIndex++;
                } else {
                    highscores[placement].name[5] = S_ABCDEFGHIJKLMNOPQRSTUVWXYZ[highscoreEntryCursorY][highscoreEntryCursorX];
                }
                if (highscoreEntryCharIndex > 5) {
                    highscoreEntryCursorY = 2;
                    highscoreEntryCursorX = 9;
                }
            }
LABEL_SKIP_ENTRY:
            if (!(highscoreEntryCursorY == 2 && highscoreEntryCursorX >= 8 && highscoreEntryCursorX <= 9)) {
                if (highscoreEntryCharIndex < 6) {
                    highscores[placement].name[highscoreEntryCharIndex] = S_ABCDEFGHIJKLMNOPQRSTUVWXYZ[highscoreEntryCursorY][highscoreEntryCursorX];
                }
            } else {
                if (highscoreEntryCharIndex < 6) {
                    highscores[placement].name[highscoreEntryCharIndex] = ' ';
                }
            }
            highscores[placement].name[highscoreEntryCharIndex + 1] = '\0';
        } else {
            if (TestButton(PAD_D)) {
                cursorIndex = (cursorIndex + 1) % 6;
                DrawSync(0);
                InitScoreGraph((void*)0x00179000, highscoreLevelScores[cursorIndex], (int *)0xffffffff, 150, maxScore, 16, 140, 280, 100, 1);
                SndPlaySfx(SFX_MENU_SELECTION_2, 0, &ZERO_SVECTOR_a3340, 8000);
            }

            if (TestButton(PAD_U)) {
                cursorIndex--;
                if (cursorIndex < 0) {
                    cursorIndex = 5;
                }
                DrawSync(0);
                InitScoreGraph((void*)0x00179000, highscoreLevelScores[cursorIndex], (int *)0xffffffff, 150, maxScore, 16, 140, 280, 100, 1);
                SndPlaySfx(SFX_MENU_SELECTION_2, 0, &ZERO_SVECTOR_a3340, 8000);
            }

            SetTextParams(26, 45, 0, -1, -1, -1);
            DrawTextCrappyFont(S__4);
            for (i = 0; i < 6; i++) {
                waveX = 0;
                if (local_50 + 50 * i >= 3072) {
                    waveX = (rsin(local_50 + 50 * i) * 250 >> 12) + 250;
                }
                SetTextParams(waveX + 11, -1, -1, -1, -1, -1);
                DrawTextCrappyFont(i == cursorIndex ? S__5 : S__4);
            }
            if (TestButton(PAD_CROSS) || TestButton(PAD_TRIANGLE) || TestButton(PAD_SQUARE) || TestButton(PAD_CIRCLE) || TestButton(PAD_START)) {
                SndPlaySfx(SFX_MENU_SELECTION_2, 37000, &ZERO_SVECTOR_a3340, 8000);
                DrawSync(0);
                VSync(0);
                return;
            }
        }

        rainbowColorR += rainbowColorDeltaR;
        if (rainbowColorR & 0x80) {
            rainbowColorDeltaR = -rainbowColorDeltaR;
            rainbowColorR += rainbowColorDeltaR;
        }
        rainbowColorG += rainbowColorDeltaG;
        if (rainbowColorG & 0x80) {
            rainbowColorDeltaG = -rainbowColorDeltaG;
            rainbowColorG += rainbowColorDeltaG;
        }
        rainbowColorB += rainbowColorDeltaB;
        if (rainbowColorB & 0x80) {
            rainbowColorDeltaB = -rainbowColorDeltaB;
            rainbowColorB += rainbowColorDeltaB;
        }
        rainbowColorR &= 0x7f;
        rainbowColorG &= 0x7f;
        rainbowColorB &= 0x7f;

        v = 54;
        SetTextParams(54, 45, 1, 128, 128, 128);
        DrawTextCrappyFont(S__4);
        for (i = 0; i < 6; i++) {
            c = i == cursorIndex ? 32 : 0;
            sprintf(buf, S_FMTs_3, highscores[i].name);
            if (local_50 + 50 * i >= 3072) {
                waveX = (rsin(local_50 + 50 * i) * 250 >> 12) + 250;
            } else {
                waveX = 0;
            }
            if (i == 0) {
                SetTextParams(v + waveX, -1, -1, rainbowColorR + 20, rainbowColorG + 20, rainbowColorB + 20);
            } else {
                SetTextParams(v + waveX, -1, -1, 96 + c, 96 + c, 96 + c);
            }
            DrawTextCrappyFont(buf);
        }

        SetTextParams(152, 45, 2, 128, 128, 128);
        DrawTextCrappyFont(S_SCORE);
        v = 156;
        for (i = 0; i < 6; i++) {
            c = i == cursorIndex ? 32 : 0;
            if (local_50 + 50 * i >= 3072) {
                waveX = (rsin(local_50 + 50 * i) * 250 >> 12) + 250;
            } else {
                waveX = 0;
            }
            if (highscores[i].score >= 0) {
                sprintf(buf, S_FMTd_4, highscores[i].score);
            } else {
                sprintf(buf, S_TIMETRIAL);
                waveX += 16;
            }
            if (i == 0) {
                SetTextParams(v + waveX, -1, -1, rainbowColorR + 20, rainbowColorG + 20, rainbowColorB + 20);
            } else {
                SetTextParams(v + waveX, -1, -1, 96 + c, 96 + c, 96 + c);
            }
            DrawTextCrappyFont(buf);
        }

        SetTextParams(208, 45, 2, 128, 128, 128);
        DrawTextCrappyFont(S_LEVEL);
        v = 198;
        for (i = 0; i < 6; i++) {
            c = i == cursorIndex ? 32 : 0;
            sprintf(buf, S_FMTd_4, (int)highscores[i].level);
            if (local_50 + 50 * i >= 3072) {
                waveX = (rsin(local_50 + 50 * i) * 250 >> 12) + 250;
            } else {
                waveX = 0;
            }
            if (i == 0) {
                SetTextParams(v + waveX, -1, -1, rainbowColorR + 20, rainbowColorG + 20, rainbowColorB + 20);
            } else {
                SetTextParams(v + waveX, -1, -1, 96 + c, 96 + c, 96 + c);
            }
            DrawTextCrappyFont(buf);
        }

        SetTextParams(268, 45, 2, 128, 128, 128);
        DrawTextCrappyFont(S_FRUITS);
        v = 255;
        for (i = 0; i < 6; i++) {
            c = i == cursorIndex ? 32 : 0;
            sprintf(buf, S_FMTd_4, (int)highscores[i].fruits);
            if (local_50 + 50 * i >= 3072) {
                waveX = (rsin(local_50 + 50 * i) * 250 >> 12) + 250;
            } else {
                waveX = 0;
            }
            if (i == 0) {
                SetTextParams(v + waveX, -1, -1, rainbowColorR + 20, rainbowColorG + 20, rainbowColorB + 20);
            } else {
                SetTextParams(v + waveX, -1, -1, 96 + c, 96 + c, 96 + c);
            }
            DrawTextCrappyFont(buf);
        }

        SetTextParams(312, 45, 2, 128, 128, 128);
        DrawTextCrappyFont(S_TIME);
        v = 316;
        for (i = 0; i < 6; i++) {
            c = i == cursorIndex ? 32 : 0;
            FormatTime(highscores[i].time, buf, 0);
            if (local_50 + 50 * i >= 3072) {
                waveX = (rsin(local_50 + 50 * i) * 250 >> 12) + 250;
            } else {
                waveX = 0;
            }
            if (i == 0) {
                SetTextParams(v + waveX, -1, -1, rainbowColorR + 20, rainbowColorG + 20, rainbowColorB + 20);
            } else {
                SetTextParams(v + waveX, -1, -1, 96 + c, 96 + c, 96 + c);
            }
            DrawTextCrappyFont(buf);
        }

        local_50 -= 20;
        if (local_50 < 2048) {
            local_50 = 2048;
        }

        if (placement > 0) {
            DrawScoreGraph();
        } else {
            SetTextParams(displayWidth / 2, 129, 1, 128, 128, 128);
            DrawTextCrappyFont(S_CONGRATULATIONS_PLEASE_WRITE_YOUR_SIGNATURE);
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 10; j++) {
                    if (j == highscoreEntryCursorX && i == highscoreEntryCursorY) {
                        SetTextParams(j * 14 + 90, i * 14 + 178, 1, 128, 128, 128);
                    } else {
                        SetTextParams(j * 14 + 90, i * 14 + 178, 1, 64, 64, 64);
                    }
                    sprintf(buf, S_FMTc, S_ABCDEFGHIJKLMNOPQRSTUVWXYZ[i][j]);
                    DrawTextCrappyFont(buf);
                }
            }
        }

        if (prevControllerButtons == controllerButtons) {
            idleTimer++;
        } else {
            idleTimer = 0;
        }

        if (idleTimer > 30000) {
            break;
        }

        DrawPsxButtonBackground();
        DrawHighScoreCubes();
        DrawWidgets(11, 0);
    }
    idleTimer = 0;
    DrawSync(0);
    VSync(0);
}
