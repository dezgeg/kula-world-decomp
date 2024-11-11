#include "common.h"

typedef struct DigitSprites {
    TSprite sprites[2][10];
} DigitSprites;

typedef struct FakeTgiFile {
    char pad[4];
    int levelBgG;
} FakeTgiFile;

extern void InitDigitSprites(DigitSprites* ds, int font, int x, int y, int r, int g, int b);
extern void InitAllDigitSprites(void);
extern void InitBonusWidgetSprites(void);
extern void InitCopyrightSprite(void);
extern void InitFruitSprites(void);
extern void InitHourglassSprites(void);
extern void InitKeySprites(void);
extern void InitLethargyEffectSprites(void);
extern void InitScreenFadePolys(void);
extern void InitTimerPausedSprite(void);
extern void InitTitleSprite(void);

extern DigitSprites levelTimeLeftDigitSprites;
extern int displayHeight;
extern int displayWidth;
extern int screenOffsetX;
extern int screenOffsetY;
extern int specialLevelType;
extern POLY_FT4 hourglassSprites[2][3];
extern POLY_FT4 lethargyEffectPoly[2];
extern Texture textures[150];
extern FakeTgiFile* tgi;
extern TPolyF4 screenFadePolys[2][1];
extern TSprite copyrightSprite[2];
extern TSprite keySprites[2][8];
extern TSprite lethargyEffectSprite[2][2];
extern TSprite timerPausedSprite[2];
extern TSprite titleSprite[2];

int drawTimerPausedWidget;
int halfFps;
int hourglassIsRotating;
int hourglassRotationTimer;
int lethargyMode;
int levelTimeLeft;
int screenFadeColor;
int screenFadeEnabled;
int screenFadeSpeed;
short* ggiPart1HourglassAnim;
static RECT hourglassClutRect;
uint firstGuiTexture;

void InitManySprites(void) {
    InitHourglassSprites();
    InitLethargyEffectSprites();
    InitKeySprites();
    InitBonusWidgetSprites();
    InitFruitSprites();
    InitScreenFadePolys();
    InitTitleSprite();
    InitCopyrightSprite();
    InitTimerPausedSprite();
    InitAllDigitSprites();
}

void InitHourglassSprites(void) {
    int i;
    int w;
    int h;

    for (i = 0; i < 3; i++) {
        setPolyFT4(&hourglassSprites[0][i]);
        w = textures[firstGuiTexture + i].w - 1;
        h = textures[firstGuiTexture + i].h - 1;
        setUV4(&hourglassSprites[0][i],
                textures[firstGuiTexture + i].u, textures[firstGuiTexture + i].v,
                textures[firstGuiTexture + i].u + w, textures[firstGuiTexture + i].v,
                textures[firstGuiTexture + i].u, textures[firstGuiTexture + i].v + h,
                textures[firstGuiTexture + i].u + w, textures[firstGuiTexture + i].v + h);
        setXY4(&hourglassSprites[0][i],
                ggiPart1HourglassAnim[320 * i + 0] + 160, ggiPart1HourglassAnim[320 * i + 1] + 26,
                ggiPart1HourglassAnim[320 * i + 2] + 160, ggiPart1HourglassAnim[320 * i + 3] + 26,
                ggiPart1HourglassAnim[320 * i + 4] + 160, ggiPart1HourglassAnim[320 * i + 5] + 26,
                ggiPart1HourglassAnim[320 * i + 6] + 160, ggiPart1HourglassAnim[320 * i + 7] + 26);
        setRGB0(&hourglassSprites[0][i], 0x80, 0x80, 0x80);
        SetSemiTrans(&hourglassSprites[0][i], textures[firstGuiTexture + i].semitrans);
        SetShadeTex(&hourglassSprites[0][i], 0);
        hourglassSprites[0][i].tpage = textures[firstGuiTexture + i].tpage;
        hourglassSprites[0][i].clut = textures[firstGuiTexture + i].clut;

        hourglassSprites[1][i] = hourglassSprites[0][i];
    }
    hourglassIsRotating = 0;
    hourglassRotationTimer = 0;
    levelTimeLeft = 4999;
    hourglassClutRect.x = (textures[firstGuiTexture + 1].clut << 4) & 0x3ff;
    hourglassClutRect.y = textures[firstGuiTexture + 1].clut >> 6;
    InitDigitSprites(&levelTimeLeftDigitSprites, 0, 152, 50, 0x80, 0x80, 0x80);
}

