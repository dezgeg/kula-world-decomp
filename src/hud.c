#include "common.h"

typedef struct DigitSprites {
    TSprite sprites[2][10];
} DigitSprites;

extern void DrawBonusWidget(void);
extern void DrawFruitWidgets(void);
extern void DrawHourglassAndTimer(void);
extern void DrawInt(DigitSprites* ds, int style, int numDigits, int max, int value);
extern void DrawKeyWidgets(void);
extern void DrawLethargyEffects(void);
extern void DrawScore(void);
extern void DrawTimeAttackWidgets(void);
extern void UpdateStaticHourglassClut(void);

int drawBonusWidget;
int drawCopycatWidgets;
int drawFruitWidgets;
int drawHourglassWidget;
int drawKeyWidget;
int drawScoreWidget;
int drawTimeAttackWidgets;
int drawTimerPausedWidget;
int hourglassIsRotating;
int hourglassRotationTimer;
int levelTimeLeft;
int numKeysInLevel;
int smoothIncrementingScore;
short* ggiPart1HourglassAnim;
uint firstGuiTexture;

extern DigitSprites copycatPlayer1ScoreDigitSprites;
extern DigitSprites copycatPlayer2ScoreDigitSprites;
extern DigitSprites levelScoreSprite;
extern DigitSprites levelTimeLeftDigitSprites;
extern DigitSprites timeAttackPlayer1CurLevelTimeDigitSprites;
extern DigitSprites timeAttackPlayer1TotalPlaytimeDigitSprites;
extern DigitSprites timeAttackPlayer2CurLevelTimeDigitSprites;
extern DigitSprites timeAttackPlayer2TotalPlaytimeDigitSprites;
extern DigitSprites totalScoreSprite;
extern DR_AREA hudDrawAreas[2];
extern DR_OFFSET hudDrawOffsets[2];
extern DR_TPAGE hudDrTpages[2];
extern int copycatPlayerScores[2];
extern int levelHasBeenCompletedByPlayer[2];
extern int levelPlayTime[2];
extern int levelScore;
extern int numKeysRemaining;
extern int numTimeTrialPlayers;
extern int totalPlayTime[2];
extern int totalScore;
extern int twoPlayerWhichPlayer;
extern int whichDrawDispEnv;
extern POLY_FT4 hourglassSprites[2][3];
extern PrimList primLists[2];
extern Texture textures[150];
extern TSprite keySprites[2][4][2];
extern TSprite timerPausedSprite[2];

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

