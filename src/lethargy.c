#include "common.h"

static int lethargyAngleCounter;
static int lethargyColorCounter;
static POLY_FT4 lethargyEffectPoly[2];
static int lethargyMode;
static int lethargySinMagnitude;
static int lethargyUnusedCounter;

extern int displayHeight;
extern int displayWidth;
extern int gteXScale;
extern int gteYScale;
extern TSprite lethargyEffectSprite[2][2];
extern int numCameras;
extern PrimList primLists[2];
extern int whichDrawDispEnv;

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
            lethargyUnusedCounter = (lethargyUnusedCounter + 0x80) % 0x1000;
            rsin(lethargyUnusedCounter);
            setRGB0(&lethargyEffectSprite[whichDrawDispEnv][0].sprt, 96, 112, 96);
            AddPrim(&primLists[whichDrawDispEnv].main, &lethargyEffectSprite[whichDrawDispEnv][0]);
            if (displayWidth > 256) {
                setRGB0(&lethargyEffectSprite[whichDrawDispEnv][1].sprt, 96, 112, 96);
                AddPrim(&primLists[whichDrawDispEnv].main, &lethargyEffectSprite[whichDrawDispEnv][1]);
            }
            break;
        case 3:
            lethargyMode = 4;
            lethargyColorCounter = 0x50;
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
        lethargySinMagnitude -= 6;
        if (lethargySinMagnitude > 0) {
            lethargyAngleCounter = (lethargyAngleCounter + 0x100) % 0x1000;
            delta = rsin(lethargyAngleCounter) * lethargySinMagnitude >> 12;
            gteXScale = delta + 0x1000;
            gteYScale = 0x1000 - delta;
        }
    }
}
