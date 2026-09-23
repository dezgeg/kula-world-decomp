#include "common.h"

extern void InitLensFlareSprites(void);
extern void InitLevelEndScreen(void);
extern void InitParticles(void);
extern void ResetTextRenderState(void);
extern void ResetTextVars(void);

void VariousInit(void) {
    InitParticles();
    InitLensFlareSprites();
    ResetTextRenderState();
    ResetTextVars();
    InitLevelEndScreen();
}

#if VER_US
void ClearScreen(void) {
    RECT rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = 640;
    rect.h = 240;
    VSync(0);
    DrawSync(0);
    ClearImage(&rect, 0, 0, 0);
    DrawSync(0);
}
#endif
