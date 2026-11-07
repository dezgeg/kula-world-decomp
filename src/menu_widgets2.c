#include "common.h"

typedef struct PmdFace {
    POLY_FT4 polys[2];
    SVECTOR vertexes[4];
} PmdFace;

typedef struct PmdCube {
    long count;
    PmdFace faces[6];
} PmdCube;

typedef struct PsxButtonBackground {
    TSprite sprites[2][200];
    short buttons[200];
} PsxButtonBackground;


extern uint Rand(int param_1);
extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

extern int displayHeight;
extern int displayWidth;
extern int firstLensFlareOrPsxButtonTexture;
extern int whichDrawDispEnv;
extern OT_TYPE otag[2][1][1026];
extern PrimList primLists[2];
extern Texture textures[NUM_TEXTURES];
extern uint firstGuiTextTexture;
extern uint firstGuiTexture;

int bigGuiSpriteFade;
int hsCube1stSinPhaseIncrementX;
int hsCube1stSinPhaseIncrementY;
int hsCube1stSinPhaseIncrementZ;
int hsCube1stSinPhaseX;
int hsCube1stSinPhaseY;
int hsCube1stSinPhaseZ;
int hsCube2ndIndexMulX;
int hsCube2ndIndexMulY;
int hsCube2ndIndexMulZ;
int hsCube2ndSinOffsetX;
int hsCube2ndSinOffsetY;
int hsCube2ndSinOffsetZ;
int hsCube2ndSinPhaseIncrementX;
int hsCube2ndSinPhaseIncrementY;
int hsCube2ndSinPhaseIncrementZ;
int hsCube2ndSinPhaseX;
int hsCube2ndSinPhaseY;
int hsCube2ndSinPhaseZ;
int hsCube2ndSinValMulX;
int hsCube2ndSinValMulY;
int hsCube2ndSinValMulZ;
int hsCube3rdIndexMulX;
int hsCube3rdIndexMulY;
int hsCube3rdIndexMulZ;
int hsCube3rdSinPhaseIncrementX;
int hsCube3rdSinPhaseIncrementY;
int hsCube3rdSinPhaseIncrementZ;
int hsCube3rdSinPhaseX;
int hsCube3rdSinPhaseY;
int hsCube3rdSinPhaseZ;
int hsCube3rdSinValMulX;
int hsCube3rdSinValMulY;
int hsCube3rdSinValMulZ;
MATRIX highScoreCubesMatrix;
PmdCube* pmdCubesPtr;
PsxButtonBackground* psxButtonBackgroundPtr;
SVECTOR hsCubeRotationVec;
SVECTOR hsCubeUnusedVec;
TSprite spinningSelectionSprite1[2];
TSprite spinningSelectionSprite2[2];
TSprite textDrawSprites[2][300];
TSprite textDrawTemplateSprites1[NUM_TEXT_SPRITES];
TSprite textDrawTemplateSprites2[NUM_TEXT_SPRITES];

STATIC_FOR_GP_ACCESS int textCharIdx;
static int fancyTextColorB;
static int fancyTextColorG;
static int fancyTextColorR;

