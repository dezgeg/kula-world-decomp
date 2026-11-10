#include "common.h"

typedef struct DigitSprites {
    TSprite sprites[2][10];
} DigitSprites;

typedef struct ScoreGraph {
    DR_TPAGE tpages[2];          // 16 bytes  (offset 0)
    u_long lines[2][3300];       // 26400 bytes (offset 16)
    POLY_G4 polys[2][300];       // 21600 bytes (offset 26416)
    POLY_G4 unusedPolys[2][301]; // 21672 bytes (offset 48016)
    DR_TPAGE tpages2[2];         // 16 bytes (offset 69688)
    POLY_G4 polys3[2][3];        // 216 bytes (offset 69704)
} ScoreGraph;

typedef struct {
    uint c;
} Col;

typedef struct Tim {
    uint magic;
    uint flags;
    uint clutLength;
    ushort clutX;
    ushort clutY;
    ushort clutWidth;
    ushort clutHeight;
    short clutData[16];
    int imageLength;
    ushort imageX;
    ushort imageY;
    ushort imageWidth;
    ushort imageHeight;
} Tim;

typedef struct BiosTextWriter {
    short unk[2][17];
    Tim tim;
} BiosTextWriter;

typedef struct Sparkle {
    uint ptrAndFlags;
    uint color;
    uint unk2;
    uint unk3;
    uint unk4;
} Sparkle;

typedef struct GemSparkleEffect {
    uint* vertexColors;
    int numColors;
    int probability;
    int addRgb;
    int maxRgb;
    int subRgb;
    int numSparkles;
    Sparkle sparkles[16];
} GemSparkleEffect;

extern uint Rand(int param_1);
extern void AddParticles(int type, SVECTOR* pos, int lightEffectId);
extern void DrawBonusWidget(void);
extern void DrawHourglassAndTimer(void);
extern void DrawInt(DigitSprites* ds, int style, int numDigits, int max, int value);
extern void DrawKeyWidgets(void);
extern void DrawLethargyEffects(void);
extern void DrawScore(void);
extern void DrawTimeAttackWidgets(void);
extern void InitPlayerSpecularSprite(void);
extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);
extern void UpdateStaticHourglassClut(void);

void ParseTextures(int* headerPtr, Texture* out, int count);
void ParseGgiInner(int* eff, int modelType, int modelIdx, int i, int j, int probability, int addRgb, int maxRgb, int subRgb, int numSparkles);

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
extern DrawDisp drawdisp[2];
extern DR_OFFSET hudDrawOffsets[2];
extern DR_TPAGE hudDrTpages[2];
extern int BONUS_WIDGET_COLOR_DATA_LEN;
extern int cameraIndex;
extern int copycatPlayerScores[2];
extern int dispenvScreenX;
extern int dispenvScreenY;
extern int displayHeight;
extern int displayWidth;
extern int fruitsCollectedBitmask;
extern int gteXScale;
extern int gteYScale;
extern int levelHasBeenCompletedByPlayer[2];
extern int levelPlayTime[2];
extern int levelScore;
extern int numCameras;
extern int numCubesRemainingInLevel[5];
extern int numKeysRemaining;
extern int numTimeTrialPlayers;
extern int specialLevelType;
extern int sunIntensityOnScreen;
extern int totalPlayTime[2];
extern int totalScore;
extern int twoPlayerWhichPlayer;
extern int whichDrawDispEnv;
extern OT_TYPE otag[2][1][1026];
extern POLY_FT4 hourglassSprites[2][3];
extern PrimList primLists[2];
extern short hourglassClut[60];
extern TSprite biosStrSprites[2][32];
extern TSprite bonusWidgetSpritesMsbBackground[2][34];
extern TSprite bonusWidgetSpritesMsbForeground[2][32];
extern TSprite bonusWidgetSpritesPiechart[2][9];
extern TSprite copyrightSprite[2];
extern TSprite fruitBonusTextSprites[2][5][2];
extern TSprite fruitSprites[2][5][2];
extern TSprite keySprites[2][4][2];
extern TSprite lethargyEffectSprite[2][2];
extern TSprite scoreGraphSprites[2][2];
extern TSprite timerPausedSprite[2];
extern TSprite titleSprite[2];
extern uint BONUS_WIDGET_COLOR_DATA[1]; // fake size

GemSparkleEffect gemRandomSparkleEffects[3][3];
int doDrawGraph;
int drawBonusWidget;
int drawCopycatWidgets;
int drawFruitWidgets;
int drawHourglassWidget;
int drawKeyWidget;
int drawScoreGraphHeight;
int* drawScoreGraphHeightsBuf;
int* drawScoreGraphLevelScores;
int* drawScoreGraphUnusedScorePtr;
int drawScoreGraphWidth;
int drawScoreGraphY;
int drawScoreWidget;
int drawTimeAttackWidgets;
int drawTimerPausedWidget;
int firstBonusWidgetTexture;
int firstFruitTexture;
int firstGuiTextTexture;
int firstLensFlareOrPsxButtonTexture;
int firstParticleTexture;
int firstUnk2Texture;
int halfFps;
int hourglassIsRotating;
int levelTimeLeft;
int numBonusWidgetTextures;
int numGuiTextTextures;
int numKeysInLevel;
int numParticleTextures;
int numUnk1Textures;
int numUnk2Textures;
int screenFadeEnabled;
ScoreGraph* drawScoreGraphPrims;
static void* ggiPart3;
static void* ggiPart4Dummy;
static void* ggiPart6Textures;
static void* ggiPart7Unused;
Texture textures[NUM_TEXTURES];
TPolyF4 screenFadePolys[2][1];
uint firstGuiTexture;
void* ggiPart0A;
void* ggiPart0B;
void* ggiPart2DepthCueingLookup;
void* ggiPart5JumpAnimData;

STATIC_FOR_GP_ACCESS int* bonusWidgetDataEnd;
STATIC_FOR_GP_ACCESS int* bonusWidgetDataPtr;
STATIC_FOR_GP_ACCESS int* bonusWidgetDataPtr2;
STATIC_FOR_GP_ACCESS int* bonusWidgetDataPtr3;
STATIC_FOR_GP_ACCESS int fruitBonusTextIndex;
STATIC_FOR_GP_ACCESS int fruitBonusTextTimer;
STATIC_FOR_GP_ACCESS int fruitWidgetDisplayMode;
STATIC_FOR_GP_ACCESS int* ggi;
STATIC_FOR_GP_ACCESS int hourglassRotationTimer;
STATIC_FOR_GP_ACCESS int lethargyMode;
STATIC_FOR_GP_ACCESS int numBonusWidgetMsbs;
STATIC_FOR_GP_ACCESS int screenFadeColor;
STATIC_FOR_GP_ACCESS int screenFadeSpeed;
STATIC_FOR_GP_ACCESS POLY_FT4 lethargyEffectPoly[2];
STATIC_FOR_GP_ACCESS short* ggiPart1HourglassAnim;
static int biosStrLen;
static int lethargyAngleCounter;
static int lethargyColorCounter;
static int lethargySinMagnitude;
static int lethargyUnusedCounter;
static int screenShakeDirectionMask;
static int screenShakeDivisor;
static int screenShakeEnabled;
static int screenShakeMagnitude;
static int screenShakeTimer;
static int smoothIncrementingScore;

