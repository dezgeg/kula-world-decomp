#include "common.h"

extern void ResetVibration(void);

extern char padData[2][34];
extern int displayHeight;
extern int displayWidth;
extern int screenOffsetX;
extern int screenOffsetY;
extern int whichDrawDispEnv;
extern long projectionDistance;
extern u_char padVibrationModeEntered[2];

DR_AREA drawAreas[2][1];
DR_AREA hudDrawAreas[2];
DR_OFFSET drawOffsets[2][1];
DR_OFFSET hudDrawOffsets[2];
DrawDisp drawdisp[2];
int dispenvScreenX;
int dispenvScreenY;

STATIC_FOR_GP_ACCESS int latestControllerSlotPolled;

void SetupDisplay(int isbg, int bgR, int bgG, int bgB, int useDithering, int use24Bit);

static inline void SetDispScreen(ushort x, ushort y) {
    drawdisp[0].disp.screen.x = drawdisp[1].disp.screen.x = x;
    drawdisp[0].disp.screen.y = drawdisp[1].disp.screen.y = y;
}

void InitStuff(void) {
    int j;
    int i;
    RECT rect;

#if VER_JP
    SetDefDrawEnv(&drawdisp[0].draw, 0, 0, 320, SCREEN_HEIGHT);
    drawdisp[0].draw.isbg = 1;
    setRGB0(&drawdisp[0].draw, 0, 0, 0);
    SetDefDispEnv(&drawdisp[0].disp, 0, 0, 320, SCREEN_HEIGHT);
    drawdisp[0].disp.screen.x = 0;
    drawdisp[0].disp.screen.y = 0;
    ResetGraph(0);
    PutDrawEnv(&drawdisp[0].draw);
    PutDispEnv(&drawdisp[0].disp);
    ResetCallback();
    SetVideoMode(VID_NTSC ? 0 : 1);
    SetGraphDebug(0);
    InitGeom();
    SetGeomScreen(projectionDistance);

    setRECT(&rect, 0, 0, 640, SCREEN_HEIGHT);
    ClearImage2(&rect, 0, 0, 0);
    DrawSync(0);
    SetDispMask(1);
    SetupDisplay(0, 0, 0, 0, 0, 0);
#else
    ResetCallback();
    ResetGraph(0);
    SetGraphDebug(0);
#endif
    MemCardInit(0);
    MemCardStart();
    PadInitDirect(&padData[0][0], &padData[1][0]);
    PadStartCom();

    for (i = 0; i < 2; i++) {
        padVibrationModeEntered[i] = 0;
        for (j = 0; j < 34; j++) {
            padData[i][j] = 0;
        }
    }

    ResetVibration();
    latestControllerSlotPolled = 0;
#if !VER_JP
    InitGeom();
    SetGeomScreen(projectionDistance);
    SetVideoMode(VID_NTSC ? 0 : 1);
    SetDispMask(1);
    rect.w = 640;
    rect.x = 0;
    rect.y = 0;
    rect.h = SCREEN_HEIGHT;
    ClearImage(&rect, 0, 0, 0);
    SetupDisplay(0, 0, 0, 0, 0, 0);
#endif
    FntLoad(576, 256);
    SetDumpFnt(FntOpen(16, 8, displayWidth - 16, displayHeight - 8, 0,
                       (displayWidth / 8 - 2) * (displayHeight / 8 - 1)));
}

void PutDrawAndDispEnvs(void) {
    PutDrawEnv(&drawdisp[whichDrawDispEnv].draw);
    PutDispEnv(&drawdisp[whichDrawDispEnv].disp);
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
