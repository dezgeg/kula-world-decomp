#include "common.h"

extern PrimList primLists[2];
extern TSprite textDrawSprites[2][300];
extern TSprite textDrawTemplateSprites1[40];
extern TSprite textDrawTemplateSprites2[40];
extern int whichDrawDispEnv;

int bigGuiSpriteFade;
int fancyTextColorB;
int fancyTextColorG;
int fancyTextColorR;
int textCharIdx;

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
            addPrim(&primLists[whichDrawDispEnv].main,
                    &textDrawSprites[whichDrawDispEnv][textCharIdx]);
            textCharIdx++;

            textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites2[17 + id];
            setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, color, color, color);
            setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, x + i * 16, y);
            addPrim(&primLists[whichDrawDispEnv].main,
                    &textDrawSprites[whichDrawDispEnv][textCharIdx]);
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

        textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites1[18 + id];
        setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, color, color, color);
        setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, x, y);
        addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
        textCharIdx++;

        textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites2[18 + id];
        setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, color, color, color);
        setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, x, y);
        addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
        textCharIdx++;
    }
}

void DrawTextFancyFont(char *str,short x,short y) {
    int startIdx;
    int endIdx;
    int finalX;
    int finalY;
    int fade;
    int r;
    int g;
    int b;
    int col;

    startIdx = 0;
    endIdx = 0;
    finalY = y;
    for (; str[startIdx]; startIdx = endIdx + 1) {
        endIdx = startIdx;
        while (str[endIdx] != '\0' && str[endIdx] != '\n') {
            endIdx++;
        }
        finalX = x + (endIdx - startIdx) * -12;
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
        
        while (startIdx < endIdx) {
            if (str[startIdx] - 0x30 < 10u) {
                if (textCharIdx + 2 > 299) {
                    return;
                }
                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites1[str[startIdx] - 0x30];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, col, col, col);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;

                textDrawSprites[whichDrawDispEnv][textCharIdx] = textDrawTemplateSprites2[str[startIdx] - 0x30];
                setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, r, g, b);
                setXY0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, finalX, finalY);
                addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                textCharIdx++;

            }
            if (str[startIdx] == '+') {
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
            if (str[startIdx] == '-') {
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
            if (str[startIdx] == '=') {
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
            if (str[startIdx] == ':') {
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
            startIdx++;
            finalX += 12;
        }

        finalY += 18;
        startIdx = endIdx;
        startIdx++;
        if (!str[endIdx])
            return;
    }
}