RECT hourglassClutRect = {0, 0, 60, 1};

static short HOURGLASS_ANIM_DATA[58] = {
    0, 0, -31711, -31711, -30654, -30654, -29597, -29597,
    -28540, -28540, -27483, -27483, -26426, -26426, -25369, -25369,
    -24312, -24312, -23255, -23255, -22198, -22198, -21141, -21141,
    -20084, -20084, -19027, -19027, -17970, -17970, -16913, -16913,
    -15856, -15856, -14799, -14799, -13742, -13742, -12685, -12685,
    -11628, -11628, -10571, -10571, -9514, -9514, -8457, -8457,
    -7400, -7400, -6343, -6343, -5286, -5286, -4229, -4229,
    -3172, 3897
};

int TIME_STR_DIGIT_DIVISORS[10] = {
    0, 360000, 36000, 3600, 0, 600, 60, 0, 10, 1
};

SVECTOR FRUIT_BONUS_TEXT_PARTICLE_POSITIONS[5] = {
    {208, SCREEN_HEIGHT - 23, -1, 0},
    {228, SCREEN_HEIGHT - 24, -1, 0},
    {253, SCREEN_HEIGHT - 24, -1, 0},
    {278, SCREEN_HEIGHT - 24, -1, 0},
    {299, SCREEN_HEIGHT - 22, -1, 0}
};

int drawCopyright = 1;

