#include "common.h"

// gprel-used variables (defined in this file)
uint * bonusWidgetDataPtr;
uint * bonusWidgetDataEnd;
uint * bonusWidgetDataPtr2;
int numBonusWidgetMsbs;

// non-gprel-used variables (extern)
extern int BONUS_WIDGET_COLOR_DATA_LEN;
extern uint BONUS_WIDGET_COLOR_DATA[];
extern TSprite bonusWidgetSpritesMsbBackground[2][34];
extern TSprite bonusWidgetSpritesMsbForeground[2][32];
extern TSprite bonusWidgetSpritesPiechart[2][9];
extern int numCubesRemainingInLevel[5];
extern PrimList primLists[2];
extern int whichDrawDispEnv;

typedef struct { uint c; } Col;

void DrawBonusWidget(void) {
    int i;
    int div8;
    int mod8;
    uint *p;
    uint *p2;

    bonusWidgetDataPtr++;
    if (bonusWidgetDataPtr >= bonusWidgetDataEnd) {
        bonusWidgetDataPtr -= BONUS_WIDGET_COLOR_DATA_LEN;
    }
    
    p = bonusWidgetDataPtr;
    for (i = 0; i < numCubesRemainingInLevel[0] >> 3; i++) {
        *(Col*)&bonusWidgetSpritesMsbForeground[whichDrawDispEnv][i].sprt.r0 = *(Col*)p;
        p += 5;
        if (p >= bonusWidgetDataEnd) {
            p -= BONUS_WIDGET_COLOR_DATA_LEN;
        }
    }
    
    bonusWidgetDataPtr2++;
    if (bonusWidgetDataPtr2 >= bonusWidgetDataEnd) {
        bonusWidgetDataPtr2 -= BONUS_WIDGET_COLOR_DATA_LEN;
    }
    
    p2 = bonusWidgetDataPtr2;
    for (i = 0; i < 8; i++) {
        *(Col*)&bonusWidgetSpritesPiechart[whichDrawDispEnv][i].sprt.r0 = *(Col*)p2;
        p2 -= 12;
        if (p2 < BONUS_WIDGET_COLOR_DATA) {
            p2 += BONUS_WIDGET_COLOR_DATA_LEN;
        }
    }
    
    mod8 = numCubesRemainingInLevel[0] & 7;
    div8 = numCubesRemainingInLevel[0] >> 3;
    
    if (mod8 == 0 && div8 > 0) {
        mod8 = 8;
        div8--;
    }
    
    for (i = 0; i < div8; i++) {
        addPrim(&primLists[whichDrawDispEnv].gui1, &bonusWidgetSpritesMsbForeground[whichDrawDispEnv][i].tpage);
    }
    
    for (i = 0; i < numBonusWidgetMsbs + 2; i++) {
        addPrim(&primLists[whichDrawDispEnv].gui1, &bonusWidgetSpritesMsbBackground[whichDrawDispEnv][i].tpage);
    }
    
    for (i = 0; i < mod8; i++) {
        addPrim(&primLists[whichDrawDispEnv].gui1, &bonusWidgetSpritesPiechart[whichDrawDispEnv][i].tpage);
    }
    
    addPrim(&primLists[whichDrawDispEnv].gui1, &bonusWidgetSpritesPiechart[whichDrawDispEnv][8].tpage);
}
