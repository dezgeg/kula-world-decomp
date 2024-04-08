#include "common.h"

int screenShakeDirectionMask;
int screenShakeDivisor;
int screenShakeEnabled;
int screenShakeMagnitude;
int screenShakeTimer;

void DisableScreenShake(void) {
    screenShakeEnabled = 0;
}

void EnableScreenShake(int param_1, int param_2, int param_3) {
    screenShakeMagnitude = param_1;
    screenShakeTimer = param_2;
    screenShakeDivisor = param_2;
    screenShakeDirectionMask = param_3;
    screenShakeEnabled = 1;
}
