#include "common.h"

int lastFrameIdWhenTextWasDrawn;
int textAlignMode;
byte textColorB;
byte textColorG;
byte textColorR;
int textPosXStart;
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

// https://decomp.me/scratch/VgNU6
INCLUDE_ASM("asm/nonmatchings/text1", DrawTextCrappyFont);
