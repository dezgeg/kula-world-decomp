#include "common.h"

typedef struct DigitSprites {
    TSprite sprites[2][10];
} DigitSprites;

extern void DrawBonusWidget(void);
extern void DrawFruitWidgets(void);
extern void DrawHourglassAndTimer(void);
extern void DrawInt(DigitSprites* sprites, int style, int digits, int max, int value);
extern void DrawKeyWidgets(void);
extern void DrawLethargyEffects(void);
extern void DrawScore(void);
extern void DrawTimeAttackWidgets(void);

int drawBonusWidget;
int drawCopycatWidgets;
int drawFruitWidgets;
int drawHourglassWidget;
int drawKeyWidget;
int drawScoreWidget;
int drawTimeAttackWidgets;
int drawTimerPausedWidget;
int smoothIncrementingScore;

extern DigitSprites copycatPlayer1ScoreDigitSprites;
extern DigitSprites copycatPlayer2ScoreDigitSprites;
extern DigitSprites levelScoreSprite;
extern DigitSprites timeAttackPlayer1CurLevelTimeDigitSprites;
extern DigitSprites timeAttackPlayer1TotalPlaytimeDigitSprites;
extern DigitSprites timeAttackPlayer2CurLevelTimeDigitSprites;
extern DigitSprites timeAttackPlayer2TotalPlaytimeDigitSprites;
extern DigitSprites totalScoreSprite;
extern int copycatPlayerScores[2];
extern int levelHasBeenCompletedByPlayer[2];
extern int levelPlayTime[2];
extern int numTimeTrialPlayers;
extern int totalPlayTime[2];
extern DR_TPAGE hudDrTpages[2];
extern DR_AREA hudDrawAreas[2];
extern DR_OFFSET hudDrawOffsets[2];
extern PrimList primLists[2];
extern TSprite timerPausedSprite[2];
extern int levelScore;
extern int totalScore;
extern int twoPlayerWhichPlayer;
extern int whichDrawDispEnv;

void DrawHud(void) {
    addPrim(&primLists[whichDrawDispEnv].gui3, &hudDrTpages[whichDrawDispEnv]);
    addPrim(&primLists[whichDrawDispEnv].gui3, &hudDrawAreas[whichDrawDispEnv]);
    addPrim(&primLists[whichDrawDispEnv].gui3, &hudDrawOffsets[whichDrawDispEnv]);

    DrawLethargyEffects();
    if (drawHourglassWidget) {
        DrawHourglassAndTimer();
    }
    if (drawKeyWidget) {
        DrawKeyWidgets();
    }
    if (drawFruitWidgets) {
        DrawFruitWidgets();
    }
    if (drawScoreWidget) {
        DrawScore();
    }
    if (drawTimeAttackWidgets) {
        DrawTimeAttackWidgets();
    }
    if (drawTimerPausedWidget) {
        addPrim(&primLists[whichDrawDispEnv].main, &timerPausedSprite[whichDrawDispEnv]);
    }
    if (drawBonusWidget) {
        DrawBonusWidget();
    }
    if (drawCopycatWidgets) {
        DrawInt(&copycatPlayer1ScoreDigitSprites, 1, 2, 100, copycatPlayerScores[0]);
        DrawInt(&copycatPlayer2ScoreDigitSprites, 1, 2, 100, copycatPlayerScores[1]);
    }
}

void DrawScore(void) {
    DrawInt(&totalScoreSprite, 2, 7, 10000000, totalScore);
    if (levelScore < smoothIncrementingScore) {
        smoothIncrementingScore = levelScore;
    }
    if (smoothIncrementingScore != levelScore) {
        smoothIncrementingScore = smoothIncrementingScore + ((levelScore - smoothIncrementingScore) / 5) + 1;
    }
    DrawInt(&levelScoreSprite, 1, 7, 10000000, smoothIncrementingScore);
}

void DrawTimeAttackWidgets(void) {
    int curPlayerTimer;

    curPlayerTimer = levelPlayTime[twoPlayerWhichPlayer] / 50;
    if (levelPlayTime[twoPlayerWhichPlayer] < 0) {
        curPlayerTimer--;
    }
    DrawTimeAttackTimer(&timeAttackPlayer1TotalPlaytimeDigitSprites,2,totalPlayTime[0],0);
    if (numTimeTrialPlayers == 2) {
        DrawTimeAttackTimer(&timeAttackPlayer2TotalPlaytimeDigitSprites,2,totalPlayTime[1],0);
        if (twoPlayerWhichPlayer == 0) {
            DrawTimeAttackTimer(&timeAttackPlayer1CurLevelTimeDigitSprites,1,curPlayerTimer,0);
            curPlayerTimer = levelPlayTime[1] / 50;
            if (levelPlayTime[1] < 0) {
                curPlayerTimer--;
            }
            if (levelHasBeenCompletedByPlayer[1] == 1) {
                DrawTimeAttackTimer(&timeAttackPlayer2CurLevelTimeDigitSprites,1,curPlayerTimer,0);
            } else {
                DrawTimeAttackTimer(&timeAttackPlayer2CurLevelTimeDigitSprites,1,0,1);
            }
        } else {
            DrawTimeAttackTimer(&timeAttackPlayer2CurLevelTimeDigitSprites,1,curPlayerTimer,0);
            curPlayerTimer = levelPlayTime[0] / 50;
            if (levelPlayTime[0] < 0) {
                curPlayerTimer--;
            }
            if (levelHasBeenCompletedByPlayer[0] != 1) {
                DrawTimeAttackTimer(&timeAttackPlayer1CurLevelTimeDigitSprites,1,0,1);
            } else {
                DrawTimeAttackTimer(&timeAttackPlayer1CurLevelTimeDigitSprites,1,curPlayerTimer,0);
            }
        }
    }
    else {
        DrawTimeAttackTimer(&timeAttackPlayer1CurLevelTimeDigitSprites,1,curPlayerTimer,0);
    }
}