static inline void SetDispScreen(ushort x, ushort y) {
    drawdisp[0].disp.screen.x = drawdisp[1].disp.screen.x = x;
    drawdisp[0].disp.screen.y = drawdisp[1].disp.screen.y = y;
}

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

    curPlayerTimer = levelPlayTime[twoPlayerWhichPlayer] / FPS;
    if (levelPlayTime[twoPlayerWhichPlayer] < 0) {
        curPlayerTimer--;
    }
    DrawTimeAttackTimer(&timeAttackPlayer1TotalPlaytimeDigitSprites, 2, totalPlayTime[0], 0);
    if (numTimeTrialPlayers == 2) {
        DrawTimeAttackTimer(&timeAttackPlayer2TotalPlaytimeDigitSprites, 2, totalPlayTime[1], 0);
        if (twoPlayerWhichPlayer == 0) {
            DrawTimeAttackTimer(&timeAttackPlayer1CurLevelTimeDigitSprites, 1, curPlayerTimer, 0);
            curPlayerTimer = levelPlayTime[1] / FPS;
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
            curPlayerTimer = levelPlayTime[0] / FPS;
            if (levelPlayTime[0] < 0) {
                curPlayerTimer--;
            }
            if (levelHasBeenCompletedByPlayer[0] == 1) {
                DrawTimeAttackTimer(&timeAttackPlayer1CurLevelTimeDigitSprites, 1, curPlayerTimer, 0);
            } else {
                DrawTimeAttackTimer(&timeAttackPlayer1CurLevelTimeDigitSprites, 1, 0, 1);
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

    if (hourglassIsRotating) {
        hourglassRotationTimer += 8;
        if (hourglassRotationTimer >= 320) {
            hourglassRotationTimer = 0;
            hourglassIsRotating = 0;
            for (j = 0; j < 3; j++) {
                setXY4(&hourglassSprites[!whichDrawDispEnv][j],
                       ggiPart1HourglassAnim[j * 320 + 0] + 160,
                       ggiPart1HourglassAnim[j * 320 + 1] + 26 + 2 * VID_NTSC,
                       ggiPart1HourglassAnim[j * 320 + 2] + 160,
                       ggiPart1HourglassAnim[j * 320 + 3] + 26 + 2 * VID_NTSC,
                       ggiPart1HourglassAnim[j * 320 + 4] + 160,
                       ggiPart1HourglassAnim[j * 320 + 5] + 26 + 2 * VID_NTSC,
                       ggiPart1HourglassAnim[j * 320 + 6] + 160,
                       ggiPart1HourglassAnim[j * 320 + 7] + 26 + 2 * VID_NTSC);
            }
        }

        for (j = 0; j < 3; j++) {
            setXY4(&hourglassSprites[whichDrawDispEnv][j],
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 0] + 160,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 1] + 26 + 2 * VID_NTSC,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 2] + 160,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 3] + 26 + 2 * VID_NTSC,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 4] + 160,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 5] + 26 + 2 * VID_NTSC,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 6] + 160,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 7] + 26 + 2 * VID_NTSC);
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

    secs = 1 + levelTimeLeft / FPS;
    if (secs <= 50) {
        w = textures[firstGuiTexture + 3].w / 13;
        if (secs >= 10) {
            dx = 0;

        } else {
            dx = -w / 2;
        }
        color = (50 - secs) * 128 / 50;

        for (j = 0; j < 10; j++) {
            setXY0(&levelTimeLeftDigitSprites.sprites[whichDrawDispEnv][j].sprt, dx + 152 + w * j, 50 + 2 * VID_NTSC);
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

    if (value > max - 1) {
        value = max - 1;
    }
    digitWidth = textures[firstGuiTexture + 3 + style].w / 13;
    pow10 = max / 10;
    shouldDraw = 0;
    smallestDrawnDigitIndex = numDigits;
    for (i = 0; i < numDigits; i++) {
        if (i == numDigits - 1) {
            shouldDraw = 1;
        }
        digit = value / pow10;
        if (digit != 0 || shouldDraw == 1) {
            if (i < smallestDrawnDigitIndex) {
                smallestDrawnDigitIndex = i;
            }
            shouldDraw = 1;
            setUV0(&ds->sprites[whichDrawDispEnv][i].sprt,
                   textures[firstGuiTexture + 3 + style].u + digit * digitWidth,
                   textures[firstGuiTexture + 3 + style].v);
            value -= digit * pow10;
        }
        pow10 = pow10 / 10;
    }

    for (i = numDigits - 1; i >= smallestDrawnDigitIndex; i--) {
        addPrim(&primLists[whichDrawDispEnv].gui1, &ds->sprites[whichDrawDispEnv][i]);
    }
}

void DrawTimeAttackTimer(DigitSprites* ds, int style, int value, int onlyDashes) {
    int divisor;
    int i;
    int smallestDrawnDigitIndex;
    int digit;
    int v;
    int charWidth;
    int shouldDraw;

    charWidth = textures[firstGuiTexture + 3 + style].w / 13;
    if (onlyDashes == 1) {
        setUV0(&ds->sprites[whichDrawDispEnv][9].sprt,
                textures[firstGuiTexture + 3 + style].u + charWidth * 11,
                textures[firstGuiTexture + 3 + style].v);
        addPrim(&primLists[whichDrawDispEnv].gui1, &ds->sprites[whichDrawDispEnv][9]);

        setUV0(&ds->sprites[whichDrawDispEnv][8].sprt,
                textures[firstGuiTexture + 3 + style].u + charWidth * 11,
                textures[firstGuiTexture + 3 + style].v);
        addPrim(&primLists[whichDrawDispEnv].gui1, &ds->sprites[whichDrawDispEnv][8]);

        setUV0(&ds->sprites[whichDrawDispEnv][7].sprt,
                textures[firstGuiTexture + 3 + style].u + charWidth * 10,
                textures[firstGuiTexture + 3 + style].v);
        addPrim(&primLists[whichDrawDispEnv].gui1, &ds->sprites[whichDrawDispEnv][7]);

        setUV0(&ds->sprites[whichDrawDispEnv][6].sprt,
                textures[firstGuiTexture + 3 + style].u + charWidth * 11,
                textures[firstGuiTexture + 3 + style].v);
        addPrim(&primLists[whichDrawDispEnv].gui1, &ds->sprites[whichDrawDispEnv][6]);
    } else {
        v = abs(value);
        if (v > 3600000) {
            v = 3599999;
        }
        smallestDrawnDigitIndex = 6;
        shouldDraw = 0;
        for (i = 1; i < 10; i++) {
            if (i == 4 || i == 7) {
                continue;
            }
            if (i > 5) {
                shouldDraw = 1;
            }
            divisor = TIME_STR_DIGIT_DIVISORS[i];
            digit = v / divisor;
            if (digit != 0 || shouldDraw == 1) {
                if (i < smallestDrawnDigitIndex) {
                    smallestDrawnDigitIndex = i;
                }
                shouldDraw = 1;
                setUV0(&ds->sprites[whichDrawDispEnv][i].sprt,
                    textures[firstGuiTexture + 3 + style].u + digit * charWidth,
                    textures[firstGuiTexture + 3 + style].v);
                v -= digit * divisor;
            }
        }

        setUV0(&ds->sprites[whichDrawDispEnv][7].sprt,
            textures[firstGuiTexture + 3 + style].u + 10 * charWidth,
            textures[firstGuiTexture + 3 + style].v);
        if (smallestDrawnDigitIndex < 4) {
            setUV0(&ds->sprites[whichDrawDispEnv][4].sprt,
                textures[firstGuiTexture + 3 + style].u + 10 * charWidth,
                textures[firstGuiTexture + 3 + style].v);
        }

        if (value != 0) {
            smallestDrawnDigitIndex--;
            setUV0(&ds->sprites[whichDrawDispEnv][smallestDrawnDigitIndex].sprt,
                   textures[firstGuiTexture + 3 + style].u + (value < 0 ? charWidth * 11 : charWidth * 12),
                   textures[firstGuiTexture + 3 + style].v);
        }

        for (i = 9; i >= smallestDrawnDigitIndex; i--) {
            addPrim(&primLists[whichDrawDispEnv].gui1, &ds->sprites[whichDrawDispEnv][i]);
        }
    }
}

void UpdateStaticHourglassClut(void) {
    int amount;
    int div28;
    int mod28;
    int i;

    amount = (levelTimeLeft * 420) / ((VER_JP ? 120 : 100) * FPS);
    div28 = amount / 28;
    mod28 = amount % 28;
    for (i = 0; i < div28; i++) {
        hourglassClut[i * 2] = HOURGLASS_ANIM_DATA[56];
        hourglassClut[i * 2 + 1] = HOURGLASS_ANIM_DATA[57];
        hourglassClut[i * 2 + 30] = HOURGLASS_ANIM_DATA[0];
        hourglassClut[i * 2 + 31] = HOURGLASS_ANIM_DATA[1];
    }
    i += 1;
    hourglassClut[div28 * 2] = HOURGLASS_ANIM_DATA[mod28 * 2];
    hourglassClut[div28 * 2 + 1] = HOURGLASS_ANIM_DATA[mod28 * 2 + 1];
    hourglassClut[div28 * 2 + 30] = HOURGLASS_ANIM_DATA[(27 - mod28) * 2];
    hourglassClut[div28 * 2 + 31] = HOURGLASS_ANIM_DATA[(27 - mod28) * 2 + 1];
    for (; i < 15; i++) {
        hourglassClut[i * 2] = HOURGLASS_ANIM_DATA[0];
        hourglassClut[i * 2 + 1] = HOURGLASS_ANIM_DATA[1];
        hourglassClut[i * 2 + 30] = HOURGLASS_ANIM_DATA[56];
        hourglassClut[i * 2 + 31] = HOURGLASS_ANIM_DATA[57];
    }
    LoadImage(&hourglassClutRect, hourglassClut);
}

void DrawKeyWidgets(void) {
    int i;

    for (i = numKeysInLevel - 1; i >= 0; i--) {
        if (i < numKeysInLevel - numKeysRemaining) {
            addPrim(&primLists[whichDrawDispEnv].gui1, &keySprites[whichDrawDispEnv][i][1]);
        } else {
            addPrim(&primLists[whichDrawDispEnv].gui1, &keySprites[whichDrawDispEnv][i][0]);
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
            for (i = 0; i < ggi[GGI_NUM_FRUIT_TEXTURES] / 2; i++) {
                if (((fruitsCollectedBitmask >> i) & 1) == 0) {
                    addPrim(&primLists[whichDrawDispEnv].gui1, &fruitSprites[whichDrawDispEnv][i][0]);
                } else {
                    addPrim(&primLists[whichDrawDispEnv].gui1, &fruitSprites[whichDrawDispEnv][i][1]);
                }
            }
            if (fruitsCollectedBitmask == 0x1f) {
                fruitWidgetDisplayMode = 1;
                fruitBonusTextIndex = 0;
                fruitBonusTextTimer = VID_NTSC ? 9 : 7;
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
                fruitBonusTextTimer = VID_NTSC ? 9 : 7;
                AddParticles(0, &FRUIT_BONUS_TEXT_PARTICLE_POSITIONS[fruitBonusTextIndex], 0);
                fruitBonusTextIndex++;
                if (fruitBonusTextIndex == 5) {
                    fruitWidgetDisplayMode = 2;
                    fruitBonusTextTimer = 2 * FPS;
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
                    fruitBonusTextTimer = VID_NTSC ? 9 : 7;
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
                fruitBonusTextTimer = VID_NTSC ? 9 : 7;
                fruitBonusTextIndex--;
                if (fruitBonusTextIndex == -1) {
                    fruitWidgetDisplayMode = 4;
                    fruitBonusTextTimer = 2 * FPS;
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
                fruitBonusTextTimer = VID_NTSC ? 9 : 7;
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

void EnableLethargy(int enable) {
    if (numCameras < 2) {
        if (enable == 1) {
            lethargyMode = enable;
        } else if (lethargyMode) {
            lethargyMode = 3;
        }
    }
}

void ResetLethargyEffect() {
    lethargyMode = 0;
    gteXScale = 0x1000;
    gteYScale = 16 * SCREEN_HEIGHT;
}

void DrawLethargyEffects(void) {
    int delta;

    switch (lethargyMode) {
        case 1:
            lethargyMode = 2;
            setXY4(&lethargyEffectPoly[whichDrawDispEnv],
                    0, 0,
                    displayWidth + 1, 0,
                    0, displayHeight + 1,
                    displayWidth + 1, displayHeight + 1);
            setRGB0(&lethargyEffectPoly[whichDrawDispEnv], 0x80, 0x80, 0x80);
            AddPrim(&primLists[whichDrawDispEnv].main, &lethargyEffectPoly[whichDrawDispEnv]);
            lethargyUnusedCounter = 0x400;
            lethargyAngleCounter = 0x400;
            lethargySinMagnitude = 0x30c;
            break;
        case 2:
            lethargyUnusedCounter = (lethargyUnusedCounter + VELOCITY(0x80)) % 0x1000;
            rsin(lethargyUnusedCounter);
            setRGB0(&lethargyEffectSprite[whichDrawDispEnv][0].sprt, 96, 112, 96);
            AddPrim(&primLists[whichDrawDispEnv].main, &lethargyEffectSprite[whichDrawDispEnv][0]);
            if (displayWidth > 256) {
                setRGB0(&lethargyEffectSprite[whichDrawDispEnv][1].sprt, 96, 112, 96);
                AddPrim(&primLists[whichDrawDispEnv].main, &lethargyEffectSprite[whichDrawDispEnv][1]);
            }
            break;
        case 3:
            setXY4(&lethargyEffectPoly[0],
                    0, 0,
                    displayWidth + 1, 0,
                    0, displayHeight + 1,
                    displayWidth + 1, displayHeight + 1);
            setXY4(&lethargyEffectPoly[1],
                    0, 0,
                    displayWidth + 1, 0,
                    0, displayHeight + 1,
                    displayWidth + 1, displayHeight + 1);
            lethargyMode = 4;
            lethargyColorCounter = 0x50;

            AddPrim(&primLists[whichDrawDispEnv].main, &lethargyEffectSprite[whichDrawDispEnv][0]);
            if (displayWidth > 256) {
                AddPrim(&primLists[whichDrawDispEnv].main, &lethargyEffectSprite[whichDrawDispEnv][1]);
            }
            break;
        case 4:
            lethargyColorCounter -= 8;
            if (lethargyColorCounter >= 0) {
                setRGB0(&lethargyEffectPoly[whichDrawDispEnv], lethargyColorCounter, lethargyColorCounter, lethargyColorCounter);
                AddPrim(&primLists[whichDrawDispEnv].main, &lethargyEffectPoly[whichDrawDispEnv]);
            } else {
                lethargyMode = 0;
            }
            break;
    }

    if (lethargyMode != 0) {
        lethargySinMagnitude -= VELOCITY(6);
        if (lethargySinMagnitude > 0) {
            lethargyAngleCounter = (lethargyAngleCounter + VELOCITY(0x100)) % 0x1000;
            delta = rsin(lethargyAngleCounter) * lethargySinMagnitude >> 12;
            gteXScale = 0x1000 + delta;
            gteYScale = 16 * SCREEN_HEIGHT - delta;
        }
    }
}

void SetRenderScreenFade(int speed, int enableHalfFps) {
    screenFadeEnabled = 1;
    halfFps = enableHalfFps;
    screenFadeSpeed = speed;

    switch (speed) {
        case 0:
            screenFadeColor = VID_NTSC ? 268 : 272;
            break;
        case 1:
            screenFadeColor = VID_NTSC ? 261 : 264;
            break;
        case 2:
            screenFadeColor = VID_NTSC ? 254 : 256;
            break;
    }
}

void DrawScreenFade(void) {
    if (screenFadeEnabled == 1) {
        switch (screenFadeSpeed) {
            case 0:
                screenFadeColor -= VELOCITY(24);
                break;
            case 1:
                screenFadeColor -= VELOCITY(16);
                break;
            case 2:
                screenFadeColor -= VELOCITY(8);
                break;
        }
        if (screenFadeColor >= 1) {
            setRGB0(&screenFadePolys[whichDrawDispEnv][cameraIndex].poly, screenFadeColor,
                    screenFadeColor, screenFadeColor);
            addPrim(&otag[whichDrawDispEnv][cameraIndex],
                    &screenFadePolys[whichDrawDispEnv][cameraIndex]);
        } else {
            screenFadeEnabled = 0;
            halfFps = 0;
        }
    }
}

void DrawTitleAndCopyrightSprites(void) {
    int color;

    color = (sunIntensityOnScreen * 30) / 100 + 0x80;
    setRGB0(&titleSprite[whichDrawDispEnv].sprt, color, color, color);
    addPrim(&primLists[whichDrawDispEnv].gui1, &titleSprite[whichDrawDispEnv]);
    if (drawCopyright == 1) {
        addPrim(&primLists[whichDrawDispEnv].gui1, &copyrightSprite[whichDrawDispEnv]);
    }
}

int InitScoreGraph(void* pBuf, int* levelScores, int* unusedScorePtr, int numLevels, int maxScore, int x, int y, int graphWidth, int graphHeight, int isHighscore) {
    int i;
    int j;
    int val;
    int isOddSection;
    int elemWidth;
    int* pDst;

    drawScoreGraphHeightsBuf = pBuf;
    drawScoreGraphY = y;
    drawScoreGraphHeight = graphHeight;
    drawScoreGraphPrims = (ScoreGraph*)((char*)pBuf + 3600);
    drawScoreGraphLevelScores = levelScores;
    drawScoreGraphUnusedScorePtr = unusedScorePtr;
    drawScoreGraphWidth = graphWidth;

    if (maxScore == -1) {
        maxScore = 0;
        for (i = 0; i < (VER_JP ? numLevels : graphWidth); i++) {
            if (levelScores != (int*)-1 && levelScores[i] > maxScore) {
                maxScore = levelScores[i];
            }
            if (drawScoreGraphUnusedScorePtr != (int*)-1 && unusedScorePtr[i] > maxScore) {
                maxScore = unusedScorePtr[i];
            }
        }
    }

    if (maxScore < 1)
        maxScore = 1;

    for (j = 0; j < 2; j++) {
        SetDrawTPage(&drawScoreGraphPrims->tpages[j], 0, 0, GetTPage(0, 1, 0, 0));
        SetDrawTPage(&drawScoreGraphPrims->tpages2[j], 0, 0, GetTPage(0, 2, 0, 0));
    }

    elemWidth = graphWidth / (numLevels - 1);
    if (elemWidth < 1)
        elemWidth = 1;

    if (drawScoreGraphLevelScores != (int*)-1) {
        pDst = drawScoreGraphHeightsBuf;
        for (i = 0; i < graphWidth; i++) {
            int section = (i * 10) / graphWidth;
            isOddSection = section % 2;
            val = drawScoreGraphLevelScores[(i * (numLevels - 1)) / graphWidth] +
                ((drawScoreGraphLevelScores[(i * (numLevels - 1)) / graphWidth + 1] -
                  drawScoreGraphLevelScores[(i * (numLevels - 1)) / graphWidth]) *
                 (i % elemWidth)) /
                    elemWidth;
            if (val < 0)
                val = 0;

            val = (val * (graphHeight << 1)) / maxScore;
            if (val & 1)
                val += 2;

            val >>= 1;

            *pDst++ = (y + graphHeight) << 12;
            *pDst++ = ((-val) << 12) / (FPS / 2);
            *pDst++ = (y + graphHeight - val) << 12;

            for (j = 0; j < 2; j++) {
                drawScoreGraphPrims->lines[j][i * 11 + 0] = 0x0a000000;
                drawScoreGraphPrims->lines[j][i * 11 + 2] = x + i;
                drawScoreGraphPrims->lines[j][i * 11 + 4] = x + i + 8;
                drawScoreGraphPrims->lines[j][i * 11 + 5] = !isOddSection ? 0x50802080 : 0x506c1b6c;
                drawScoreGraphPrims->lines[j][i * 11 + 6] = (y + graphHeight) << 16 | (x + i);
                drawScoreGraphPrims->lines[j][i * 11 + 7] = !isOddSection ? 0x00b02020 : 0x00951b1b;
                drawScoreGraphPrims->lines[j][i * 11 + 8] = x + i;
                drawScoreGraphPrims->lines[j][i * 11 + 10] = x + i;

                *(u_long*)&(drawScoreGraphPrims->polys[j] + i)->r2 =
                    !isOddSection ? 0x00802020 : 0x006c1b1b;
                *(u_long*)&(drawScoreGraphPrims->polys[j] + i)->r3 =
                    !isOddSection ? 0x00802020 : 0x006c1b1b;

                (drawScoreGraphPrims->polys[j] + i)->x0 = x + i;
                (drawScoreGraphPrims->polys[j] + i)->y0 = y + graphHeight + (VER_JP ? 2 : 1);
                (drawScoreGraphPrims->polys[j] + i)->x1 = x + i + 9;
                (drawScoreGraphPrims->polys[j] + i)->y1 = y + graphHeight - (VER_JP ? 6 : 7);
                (drawScoreGraphPrims->polys[j] + i)->x2 = x + i;
                (drawScoreGraphPrims->polys[j] + i)->y2 = 0;
                (drawScoreGraphPrims->polys[j] + i)->x3 = x + i + 9;
                (drawScoreGraphPrims->polys[j] + i)->y3 = 0;

                SetPolyG4(drawScoreGraphPrims->polys[j] + i);
                SetSemiTrans(drawScoreGraphPrims->polys[j] + i, 0);
            }
        }
    }

    if (drawScoreGraphUnusedScorePtr != (int*)-1) {
        for (i = 0; i < graphWidth; i++) {
            val = drawScoreGraphUnusedScorePtr[i];
            if (val < 0)
                val = 0;
            val = (val * (graphHeight << 1)) / maxScore;
            if (val & 1)
                val += 2;
            val >>= 1;
            for (j = 0; j < 2; j++) {
                drawScoreGraphPrims->unusedPolys[j][i].x0 = x + i * 2 - 8;
                drawScoreGraphPrims->unusedPolys[j][i].x1 = x + i * 2 - 6;
                drawScoreGraphPrims->unusedPolys[j][i].x2 = x + i * 2;
                drawScoreGraphPrims->unusedPolys[j][i].x3 = x + i * 2 + 2;

                drawScoreGraphPrims->unusedPolys[j][i].y1 = y + graphHeight - val + 8;
                drawScoreGraphPrims->unusedPolys[j][i].y3 = y + graphHeight - val;
                drawScoreGraphPrims->unusedPolys[j][i + 1].y0 = y + graphHeight - val + 8;
                drawScoreGraphPrims->unusedPolys[j][i + 1].y2 = y + graphHeight - val;

                if (drawScoreGraphPrims->unusedPolys[0][i].y0 >=
                    drawScoreGraphPrims->unusedPolys[0][i].y1 + 2) {
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r0 = 0x404040;
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r1 = 0x404040;
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r2 = 0;
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r3 = 0;
                } else {
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r0 = 0xa0a0a0;
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r1 = 0xa0a0a0;
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r2 = 0;
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r3 = 0;
                }

                SetPolyG4(&drawScoreGraphPrims->unusedPolys[j][i]);
                SetSemiTrans(&drawScoreGraphPrims->unusedPolys[j][i], 1);
            }
        }
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            switch (i) {
                case 0:
                    if (isHighscore == 0) {
                        (drawScoreGraphPrims->polys3[j] + i)->x0 = x;
                        (drawScoreGraphPrims->polys3[j] + i)->y0 = y + graphHeight + 1;
                        (drawScoreGraphPrims->polys3[j] + i)->x1 = x + graphWidth + 1;
                        (drawScoreGraphPrims->polys3[j] + i)->y1 = y + graphHeight + 1;
                        (drawScoreGraphPrims->polys3[j] + i)->x2 = x + 8;
                        (drawScoreGraphPrims->polys3[j] + i)->y2 = y + graphHeight - 8;
                        (drawScoreGraphPrims->polys3[j] + i)->x3 = x + graphWidth + 9;
                        (drawScoreGraphPrims->polys3[j] + i)->y3 = y + graphHeight - 8;
                    } else {
                        (drawScoreGraphPrims->polys3[j] + i)->x0 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y0 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->x1 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y1 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->x2 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y2 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->x3 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y3 = 0;
                    }
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r0 = 0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r1 = 0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r2 = 0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r3 = 0;
                    SetPolyG4(drawScoreGraphPrims->polys3[j] + i);
                    SetSemiTrans(drawScoreGraphPrims->polys3[j] + i, 0);
                    break;
                case 1:
                    if (isHighscore == 0) {
                        (drawScoreGraphPrims->polys3[j] + i)->x0 = x + 8;
                        (drawScoreGraphPrims->polys3[j] + i)->y0 = y + graphHeight - 7;
                        (drawScoreGraphPrims->polys3[j] + i)->x1 = x + graphWidth + 9;
                        (drawScoreGraphPrims->polys3[j] + i)->y1 = y + graphHeight - 7;
                        (drawScoreGraphPrims->polys3[j] + i)->x2 = x + 8;
                        (drawScoreGraphPrims->polys3[j] + i)->y2 = y - 8;
                        (drawScoreGraphPrims->polys3[j] + i)->x3 = x + graphWidth + 9;
                        (drawScoreGraphPrims->polys3[j] + i)->y3 = y - 8;
                    } else {
                        (drawScoreGraphPrims->polys3[j] + i)->x0 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y0 = 256;
                        (drawScoreGraphPrims->polys3[j] + i)->x1 = 320;
                        (drawScoreGraphPrims->polys3[j] + i)->y1 = 256;
                        (drawScoreGraphPrims->polys3[j] + i)->x2 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y2 = 128;
                        (drawScoreGraphPrims->polys3[j] + i)->x3 = 320;
                        (drawScoreGraphPrims->polys3[j] + i)->y3 = 128;
                    }
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r0 = 0x00d0d0d0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r1 = 0x00d0d0d0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r2 = 0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r3 = 0;
                    SetPolyG4(drawScoreGraphPrims->polys3[j] + i);
                    SetSemiTrans(drawScoreGraphPrims->polys3[j] + i, 1);
                    break;
                case 2:
                    if (isHighscore == 0) {
                        (drawScoreGraphPrims->polys3[j] + i)->x0 = x;
                        (drawScoreGraphPrims->polys3[j] + i)->y0 = y + graphHeight + 1;
                        (drawScoreGraphPrims->polys3[j] + i)->x1 = x + 9;
                        (drawScoreGraphPrims->polys3[j] + i)->y1 = y + graphHeight - 7;
                        (drawScoreGraphPrims->polys3[j] + i)->x2 = x;
                        (drawScoreGraphPrims->polys3[j] + i)->y2 = y;
                        (drawScoreGraphPrims->polys3[j] + i)->x3 = x + 9;
                        (drawScoreGraphPrims->polys3[j] + i)->y3 = y - 8;
                    } else {
                        (drawScoreGraphPrims->polys3[j] + i)->x0 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y0 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->x1 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y1 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->x2 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y2 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->x3 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y3 = 0;
                    }
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r0 = 0x00d0d0d0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r1 = 0x00d0d0d0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r2 = 0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r3 = 0;
                    SetPolyG4(drawScoreGraphPrims->polys3[j] + i);
                    SetSemiTrans(drawScoreGraphPrims->polys3[j] + i, 1);
                    break;
            }
        }
    }

    if (drawScoreGraphLevelScores != (int*)-1) {
        doDrawGraph = 1;
    } else {
        doDrawGraph = 0;
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            TSpritePrim(&scoreGraphSprites[i][j], 0, 0,
                        textures[firstGuiTexture + 20 + j].tpage);

            if (isHighscore == 0) {
                if (j == 0) {
                    setRGB0(&scoreGraphSprites[i][j].sprt, 64, 64, 64);
                } else {
                    setRGB0(&scoreGraphSprites[i][j].sprt, 72, 72, 72);
                }
            } else {
                setRGB0(&scoreGraphSprites[i][j].sprt, 72, 72, 72);
            }

            if (j == 0) {
                setXY0(&scoreGraphSprites[i][0].sprt, x + (VER_JP ? 0 : 2), y + 1);
            } else {
                setXY0(&scoreGraphSprites[i][1].sprt, x + graphWidth - (VER_JP ? 46 : 50), y + graphHeight - (VER_JP ? 7 : 6));
            }

            SetSemiTrans(&scoreGraphSprites[i][j].sprt,
                         textures[firstGuiTexture + 20 + j].semitrans);
            SetShadeTex(&scoreGraphSprites[i][j].sprt, 0);

            scoreGraphSprites[i][j].sprt.clut = textures[firstGuiTexture + 20 + j].clut;
            scoreGraphSprites[i][j].sprt.w = textures[firstGuiTexture + 20 + j].w;
            scoreGraphSprites[i][j].sprt.h = textures[firstGuiTexture + 20 + j].h;
            setUV0(&scoreGraphSprites[i][j].sprt, textures[firstGuiTexture + 20 + j].u,
                   textures[firstGuiTexture + 20 + j].v);
        }
    }
    return 3600 + sizeof(ScoreGraph);
}

void DrawScoreGraph(void) {
    int i;
    int heightVal;
    int isVisible;
    int scale;
    int unk1;
    int unk2;
    int disableDraw;
    int y;
    int *hbuf;
    short *l0;
    if (doDrawGraph == 1) {
        disableDraw = 1;
        hbuf = drawScoreGraphHeightsBuf;
        for (i = 0; i < drawScoreGraphWidth; hbuf += 3, i++) {
            y = hbuf[0];
            y += hbuf[1];
            if (hbuf[2] > y) {
                y = hbuf[2];
                hbuf[0] = y;
                hbuf[1] = 0;
            } else {
                disableDraw = 0;
                hbuf[0] = y;
            }

            y /= 4096;

            l0 = (short *)&drawScoreGraphPrims->lines[0][i * 11];
            l0[5] = y - 1;
            l0[9] = y - 9;
            l0[17] = y;
            l0[21] = y;

            l0 = (short *)&drawScoreGraphPrims->lines[1][i * 11];
            l0[5] = y - 1;
            l0[9] = y - 9;
            l0[17] = y;
            l0[21] = y;

            drawScoreGraphPrims->polys[0][i].y2 = y - 1;
            drawScoreGraphPrims->polys[0][i].y3 = y - 9;
            drawScoreGraphPrims->polys[1][i].y2 = y - 1;
            drawScoreGraphPrims->polys[1][i].y3 = y - 9;

            if (i > 0) {
                drawScoreGraphPrims->polys[0][i - 1].y0 = y - 1;
                drawScoreGraphPrims->polys[0][i - 1].y1 = y - 9;
                drawScoreGraphPrims->polys[1][i - 1].y0 = y - 1;
                drawScoreGraphPrims->polys[1][i - 1].y1 = y - 9;
            }
        }

        if (disableDraw == 1) {
            doDrawGraph = 0;
        }
    }

    addPrim(&primLists[whichDrawDispEnv].gui1, &scoreGraphSprites[whichDrawDispEnv][0]);
    addPrim(&primLists[whichDrawDispEnv].gui1, &scoreGraphSprites[whichDrawDispEnv][1]);

    if (drawScoreGraphUnusedScorePtr != (int *)-1) {
        for (i = drawScoreGraphWidth - 1; i > 0; i--) {
            addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->unusedPolys[whichDrawDispEnv][i]);
        }
        addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->tpages[whichDrawDispEnv]);
    }

    if (drawScoreGraphLevelScores != (int *)-1) {
        for (i = drawScoreGraphWidth - 1; 0 <= i; i--) {
            heightVal = ((i * 10) / drawScoreGraphWidth) % 2;

            if (((short *)&drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11])[21] < drawScoreGraphY + drawScoreGraphHeight) {
                addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11]);
            }

            isVisible = 0;
            if (i == drawScoreGraphWidth - 1) {
                if (((short *)&drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11])[21] < drawScoreGraphY + drawScoreGraphHeight) {
                    addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->polys[whichDrawDispEnv][i]);
                    isVisible = 1;

                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 1] = !heightVal ? 0x50e03838 : 0x50be2f2f;
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 3] = !heightVal ? 0x00801010 : 0x006c0d0d;
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 9] = !heightVal ? 0x68e03838 : 0x68be2f2f;
                }
            } else {
                if (drawScoreGraphPrims->polys[whichDrawDispEnv][i].y2 < drawScoreGraphPrims->polys[whichDrawDispEnv][i + 1].y2) {
                    addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->polys[whichDrawDispEnv][i]);
                    isVisible = 1;
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 1] = !heightVal ? 0x50e03838 : 0x50be2f2f;
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 3] = !heightVal ? 0x00801010 : 0x006c0d0d;
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 9] = !heightVal ? 0x68e03838 : 0x68be2f2f;
                } else {
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 1] = !heightVal ? 0x50f05050 : 0x50cc4444;
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 3] = !heightVal ? 0x00a02020 : 0x00881b1b;
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 9] = !heightVal ? 0x68f87070 : 0x68d25f5f;
                }
            }

            if (isVisible == 1) {
                scale = (drawScoreGraphPrims->polys[whichDrawDispEnv][i].y0 - drawScoreGraphPrims->polys[whichDrawDispEnv][i].y2) * 4096 / ((drawScoreGraphY + drawScoreGraphHeight) - drawScoreGraphPrims->polys[whichDrawDispEnv][i].y2);

                unk1 = (scale * 64) >> 12;
                unk2 = -(scale * 32) >> 12;
                if (heightVal == 1) {
                    unk1 = (unk1 * 85) / 100;
                    unk2 = (unk2 * 85) / 100;
                }
                unk1 += 32;
                unk2 += 128;
                *(u_long *)&drawScoreGraphPrims->polys[whichDrawDispEnv][i].r0 = (unk2 << 16) | (unk1 | 0x38002000);

                unk1 = (scale * 64) >> 12;
                unk2 = -(scale * 32) >> 12;
                if (heightVal == 1) {
                    unk1 = (unk1 * 85) / 100;
                    unk2 = (unk2 * 85) / 100;
                }
                unk1 += 32;
                unk2 += 128;
                *(u_long *)&drawScoreGraphPrims->polys[whichDrawDispEnv][i].r1 = (unk2 << 16) | (unk1 | 0x2000);
            }
        }
    }
    addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->polys3[whichDrawDispEnv][0]);
    addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->polys3[whichDrawDispEnv][1]);
    addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->polys3[whichDrawDispEnv][2]);
    addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->tpages2[whichDrawDispEnv]);
}