void InitVariousUiSpriteTemplates(void) {
    int i;

    textCharIdx = 0;
    for (i = 0; i < NUM_TEXT_SPRITES; i++) {
        TSpritePrim(&textDrawTemplateSprites1[i], 0, 0, textures[firstGuiTextTexture].tpage);
        SetSemiTrans(&textDrawTemplateSprites1[i].sprt, textures[firstGuiTextTexture].semitrans);
        SetShadeTex(&textDrawTemplateSprites1[i].sprt, 0);

        TSpritePrim(&textDrawTemplateSprites2[i], 0, 0, textures[firstGuiTextTexture + 1].tpage);
        SetSemiTrans(&textDrawTemplateSprites2[i].sprt, textures[firstGuiTextTexture + 1].semitrans);
        SetShadeTex(&textDrawTemplateSprites2[i].sprt, 0);
    }

    for (i = 0; i < 16; i++) {
        setUV0(&textDrawTemplateSprites1[i].sprt,
               textures[firstGuiTextTexture].u + 12 * i,
               textures[firstGuiTextTexture].v);
        textDrawTemplateSprites1[i].sprt.clut = textures[firstGuiTextTexture].clut;
        textDrawTemplateSprites1[i].sprt.w = 12;
        textDrawTemplateSprites1[i].sprt.h = 16;

        setUV0(&textDrawTemplateSprites2[i].sprt,
               textures[firstGuiTextTexture + 1].u + 12 * i,
               textures[firstGuiTextTexture + 1].v);
        textDrawTemplateSprites2[i].sprt.clut = textures[firstGuiTextTexture + 1].clut;
        textDrawTemplateSprites2[i].sprt.w = 12;
        textDrawTemplateSprites2[i].sprt.h = 16;
    }

    for (i = 16; i < 19; i++) {
#if !VER_EU
        TSpritePrim(&textDrawTemplateSprites1[i], 0, 0,
                    textures[firstGuiTextTexture + 2].tpage);
        SetSemiTrans(&textDrawTemplateSprites1[i].sprt, textures[firstGuiTextTexture].semitrans);
        SetShadeTex(&textDrawTemplateSprites1[i].sprt, 0);
#endif
        setUV0(&textDrawTemplateSprites1[i].sprt,
               textures[firstGuiTextTexture + 2].u + 4 * (i - 16),
               textures[firstGuiTextTexture + 2].v);
        textDrawTemplateSprites1[i].sprt.clut = textures[firstGuiTextTexture + 2].clut;
        textDrawTemplateSprites1[i].sprt.w = 16;
        textDrawTemplateSprites1[i].sprt.h = 8;

#if !VER_EU
        TSpritePrim(&textDrawTemplateSprites2[i], 0, 0,
                    textures[firstGuiTextTexture + 3].tpage);
        SetSemiTrans(&textDrawTemplateSprites2[i].sprt,
                     textures[firstGuiTextTexture + 1].semitrans);
        SetShadeTex(&textDrawTemplateSprites2[i].sprt, 0);
#endif
        setUV0(&textDrawTemplateSprites2[i].sprt,
               textures[firstGuiTextTexture + 3].u + 4 * (i - 16),
               textures[firstGuiTextTexture + 3].v);
        textDrawTemplateSprites2[i].sprt.clut = textures[firstGuiTextTexture + 3].clut;
        textDrawTemplateSprites2[i].sprt.w = 16;
        textDrawTemplateSprites2[i].sprt.h = 8;
    }

    for (i = 19; i < NUM_TEXT_SPRITES; i++) {
        TSpritePrim(&textDrawTemplateSprites1[i], 0, 0,
                    textures[firstGuiTextTexture + 4 + (i - 19) * 2].tpage);
        // bug, wrong texture idx used
        SetSemiTrans(&textDrawTemplateSprites1[i].sprt, textures[firstGuiTextTexture].semitrans);
        SetShadeTex(&textDrawTemplateSprites1[i].sprt, 0);
        setUV0(&textDrawTemplateSprites1[i].sprt,
               textures[firstGuiTextTexture + 4 + (i - 19) * 2].u,
               textures[firstGuiTextTexture + 4 + (i - 19) * 2].v);
        textDrawTemplateSprites1[i].sprt.clut =
            textures[firstGuiTextTexture + 4 + (i - 19) * 2].clut;
        // bug, 5 instead of 4
        textDrawTemplateSprites1[i].sprt.w =
            textures[firstGuiTextTexture + 5 + (i - 19) * 2].w;
        // bug, 5 instead of 4
        textDrawTemplateSprites1[i].sprt.h =
            textures[firstGuiTextTexture + 5 + (i - 19) * 2].h;

        TSpritePrim(&textDrawTemplateSprites2[i], 0, 0,
                    textures[firstGuiTextTexture + 5 + (i - 19) * 2].tpage);
        // bug, wrong texture idx used
        SetSemiTrans(&textDrawTemplateSprites2[i].sprt,
                     textures[firstGuiTextTexture + 1].semitrans);
        SetShadeTex(&textDrawTemplateSprites2[i].sprt, 0);
        setUV0(&textDrawTemplateSprites2[i].sprt,
               textures[firstGuiTextTexture + 5 + (i - 19) * 2].u,
               textures[firstGuiTextTexture + 5 + (i - 19) * 2].v);
        textDrawTemplateSprites2[i].sprt.clut =
            textures[firstGuiTextTexture + 5 + (i - 19) * 2].clut;
        textDrawTemplateSprites2[i].sprt.w = textures[firstGuiTextTexture + 5 + (i - 19) * 2].w;
        textDrawTemplateSprites2[i].sprt.h = textures[firstGuiTextTexture + 5 + (i - 19) * 2].h;
    }
}