void InitDigitSprites(DigitSprites* ds, int font, int x, int y, int r, int g, int b) {
    int textureIdx;
    int tmp;
    int w;
    int i;

    tmp = textures[firstGuiTexture + 3 + font].w / 13;
    if (font == 0) {
        w = tmp;
    } else {
        w = tmp - 4;
    }

    for (i = 0; i < 10; i++) {
        textureIdx = font + 3;
        TSpritePrim(&ds->sprites[0][i], 0, 0, textures[firstGuiTexture + textureIdx].tpage);
        setRGB0(&ds->sprites[0][i].sprt, r, g, b);
        setXY0(&ds->sprites[0][i].sprt, x + i * w, y);
        SetSemiTrans(&ds->sprites[0][i].sprt, textures[firstGuiTexture + textureIdx].semitrans);
        SetShadeTex(&ds->sprites[0][i].sprt, 0);
        ds->sprites[0][i].sprt.clut = textures[firstGuiTexture + textureIdx].clut;
        setWH(&ds->sprites[0][i].sprt,
                textures[firstGuiTexture + textureIdx].w / 13,
                textures[firstGuiTexture + textureIdx].h);
        ds->sprites[1][i] = ds->sprites[0][i];
    }
}

void InitKeySprites(void) {
    int i;
    int textureIdx;

    for (i = 0; i < 8; i++) {
        textureIdx = i % 2 + 7;
        TSpritePrim(&keySprites[0][i], 0, 0, textures[firstGuiTexture + textureIdx].tpage);
        setRGB0(&keySprites[0][i].sprt, 0x80, 0x80, 0x80);
        SetSemiTrans(&keySprites[0][i].sprt, textures[firstGuiTexture + textureIdx].semitrans);
        SetShadeTex(&keySprites[0][i].sprt, 0);
        keySprites[0][i].sprt.clut = textures[firstGuiTexture + textureIdx].clut;
        keySprites[0][i].sprt.w = textures[firstGuiTexture + textureIdx].w;
        keySprites[0][i].sprt.h = textures[firstGuiTexture + textureIdx].h;
        setUV0(&keySprites[0][i].sprt,
                textures[firstGuiTexture + textureIdx].u,
                textures[firstGuiTexture + textureIdx].v);

        keySprites[1][i] = keySprites[0][i];
    }
}

INCLUDE_ASM("asm/nonmatchings/sprite_init", InitFruitSprites);

void InitLethargyEffectSprites(void) {
    int i;

    lethargyMode = 0;
    for (i = 0; i < 2; i++) {
        setPolyFT4(&lethargyEffectPoly[i]);
        setUV4(&lethargyEffectPoly[i],
            textures[firstGuiTexture + 6].u, textures[firstGuiTexture + 6].v,
            textures[firstGuiTexture + 6].u + textures[firstGuiTexture + 6].w - 1, textures[firstGuiTexture + 6].v,
            textures[firstGuiTexture + 6].u, textures[firstGuiTexture + 6].v + textures[firstGuiTexture + 6].h - 1,
            textures[firstGuiTexture + 6].u + textures[firstGuiTexture + 6].w - 1, textures[firstGuiTexture + 6].v + textures[firstGuiTexture + 6].h - 1);

        setRGB0(&lethargyEffectPoly[i], 0x80, 0x80, 0x80);
        SetSemiTrans(&lethargyEffectPoly[i], textures[firstGuiTexture + 6].semitrans);
        SetShadeTex(&lethargyEffectPoly[i], 0);
        lethargyEffectPoly[i].tpage = textures[firstGuiTexture + 6].tpage;
        lethargyEffectPoly[i].clut = textures[firstGuiTexture + 6].clut;

        TSpritePrim(&lethargyEffectSprite[i][0], 0, tgi->levelBgG, GetTPage(2,0,i == 1 ? 0 : displayWidth,0));
        setXY0(&lethargyEffectSprite[i][0].sprt, 0, 0);
        setWH(&lethargyEffectSprite[i][0].sprt, 256, displayHeight);
        setUV0(&lethargyEffectSprite[i][0].sprt, 0, 0);
        SetSemiTrans(&lethargyEffectSprite[i][0].sprt,1);
        SetShadeTex(&lethargyEffectSprite[i][0].sprt,0);
        if (displayWidth > 256) {
            TSpritePrim(&lethargyEffectSprite[i][1], 0,tgi->levelBgG, GetTPage(2,0,i == 1 ? 256 : 256 + displayWidth,0));
            setXY0(&lethargyEffectSprite[i][1].sprt, 256, 0);
            setWH(&lethargyEffectSprite[i][1].sprt, displayWidth - 256, displayHeight);
            setUV0(&lethargyEffectSprite[i][1].sprt, 0, 0);
            SetSemiTrans(&lethargyEffectSprite[i][1].sprt,1);
            SetShadeTex(&lethargyEffectSprite[i][1].sprt,0);
        }
    }
}

