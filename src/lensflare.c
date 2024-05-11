#include "common.h"

typedef struct SPair {
    short first;
    short second;
} SPair;

typedef struct SinCos {
    short sin;
    short cos;
    short unk0;
    short pad;
} SinCos;

typedef struct FramebufferFetch {
    short x;
    short y;
    ushort texel;
} FramebufferFetch;

extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

int lensFlareScreenX;
int lensFlareScreenY;
int numLensFlareSprites;
int sunIntensityOnScreen;

extern SVECTOR SUN_POS_IN_SPECIAL_LEVEL[2];
extern SVECTOR SUN_POS_IN_WORLD[13];
extern short LENS_FLARE_COLOR_THRESHOLDS_IN_WORLD[13][3];
extern short LENS_FLARE_COLOR_THRESHOLDS_IN_SPECIAL_LEVEL[2][3];

extern FramebufferFetch framebufferFetches[16];
extern MATRIX perspMatrixes[1];
extern SPair lensFlareSpriteMidpoints[16];
extern SinCos sinCosArr23[/*16*/];
extern SinCos sinCosArr80[/*16*/];
extern TSprite lensFlareSprites[2][16];
extern Texture textures[150];
extern int cameraIndex;
extern int curWorld;
extern int displayHeight;
extern int displayWidth;
extern int firstLensFlareOrPsxButtonTexture;
extern int firstParticleTexture;
extern int gteXScale;
extern int gteYScale;
extern int screenOffsetX;
extern int screenOffsetY;
extern int specialLevelType;
extern int whichDrawDispEnv;
extern short lensFlarePositionMultipliers[16];
extern void* otag[2][1][1026];

void InitLensFlareSprite(short posMul, short textureIdx, int scaleHeight) {
    if (numLensFlareSprites < 16) {
        lensFlarePositionMultipliers[numLensFlareSprites] = posMul;
        TSpritePrim(&lensFlareSprites[0][numLensFlareSprites], 0, 0, textures[textureIdx].tpage);
        setUV0(&lensFlareSprites[0][numLensFlareSprites].sprt,
                textures[textureIdx].u,
                textures[textureIdx].v);
        setRGB0(&lensFlareSprites[0][numLensFlareSprites].sprt, 0x80, 0x80, 0x80);
        SetSemiTrans(&lensFlareSprites[0][numLensFlareSprites].sprt, textures[textureIdx].semitrans);
        SetShadeTex(&lensFlareSprites[0][numLensFlareSprites].sprt, 0);
        lensFlareSprites[0][numLensFlareSprites].sprt.clut = textures[textureIdx].clut;
        lensFlareSprites[0][numLensFlareSprites].sprt.w = textures[textureIdx].w;
        lensFlareSprites[0][numLensFlareSprites].sprt.h = textures[textureIdx].h;

        lensFlareSprites[1][numLensFlareSprites] = lensFlareSprites[0][numLensFlareSprites];

        lensFlareSpriteMidpoints[numLensFlareSprites].first = lensFlareSprites[0][numLensFlareSprites].sprt.w / 2;
        if (scaleHeight) {
            lensFlareSpriteMidpoints[numLensFlareSprites].second = (lensFlareSprites[0][numLensFlareSprites].sprt.h * 1.236) / 2.0;
        } else {
            lensFlareSpriteMidpoints[numLensFlareSprites].second = lensFlareSprites[0][numLensFlareSprites].sprt.h / 2;
        }
        numLensFlareSprites++;
    }
}

