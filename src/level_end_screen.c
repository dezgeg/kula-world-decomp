#include "common.h"

int bigGuiSpriteFade;
int gameOverScreenState;
int whichLevelEndSpriteLoaded;

void InitLevelEndScreen(void) {
    whichLevelEndSpriteLoaded = 48;
    gameOverScreenState = 0;
    bigGuiSpriteFade = 0;
}
