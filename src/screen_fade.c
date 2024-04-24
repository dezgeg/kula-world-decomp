#include "common.h"

int halfFps;
int screenFadeEnabled;
int screenFadeSpeed;
int screenFadeColor;

extern void* otag[2][1][1026];
extern int cameraIndex;
extern TPolyF4 screenFadePolys[2][1];
extern int whichDrawDispEnv;

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

void DrawScreenFade(void) {
    if (screenFadeEnabled == 1) {
        switch (screenFadeSpeed) {
            case 0:
                screenFadeColor = screenFadeColor + -24;
                break;
            case 1:
                screenFadeColor = screenFadeColor + -16;
                break;
            case 2:
                screenFadeColor = screenFadeColor + -8;
                break;
        }
        if (screenFadeColor >= 1) {
            setRGB0(&screenFadePolys[whichDrawDispEnv][cameraIndex].poly, screenFadeColor,
                    screenFadeColor, screenFadeColor);
            addPrim(&otag[whichDrawDispEnv][cameraIndex],
                    &screenFadePolys[whichDrawDispEnv][cameraIndex]);
        } else {
            screenFadeEnabled = 0;
            halfFps = 0;
        }
    }
}
