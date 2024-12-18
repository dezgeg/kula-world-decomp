#include "common.h"

#include <string.h>

extern void DrawStaticUiSprite(short id, short x, short y, short count);
extern void DrawTextFancyFont(char* str, short x, short y);
extern void FormatTime(int time, char* s, int showPlus);
extern void FullyFinishLevelEndScreenFades(void);
extern int GetControllerButtons(int slot);
extern int GetControllerStatus(int slot);
extern void TimeTrialFailedToQualifyScreen(void);

extern char scoreText[128];
extern int byteCountToReceiveFromSio;
extern int controllerButtons;
extern int copycatPlayerScores[2];
extern int curController;
extern int curLevel;
extern int curWorld;
extern int curWorld2;
extern int debugBonusLevels;
extern int debugDisableTimer;
extern int gameMode;
extern int gameState;
extern int gotSioData;
extern int isDemoMode;
extern int isFinal;
extern int latestPlayerToFinish;
extern int levelEndReason;
extern int levelHasBeenCompletedByPlayer[2];
extern int levelPlayTime[2];
extern int levelScore;
extern int loadNewWorld;
extern int numTimeTrialPlayers;
extern int specialLevelType;
extern int totalPlayTime[2];
extern int totalScore;
extern int twoPlayerWhichPlayer;
extern uint prevControllerButtons;

int copycatModeStartingPlayer;
int curMenu;
int cursorPosInMenu[8];
int DAT_000a3374;
int idleTimer;
int isPaused;
int levelScoreSummaryConfirmed;
int levelScoreSummaryScoreTicker;
int menuUnkUnused;
int newScore;
int oldScore;
int timeTrialAtEndOfWorld;
int wasPausedPreviousFrame;
short qualifyScreenCursorX;
short qualifyScreenCursorY;

static inline int TestButton(int button) {
    return (controllerButtons & (~prevControllerButtons & button)) != 0;
}

extern char S_[];
extern char S_1[];
extern char S__2[];
extern char S_2[];
extern char S__3[];
extern char S_FMTd_3[];
extern char S_FMTd_4[];
extern char S_FMTd_6[];
extern char S_FMTd_FMTd[];
extern char S_FMTd_FMTdFMTd[];
extern char S_FMTd_FMTdFMTd_FMTdFMTd[];

void UpdateScoreAtEndOfLevel(void) {
    int bonusScore;

    if (gameMode == 0) {
        if (levelEndReason > 0 || specialLevelType == 1) {
            oldScore = totalScore - levelScore;
        } else {
            if (!isFinal) {
                bonusScore = (curWorld2 * 15 + curLevel + 1) * 50;
            } else {
                bonusScore = (curWorld2 * 2 + curLevel + 1) * 50;
            }
            if (bonusScore > 5000) {
                bonusScore = 5000;
            }
            oldScore = totalScore + levelScore + bonusScore;
        }
        menuUnkUnused = 0;
        prevControllerButtons = 0xffffffff;
        levelScoreSummaryScoreTicker = levelScore;
        newScore = oldScore;
    }
    if (gameMode == 1) {
        prevControllerButtons = 0xffffffff;
    }
    if (gameMode == 2) {
        DAT_000a3374 = 0;
        levelScoreSummaryScoreTicker = levelPlayTime[latestPlayerToFinish] / 50;
        if (levelPlayTime[latestPlayerToFinish] < 0) {
            levelScoreSummaryScoreTicker--;
        }
        if (levelEndReason < 0 && numTimeTrialPlayers == 2) {
            levelScoreSummaryScoreTicker = 4;
        }
        newScore = totalPlayTime[latestPlayerToFinish] - levelScoreSummaryScoreTicker;
        oldScore = newScore;

        menuUnkUnused = 0;
        if (levelEndReason == -10 && numTimeTrialPlayers == 1) {
            levelScoreSummaryConfirmed = 1;
        }
        if (curLevel == 0 && levelEndReason > 0 && numTimeTrialPlayers == 1) {
            qualifyScreenCursorX = 0;
            qualifyScreenCursorY = 0;
            timeTrialAtEndOfWorld = 1;
        }
    }
}