void SetFancyTextColor(int r, int g, int b) {
    fancyTextColorR = r;
    fancyTextColorG = g;
    fancyTextColorB = b;
}

void ResetTextVars(void) {
    textCharIdx = 0;
    fancyTextColorR = 0;
    fancyTextColorG = 0;
    fancyTextColorB = 0;
}

void DrawStaticUiSprite(short id, short x, short y, short count) {
    int i;
    int color;

    color = bigGuiSpriteFade - y / 2;
    if (color < 0) {
        color = 0;
    }
    if (color > 0x80) {
        color = 0x80;
    }
    if (id == 0) {
        if (textCharIdx + 2 > 299) {
            return;
        }
        textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites1[16 + id];
        setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, color, color, color);
        setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, x, y);
        addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
        textCharIdx++;

        textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites2[16 + id];
        setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, color, color, color);
        setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, x, y);
        addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
        textCharIdx++;

        for (i = 1; i < count; i++) {
            if (textCharIdx + 2 > 299) {
                return;
            }
            textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites1[17 + id];
            setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, color, color, color);
            setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, x + i * 16, y);
            addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
            textCharIdx++;

            textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites2[17 + id];
            setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, color, color, color);
            setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, x + i * 16, y);
            addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
            textCharIdx++;
        }
        if (textCharIdx + 2 > 299) {
            return;
        }

        textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites1[18 + id];
        setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, color, color, color);
        setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, x + i * 16, y);
        addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
        textCharIdx++;

        textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites2[18 + id];
        setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, color, color, color);
        setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, x + i * 16, y);
        addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
        textCharIdx++;
    } else {
        if (textCharIdx + 2 > 299) {
            return;
        }

        if (VER_JP ? (id < 6 || id > 9) : 1) {
            textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites1[18 + id];
            setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, color, color, color);
            setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, x, y);
            addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
            textCharIdx++;
        }

        textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites2[18 + id];
        setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, color, color, color);
        setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, x, y);
        addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
        textCharIdx++;
    }
}

void DrawTextFancyFont(char* str, short x, short y) {
    int startIdx;
    int endIdx;
    int finalX;
    int finalY;
    int r;
    int g;
    int b;
    int col;

    startIdx = 0;
    finalY = y;
    while (str[startIdx] != '\0') {
        int i;
        endIdx = startIdx;
        while (str[endIdx] != '\0' && str[endIdx] != '\n') {
            endIdx++;
        }
        finalX = x - (endIdx - startIdx) * 12;
        r = bigGuiSpriteFade - finalY / 2 - fancyTextColorR;
        g = bigGuiSpriteFade - finalY / 2 - fancyTextColorG;
        b = bigGuiSpriteFade - finalY / 2 - fancyTextColorB;
        col = bigGuiSpriteFade - finalY / 2;
        if (r < 0) {
            r = 0;
        }
        if (r > 0x80) {
            r = 0x80;
        }

        if (g < 0) {
            g = 0;
        }
        if (g > 0x80) {
            g = 0x80;
        }

        if (b < 0) {
            b = 0;
        }
        if (b > 0x80) {
            b = 0x80;
        }

        if (col < 0) {
            col = 0;
        }
        if (col > 0x80) {
            col = 0x80;
        }

        for (i = startIdx; i < endIdx; i++) {
            if (str[i] >= '0' && str[i] <= '9') {
                if (textCharIdx + 2 > 299) {
                    return;
                }
                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites1[str[i] - 0x30];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, col, col, col);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;

                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites2[str[i] - 0x30];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, r, g, b);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;
            }
            if (str[i] == '+') {
                if (textCharIdx + 2 > 299) {
                    return;
                }

                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites1[10];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, col, col, col);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;

                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites2[10];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, r, g, b);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;
            }
            if (str[i] == '-') {
                if (textCharIdx + 2 > 299) {
                    return;
                }
                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites1[11];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, col, col, col);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;

                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites2[11];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, r, g, b);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;
            }
