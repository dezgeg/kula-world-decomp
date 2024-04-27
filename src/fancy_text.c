#include "common.h"

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
