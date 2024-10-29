#include "common.h"

// non-gprel-used variables (extern)
extern TSprite biosStrSprites[2][32];
extern PrimList primLists[2];
extern int whichDrawDispEnv;

// gprel-used variables (defined in this file)
int biosStrLen;

INCLUDE_ASM("asm/nonmatchings/bios_font", SetupBiosFontStr);

void DrawBiosFontString(void) {
    int i;

    for (i = 0; i < biosStrLen; i++) {
        addPrim(&primLists[whichDrawDispEnv].gui1, &biosStrSprites[whichDrawDispEnv][i]);
    }
}