#if !VER_EU
            if (str[i] == '*') {
                if (textCharIdx + 2 > 299) {
                    return;
                }
                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites1[12];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, col, col, col);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;

                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites2[12];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, r, g, b);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;
            }
            if (str[i] == '=') {
                if (textCharIdx + 2 > 299) {
                    return;
                }
                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites1[13];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, col, col, col);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;

                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites2[13];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, r, g, b);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;
            }
#else
            if (str[i] == '=') {
                if (textCharIdx + 2 > 299) {
                    return;
                }
                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites1[12];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, col, col, col);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;

                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites2[12];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, r, g, b);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;
            }
#endif
            if (str[i] == ':') {
                if (textCharIdx + 2 > 299) {
                    return;
                }
                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites1[14];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, col, col, col);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;

                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites2[14];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, r, g, b);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;
            }
            finalX += 12;
        }

        finalY += 18;
        if (!str[endIdx]) {
            break;
        }
        startIdx = endIdx + 1;
    }
}

static int enableGuiBackgroundScroll;

void InitHighscoreCubes(void) {
    u_char rgb;
    int i;
    int j;
    ushort tpage;
    ushort clut;
    byte u0;
    byte v0;
    int semitrans;

    SVECTOR zero = {};
    SVECTOR v1 = { -100, -100, 100 };
    SVECTOR v2 = { 100, -100, 100 };
    SVECTOR v3 = { -100, -100, -100 };
    SVECTOR v4 = { 100, -100, -100 };
    SVECTOR v5 = { -100, 100, 100 };
    SVECTOR v6 = { 100, 100, 100 };
    SVECTOR v7 = { -100, 100, -100 };
    SVECTOR v8 = { 100, 100, -100 };

    hsCubeRotationVec = zero;
    hsCubeUnusedVec = zero;
    hsCubeUnusedVec.vz = 1000;

    hsCube2ndSinValMulX = 900;
    hsCube2ndSinPhaseX = 0;
    hsCube2ndSinPhaseIncrementX = 30;
    hsCube2ndSinOffsetX = 700;
    hsCube3rdSinValMulX = 500;
    hsCube3rdSinPhaseX = 0;
    hsCube3rdSinPhaseIncrementX = 13;
    hsCube3rdIndexMulX = 860;
    hsCube2ndIndexMulX = 400;
    hsCube1stSinPhaseX = 0;
    hsCube1stSinPhaseIncrementX = 7;

    hsCube2ndSinValMulY = 800;
    hsCube2ndSinPhaseY = 0;
    hsCube2ndSinPhaseIncrementY = 19;
    hsCube2ndSinOffsetY = 600;
    hsCube3rdSinValMulY = 400;
    hsCube3rdSinPhaseY = 0;
    hsCube3rdSinPhaseIncrementY = 20;
    hsCube3rdIndexMulY = 730;
    hsCube2ndIndexMulY = 150;
    hsCube1stSinPhaseY = 0;
    hsCube1stSinPhaseIncrementY = 19;

    hsCube2ndSinValMulZ = 400;
    hsCube2ndSinPhaseZ = 0;
    hsCube2ndSinPhaseIncrementZ = 7;
    hsCube2ndSinOffsetZ = 37;
    hsCube3rdSinValMulZ = 200;
    hsCube3rdSinPhaseZ = 0;
    hsCube3rdSinPhaseIncrementZ = 20;
    hsCube3rdIndexMulZ = 70;
    hsCube2ndIndexMulZ = 0;
    hsCube1stSinPhaseZ = 0;
    hsCube1stSinPhaseIncrementZ = 0;

    pmdCubesPtr = HIGHSCORE_CUBES_BUF;

    for (i = 0; i < 10; i++) {
        pmdCubesPtr[i].count = 6;
        GetHighscoreCubeStyle(&tpage, &clut, &u0, &v0, &semitrans);
        for (j = 0; j < 6; j++) {
            SetPolyFT4(&pmdCubesPtr[i].faces[j].polys[0]);
            rgb = j * 16 + ((i & 1) * 8 + 64);
            setRGB0(&pmdCubesPtr[i].faces[j].polys[0], rgb, rgb, rgb);
            SetSemiTrans(&pmdCubesPtr[i].faces[j].polys[0], semitrans);
            setUV4(&pmdCubesPtr[i].faces[j].polys[0],
                u0, v0,
                u0 + 31, v0,
                u0, v0 + 31,
                u0 + 31, v0 + 31);
            pmdCubesPtr[i].faces[j].polys[0].clut = clut;
            pmdCubesPtr[i].faces[j].polys[0].tpage = tpage;
            pmdCubesPtr[i].faces[j].polys[1] = pmdCubesPtr[i].faces[j].polys[0];
        }
        pmdCubesPtr[i].faces[0].vertexes[0] = v1;
        pmdCubesPtr[i].faces[0].vertexes[1] = v2;
        pmdCubesPtr[i].faces[0].vertexes[2] = v3;
        pmdCubesPtr[i].faces[0].vertexes[3] = v4;
        pmdCubesPtr[i].faces[1].vertexes[0] = v7;
        pmdCubesPtr[i].faces[1].vertexes[1] = v8;
        pmdCubesPtr[i].faces[1].vertexes[2] = v5;
        pmdCubesPtr[i].faces[1].vertexes[3] = v6;
        pmdCubesPtr[i].faces[2].vertexes[0] = v1;
        pmdCubesPtr[i].faces[2].vertexes[1] = v3;
        pmdCubesPtr[i].faces[2].vertexes[2] = v5;
        pmdCubesPtr[i].faces[2].vertexes[3] = v7;
        pmdCubesPtr[i].faces[3].vertexes[0] = v4;
        pmdCubesPtr[i].faces[3].vertexes[1] = v2;
        pmdCubesPtr[i].faces[3].vertexes[2] = v8;
        pmdCubesPtr[i].faces[3].vertexes[3] = v6;
        pmdCubesPtr[i].faces[4].vertexes[0] = v3;
        pmdCubesPtr[i].faces[4].vertexes[1] = v4;
        pmdCubesPtr[i].faces[4].vertexes[2] = v7;
        pmdCubesPtr[i].faces[4].vertexes[3] = v8;
        pmdCubesPtr[i].faces[5].vertexes[0] = v2;
        pmdCubesPtr[i].faces[5].vertexes[1] = v1;
        pmdCubesPtr[i].faces[5].vertexes[2] = v6;
        pmdCubesPtr[i].faces[5].vertexes[3] = v5;
    }
}

