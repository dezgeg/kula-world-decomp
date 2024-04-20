#include "common.h"

int drawCopyright;

extern TSprite copyrightSprite[2];
extern PrimList primLists[2];
extern int sunIntensityOnScreen;
extern TSprite titleSprite[2];
extern int whichDrawDispEnv;

void DrawTitleAndCopyrightSprites(void) {
    int color;
    int pad[5];

    color = (sunIntensityOnScreen * 30) / 100 + 0x80;
    titleSprite[whichDrawDispEnv].sprt.r0 = color;
    titleSprite[whichDrawDispEnv].sprt.g0 = color;
    titleSprite[whichDrawDispEnv].sprt.b0 = color;
    addPrim(&primLists[whichDrawDispEnv].gui1, &titleSprite[whichDrawDispEnv]);
    if (drawCopyright == 1) {
        addPrim(&primLists[whichDrawDispEnv].gui1, &copyrightSprite[whichDrawDispEnv]);
    }
}
