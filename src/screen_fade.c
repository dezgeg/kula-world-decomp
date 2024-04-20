#include "common.h"

int halfFps;
int screenFadeEnabled;
int screenFadeSpeed;
int screenFadeColor;

void SetRenderScreenFade(int speed, int enableHalfFps) {
    screenFadeEnabled = 1;
    halfFps = enableHalfFps;
    screenFadeSpeed = speed;

    switch (speed) {
    case 1:
        screenFadeColor = 264;
        break;
    case 0:
        screenFadeColor = 272;
        break;
    case 2:
        screenFadeColor = 256;
        break;
    }
}
