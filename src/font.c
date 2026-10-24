#include "common.h"

extern void InitWidgetSprites(void);
extern void SetTextParams(int posX, int posY, int align, int colorR, int colorG, int colorB);
extern void SetFontEntry(char c, uchar u, uchar v, uchar width, uchar height, uchar x, uchar y, int texIndex);
extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

extern int displayWidth;
extern int whichDrawDispEnv;
extern PrimList primLists[2];
extern Texture textures[150];
extern TSprite fontSprites[96];
extern TSprite textDrawSprites[2][300];
extern uint firstGuiTexture;

TSprite fontSprites[96];

STATIC_FOR_GP_ACCESS int textCharIdx;
static byte textColorB;
static byte textColorG;
static byte textColorR;
static int lastFrameIdWhenTextWasDrawn;
static int textAlignMode;
static int textPosXEnd;
static int textPosXStart;
static int textPosYCur;
static int textPosYEnd;
static int textPosYStart;

void SetFontEntry(char c, uchar u, uchar v, uchar width, uchar height, uchar x, uchar y, int texIndex) {
    uchar idx = c - 0x20;
    if (idx < 0x60) {
        TSpritePrim(fontSprites + idx, 0, 0, textures[texIndex].tpage);
        setXY0(&fontSprites[idx].sprt, x, y);
        setRGB0(&fontSprites[idx].sprt, 0x80, 0x80, 0x80);
        SetSemiTrans(&fontSprites[idx].sprt, textures[texIndex].semitrans);
        SetShadeTex(&fontSprites[idx].sprt, 0);
        fontSprites[idx].sprt.clut = textures[texIndex].clut;
        fontSprites[idx].sprt.w = width;
        fontSprites[idx].sprt.h = height;
        setUV0(&fontSprites[idx].sprt, textures[texIndex].u + u, textures[texIndex].v + v);
    }
}

void InitFont(void) {
    int i;

    InitWidgetSprites();
    SetTextParams(displayWidth >> 1, 0, 1, 128, 128, 128);
    for (i = 0; i < 128; i++) {
        SetFontEntry(i, 0, 0, 0, 0, 0, 0, firstGuiTexture + 9);
    }
    SetFontEntry('e', 0, 0, 11, 11, 0, 2, firstGuiTexture + 10);
    SetFontEntry('f', 12, 0, 11, 11, 0, 2, firstGuiTexture + 10);
    SetFontEntry('g', 24, 0, 11, 11, 0, 2, firstGuiTexture + 10);
    SetFontEntry('h', 36, 0, 11, 11, 0, 2, firstGuiTexture + 10);
    SetFontEntry('A', 0, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('B', 8, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('C', 16, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('D', 24, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('E', 32, 0, 6, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('F', 40, 0, 6, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('G', 48, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('H', 56, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('I', 64, 0, 4, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('J', 72, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('K', 80, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('L', 88, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('M', 96, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('N', 104, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('O', 112, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('P', 120, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('Q', 128, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('R', 136, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('S', 144, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('T', 152, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('U', 160, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('V', 168, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('W', 176, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('X', 184, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('Y', 192, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('Z', 200, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('0', 232, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('1', 240, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('2', 248, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('3', 0, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('4', 8, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('5', 16, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('6', 24, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('7', 32, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('8', 40, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('9', 48, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('.', 56, 12, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(',', 64, 12, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(':', 72, 12, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(';', 80, 12, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('-', 88, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('+', 96, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('\'', 104, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('`', 104, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('!', 112, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('?', 120, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('\"', 128, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('#', 136, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('%', 144, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('&', 152, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('/', 160, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('\\', 168, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('(', 176, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(')', 184, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('[', 176, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(']', 184, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('{', 176, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('}', 184, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('=', 192, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('>', 200, 12, 12, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('<', 212, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('|', 220, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(' ', 0, 0, 4, 0, 0, 2, firstGuiTexture + 9);
}

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
    if (colorR > 0x80) {
        colorR = 0x80;
    }
    if (colorG > 0x80) {
        colorG = 0x80;
    }
    if (colorB > 0x80) {
        colorB = 0x80;
    }
    if (colorR >= 0) {
        textColorR = (byte)colorR;
    }
    if (colorG >= 0) {
        textColorG = (byte)colorG;
    }
    if (colorB >= 0) {
        textColorB = (byte)colorB;
    }
}

void DrawTextCrappyFont(char* str) {
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
        if (VER_EU || str[i] - 0x20 >= 0)
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

            for (j = lineStartIndex; str[j] != '\n' && str[j]; j++) {
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