void DrawHighScoreCubes(void) {
    int sinVal2ndX;
    int sinVal3rdX;
    int sinVal2ndY;
    int sinVal3rdY;
    int sinVal2ndZ;
    int sinVal3rdZ;
    int i;
    SVECTOR rotVec;

    hsCubeRotationVec.vx = (hsCubeRotationVec.vx + 25) & 0xfff;
    hsCubeRotationVec.vy = (hsCubeRotationVec.vy + 11) & 0xfff;
    hsCubeRotationVec.vz = (hsCubeRotationVec.vz + 41) & 0xfff;

    rotVec = hsCubeRotationVec;
    hsCube2ndSinPhaseX = hsCube2ndSinPhaseX + hsCube2ndSinPhaseIncrementX & 0xfff;
    hsCube3rdSinPhaseX = hsCube3rdSinPhaseX + hsCube3rdSinPhaseIncrementX & 0xfff;
    hsCube1stSinPhaseX = hsCube1stSinPhaseX + hsCube1stSinPhaseIncrementX & 0xfff;
    hsCube2ndSinPhaseY = hsCube2ndSinPhaseY + hsCube2ndSinPhaseIncrementY & 0xfff;
    hsCube3rdSinPhaseY = hsCube3rdSinPhaseY + hsCube3rdSinPhaseIncrementY & 0xfff;
    hsCube1stSinPhaseY = hsCube1stSinPhaseY + hsCube1stSinPhaseIncrementY & 0xfff;
    hsCube2ndSinPhaseZ = hsCube2ndSinPhaseY + hsCube2ndSinPhaseIncrementZ & 0xfff;
    hsCube3rdSinPhaseZ = hsCube3rdSinPhaseY + hsCube3rdSinPhaseIncrementZ & 0xfff;
    hsCube1stSinPhaseZ = hsCube1stSinPhaseY + hsCube1stSinPhaseIncrementZ & 0xfff;
    for (i = 0; i < 10; i++) {
        rotVec.vx = (rotVec.vx + 210) & 0xfff;
        rotVec.vy = (rotVec.vy + 133) & 0xfff;
        rotVec.vz = (rotVec.vz + 93) & 0xfff;
        RotMatrix(&rotVec, &highScoreCubesMatrix);

        sinVal2ndX = rsin(hsCube2ndSinPhaseX + (hsCube2ndSinOffsetX + hsCube2ndIndexMulX * rsin(hsCube1stSinPhaseX) * i) >> 12);
        sinVal3rdX = rsin(hsCube3rdSinPhaseX + hsCube3rdIndexMulX * i);
        highScoreCubesMatrix.t[0] = hsCube2ndSinValMulX * sinVal2ndX + hsCube3rdSinValMulX * sinVal3rdX >> 12;

        sinVal2ndY = rsin(hsCube2ndSinPhaseY + (hsCube2ndSinOffsetY + hsCube2ndIndexMulY * rsin(hsCube1stSinPhaseY) * i) >> 12);
        sinVal3rdY = rsin(hsCube3rdSinPhaseY + hsCube3rdIndexMulY * i);
        highScoreCubesMatrix.t[1] = hsCube2ndSinValMulY * sinVal2ndY + hsCube3rdSinValMulY * sinVal3rdY >> 12;

        sinVal2ndZ = rsin(hsCube2ndSinPhaseZ + (hsCube2ndSinOffsetZ + hsCube2ndIndexMulZ * rsin(hsCube1stSinPhaseZ) * i) >> 12);
        sinVal3rdZ = rsin(hsCube3rdSinPhaseZ + hsCube3rdIndexMulZ * i);
        highScoreCubesMatrix.t[2] = (hsCube2ndSinValMulZ * sinVal2ndZ + hsCube3rdSinValMulZ * sinVal3rdZ >> 12) + 1000;

        SetRotMatrix(&highScoreCubesMatrix);
        SetTransMatrix(&highScoreCubesMatrix);
        RotSMD_FT4((long*)&pmdCubesPtr[i], &otag[whichDrawDispEnv][0][0], 1024, whichDrawDispEnv, 1,
                   displayWidth, displayHeight, 1);
    }
}

