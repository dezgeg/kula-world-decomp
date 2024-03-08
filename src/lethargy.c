#include "common.h"

extern int numCameras;
extern unsigned lethargyMode;
extern unsigned gteXScale;
extern unsigned gteYScale;

void EnableLethargy(int enable) {
    // HACK: don't use GP-relative for these variables
    __asm__(".extern numCameras,999");
    if (numCameras < 2) {
        if (enable == 1) {
            lethargyMode = enable;
        } else if (lethargyMode) {
            lethargyMode = 3;
        }
    }
}

void ResetLethargyEffect() {
    // HACK: don't use GP-relative for these variables
    __asm__(".extern gteXScale,999");
    __asm__(".extern gteYScale,999");
    lethargyMode = 0;
    gteXScale = 0x1000;
    gteYScale = 0x1000;
}
