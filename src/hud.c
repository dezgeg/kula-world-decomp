#include "common.h"

typedef struct DigitSprites {
    TSprite sprites[2][10];
} DigitSprites;

extern void AddParticles(int type, SVECTOR* pos, int lightEffectId);
extern void DrawBonusWidget(void);
extern void DrawHourglassAndTimer(void);
extern void DrawInt(DigitSprites* ds, int style, int numDigits, int max, int value);
extern void DrawKeyWidgets(void);
extern void DrawLethargyEffects(void);
extern void DrawScore(void);
extern void DrawTimeAttackWidgets(void);
extern void UpdateStaticHourglassClut(void);

extern int BONUS_WIDGET_COLOR_DATA_LEN;
extern uint BONUS_WIDGET_COLOR_DATA[96];
extern TSprite fruitBonusTextSprites[2][5][2];
extern TSprite fruitSprites[2][5][2];
extern int fruitsCollectedBitmask;
extern int specialLevelType;
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
extern short hourglassClut[60];
extern Texture textures[150];
extern TSprite keySprites[2][4][2];
extern TSprite timerPausedSprite[2];

int* bonusWidgetDataEnd;
int* bonusWidgetDataPtr3;
int drawBonusWidget;
int drawCopycatWidgets;
int drawFruitWidgets;
int drawHourglassWidget;
int drawKeyWidget;
int drawScoreWidget;
int drawTimeAttackWidgets;
int drawTimerPausedWidget;
int fruitBonusTextIndex;
int fruitBonusTextTimer;
int fruitWidgetDisplayMode;
int hourglassIsRotating;
int hourglassRotationTimer;
int levelTimeLeft;
int numKeysInLevel;
int smoothIncrementingScore;
GgiFile* ggi;
short* ggiPart1HourglassAnim;
uint firstGuiTexture;

RECT hourglassClutRect = {0, 0, 60, 1};

static short HOURGLASS_ANIM_DATA[29][2] = {
    {0, 0}, {-31711, -31711}, {-30654, -30654}, {-29597, -29597},
    {-28540, -28540}, {-27483, -27483}, {-26426, -26426}, {-25369, -25369},
    {-24312, -24312}, {-23255, -23255}, {-22198, -22198}, {-21141, -21141},
    {-20084, -20084}, {-19027, -19027}, {-17970, -17970}, {-16913, -16913},
    {-15856, -15856}, {-14799, -14799}, {-13742, -13742}, {-12685, -12685},
    {-11628, -11628}, {-10571, -10571}, {-9514, -9514}, {-8457, -8457},
    {-7400, -7400}, {-6343, -6343}, {-5286, -5286}, {-4229, -4229},
    {-3172, 3897}
};

static int pad = 0; // hack

int TIME_STR_DIGIT_DIVISORS[9] = {
    360000, 36000, 3600, 0, 600, 60, 0, 10, 1
};

