#include "common.h"

extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

extern int whichDrawDispEnv;
extern PrimList primLists[2];
extern Texture textures[NUM_TEXTURES];
extern uint firstGuiTextTexture;

int bigGuiSpriteFade;
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