void InitScreenFadePolys(void) {
    int i;
    int j;

    screenFadeEnabled = 0;
    halfFps = 0;
    if (specialLevelType == 1) {
        screenFadeEnabled = 1;
        halfFps = 0;
        screenFadeSpeed = 2;
        screenFadeColor = 0x100;
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 1; j++) {
            TPolyF4Prim(&screenFadePolys[i][j], 0, 0, GetTPage(0, 1, 0, 0));
            setXY4(&screenFadePolys[i][j].poly,
                    0, 0,
                    screenOffsetX + 1, 0,
                    0, screenOffsetY + 1,
                    screenOffsetX + 1, screenOffsetY + 1);
            SetSemiTrans(&screenFadePolys[i][j].poly, 1);
        }
    }
}

void InitTitleSprite(void) {
    int i;
    for (i = 0; i < 2; i++) {
        TSpritePrim(&titleSprite[i], 0, 0, textures[firstGuiTexture + 16].tpage);
        setRGB0(&titleSprite[i].sprt, 128, 128, 128);
        setXY0(&titleSprite[i].sprt, 38, 25);
        SetSemiTrans(&titleSprite[i].sprt, textures[firstGuiTexture + 16].semitrans);
        SetShadeTex(&titleSprite[i].sprt, 0);
        titleSprite[i].sprt.clut = textures[firstGuiTexture + 16].clut;
        titleSprite[i].sprt.u0 = textures[firstGuiTexture + 16].u;
        titleSprite[i].sprt.v0 = textures[firstGuiTexture + 16].v;
        titleSprite[i].sprt.w = textures[firstGuiTexture + 16].w;
        titleSprite[i].sprt.h = textures[firstGuiTexture + 16].h;
    }
}

void InitCopyrightSprite(void) {
    int i;
    for (i = 0; i < 2; i++) {
        TSpritePrim(&copyrightSprite[i], 0, 0, textures[firstGuiTexture + 17].tpage);
        setRGB0(&copyrightSprite[i].sprt, 128, 128, 128);
        setXY0(&copyrightSprite[i].sprt, 46, 218);
        SetSemiTrans(&copyrightSprite[i].sprt, textures[firstGuiTexture + 17].semitrans);
        SetShadeTex(&copyrightSprite[i].sprt, 0);
        copyrightSprite[i].sprt.clut = textures[firstGuiTexture + 17].clut;
        copyrightSprite[i].sprt.u0 = textures[firstGuiTexture + 17].u;
        copyrightSprite[i].sprt.v0 = textures[firstGuiTexture + 17].v;
        copyrightSprite[i].sprt.w = textures[firstGuiTexture + 17].w;
        copyrightSprite[i].sprt.h = textures[firstGuiTexture + 17].h;
    }
}

void InitTimerPausedSprite(void) {
    int i;

    drawTimerPausedWidget = 0;
    for (i = 0; i < 2; i++) {
        TSpritePrim(&timerPausedSprite[i], 0, 0, textures[firstGuiTexture + 19].tpage);
        setRGB0(&timerPausedSprite[i].sprt, 128, 128, 128);
        setXY0(&timerPausedSprite[i].sprt, 140, 6);
        SetSemiTrans(&timerPausedSprite[i].sprt, textures[firstGuiTexture + 19].semitrans);
        SetShadeTex(&timerPausedSprite[i].sprt, 0);
        timerPausedSprite[i].sprt.clut = textures[firstGuiTexture + 19].clut;
        timerPausedSprite[i].sprt.u0 = textures[firstGuiTexture + 19].u;
        timerPausedSprite[i].sprt.v0 = textures[firstGuiTexture + 19].v;
        timerPausedSprite[i].sprt.w = textures[firstGuiTexture + 19].w;
        timerPausedSprite[i].sprt.h = textures[firstGuiTexture + 19].h;
    }
}

INCLUDE_ASM("asm/nonmatchings/sprite_init", InitBonusWidgetSprites);

INCLUDE_ASM("asm/nonmatchings/sprite_init", InitAllDigitSprites);
