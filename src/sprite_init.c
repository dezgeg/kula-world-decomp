#include "common.h"

typedef struct DigitSprites {
    TSprite sprites[2][10];
} DigitSprites;

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

extern POLY_FT4 hourglassSprites[2][3];
extern DigitSprites levelTimeLeftDigitSprites;
extern Texture textures[150];

uint firstGuiTexture;
short* ggiPart1HourglassAnim;
static RECT hourglassClutRect;
int hourglassIsRotating;
int hourglassRotationTimer;
int levelTimeLeft;

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
                ggiPart1HourglassAnim[320*i + 0] + 160, ggiPart1HourglassAnim[320 * i + 1] + 26,
                ggiPart1HourglassAnim[320*i + 2] + 160, ggiPart1HourglassAnim[320 * i + 3] + 26,
                ggiPart1HourglassAnim[320*i + 4] + 160, ggiPart1HourglassAnim[320 * i + 5] + 26,
                ggiPart1HourglassAnim[320*i + 6] + 160, ggiPart1HourglassAnim[320 * i + 7] + 26);
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
