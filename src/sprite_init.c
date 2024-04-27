#include "common.h"

extern void InitAllDigitSprites(void);
extern void InitBonusWidgetSprites(void);
extern void InitCopyrightSprite(void);
extern void InitFruitSprites(void);
extern void InitHourglassSprites(void);
extern void InitKeySprites(void);
extern void InitLethargyEffectSprites(void);
extern void InitScreenFadePolys(void);
extern void InitTimerPausedSprite(void);
extern void InitTitleSprite(void);

void InitManySprites(void) {
    InitHourglassSprites();
    InitLethargyEffectSprites();
    InitKeySprites();
    InitBonusWidgetSprites();
    InitFruitSprites();
    InitScreenFadePolys();
    InitTitleSprite();
    InitCopyrightSprite();
    InitTimerPausedSprite();
    InitAllDigitSprites();
}