void FormatTime(int time, char* s, int showPlus) {
    if (time < 0) {
        s += sprintf(s, S_);
    }
    if (time > 0 && showPlus > 0) {
        s += sprintf(s, S__2);
    }

    if (abs(time) > 3600) {
        sprintf(s, S_FMTd_FMTdFMTd_FMTdFMTd, abs(time) / 3600,
                abs(time) / 600 + (abs(time) / 3600) * -6, abs(time) / 60 + (abs(time) / 600) * -10,
                abs(time) / 10 + (abs(time) / 60) * -6, abs(time) % 10);
    } else {
        sprintf(s, S_FMTd_FMTdFMTd, abs(time) / 60, abs(time) / 10 + (abs(time) / 60) * -6,
                abs(time) % 10);
    }
}

void DrawLevelScoreSummary(void) {
    int x;
    int penalty;
    ushort y1;
    ushort y2off;
    int iVar2;
    int i;
    char buf[16];
    int controller;
    int timeSecs;

    if (levelScoreSummaryConfirmed == 1) {
        return;
    }

    if (gameMode == 0) {
        levelScoreSummaryScoreTicker -= 100;
        if (levelScoreSummaryScoreTicker > 0) {
            newScore = newScore + 100;
        } else {
            levelScoreSummaryScoreTicker = 0;
            newScore = totalScore;
            DrawStaticUiSprite(6, 124, 225, 0);
        }
        if (levelEndReason > 0 || specialLevelType == 1) {
            y2off = 18;
            sprintf(scoreText, S_FMTd_FMTd, oldScore, levelScore - levelScoreSummaryScoreTicker);
        } else {
            y2off = 0;
            if (isFinal == 0) {
                penalty = (curWorld2 * 15 + curLevel + 1) * 50;
            } else {
                penalty = (curWorld2 * 2 + curLevel + 1) * 50;
            }
            if (penalty > 5000) {
                penalty = 5000;
            }
            sprintf(scoreText, S_FMTd_6, penalty);
            DrawStaticUiSprite(13, 64, 176, 0);
            DrawTextFancyFont(scoreText, 266, 176);
            sprintf(scoreText, S_FMTd_FMTd, oldScore, levelScoreSummaryScoreTicker - levelScore);
        }
        y1 = y2off | 0x8c;
        DrawTextFancyFont(scoreText, 266, y1);
        DrawStaticUiSprite(4, 64, y1, 0);
        DrawStaticUiSprite(5, 96, y1, 0);
        DrawStaticUiSprite(3, 64, y2off + 158, 0);
        DrawStaticUiSprite(5, 107, y2off + 158, 0);
        sprintf(scoreText, S_FMTd_3, newScore);
        DrawStaticUiSprite(0, 54, 193, 12);
        DrawTextFancyFont(scoreText, 266, 203);
        DrawStaticUiSprite(2, 64, 203, 0);
        DrawStaticUiSprite(5, 102, 203, 0);
        if (controllerButtons & PAD_CROSS) {
            FullyFinishLevelEndScreenFades();
            if (levelScoreSummaryScoreTicker > 0) {
                levelScoreSummaryScoreTicker = 0;
                prevControllerButtons = 0xffffffff;
                newScore = totalScore;
            } else {
                if (TestButton(PAD_CROSS)) {
                    levelScoreSummaryConfirmed = 1;
                }
            }
        }
    }

    if (gameMode == 1) {
        FullyFinishLevelEndScreenFades();
        i = curController;
        if (levelEndReason < 0) {
            i = (copycatModeStartingPlayer + 1) % 2;
            if (GetControllerStatus(i) != 0) {
                controllerButtons = GetControllerButtons((copycatModeStartingPlayer + 1) % 2);
            }
        }
        DrawStaticUiSprite(7, 90, 176, 0);
        DrawStaticUiSprite(i + 8, 144, 177, 0);
        DrawStaticUiSprite(6, 162, 176, 0);
        if (TestButton(PAD_CROSS)) {
            levelScoreSummaryConfirmed = 1;
            if (curLevel == 0 && (copycatPlayerScores[0] > 5 || copycatPlayerScores[1] > 5)) {
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
        }
    }
    if (gameMode == 2) {
        if ((curLevel == 0 || timeTrialAtEndOfWorld == 1) && numTimeTrialPlayers == 1 && levelEndReason > 0 && DAT_000a3374 == 1) {
            TimeTrialFailedToQualifyScreen();
            return;
        }
        if (levelScoreSummaryScoreTicker > 0) {
            newScore++;
            levelScoreSummaryScoreTicker--;
        }
        if (levelScoreSummaryScoreTicker < 0) {
            newScore--;
            levelScoreSummaryScoreTicker++;
        }
        timeSecs = levelPlayTime[latestPlayerToFinish] / 50 - levelScoreSummaryScoreTicker;
        if (levelPlayTime[latestPlayerToFinish] < 0) {
            timeSecs--;
        }
        if (numTimeTrialPlayers != 1 || levelEndReason <= 0) {
            if (numTimeTrialPlayers == 2) {
                if (levelEndReason < 0) {
                    timeSecs = 4 - levelScoreSummaryScoreTicker;
                    goto LAB_0004b708;
                }
                goto LAB_0004b810;
            }
        } else {
    LAB_0004b708:
            FormatTime(oldScore,buf,1);
            DrawTextFancyFont(buf,254,158);
            FormatTime(timeSecs,buf,1);
            DrawStaticUiSprite(4,64,158,0);
            DrawStaticUiSprite(1,96,158,0);
            DrawTextFancyFont(buf,254,176);
            if (levelEndReason > 0) {
                DrawStaticUiSprite(3, 64, 176, 0);
                DrawStaticUiSprite(1, 107, 176, 0);
            } else {
                DrawStaticUiSprite(13, 64, 176, 0);
            }
            DrawStaticUiSprite(0,54,193,12);
            DrawStaticUiSprite(2,64,203,0);
            DrawStaticUiSprite(1,102,203,0);
            FormatTime(newScore,buf,1);
            DrawTextFancyFont(buf,254,203);
    LAB_0004b810:
            if (numTimeTrialPlayers == 2 && levelEndReason > 0) {
                 if (curLevel != 14 || levelHasBeenCompletedByPlayer[0] != 1 || levelHasBeenCompletedByPlayer[1] != 1) {
                    for (i = 0; i < 2; i++) {
                        if (latestPlayerToFinish == i || levelHasBeenCompletedByPlayer[i] == 1) {
                            if (i == latestPlayerToFinish) {
                                FormatTime(newScore - timeSecs, buf, 1);
                                DrawTextFancyFont(buf, 0xc1 + 0x53 * i, 158);
                                FormatTime(timeSecs, buf, 1);
                                DrawTextFancyFont(buf, 0xc1 + 0x53 * i, 176);
                                FormatTime(newScore, buf, 1);
                                DrawTextFancyFont(buf, 0xc1 + 0x53 * i, 0xcb);
                            } else {
                                iVar2 = levelPlayTime[i] / 50;
                                if (levelPlayTime[i] < 0) {
                                    iVar2 = iVar2 + -1;
                                }
                                FormatTime(totalPlayTime[i] - iVar2, buf, 1);
                                DrawTextFancyFont(buf, 0xc1 + 0x53 * i, 0x9e);
                                FormatTime(iVar2, buf, 1);
                                DrawTextFancyFont(buf, 0xc1 + 0x53 * i, 0xb0);
                                FormatTime(totalPlayTime[i], buf, 1);
                                DrawTextFancyFont(buf, 0xc1 + 0x53 * i, 0xcb);
                            }
                        } else {
                            FormatTime(totalPlayTime[i], buf, 1);
                            DrawTextFancyFont(buf, 0xc1 + 0x53 * i, 158);
                            DrawTextFancyFont(S__3, 0xc1 + 0x53 * i, 176);
                            DrawTextFancyFont(S__3, 0xc1 + 0x53 * i, 0xcb);
                        }
                    }
                    DrawStaticUiSprite(14, 134, 140, 0);
                    DrawTextFancyFont(S_1, 201, 140);
                    DrawStaticUiSprite(14, 217, 140, 0);
                    DrawTextFancyFont(S_2, 284, 140);
                    DrawStaticUiSprite(4, 42, 158, 0);
                    DrawStaticUiSprite(1, 72, 158, 0);
                    DrawStaticUiSprite(3, 42, 176, 0);
                    DrawStaticUiSprite(1, 85, 176, 0);
                    DrawStaticUiSprite(0, 30, 193, 15);
                    DrawStaticUiSprite(2, 42, 203, 0);
                    DrawStaticUiSprite(1, 80, 203, 0);
                } else {
                    for (i = 0; i < 2; i++) {
                        DrawStaticUiSprite(0xe, 0x49 + i * 0x67, 0xa0, 0);
                        sprintf(scoreText, S_FMTd_4, i);
                        if (i == 0) {
                            DrawTextFancyFont(S_1, 0x8c, 0xa0);
                        } else {
                            DrawTextFancyFont(S_2, 0x8c + i * 0x67, 0xa0);
                        }
                        FormatTime(totalPlayTime[i], buf, 1);
                        DrawTextFancyFont(buf, 0x84 + 0x67 * i, 0xba);
                    }
                }
            }
        }
        if (levelScoreSummaryScoreTicker == 0) {
            if (numTimeTrialPlayers == 1 || (numTimeTrialPlayers == 2 && curLevel == 14 && (levelHasBeenCompletedByPlayer[0] == 1 || levelHasBeenCompletedByPlayer[1] == 1))) {
                if (levelEndReason > 0) {
                    DrawStaticUiSprite(6, 124, 225, 0);
                } else {
                    DrawStaticUiSprite(6, 124, 176, 0);
                }
            } else {
                if (GetControllerStatus(latestPlayerToFinish) != 0) {
                    controllerButtons = GetControllerButtons(latestPlayerToFinish);
                }
                DrawStaticUiSprite(6, 124, 225, 0);
            }
        }
        if ((controllerButtons & PAD_CROSS) != 0) {
            FullyFinishLevelEndScreenFades();
            if (levelScoreSummaryScoreTicker != 0) {
                newScore += levelScoreSummaryScoreTicker;
                menuUnkUnused = 1;
                levelScoreSummaryScoreTicker = 0;
                prevControllerButtons = 0xffffffff;
            } else {
                if (TestButton(PAD_CROSS)) {
                    if (numTimeTrialPlayers == 2 && curLevel == 14 && levelHasBeenCompletedByPlayer[0] == 1 && levelHasBeenCompletedByPlayer[1] == 1) {
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
                    if ((curLevel == 0 || timeTrialAtEndOfWorld == 1) && numTimeTrialPlayers == 1 && levelEndReason > 0) {
                        DAT_000a3374 = 1;
                    } else {
                        levelScoreSummaryConfirmed = 1;
                    }
                }
            }
        }
    }

    if (prevControllerButtons == controllerButtons) {
        idleTimer++;
    } else {
        idleTimer = 0;
    }
    if (idleTimer > 500 && isDemoMode == 1) {
        idleTimer = 0;
        levelScoreSummaryConfirmed = 1;
    }
}
