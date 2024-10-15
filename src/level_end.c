#include "common.h"

#include <string.h>

// gprel-used variables (defined in this file)
int DAT_000a3374;
int levelScoreSummaryConfirmed;
int levelScoreSummaryScoreTicker;
int menuUnkUnused;
int newScore;
int oldScore;
short qualifyScreenCursorX;
short qualifyScreenCursorY;
int timeTrialAtEndOfWorld;

// non-gprel-used variables (extern)
extern int curLevel;
extern int curWorld2;
extern int gameMode;
extern int isFinal;
extern int latestPlayerToFinish;
extern int levelEndReason;
extern int levelPlayTime[2];
extern int levelScore;
extern int numTimeTrialPlayers;
extern uint prevControllerButtons;
extern int specialLevelType;
extern int totalPlayTime[2];
extern int totalScore;

extern char S_[];
extern char S__2[];
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

INCLUDE_ASM("asm/nonmatchings/level_end", DrawLevelScoreSummary);
