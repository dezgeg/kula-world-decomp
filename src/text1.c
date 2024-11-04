#include "common.h"

extern TSprite fontSprites[96];
extern PrimList primLists[2];
extern TSprite textDrawSprites[2][300];
extern int whichDrawDispEnv;

byte textColorB;
byte textColorG;
byte textColorR;
int lastFrameIdWhenTextWasDrawn;
int textAlignMode;
int textCharIdx;
int textPosXEnd;
int textPosXStart;
int textPosYCur;
int textPosYEnd;
int textPosYStart;

void ResetTextRenderState(void) {
    lastFrameIdWhenTextWasDrawn = -1;
}

void SetTextParams(int posX, int posY, int align, int colorR, int colorG, int colorB) {
    if (-1 < align) {
        textAlignMode = align;
    }
    if (posX != -1) {
        textPosXStart = posX;
    }
    if (posY != -1) {
        textPosYStart = posY;
        textPosYEnd = posY;
    }
    if (0x80 < colorR) {
        colorR = 0x80;
    }
    if (0x80 < colorG) {
        colorG = 0x80;
    }
    if (0x80 < colorB) {
        colorB = 0x80;
    }
    if (-1 < colorR) {
        textColorR = (byte)colorR;
    }
    if (-1 < colorG) {
        textColorG = (byte)colorG;
    }
    if (-1 < colorB) {
        textColorB = (byte)colorB;
    }
}

void DrawTextCrappyFont(char *str) {
    int i;
    int j;
    int width;
    int lineStartIndex;

    if (whichDrawDispEnv != lastFrameIdWhenTextWasDrawn) {
        lastFrameIdWhenTextWasDrawn = whichDrawDispEnv;
        textPosXEnd = textPosXStart;
        textPosYCur = textPosYStart;
        textPosYEnd = textPosYStart;
    }

    textPosYCur = textPosYEnd;
    lineStartIndex = 0;
    for (width = i = 0; str[i]; i++) {
        width += fontSprites[str[i] - 0x20].sprt.w + 1;
        if (str[i] == '\n' || str[i] == 0) {
            if (textAlignMode == 0) {
                textPosXEnd = textPosXStart;
            }
            if (textAlignMode == 1) {
                textPosXEnd = textPosXStart - width / 2;
            }
            if (textAlignMode == 2) {
                textPosXEnd = textPosXStart - width;
            }

            for (j = lineStartIndex; str[j] != '\n' && str[j] ;j++) {
                if (fontSprites[str[j] - 0x20].sprt.h != 0 && str[j] - 0x20 >= 0) {
                    if (textCharIdx + 1 >= 400) {
                        return;
                    }
                    memcpy(&textDrawSprites[whichDrawDispEnv][textCharIdx], &fontSprites[str[j] - 0x20], sizeof(TSprite));
                    if (str[j] != '|') {
                        setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, textColorR, textColorG, textColorB);
                    } else {
                        setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, textColorR, 0, 0);
                    }
                    textDrawSprites[whichDrawDispEnv][textCharIdx].sprt.x0 += textPosXEnd;
                    textDrawSprites[whichDrawDispEnv][textCharIdx].sprt.y0 += textPosYCur;
                    addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                    textCharIdx++;
                }
                textPosXEnd += fontSprites[str[j] - 0x20].sprt.w + 1;
            }
            width = 0;
            lineStartIndex = i + 1;
            textPosYCur += 11;
        }
    }
    textPosYEnd = textPosYCur;
}