void InitLensFlareSprites(void) {
    int a;
    int i;
    int m23 = 23;
    int m80 = 80;

    numLensFlareSprites = 0;
    lensFlareScreenX = 0;
    lensFlareScreenY = 0;

    InitLensFlareSprite(-0x830, firstLensFlareOrPsxButtonTexture, 0);
    InitLensFlareSprite(0, firstParticleTexture, 0);
    InitLensFlareSprite(0x1000, firstLensFlareOrPsxButtonTexture, 0);
    InitLensFlareSprite(0x1db0, firstLensFlareOrPsxButtonTexture + 1, 0);
    InitLensFlareSprite(0x21ca, firstLensFlareOrPsxButtonTexture + 2, 0);
    InitLensFlareSprite(0x564, firstLensFlareOrPsxButtonTexture + 5, 0);
    InitLensFlareSprite(0x9ec, firstLensFlareOrPsxButtonTexture + 5, 0);
    InitLensFlareSprite(0xd70, firstLensFlareOrPsxButtonTexture + 5, 0);
    InitLensFlareSprite(0x1284, firstLensFlareOrPsxButtonTexture + 5, 0);
    InitLensFlareSprite(0x13c4, firstLensFlareOrPsxButtonTexture + 3, 1);
    InitLensFlareSprite(0x193c, firstLensFlareOrPsxButtonTexture + 3, 1);
    InitLensFlareSprite(0x1720, firstLensFlareOrPsxButtonTexture + 4, 1);
    InitLensFlareSprite(0x794, firstLensFlareOrPsxButtonTexture + 4, 1);
    InitLensFlareSprite(0xb18, firstLensFlareOrPsxButtonTexture + 4, 1);
    InitLensFlareSprite(0x1162, firstLensFlareOrPsxButtonTexture + 4, 1);
    InitLensFlareSprite(0x15a4, firstLensFlareOrPsxButtonTexture + 4, 1);

    sinCosArr23[0].sin = 0;
    sinCosArr23[0].cos = 0;
    sinCosArr23[0].unk0 = 0;
    sinCosArr80[0] = sinCosArr23[0];
    for (i = 1; i < 16; i++) {
        a = 300 + (i << 12) / 15;
        sinCosArr23[i].sin = (m23 * rsin(a)) >> 12;
        sinCosArr23[i].cos = (m23 * rcos(a)) >> 12;
        sinCosArr23[i].unk0 = 0;
        sinCosArr80[i].sin = (m80 * rsin(a)) >> 12;
        sinCosArr80[i].cos = (m80 * rcos(a)) >> 12;
        sinCosArr80[i].unk0 = 0;
    }
}