void DrawHourglassAndTimer(void) {
    int i;
    int j;
    int color;
    int w;
    int dx;
    int secs;
    short* p;

    if (hourglassIsRotating) {
        hourglassRotationTimer += 8;
        if (hourglassRotationTimer >= 320) {
            hourglassRotationTimer = 0;
            hourglassIsRotating = 0;
            p = &ggiPart1HourglassAnim[hourglassRotationTimer];
            for (j = 0; j < 3; j++) {
                setXY4(&hourglassSprites[!whichDrawDispEnv][j],
                        p[j * 320 + 0] + 160, p[j * 320 + 1] + 26,
                        p[j * 320 + 2] + 160, p[j * 320 + 3] + 26,
                        p[j * 320 + 4] + 160, p[j * 320 + 5] + 26,
                        p[j * 320 + 6] + 160, p[j * 320 + 7] + 26);
            }
        }

        for (j = 0; j < 3; j++) {
            setXY4(&hourglassSprites[whichDrawDispEnv][j],
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 0] + 160,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 1] + 26,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 2] + 160,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 3] + 26,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 4] + 160,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 5] + 26,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 6] + 160,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 7] + 26);
        }

        switch (hourglassRotationTimer) {
            case 0:
                for (i = 0; i < 2; i++) {
                    for (j = 1; j < 3; j++) {
                        setUV4(&hourglassSprites[i][j], textures[firstGuiTexture + j].u,
                               textures[firstGuiTexture + j].v,
                               textures[firstGuiTexture + j].u + textures[firstGuiTexture + j].w + 0xff,
                               textures[firstGuiTexture + j].v, textures[firstGuiTexture + j].u,
                               textures[firstGuiTexture + j].v + textures[firstGuiTexture + j].h + 0xff,
                               textures[firstGuiTexture + j].u + textures[firstGuiTexture + j].w + 0xff,
                               textures[firstGuiTexture + j].v + textures[firstGuiTexture + j].h + 0xff);
                    }
                }
                break;
            case 160:
                UpdateStaticHourglassClut();
                setUV4(&hourglassSprites[whichDrawDispEnv][1],
                       textures[firstGuiTexture + 1].u + textures[firstGuiTexture + 1].w + 0xff,
                       textures[firstGuiTexture + 1].v + textures[firstGuiTexture + 1].h + 0xff,
                       textures[firstGuiTexture + 1].u,
                       textures[firstGuiTexture + 1].v + textures[firstGuiTexture + 1].h + 0xff,
                       textures[firstGuiTexture + 1].u + textures[firstGuiTexture + 1].w + 0xff,
                       textures[firstGuiTexture + 1].v, textures[firstGuiTexture + 1].u,
                       textures[firstGuiTexture + 1].v);
                for (i = 0; i < 2; i++) {
                    setUV4(&hourglassSprites[i][2],
                           textures[firstGuiTexture + 2].u + textures[firstGuiTexture + 2].w + 0xff,
                           textures[firstGuiTexture + 2].v + textures[firstGuiTexture + 2].h + 0xff,
                           textures[firstGuiTexture + 2].u,
                           textures[firstGuiTexture + 2].v + textures[firstGuiTexture + 2].h + 0xff,
                           textures[firstGuiTexture + 2].u + textures[firstGuiTexture + 2].w + 0xff,
                           textures[firstGuiTexture + 2].v, textures[firstGuiTexture + 2].u,
                           textures[firstGuiTexture + 2].v);
                }
                break;
            case 168:
                setUV4(&hourglassSprites[whichDrawDispEnv][1],
                       textures[firstGuiTexture + 1].u + textures[firstGuiTexture + 1].w + 0xff,
                       textures[firstGuiTexture + 1].v + textures[firstGuiTexture + 1].h + 0xff,
                       textures[firstGuiTexture + 1].u,
                       textures[firstGuiTexture + 1].v + textures[firstGuiTexture + 1].h + 0xff,
                       textures[firstGuiTexture + 1].u + textures[firstGuiTexture + 1].w + 0xff,
                       textures[firstGuiTexture + 1].v, textures[firstGuiTexture + 1].u,
                       textures[firstGuiTexture + 1].v);
        }
    } else {
        UpdateStaticHourglassClut();
    }

    secs = 1 + levelTimeLeft / 50;
    if (secs <= 50) {
        w = textures[firstGuiTexture + 3].w / 13;
        if (secs >= 10) {
            dx = 0;

        } else {
            dx = -w / 2;
        }
        color = (50 - secs) * 128 / 50;

        for (j = 0; j < 10; j++) {
            setXY0(&levelTimeLeftDigitSprites.sprites[whichDrawDispEnv][j].sprt, dx + 152 + w * j, 50);
            setRGB0(&levelTimeLeftDigitSprites.sprites[whichDrawDispEnv][j].sprt, color, color, color);
        }

        DrawInt(&levelTimeLeftDigitSprites, 0, 2, 100, secs);
    }

    addPrim(&primLists[whichDrawDispEnv].gui1, &hourglassSprites[whichDrawDispEnv][2]);
    addPrim(&primLists[whichDrawDispEnv].gui1, &hourglassSprites[whichDrawDispEnv][1]);
    addPrim(&primLists[whichDrawDispEnv].gui1, &hourglassSprites[whichDrawDispEnv][0]);
}

void DrawInt(DigitSprites *ds,int style,int numDigits,int max,int value) {
    int i;
    int smallestDrawnDigitIndex;
    int digit;
    int digitWidth;
    int pow10;
    int shouldDraw;
    int tex;

    if (value > max - 1) {
        value = max - 1;
    }
    pow10 = max / 10;
    tex = style + 3;
    digitWidth = textures[firstGuiTexture + tex].w / 13;
    shouldDraw = 0;
    smallestDrawnDigitIndex = numDigits;
    for (i = 0; i < numDigits; i++) {
        if (i == numDigits - 1) {
            shouldDraw = 1;
        }
        // XXX permuter ugliness
        digit = value;
        digit /= pow10;
        if (digit != 0 || shouldDraw == 1) {
            if (i < smallestDrawnDigitIndex) {
                smallestDrawnDigitIndex = i;
            }
            shouldDraw = 1;
            setUV0(&ds->sprites[whichDrawDispEnv][i].sprt,
                textures[firstGuiTexture + tex].u + digit * digitWidth,
                textures[firstGuiTexture + tex].v);
            value -= digit * pow10;
        }
        pow10 = pow10 / 10;
    }

    for (i = numDigits - 1; i >= smallestDrawnDigitIndex; i--) {
        addPrim(&primLists[whichDrawDispEnv].gui1, &ds->sprites[whichDrawDispEnv][i]);
    }
}

// https://decomp.me/scratch/1yWKd
INCLUDE_ASM("asm/nonmatchings/hud", DrawTimeAttackTimer);

INCLUDE_ASM("asm/nonmatchings/hud", UpdateStaticHourglassClut);

void DrawKeyWidgets(void) {
    int i;

    for (i = numKeysInLevel - 1; i >= 0; i--) {
        if (i < numKeysInLevel - numKeysRemaining){
            addPrim(&primLists[whichDrawDispEnv].gui1,
                    &keySprites[whichDrawDispEnv][i][1]);
        } else {
            addPrim(&primLists[whichDrawDispEnv].gui1,
                    &keySprites[whichDrawDispEnv][i][0]);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/hud", DrawFruitWidgets);
