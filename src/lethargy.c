#include "common.h"

unsigned lethargyMode;

extern int numCameras;
extern unsigned gteXScale;
extern unsigned gteYScale;

void EnableLethargy(int enable) {
    if (numCameras < 2) {
        if (enable == 1) {
            lethargyMode = enable;
        } else if (lethargyMode) {
            lethargyMode = 3;
        }
    }
}

void ResetLethargyEffect() {
    lethargyMode = 0;
    gteXScale = 0x1000;
    gteYScale = 0x1000;
}