static int guiBackgroundFadeIn = 0; // .sdata
static int guiBackgroundAnimCounter = 0; // .sdata

void InitPsxButtonBackgroundSprites(int param_1) {
    uint rng;
    int relativeTex;
    int i;

#if !VER_EU
    guiBackgroundAnimCounter = 424;
#endif
    psxButtonBackgroundPtr = PSX_BUTTON_BG_BUF;
    for (i = 0; i < 200; i++) {
        rng = Rand(4);
        psxButtonBackgroundPtr->buttons[i] = rng;
        relativeTex = rng + 6;
        TSpritePrim(&psxButtonBackgroundPtr->sprites[0][i], 0, 0,
                    textures[firstLensFlareOrPsxButtonTexture + relativeTex].tpage);
        SetSemiTrans(&psxButtonBackgroundPtr->sprites[0][i].sprt,
                     textures[firstLensFlareOrPsxButtonTexture + relativeTex].semitrans);
        SetShadeTex(&psxButtonBackgroundPtr->sprites[0][i].sprt, 0);
        setRGB0(&psxButtonBackgroundPtr->sprites[0][i].sprt, guiBackgroundFadeIn,
                guiBackgroundFadeIn, guiBackgroundFadeIn);

        psxButtonBackgroundPtr->sprites[0][i].sprt.clut =
            textures[firstLensFlareOrPsxButtonTexture + relativeTex].clut;
        psxButtonBackgroundPtr->sprites[0][i].sprt.w =
            textures[firstLensFlareOrPsxButtonTexture + relativeTex].w;
        psxButtonBackgroundPtr->sprites[0][i].sprt.h =
            textures[firstLensFlareOrPsxButtonTexture + relativeTex].h;
        setUV0(&psxButtonBackgroundPtr->sprites[0][i].sprt,
               textures[firstLensFlareOrPsxButtonTexture + relativeTex].u,
               textures[firstLensFlareOrPsxButtonTexture + relativeTex].v);
        psxButtonBackgroundPtr->sprites[1][i] = psxButtonBackgroundPtr->sprites[0][i];
    }

    guiBackgroundFadeIn = 0;
    switch (param_1) {
        case 0:
            enableGuiBackgroundScroll = 0;
            break;
        case 1:
            enableGuiBackgroundScroll = 1;
            break;
    }
}