void DrawLensFlares(void) {
    MATRIX matrix;
    short screenCoords[10];
    long dummy;
    long sz;
    int i;
    int j;
    int sy;
    int sx;
    int halfWidth;
    int halfHeight;
    int texel;

    sunIntensityOnScreen = 0;
    halfWidth = displayWidth / 2;
    halfHeight = displayHeight / 2;

    matrix = perspMatrixes[cameraIndex];
    matrix.t[0] = matrix.t[0] - 0x2000 >> 4;
    matrix.t[1] = matrix.t[1] - 0x2000 >> 4;
    matrix.t[2] = matrix.t[2] - 0x2000 >> 4;
    for (i = 0; i < 3; i++) {
        matrix.m[0][i] = matrix.m[0][i] * gteXScale >> 12;
        matrix.m[1][i] = matrix.m[1][i] * gteYScale >> 12;
    }
    matrix.t[0] = matrix.t[0] * gteXScale >> 12;
    matrix.t[1] = matrix.t[1] * gteYScale >> 12;
    SetRotMatrix(&matrix);
    SetTransMatrix(&matrix);

    if (specialLevelType == 0) {
        sz = RotTransPers(&SUN_POS_IN_WORLD[curWorld], (long*)&screenCoords[0], (long*)&screenCoords[2], (long*)&screenCoords[2]);
        if (sz << 2 > 0) {
            sx = screenCoords[0];
            sy = screenCoords[1];
            for (i = 0; i < 16; i++) {
                framebufferFetches[i].x = sinCosArr23[i].sin + lensFlareScreenX;
                framebufferFetches[i].y = sinCosArr23[i].cos + lensFlareScreenY;
                if ((sx + sinCosArr80[i].sin & 0x7fff) < screenOffsetX &&
                    (sy + sinCosArr80[i].cos & 0x7fff) < screenOffsetY) {

                    texel = framebufferFetches[i].texel;
                    if ((texel & 0x7c00) >= LENS_FLARE_COLOR_THRESHOLDS_IN_WORLD[curWorld][0] &&
                        (texel & 0x3e0) >= LENS_FLARE_COLOR_THRESHOLDS_IN_WORLD[curWorld][1] &&
                        (texel & 0x1f) >= LENS_FLARE_COLOR_THRESHOLDS_IN_WORLD[curWorld][2]) {
                        sunIntensityOnScreen += 5;
                    }
                }
            }
            lensFlareScreenX = sx;
            lensFlareScreenY = sy;
            if (sunIntensityOnScreen > 0) {
                for (i = 0; i < numLensFlareSprites; i++) {
                    if (lensFlarePositionMultipliers[i] != 0) {
                        setRGB0(&lensFlareSprites[whichDrawDispEnv][i].sprt,
                                sunIntensityOnScreen, sunIntensityOnScreen, sunIntensityOnScreen);
                        lensFlareSprites[whichDrawDispEnv][i].sprt.x0 = (sx + (lensFlarePositionMultipliers[i] * (halfWidth - sx) >> 12)) - lensFlareSpriteMidpoints[i].first;
                        lensFlareSprites[whichDrawDispEnv][i].sprt.y0 = (sy + (lensFlarePositionMultipliers[i] * (halfHeight - sy) >> 12)) - lensFlareSpriteMidpoints[i].second;
                        addPrim(&otag[whichDrawDispEnv][cameraIndex][1], &lensFlareSprites[whichDrawDispEnv][i]);
                    }
                }
            }
        }
    }
    if (specialLevelType == 1) {
        for (j = 0; j < 2; j++) {
            if (sunIntensityOnScreen > 0) {
                return;
            }
            sz = RotTransPers(&SUN_POS_IN_SPECIAL_LEVEL[j], (long*)&screenCoords[0], (long*)&screenCoords[2], (long*)&screenCoords[2]);
            if (0 < sz << 2) {
                sx = screenCoords[0];
                sy = screenCoords[1];
                for (i = 0; i < 16; i++) {
                    framebufferFetches[i].x = sinCosArr23[i].sin + lensFlareScreenX;
                    framebufferFetches[i].y = sinCosArr23[i].cos + lensFlareScreenY;
                    if ((sx + sinCosArr80[i].sin & 0x7fff) < screenOffsetX &&
                        (sy + sinCosArr80[i].cos & 0x7fff) < screenOffsetY) {
                        texel = framebufferFetches[i].texel;
                        if ((texel & 0x7c00) >= LENS_FLARE_COLOR_THRESHOLDS_IN_SPECIAL_LEVEL[j][0] &&
                            (texel & 0x3e0) >= LENS_FLARE_COLOR_THRESHOLDS_IN_SPECIAL_LEVEL[j][1] &&
                            (texel & 0x1f) >= LENS_FLARE_COLOR_THRESHOLDS_IN_SPECIAL_LEVEL[j][2]) {
                            sunIntensityOnScreen += 5;
                        }
                    }
                }
                lensFlareScreenX = sx;
                lensFlareScreenY = sy;
                if (sunIntensityOnScreen > 0) {
                    for (i = 0; i < numLensFlareSprites; i++) {
                        setRGB0(&lensFlareSprites[whichDrawDispEnv][i].sprt,
                                sunIntensityOnScreen, sunIntensityOnScreen, sunIntensityOnScreen);
                        lensFlareSprites[whichDrawDispEnv][i].sprt.x0 = (sx + (lensFlarePositionMultipliers[i] * (halfWidth - sx) >> 12)) - lensFlareSpriteMidpoints[i].first;
                        lensFlareSprites[whichDrawDispEnv][i].sprt.y0 = (sy + (lensFlarePositionMultipliers[i] * (halfHeight - sy) >> 12)) - lensFlareSpriteMidpoints[i].second;
                        if (lensFlarePositionMultipliers[i] != 0) {
                            addPrim(&otag[whichDrawDispEnv][cameraIndex][1], &lensFlareSprites[whichDrawDispEnv][i]);
                        } else {
                            setRGB0(&lensFlareSprites[whichDrawDispEnv][i].sprt,
                                sunIntensityOnScreen, sunIntensityOnScreen, sunIntensityOnScreen);
                            addPrim(&otag[whichDrawDispEnv][cameraIndex][1024], &lensFlareSprites[whichDrawDispEnv][i]);
                        }
                    }
                }
            }
        }
    }
}