void DrawBonusWidget(void) {
    int i;
    int div8;
    int mod8;
    uint* p;
    uint* p2;

    bonusWidgetDataPtr++;
    if (bonusWidgetDataPtr >= bonusWidgetDataEnd) {
        bonusWidgetDataPtr -= BONUS_WIDGET_COLOR_DATA_LEN;
    }

    p = bonusWidgetDataPtr;
    for (i = 0; i < numCubesRemainingInLevel[0] >> 3; i++) {
        *(Col*)&bonusWidgetSpritesMsbForeground[whichDrawDispEnv][i].sprt.r0 = *(Col*)p;
        p += 5;
        if (p >= bonusWidgetDataEnd) {
            p -= BONUS_WIDGET_COLOR_DATA_LEN;
        }
    }

    bonusWidgetDataPtr2++;
    if (bonusWidgetDataPtr2 >= bonusWidgetDataEnd) {
        bonusWidgetDataPtr2 -= BONUS_WIDGET_COLOR_DATA_LEN;
    }

    p2 = bonusWidgetDataPtr2;
    for (i = 0; i < 8; i++) {
        *(Col*)&bonusWidgetSpritesPiechart[whichDrawDispEnv][i].sprt.r0 = *(Col*)p2;
        p2 -= 12;
        if (p2 < BONUS_WIDGET_COLOR_DATA) {
            p2 += BONUS_WIDGET_COLOR_DATA_LEN;
        }
    }

    mod8 = numCubesRemainingInLevel[0] & 7;
    div8 = numCubesRemainingInLevel[0] >> 3;

    if (mod8 == 0 && div8 > 0) {
        mod8 = 8;
        div8--;
    }

    for (i = 0; i < div8; i++) {
        addPrim(&primLists[whichDrawDispEnv].gui1, &bonusWidgetSpritesMsbForeground[whichDrawDispEnv][i].tpage);
    }

    for (i = 0; i < numBonusWidgetMsbs + 2; i++) {
        addPrim(&primLists[whichDrawDispEnv].gui1, &bonusWidgetSpritesMsbBackground[whichDrawDispEnv][i].tpage);
    }

    for (i = 0; i < mod8; i++) {
        addPrim(&primLists[whichDrawDispEnv].gui1, &bonusWidgetSpritesPiechart[whichDrawDispEnv][i].tpage);
    }

    addPrim(&primLists[whichDrawDispEnv].gui1, &bonusWidgetSpritesPiechart[whichDrawDispEnv][8].tpage);
}

