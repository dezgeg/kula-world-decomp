#include "common.h"

extern uint Rand(int param_1);

extern int dispenvScreenX;
extern int dispenvScreenY;
extern DrawDisp drawdisp[2];
extern int whichDrawDispEnv;

int screenShakeDirectionMask;
int screenShakeDivisor;
int screenShakeEnabled;
int screenShakeMagnitude;
int screenShakeTimer;

static inline void SetDispScreen(ushort x, ushort y) {
    drawdisp[0].disp.screen.x = drawdisp[1].disp.screen.x = x;
    drawdisp[0].disp.screen.y = drawdisp[1].disp.screen.y = y;
}

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

void ProcessScreenShake(void) {
    int rng;
    int v;

    if (screenShakeEnabled == 1) {
        screenShakeTimer--;
        if (screenShakeTimer < 0) {
            screenShakeEnabled = 0;
            SetDispScreen(dispenvScreenX, dispenvScreenY);
        } else {
            if ((screenShakeDirectionMask & 1) != 0) {
                rng = Rand(screenShakeMagnitude << 1);
                v = dispenvScreenX + ((rng - screenShakeMagnitude) * screenShakeTimer) /
                                             screenShakeDivisor;
                if (v < 0) {
                    v = 0;
                }
                drawdisp[whichDrawDispEnv].disp.screen.x = v;
            }
            if ((screenShakeDirectionMask & 2) != 0) {
                rng = Rand(screenShakeMagnitude << 1);
                v = dispenvScreenY + ((rng - screenShakeMagnitude) * screenShakeTimer) /
                                             screenShakeDivisor;
                if (v < 0) {
                    v = 0;
                }
                drawdisp[whichDrawDispEnv].disp.screen.y = v;
            }
        }
    }
}
