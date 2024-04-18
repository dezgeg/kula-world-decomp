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