#if !VER_EU
void DrawPsxButtonBackground(int fadeIn) {
#else
void DrawPsxButtonBackground(void) {
#endif
    int y;
    int x;
    int i;
    uint* ot;
    int b;

    for (i = 0; i < 200; i++) {
        x = (i % 20) * 48 + (rsin(guiBackgroundAnimCounter) * (480 - displayWidth / 2) >> 12) -
            480 + displayWidth / 2;
        y = (i / 20) * 48 + (rcos(guiBackgroundAnimCounter) * (240 - displayHeight / 2) >> 12) -
            240 + displayHeight / 2;
        setXY0(&psxButtonBackgroundPtr->sprites[whichDrawDispEnv][i].sprt, x, y);

#if !VER_EU
        if (fadeIn == 1) {
            setRGB0(&psxButtonBackgroundPtr->sprites[whichDrawDispEnv][i].sprt, guiBackgroundFadeIn,
                    guiBackgroundFadeIn, guiBackgroundFadeIn);
        } else {
            setRGB0(&psxButtonBackgroundPtr->sprites[whichDrawDispEnv][i].sprt, 128, 128, 128);
        }
#else
        setRGB0(&psxButtonBackgroundPtr->sprites[whichDrawDispEnv][i].sprt, guiBackgroundFadeIn,
                guiBackgroundFadeIn, guiBackgroundFadeIn);
#endif

        if (x > -48 && x < displayWidth + 48 && y > -48 && y < displayHeight + 48) {
            addPrim(&otag[whichDrawDispEnv][0][1024] - (psxButtonBackgroundPtr->buttons[i] + 1),
                    &psxButtonBackgroundPtr->sprites[whichDrawDispEnv][i]);
        }
    }

    if (enableGuiBackgroundScroll == 1) {
        guiBackgroundAnimCounter += 9;
    }
    guiBackgroundFadeIn += 6;
    if (guiBackgroundFadeIn > 0x80) {
        guiBackgroundFadeIn = 0x80;
    }
}

void FadePsxButtonBackground(void) {
    guiBackgroundFadeIn -= 20;
    if (guiBackgroundFadeIn < 0) {
        guiBackgroundFadeIn = 0;
    }
}

static int spinningSelectionPhase = 0; // .sdata

void InitSpinningSelectionSprites(void) {
    TSpritePrim(&spinningSelectionSprite1[0], 0, 0, textures[firstGuiTexture + 13].tpage);
    setXY0(&spinningSelectionSprite1[0].sprt, 0, 0);
    setUV0(&spinningSelectionSprite1[0].sprt,
           textures[firstGuiTexture + 13].u,
           textures[firstGuiTexture + 13].v);
    setRGB0(&spinningSelectionSprite1[0].sprt, 128, 128, 128);
    SetSemiTrans(&spinningSelectionSprite1[0].sprt, textures[firstGuiTexture + 13].semitrans);
    SetShadeTex(&spinningSelectionSprite1[0].sprt, 0);
    spinningSelectionSprite1[0].sprt.clut = textures[firstGuiTexture + 13].clut;
    spinningSelectionSprite1[0].sprt.w = textures[firstGuiTexture + 13].w;
    spinningSelectionSprite1[0].sprt.h = textures[firstGuiTexture + 13].h;

    spinningSelectionSprite2[1] = spinningSelectionSprite2[0] = spinningSelectionSprite1[1] = spinningSelectionSprite1[0];
}

void UpdateMemcardMenuSaveSelectionSprites(int slot) {
    spinningSelectionPhase = spinningSelectionPhase + 60 & 0xfff;
    spinningSelectionSprite1[whichDrawDispEnv].sprt.x0 =
        55 + slot * 70 + ((rsin(spinningSelectionPhase) * 10) >> 12) - textures[firstGuiTexture + 13].w / 2;
    spinningSelectionPhase = spinningSelectionPhase + 60 & 0xfff;
    spinningSelectionSprite1[whichDrawDispEnv].sprt.y0 =
        64 + (rcos(spinningSelectionPhase) * 10 >> 12) - textures[firstGuiTexture + 13].w / 2;
    addPrim(&primLists[whichDrawDispEnv].main, &spinningSelectionSprite1[whichDrawDispEnv]);

    spinningSelectionSprite2[whichDrawDispEnv].sprt.x0 =
        55 + slot * 70 + (rsin(spinningSelectionPhase + 0x800 & 0xfff) * 10 >> 12) - textures[firstGuiTexture + 13].w / 2;
    spinningSelectionSprite2[whichDrawDispEnv].sprt.y0 =
        64 + (rcos(spinningSelectionPhase + 0x800 & 0xfff) * 10 >> 12) - textures[firstGuiTexture + 13].w / 2;
    addPrim(&primLists[whichDrawDispEnv].main, &spinningSelectionSprite2[whichDrawDispEnv]);
}

void Update2PlayerLevelSelectionSprites(int x, int y) {
    spinningSelectionPhase = spinningSelectionPhase + 60 & 0xfff;
    spinningSelectionSprite1[whichDrawDispEnv].sprt.x0 =
        x + ((rsin(spinningSelectionPhase) * 7) >> 12) - textures[firstGuiTexture + 13].w / 2;
    spinningSelectionPhase = spinningSelectionPhase + 60 & 0xfff;
    spinningSelectionSprite1[whichDrawDispEnv].sprt.y0 =
        y + (rcos(spinningSelectionPhase) * 7 >> 12) - textures[firstGuiTexture + 13].w / 2;
    addPrim(&primLists[whichDrawDispEnv].main, &spinningSelectionSprite1[whichDrawDispEnv]);

    spinningSelectionSprite2[whichDrawDispEnv].sprt.x0 =
        x + (rsin(spinningSelectionPhase + 0x800 & 0xfff) * 7 >> 12) - textures[firstGuiTexture + 13].w / 2;
    spinningSelectionSprite2[whichDrawDispEnv].sprt.y0 =
        y + (rcos(spinningSelectionPhase + 0x800 & 0xfff) * 7 >> 12) - textures[firstGuiTexture + 13].w / 2;
    addPrim(&primLists[whichDrawDispEnv].main, &spinningSelectionSprite2[whichDrawDispEnv]);
}
