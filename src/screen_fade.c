#include "common.h"

extern int cameraIndex;
extern int displayHeight;
extern int displayWidth;
extern int gteXScale;
extern int gteYScale;
extern int numCameras;
extern int whichDrawDispEnv;
extern OT_TYPE otag[2][1][1026];
extern PrimList primLists[2];
extern TSprite lethargyEffectSprite[2][2];

int halfFps;
int screenFadeEnabled;
TPolyF4 screenFadePolys[2][1];

STATIC_FOR_GP_ACCESS int lethargyMode;
STATIC_FOR_GP_ACCESS int screenFadeColor;
STATIC_FOR_GP_ACCESS int screenFadeSpeed;
STATIC_FOR_GP_ACCESS POLY_FT4 lethargyEffectPoly[2];
static int lethargyAngleCounter;
static int lethargyColorCounter;
static int lethargySinMagnitude;
static int lethargyUnusedCounter;

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
    gteYScale = 16 * SCREEN_HEIGHT;
}

void DrawLethargyEffects(void) {
    int delta;

    switch (lethargyMode) {
        case 1:
            lethargyMode = 2;
            setXY4(&lethargyEffectPoly[whichDrawDispEnv],
                    0, 0,
                    displayWidth + 1, 0,
                    0, displayHeight + 1,
                    displayWidth + 1, displayHeight + 1);
            setRGB0(&lethargyEffectPoly[whichDrawDispEnv], 0x80, 0x80, 0x80);
            AddPrim(&primLists[whichDrawDispEnv].main, &lethargyEffectPoly[whichDrawDispEnv]);
            lethargyUnusedCounter = 0x400;
            lethargyAngleCounter = 0x400;
            lethargySinMagnitude = 0x30c;
            break;
        case 2:
            lethargyUnusedCounter = (lethargyUnusedCounter + VELOCITY(0x80)) % 0x1000;
            rsin(lethargyUnusedCounter);
            setRGB0(&lethargyEffectSprite[whichDrawDispEnv][0].sprt, 96, 112, 96);
            AddPrim(&primLists[whichDrawDispEnv].main, &lethargyEffectSprite[whichDrawDispEnv][0]);
            if (displayWidth > 256) {
                setRGB0(&lethargyEffectSprite[whichDrawDispEnv][1].sprt, 96, 112, 96);
                AddPrim(&primLists[whichDrawDispEnv].main, &lethargyEffectSprite[whichDrawDispEnv][1]);
            }
            break;
        case 3:
            setXY4(&lethargyEffectPoly[0],
                    0, 0,
                    displayWidth + 1, 0,
                    0, displayHeight + 1,
                    displayWidth + 1, displayHeight + 1);
            setXY4(&lethargyEffectPoly[1],
                    0, 0,
                    displayWidth + 1, 0,
                    0, displayHeight + 1,
                    displayWidth + 1, displayHeight + 1);
            lethargyMode = 4;
            lethargyColorCounter = 0x50;

            AddPrim(&primLists[whichDrawDispEnv].main, &lethargyEffectSprite[whichDrawDispEnv][0]);
            if (displayWidth > 256) {
                AddPrim(&primLists[whichDrawDispEnv].main, &lethargyEffectSprite[whichDrawDispEnv][1]);
            }
            break;
        case 4:
            lethargyColorCounter -= 8;
            if (lethargyColorCounter >= 0) {
                setRGB0(&lethargyEffectPoly[whichDrawDispEnv], lethargyColorCounter, lethargyColorCounter, lethargyColorCounter);
                AddPrim(&primLists[whichDrawDispEnv].main, &lethargyEffectPoly[whichDrawDispEnv]);
            } else {
                lethargyMode = 0;
            }
            break;
    }

    if (lethargyMode != 0) {
        lethargySinMagnitude -= VELOCITY(6);
        if (lethargySinMagnitude > 0) {
            lethargyAngleCounter = (lethargyAngleCounter + VELOCITY(0x100)) % 0x1000;
            delta = rsin(lethargyAngleCounter) * lethargySinMagnitude >> 12;
            gteXScale = 0x1000 + delta;
            gteYScale = 16 * SCREEN_HEIGHT - delta;
        }
    }
}

void SetRenderScreenFade(int speed, int enableHalfFps) {
    screenFadeEnabled = 1;
    halfFps = enableHalfFps;
    screenFadeSpeed = speed;

    switch (speed) {
        case 0:
            screenFadeColor = VID_NTSC ? 268 : 272;
            break;
        case 1:
            screenFadeColor = VID_NTSC ? 261 : 264;
            break;
        case 2:
            screenFadeColor = VID_NTSC ? 254 : 256;
            break;
    }
}

void DrawScreenFade(void) {
    if (screenFadeEnabled == 1) {
        switch (screenFadeSpeed) {
            case 0:
                screenFadeColor -= VELOCITY(24);
                break;
            case 1:
                screenFadeColor -= VELOCITY(16);
                break;
            case 2:
                screenFadeColor -= VELOCITY(8);
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
