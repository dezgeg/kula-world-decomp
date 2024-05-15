#include "common.h"

int dispModeChangeNewWidth;
int dispModeChangeState;
int displayHeight;
int displayWidth;
int gteXScale;

extern DrawDisp drawdisp[2];

void UnusedSetDisplayModeChange(int param_1) {
    dispModeChangeNewWidth = param_1;
    dispModeChangeState = 1;
}

void UnusedProcessDisplayModeChange(void) {
    switch (dispModeChangeState) {
        case 1:
            displayWidth = dispModeChangeNewWidth;
            gteXScale = (dispModeChangeNewWidth << 12) / 320;
            SetGeomOffset(dispModeChangeNewWidth / 2, displayHeight / 2);
            dispModeChangeState = 2;
            break;
        case 2:
            dispModeChangeState = 3;
            drawdisp[0].draw.clip.w = displayWidth;
            drawdisp[1].draw.clip.w = displayWidth;
            break;
        case 3:
            dispModeChangeState = 0;
            drawdisp[0].disp.disp.w = displayWidth;
            drawdisp[1].disp.disp.w = displayWidth;
            break;
    }
}