void DisableScreenShake(void) {
    screenShakeEnabled = 0;
}

void EnableScreenShake(int param_1, int param_2, int param_3) {
    screenShakeMagnitude = param_1;
    screenShakeTimer = param_2;
    screenShakeDivisor = param_2;
    screenShakeDirectionMask = param_3;
    screenShakeEnabled = 1;
}

void ProcessScreenShake(void) {
    int rng;
    int v;

    if (screenShakeEnabled == 1) {
        screenShakeTimer--;
        if (screenShakeTimer < 0) {
            screenShakeEnabled = 0;
            SetDispScreen(dispenvScreenX, dispenvScreenY);
        } else {
            if (screenShakeDirectionMask & 1) {
                rng = Rand(screenShakeMagnitude << 1);
                v = dispenvScreenX + ((rng - screenShakeMagnitude) * screenShakeTimer) / screenShakeDivisor;
                if (v < 0) {
                    v = 0;
                }
                drawdisp[whichDrawDispEnv].disp.screen.x = v;
            }
            if (screenShakeDirectionMask & 2) {
                rng = Rand(screenShakeMagnitude << 1);
                v = dispenvScreenY + ((rng - screenShakeMagnitude) * screenShakeTimer) / screenShakeDivisor;
                if (v < 0) {
                    v = 0;
                }
                drawdisp[whichDrawDispEnv].disp.screen.y = v;
            }
        }
    }
}

