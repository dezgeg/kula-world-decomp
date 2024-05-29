#include "common.h"

int bigGuiSpriteFade;

void FadeOutBigGuiSprite(void) {
    bigGuiSpriteFade -= 20;
    if (bigGuiSpriteFade < 0) {
        bigGuiSpriteFade = 0;
    }
}

void SetBigGuiSpriteVisible(void) {
    bigGuiSpriteFade = 0x100;
}
