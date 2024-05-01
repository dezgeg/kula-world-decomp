#include "common.h"

int dispenvScreenX;
int dispenvScreenY;

extern DR_AREA drawAreas[2][1];
extern DR_AREA hudDrawAreas[2];
extern DR_OFFSET drawOffsets[2][1];
extern DR_OFFSET hudDrawOffsets[2];
extern DrawDisp drawdisp[2];
extern int displayHeight;
extern int displayWidth;
extern int screenOffsetX;
extern int screenOffsetY;
extern int whichDrawDispEnv;

void PutDrawAndDispEnvs(void) {
    PutDrawEnv(&drawdisp[whichDrawDispEnv].draw);
    PutDispEnv(&drawdisp[whichDrawDispEnv].disp);
}

static inline void SetDispScreen(ushort x, ushort y) {
    drawdisp[0].disp.screen.x = drawdisp[1].disp.screen.x = x;
    drawdisp[0].disp.screen.y = drawdisp[1].disp.screen.y = y;
}

void SetupDisplay(int isbg, int bgR, int bgG, int bgB, int useDithering, int use24Bit) {
    int i;
    int j;
    short offs[2];
    RECT rect;
    int xOff;
    int yOff;

    DrawSync(0);
    VSync(0);

    SetDefDispEnv(&drawdisp[0].disp, 0, 0, displayWidth, displayHeight);
    SetDefDrawEnv(&drawdisp[0].draw, displayWidth, 0, displayWidth, displayHeight);
    SetDefDispEnv(&drawdisp[1].disp, displayWidth, 0, displayWidth, displayHeight);
    SetDefDrawEnv(&drawdisp[1].draw, 0, 0, displayWidth, displayHeight);

    xOff = 0;
    yOff = 0;
    for (i = 0; i < 1; i++) {
        for (j = 0; j < 2; j++) {
            rect.x = xOff + j * displayWidth;
            rect.y = yOff;
            rect.w = screenOffsetX;
            rect.h = screenOffsetY;
            SetDrawArea(&drawAreas[j][i], &rect);
            offs[0] = xOff + j * displayWidth;
            offs[1] = yOff;
            SetDrawOffset(&drawOffsets[j][i], offs);
        }
        xOff += screenOffsetX;
        if (xOff >= displayWidth) {
            xOff = 0;
            yOff += screenOffsetY;
        }
    }

    for (i = 0; i < 2; i++) {
        rect.x = i * displayWidth;
        rect.y = 0;
        rect.w = displayWidth;
        rect.h = displayHeight;
        SetDrawArea(&hudDrawAreas[i], &rect);
        offs[0] = i * displayWidth;
        offs[1] = 0;
        SetDrawOffset(&hudDrawOffsets[i], offs);
    }

    if (displayHeight > 256) {
        yOff = displayHeight / 2;
    } else {
        yOff = displayHeight;
    }
    SetDispScreen(dispenvScreenX, dispenvScreenY);
    drawdisp[0].disp.screen.h = drawdisp[1].disp.screen.h = yOff;
    drawdisp[0].draw.dtd = drawdisp[1].draw.dtd = useDithering;
    drawdisp[0].draw.isbg = drawdisp[1].draw.isbg = isbg;
    drawdisp[0].disp.isrgb24 = drawdisp[1].disp.isrgb24 = use24Bit;
    setRGB0(&drawdisp[0].draw, bgR, bgG, bgB);
    setRGB0(&drawdisp[1].draw, bgR, bgG, bgB);
    SetGeomOffset(screenOffsetX / 2, screenOffsetY / 2);
}