#ifndef SKIP_UNUSED_CODE
void SetupBiosFontStr(ushort* str, int x, int y, int param_4, int fg, int bg, int param_7, Tim* tim) {
    RECT rect;
    int i;
    int j;
    int end;
    int idx;
    short* ptr;

    end = 0;
    idx = 0;
    ptr = tim;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 16; j++) {
            if (str[idx] == 0) {
                end = 1;
            }
            if (!end) {
                *ptr++ = str[idx++];
            } else {
                *ptr++ = 0;
            }
        }
        *ptr++ = 0;
    }
    ptr = tim;
    tim = (char*)tim + 0x44;
    biosStrLen = 0;

    for (i = 0; i < 2; i++) {
        Krom2Tim(&ptr[i * 17], tim, i * 64, 0xf0, 0x270, 0xf0, fg, bg);
        j = tim->imageLength - 12;
        if (j > 0) {
            biosStrLen += j / 128;
            for (j = 0; j < 16; j++) {
                if (tim->clutData[j]) {
                    tim->clutData[j] |= 0x8000;
                }
            }
            rect.x = tim->clutX;
            rect.y = tim->clutY;
            rect.w = tim->clutWidth;
            rect.h = tim->clutHeight;
            DrawSync(0);
            LoadImage(&rect, tim->clutData);
            DrawSync(0);

            rect.x = tim->imageX;
            rect.y = tim->imageY;
            rect.w = tim->imageWidth;
            rect.h = tim->imageHeight;
            LoadImage(&rect, tim + 1);
        }
        if (biosStrLen < 16)
            break;
    }

    for (i = 0; i < biosStrLen; i++) {
        TSpritePrim(&biosStrSprites[0][i], 0, 0, GetTPage(0, param_7 == -1 ? 0 : param_7, (i / 16) << 6, 0xf0));
        setRGB0(&biosStrSprites[0][i].sprt, 0x80, 0x80, 0x80);
        setXY0(&biosStrSprites[0][i].sprt,
            x + ((i * 16) % (param_4 << 4)),
            y + (i / param_4) * 16);
        SetSemiTrans(&biosStrSprites[0][i].sprt, param_7 != -1);
        SetShadeTex(&biosStrSprites[0][i].sprt, 0);
        biosStrSprites[0][i].sprt.clut = GetClut(0x270, 0xf0);
        biosStrSprites[0][i].sprt.w = 16;
        biosStrSprites[0][i].sprt.h = 16;
        setUV0(&biosStrSprites[0][i].sprt, (i * 16 % 256), 0xf0);

        *(biosStrSprites[1] + i) = biosStrSprites[0][i];
    }
}

