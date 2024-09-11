#include "common.h"

extern TSprite keySprites[2][4][2];
extern int numKeysRemaining;
extern PrimList primLists[2];
extern int whichDrawDispEnv;

int numKeysInLevel;

INCLUDE_ASM("asm/nonmatchings/hud2", DrawTimeAttackTimer);

INCLUDE_ASM("asm/nonmatchings/hud2", UpdateStaticHourglassClut);

void DrawKeyWidgets(void) {
    int i;

    for (i = numKeysInLevel - 1; i >= 0; i--) {
        if (i < numKeysInLevel - numKeysRemaining){
            addPrim(&primLists[whichDrawDispEnv].gui1,
                    &keySprites[whichDrawDispEnv][i][1]);
        } else {
            addPrim(&primLists[whichDrawDispEnv].gui1,
                    &keySprites[whichDrawDispEnv][i][0]);
        }
    }
}
