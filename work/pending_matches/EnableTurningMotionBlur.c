#include "common.h"

extern int turningMotionBlurEnabled;
extern int turningMotionBlurPhase;

void EnableTurningMotionBlur(void) {
    if (turningMotionBlurEnabled == 0) {
        turningMotionBlurEnabled = 1;
        turningMotionBlurPhase = 0;
    }
}