void DrawBiosFontString(void) {
    int i;

    for (i = 0; i < biosStrLen; i++) {
        addPrim(&primLists[whichDrawDispEnv].gui1, &biosStrSprites[whichDrawDispEnv][i]);
    }
}
#endif

void* ParseGGI(int* ggi_ptr) {
    int* pOff;
    int i, j;

    ggi = ggi_ptr;

    pOff = ggi_ptr + GGI_PART0A_OFF;
    ggiPart0A = (void*)((int)pOff + pOff[0] / 4 * 4);
    ggiPart0B = (void*)((int)pOff + pOff[1] / 4 * 4);

    ggiPart1HourglassAnim = (void*)((int)pOff + ggi_ptr[GGI_PART0_LEN] * 2);
    ggiPart2DepthCueingLookup = (void*)((int)ggiPart1HourglassAnim + ggi_ptr[GGI_PART1_LEN] * 2);
    ggiPart3 = (void*)((int)ggiPart2DepthCueingLookup + ggi_ptr[GGI_PART2_LEN] * 2);
    ggiPart4Dummy = (void*)((int)ggiPart3 + ggi_ptr[GGI_PART3_LEN] * 2);
    ggiPart5JumpAnimData = (void*)((int)ggiPart4Dummy + ggi_ptr[GGI_PART4_LEN] * 2);
    ggiPart6Textures = (void*)((int)ggiPart5JumpAnimData + ggi_ptr[GGI_PART5_LEN] * 2);
    ggiPart7Unused = (void*)((int)ggiPart6Textures + ggi_ptr[GGI_PART6_LEN] * 2);

    ParseTextures(ggiPart6Textures, textures, NUM_TEXTURES);

    firstFruitTexture = 0;
    firstParticleTexture = ggi[GGI_NUM_FRUIT_TEXTURES];
    numParticleTextures = ggi[GGI_NUM_PARTICLE_TEXTURES];
    firstLensFlareOrPsxButtonTexture = firstParticleTexture + numParticleTextures;
    numUnk1Textures = ggi[GGI_NUM_UNK1_TEXTURES];
    firstGuiTextTexture = firstLensFlareOrPsxButtonTexture + numUnk1Textures;
    numGuiTextTextures = ggi[GGI_NUM_GUI_TEXT_TEXTURES];
    firstUnk2Texture = firstGuiTextTexture + numGuiTextTextures;
    numUnk2Textures = ggi[GGI_NUM_UNK2_TEXTURES];
    firstBonusWidgetTexture = firstUnk2Texture + numUnk2Textures;
    numBonusWidgetTextures = ggi[GGI_NUM_BONUS_WIDGET_TEXTURES];
    firstGuiTexture = firstBonusWidgetTexture + numBonusWidgetTextures;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            ParseGgiInner((int*)&gemRandomSparkleEffects[i][j], 1, 36, i, j, 3, 0x808080, 0xfefefe, 0x101010, 0x10);
        }
    }

