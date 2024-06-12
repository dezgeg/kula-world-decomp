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
