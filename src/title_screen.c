#include "common.h"

extern TSprite copyrightSprite[2];
extern PrimList primLists[2];
extern int sunIntensityOnScreen;
extern TSprite titleSprite[2];
extern int whichDrawDispEnv;

int drawCopyright = 1;

void DrawTitleAndCopyrightSprites(void) {
    int color;

    color = (sunIntensityOnScreen * 30) / 100 + 0x80;
    setRGB0(&titleSprite[whichDrawDispEnv].sprt, color, color, color);
    addPrim(&primLists[whichDrawDispEnv].gui1, &titleSprite[whichDrawDispEnv]);
    if (drawCopyright == 1) {
        addPrim(&primLists[whichDrawDispEnv].gui1, &copyrightSprite[whichDrawDispEnv]);
    }
}