#if VER_JP
    CustomBallInitAllModels();
#endif
#if !VER_EU
    InitPlayerSpecularSprite();
#endif

    return ggiPart6Textures;
}

void ParseTextures(int* headerPtr, Texture* out, int unused_count) {
    int count = *headerPtr;
    short* p;
    int i;
    int blendMode;
    int bpp;
    int alreadyLoaded;
    RECT rect;

    DrawSync(0);
    p = (short*)(headerPtr + 1);
    for (i = 0; i < count; i++) {
        bpp = *p++;
        if (bpp == -1) {
            // padding word
            p++;
            continue;
        }

        out[i].bitsPerPixel = bpp;
        blendMode = *p++;
        out[i].semitrans = (blendMode != -1);
        if (blendMode == -1)
            blendMode = 0;

        alreadyLoaded = p[3];

        if (bpp != 16) {
            rect.x = *p++;
            rect.y = *p++;
            out[i].clut = GetClut(rect.x, rect.y);
            if (p[0] == 0) {
                rect.w = 1 << bpp;
                rect.h = 1;
                LoadImage(&rect, p + 2);
                DrawSync(0);
                p += rect.w + 2;
            } else {
                // CLUT already loaded by some other texture
                p += 2;
            }
        } else {
            p += 4;
        }

        rect.x = *p++;
        rect.y = *p++;
        out[i].w = *p++;
        rect.w = (out[i].w * bpp) / 16;
        out[i].h = *p++;
        rect.h = out[i].h;

        if (alreadyLoaded == 0) {
            LoadImage(&rect, p);
            DrawSync(0);
            p += rect.w * rect.h;
            if ((rect.w * rect.h) & 1) {
                p++;
            }
        }

        out[i].tpage = GetTPage(bpp >> 3, blendMode, rect.x, rect.y);
        out[i].u = (rect.x % 64) * (16 / bpp);
        out[i].v = rect.y % 256;
    }
}

void ParseGgiInner(int* eff, int modelType, int modelIdx, int lodIdx, int variantIdx,
                   int probability, int addRgb, int maxRgb, int subRgb, int numSparkles) {
    int* p;
    int k;

    // Calculate pointer to ModelData
    if (modelType == 0) {
        p = (int*)((char*)ggiPart0A + (*(int*)((char*)ggiPart0A + modelIdx * 16 + lodIdx * 4) / 4) * 4);
    } else {
        p = (int*)((char*)ggiPart0B + (*(int*)((char*)ggiPart0B + modelIdx * 64 + lodIdx * 16 + variantIdx * 4) / 4) * 4);
    }

    // Advance to VertexColors structure
    p = (int*)((char*)p + (p[5] / 4) * 4);

    eff[0] = (uint*)((char*)p + 8); // pointer to colors array
    eff[1] = p[1] / 16;             // number of vertex colors
    eff[2] = probability;
    eff[3] = addRgb;
    eff[4] = maxRgb;
    eff[5] = subRgb;
    eff[6] = numSparkles;
    eff += 7;

    for (k = 0; k < numSparkles; k++, eff += 5) {
        eff[0] = 0;
    }
}
