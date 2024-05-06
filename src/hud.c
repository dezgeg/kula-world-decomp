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

extern DigitSprites copycatPlayer1ScoreDigitSprites;
extern DigitSprites copycatPlayer2ScoreDigitSprites;
extern int copycatPlayerScores[2];
extern DR_TPAGE hudDrTpages[2];
extern DR_AREA hudDrawAreas[2];
extern DR_OFFSET hudDrawOffsets[2];
extern PrimList primLists[2];
extern TSprite timerPausedSprite[2];
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