SVECTOR FRUIT_BONUS_TEXT_PARTICLE_POSITIONS[5] = {
    {208, 233, -1, 0},
    {228, 232, -1, 0},
    {253, 232, -1, 0},
    {278, 232, -1, 0},
    {299, 234, -1, 0}
};

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
    DrawTimeAttackTimer(&timeAttackPlayer1TotalPlaytimeDigitSprites, 2, totalPlayTime[0], 0);
    if (numTimeTrialPlayers == 2) {
        DrawTimeAttackTimer(&timeAttackPlayer2TotalPlaytimeDigitSprites, 2, totalPlayTime[1], 0);
        if (twoPlayerWhichPlayer == 0) {
            DrawTimeAttackTimer(&timeAttackPlayer1CurLevelTimeDigitSprites, 1, curPlayerTimer, 0);
            curPlayerTimer = levelPlayTime[1] / 50;
            if (levelPlayTime[1] < 0) {
                curPlayerTimer--;
            }
            if (levelHasBeenCompletedByPlayer[1] == 1) {
                DrawTimeAttackTimer(&timeAttackPlayer2CurLevelTimeDigitSprites, 1, curPlayerTimer, 0);
            } else {
                DrawTimeAttackTimer(&timeAttackPlayer2CurLevelTimeDigitSprites, 1, 0, 1);
            }
        } else {
            DrawTimeAttackTimer(&timeAttackPlayer2CurLevelTimeDigitSprites, 1, curPlayerTimer, 0);
            curPlayerTimer = levelPlayTime[0] / 50;
            if (levelPlayTime[0] < 0) {
                curPlayerTimer--;
            }
            if (levelHasBeenCompletedByPlayer[0] != 1) {
                DrawTimeAttackTimer(&timeAttackPlayer1CurLevelTimeDigitSprites, 1, 0, 1);
            } else {
                DrawTimeAttackTimer(&timeAttackPlayer1CurLevelTimeDigitSprites,1,curPlayerTimer,0);
            }
        }
    } else {
        DrawTimeAttackTimer(&timeAttackPlayer1CurLevelTimeDigitSprites, 1, curPlayerTimer, 0);
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

void DrawInt(DigitSprites* ds, int style, int numDigits, int max, int value) {
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
void DrawTimeAttackTimer(DigitSprites *ds, int style, int value, int onlyDashes) {
    int divisor;
    int i;
    int smallestDrawnDigitIndex;
    int digit;
    int v;
    int charWidth;
    int shouldDraw;
    int w;
    // FIXME: complete mess
    int texId;
    int texId2;
    int texId3;
    int three = 3;

    texId = style + 3;
    charWidth = textures[firstGuiTexture + texId].w / 13;
    if (onlyDashes == 1) {
        setUV0(&ds->sprites[whichDrawDispEnv][9].sprt,
                textures[firstGuiTexture + texId].u + charWidth * 11,
                textures[firstGuiTexture + texId].v);
        addPrim(&primLists[whichDrawDispEnv].gui1, &ds->sprites[whichDrawDispEnv][9]);

        setUV0(&ds->sprites[whichDrawDispEnv][8].sprt,
                textures[firstGuiTexture + texId].u + charWidth * 11,
                textures[firstGuiTexture + texId].v);
        addPrim(&primLists[whichDrawDispEnv].gui1, &ds->sprites[whichDrawDispEnv][8]);

        setUV0(&ds->sprites[whichDrawDispEnv][7].sprt,
                textures[firstGuiTexture + texId].u + charWidth * 10,
                textures[firstGuiTexture + texId].v);
        addPrim(&primLists[whichDrawDispEnv].gui1, &ds->sprites[whichDrawDispEnv][7]);

        setUV0(&ds->sprites[whichDrawDispEnv][6].sprt,
                textures[firstGuiTexture + texId].u + charWidth * 11,
                textures[firstGuiTexture + texId].v);
        addPrim(&primLists[whichDrawDispEnv].gui1, &ds->sprites[whichDrawDispEnv][6]);
    } else {
        v = abs(value);
        if (v > 3600000) {
            v = 3599999;
        }
        smallestDrawnDigitIndex = 6;
        shouldDraw = 0;
        i = 1;
        texId3 = texId;
        for (; i < 10; i++) {
            if (i == 4 || i == 7) {
                continue;
            }
            if (i > 5) {
                shouldDraw = 1;
            }
            divisor = TIME_STR_DIGIT_DIVISORS[i - 1];
            digit = v / divisor;
            if (digit != 0 || shouldDraw == 1) {
                if (i < smallestDrawnDigitIndex) {
                    smallestDrawnDigitIndex = i;
                }
                shouldDraw = 1;
                setUV0(&ds->sprites[whichDrawDispEnv][i].sprt,
                    textures[firstGuiTexture + texId3].u + digit * charWidth,
                    textures[firstGuiTexture + texId3].v);
                v -= digit * divisor;
            }
        }
        texId2 = style + 3;

        setUV0(&ds->sprites[whichDrawDispEnv][7].sprt,
            textures[firstGuiTexture + texId2].u + 10 * charWidth,
            textures[firstGuiTexture + texId2].v);
        if (smallestDrawnDigitIndex < 4) {
            setUV0(&ds->sprites[whichDrawDispEnv][4].sprt,
                textures[firstGuiTexture + texId2].u + 10 * charWidth,
                textures[firstGuiTexture + texId2].v);
        }

        if (value != 0) {
            smallestDrawnDigitIndex--;
            setUV0(&ds->sprites[whichDrawDispEnv][smallestDrawnDigitIndex].sprt,
                textures[firstGuiTexture + texId2].u + (value < 0 ? charWidth * 11 : charWidth * 12 ),
                textures[firstGuiTexture + (style + three)].v);
        }

        for (i = 9; i >= smallestDrawnDigitIndex; i--) {
            addPrim(&primLists[whichDrawDispEnv].gui1, &ds->sprites[whichDrawDispEnv][i]);
        }
    }
}

void UpdateStaticHourglassClut(void) {
    int i;
    int amount;
    int div28;
    int mod28;
    int dummy[4];

    amount = (levelTimeLeft * 420) / 5000;
    div28 = amount / 28;
    mod28 = amount % 28;
    for (i = 0; i < div28; i++) {
        hourglassClut[i * 2] = HOURGLASS_ANIM_DATA[28][0];
        hourglassClut[i * 2 + 1] = HOURGLASS_ANIM_DATA[28][1];
        hourglassClut[i * 2 + 30] = HOURGLASS_ANIM_DATA[0][0];
        hourglassClut[i * 2 + 31] = HOURGLASS_ANIM_DATA[0][1];
    }
    i += 1;
    hourglassClut[div28 * 2] = HOURGLASS_ANIM_DATA[mod28][0];
    hourglassClut[div28 * 2 + 1] = HOURGLASS_ANIM_DATA[mod28][1];
    hourglassClut[div28 * 2 + 30] = HOURGLASS_ANIM_DATA[27 - mod28][0];
    hourglassClut[div28 * 2 + 31] = HOURGLASS_ANIM_DATA[27 - mod28][1];
    for (; i < 15; i++) {
        hourglassClut[i*2] = HOURGLASS_ANIM_DATA[0][0];
        hourglassClut[i*2 + 1] = HOURGLASS_ANIM_DATA[0][1];
        hourglassClut[i*2 + 30] = HOURGLASS_ANIM_DATA[28][0];
        hourglassClut[i*2 + 31] = HOURGLASS_ANIM_DATA[28][1];
    }
    LoadImage(&hourglassClutRect,hourglassClut);
}

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

void DrawFruitWidgets(void) {
    int i;
    int* p;
    int len;
    int wd;

    switch (fruitWidgetDisplayMode) {
    case 0:
        for (i = 0; i < ggi->numFruitTextures / 2; i++) {
            if (((fruitsCollectedBitmask >> i) & 1) == 0) {
                addPrim(&primLists[whichDrawDispEnv].gui1, &fruitSprites[whichDrawDispEnv][i][0]);
            } else {
                addPrim(&primLists[whichDrawDispEnv].gui1, &fruitSprites[whichDrawDispEnv][i][1]);
            }
        }
        if (fruitsCollectedBitmask == 0x1f) {
            fruitWidgetDisplayMode = 1;
            fruitBonusTextIndex = 0;
            fruitBonusTextTimer = 7;
        }
        break;
    case 1:
        for (i = 0; i < 5; i++) {
            if (i >= fruitBonusTextIndex) {
                addPrim(&primLists[whichDrawDispEnv].gui1, &fruitSprites[whichDrawDispEnv][i][1]);
            } else {
                addPrim(&primLists[whichDrawDispEnv].gui1, &fruitBonusTextSprites[whichDrawDispEnv][i][1]);
                addPrim(&primLists[whichDrawDispEnv].gui1, &fruitBonusTextSprites[whichDrawDispEnv][i][0]);
            }
        }
        fruitBonusTextTimer--;
        if (fruitBonusTextTimer < 0) {
            fruitBonusTextTimer = 7;
            AddParticles(0, &FRUIT_BONUS_TEXT_PARTICLE_POSITIONS[fruitBonusTextIndex], 0);
            fruitBonusTextIndex++;
            if (fruitBonusTextIndex == 5) {
                fruitWidgetDisplayMode = 2;
                fruitBonusTextTimer = 100;
            }
        }
        break;
    case 2:
        for (i = 0; i < 5; i++) {
            addPrim(&primLists[whichDrawDispEnv].gui1, &fruitBonusTextSprites[whichDrawDispEnv][i][1]);
            addPrim(&primLists[whichDrawDispEnv].gui1, &fruitBonusTextSprites[whichDrawDispEnv][i][0]);
        }
        if (specialLevelType != 1) {
            fruitBonusTextTimer--;
            if (fruitBonusTextTimer < 0) {
                fruitWidgetDisplayMode = 3;
                fruitBonusTextIndex = 5;
                fruitBonusTextTimer = 7;
            }
        }
        break;
    case 3:
        for (i = 0; i < 5; i++) {
            if (i >= fruitBonusTextIndex) {
                addPrim(&primLists[whichDrawDispEnv].gui1, &fruitSprites[whichDrawDispEnv][i][1]);
            } else {
                addPrim(&primLists[whichDrawDispEnv].gui1, &fruitBonusTextSprites[whichDrawDispEnv][i][1]);
                addPrim(&primLists[whichDrawDispEnv].gui1, &fruitBonusTextSprites[whichDrawDispEnv][i][0]);
            }
        }
        fruitBonusTextTimer--;
        if (fruitBonusTextTimer < 0) {
            fruitBonusTextTimer = 7;
            fruitBonusTextIndex--;
            if (fruitBonusTextIndex == -1) {
                fruitWidgetDisplayMode = 4;
                fruitBonusTextTimer = 100;
            } else {
                AddParticles(0, &FRUIT_BONUS_TEXT_PARTICLE_POSITIONS[fruitBonusTextIndex], 0);
            }
        }
        break;
    case 4:
        for (i = 0; i < 5; i++) {
            addPrim(&primLists[whichDrawDispEnv].gui1, &fruitSprites[whichDrawDispEnv][i][1]);
        }
        fruitBonusTextTimer--;
        if (fruitBonusTextTimer < 0) {
            fruitWidgetDisplayMode = 1;
            fruitBonusTextIndex = 0;
            fruitBonusTextTimer = 7;
        }
        break;
    }
    if (fruitWidgetDisplayMode != 0) {
        bonusWidgetDataPtr3++;
        if (bonusWidgetDataPtr3 >= bonusWidgetDataEnd) {
            bonusWidgetDataPtr3 -= BONUS_WIDGET_COLOR_DATA_LEN;
        }
        p = bonusWidgetDataPtr3;
        len = BONUS_WIDGET_COLOR_DATA_LEN;
        wd = whichDrawDispEnv;
        i = 0;
        for (; i < 5; i++) {
            *(uint*)&fruitBonusTextSprites[wd][i][1].sprt.r0 = *p & 0xfdffffff;
            p -= 12;
            if (p < BONUS_WIDGET_COLOR_DATA) {
                p += len;
            }
        }
    }
}
