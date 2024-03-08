#include "common.h"

extern unsigned lethargyMode;
extern unsigned gteXScale;
extern unsigned gteYScale;

void ResetLethargyEffect() {
    // HACK: don't use GP-relative for these variables
    __asm__(".extern gteXScale,999");
    __asm__(".extern gteYScale,999");
    lethargyMode = 0;
    gteXScale = 0x1000;
    gteYScale = 0x1000;
}
