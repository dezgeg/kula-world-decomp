#include "common.h"
#include <LIBPAD.H>

void ResetVibration();
void SetupDisplay(int isbg, int bgR, int bgG, int bgB, int useDithering, int use24Bit);

extern int displayHeight;
extern int displayWidth;
extern int isDemoMode;
extern int screenOffsetX;
extern int screenOffsetY;
extern int whichDrawDispEnv;
extern long projectionDistance;

DR_AREA drawAreas[2][1];
DR_AREA hudDrawAreas[2];
DrawDisp drawdisp[2];
DR_OFFSET drawOffsets[2][1];
DR_OFFSET hudDrawOffsets[2];
int dispenvScreenX;
int dispenvScreenY;

static unsigned char VIBRATION_ACT_ALIGN[6] = { 0x00, 0x01, 0xff, 0xff, 0xff, 0xff };
int vibrationEnabled = 1;

static char* vibrationSeqPtr;
STATIC_FOR_GP_ACCESS int latestControllerSlotPolled;
STATIC_FOR_GP_ACCESS u_char padData[2][34];
STATIC_FOR_GP_ACCESS u_char padVibrationModeEntered[2];
static int lastPadPollFrame;
static int vibrationAngleIncrement;
static int vibrationCounter;
static int vibrationCounterMax;
static int vibrationDecayIntensity;
static int vibrationDecayTimer;
static int vibrationDirectSmallMotor;
static int vibrationLargeMotor;
static int vibrationMode;
static int vibrationSinConst;
static int vibrationSinMagnitude;
static int vibrationSinPhase;
static int vibrationSmallMotor;
static u_char vibrationBuf[2][2];

static char VIBRATION_SEQ_DATA[];

static char* VIBRATION_SEQS[] = {
    &VIBRATION_SEQ_DATA,
    &VIBRATION_SEQ_DATA[90],
};

static char VIBRATION_SEQ_DATA[] = {
    0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x00, 0x80, 0x00, 0x7d, 0x00, 0x7a, 0x00, 0x77,
    0x00, 0x74, 0x00, 0x71, 0x00, 0x6e, 0x00, 0x6b, 0x00, 0x68, 0x00, 0x65, 0x00, 0x62, 0x00, 0x5f,
    0x00, 0x5c, 0x00, 0x59, 0x00, 0x56, 0x00, 0x53, 0x00, 0x50, 0x00, 0x4d, 0x00, 0x4a, 0x00, 0x47,
    0x00, 0x44, 0x00, 0x41, 0x00, 0x3e, 0x00, 0x3b, 0x00, 0x38, 0x00, 0x35, 0x00, 0x32, 0x00, 0x2f,
    0x00, 0x2c, 0x00, 0x29, 0x00, 0x26, 0x00, 0x23, 0x00, 0x20, 0x00, 0x1d, 0x00, 0x1a, 0x00, 0x17,
    0x00, 0x14, 0x00, 0x11, 0x00, 0x0e, 0x00, 0x0b, 0xff, 0xff,
    // Can't make another symbol here, or gcc wants to add alignment padding
    0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0x64, 0x00, 0x61,
    0x00, 0x5e, 0x00, 0x5b, 0x00, 0x58, 0x00, 0x55, 0x00, 0x52, 0x00, 0x4f, 0x00, 0x4c, 0x00, 0x49,
    0x00, 0x46, 0x00, 0x43, 0x00, 0x40, 0x00, 0x3d, 0x00, 0x3a, 0x00, 0x37, 0x00, 0x34, 0x00, 0x31,
    0x00, 0x2e, 0x00, 0x2b, 0x00, 0x28, 0x00, 0x25, 0x00, 0x22, 0x00, 0x1f, 0x00, 0x1c, 0x00, 0x19,
    0x00, 0x16, 0x00, 0x13, 0x00, 0x10, 0x00, 0x0d, 0x00, 0x0a, 0x00, 0x07, 0x00, 0x04, 0xff, 0xff,
};
static char PAD[] = { 0x00, 0x00, 0x00, 0x00 }; // hack

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

int GetControllerButtons(int slot) {
    int status;

    latestControllerSlotPolled = slot;

    if (GetControllerStatus(slot) != 0) {
        status = PadGetState(slot << 4);
        if (status == 1) {
            padVibrationModeEntered[slot] = 0;
            ResetVibration();
        }
        if (!padVibrationModeEntered[slot]) {
            PadSetAct(slot << 4, &vibrationBuf[slot][0], 2);
            if (status == 2 || (status == 6 && PadSetActAlign(slot << 4, VIBRATION_ACT_ALIGN))) {
                padVibrationModeEntered[slot] = 1;
                ResetVibration();
            }
        }

        // XXX: pretty ugly
        return (~*(short*)&padData[slot][2]) & 0xffff;
    }
    return 0;
}

int GetControllerStatus(int slot) {
    unsigned typeMsb;

    if (padData[slot][0] != 0) {
        return 0;
    }
    typeMsb = padData[slot][1] >> 4;
    if (typeMsb == 4 || typeMsb == 5 || typeMsb == 7) {
        return 1;
    }
    return 0;
}

#ifndef SKIP_UNUSED_CODE
int Return1(void) {
    return 1;
}

void VibrateDirectlyToBuf(int value) {
    if (vibrationEnabled) {
        vibrationBuf[latestControllerSlotPolled][0] = value;
    }
}
#endif

void Vibrate100(int constant, int magnitude, int angleIncrement, int max) {
    if (vibrationEnabled) {
#if !VER_JP
        vibrationBuf[latestControllerSlotPolled][0] = 0;
#endif
        vibrationMode = 100;
        vibrationSinConst = constant;
        vibrationSinMagnitude = magnitude;
        vibrationSinPhase = 0;
        vibrationAngleIncrement = angleIncrement;
        vibrationCounterMax = max;
        vibrationCounter = 1;
    }
}

void Vibrate101(int param_1) {
    if (vibrationEnabled && vibrationMode == 100) {
        vibrationMode = 101;
        vibrationCounterMax = param_1;
        vibrationCounter = param_1;
    }
}

void Vibrate99(int magnitude1, int magnitude2, int count) {
    if (vibrationEnabled) {
        vibrationMode = 99;
        vibrationCounter = count;
        vibrationBuf[latestControllerSlotPolled][0] = magnitude1;
        vibrationBuf[latestControllerSlotPolled][1] = magnitude2;
    }
}

void Vibrate98(int seq) {
    if (vibrationEnabled) {
        vibrationMode = 98;
        vibrationSeqPtr = VIBRATION_SEQS[seq];
    }
}

void UpdateVibration(void) {
    int amount;
    int res;
    int phi;
    int phi2;

    if (vibrationEnabled == 0) {
        return;
    }
    if (isDemoMode == 1) {
        vibrationMode = -1;
        vibrationBuf[latestControllerSlotPolled][0] = 0;
        vibrationBuf[latestControllerSlotPolled][1] = 0;
        return;
    }
    switch (vibrationMode) {
        case 98:
            if (*vibrationSeqPtr != 0xff) {
                vibrationBuf[latestControllerSlotPolled][0] = *vibrationSeqPtr++;
                vibrationBuf[latestControllerSlotPolled][1] = *vibrationSeqPtr++;
                return;
            }
            vibrationMode = -1;
            vibrationBuf[latestControllerSlotPolled][0] = 0;
            vibrationBuf[latestControllerSlotPolled][1] = 0;
            return;
        case 99:
            vibrationCounter--;
            if (vibrationCounter <= -1) {
                vibrationMode = -1;
                vibrationBuf[latestControllerSlotPolled][0] = 0;
                vibrationBuf[latestControllerSlotPolled][1] = 0;
            }
            return;
        case 100:
            vibrationCounter++;
            if (vibrationCounterMax < vibrationCounter) {
                vibrationCounter = vibrationCounterMax;
            }
            break;
        case 101:
            vibrationCounter--;
            if (vibrationCounter < 0) {
                vibrationMode = -1;
                return;
            }
            break;
        default:
            return;
    }

    // looks weird, maybe some simpler expression expanded?
    phi = vibrationSinPhase + vibrationAngleIncrement;
    if (phi < 0) {
        phi2 = phi + 0xfff;
    } else {
        phi2 = phi;
    }
    vibrationSinPhase = phi + (phi2 >> 0xc) * -0x1000;
    amount = rsin(vibrationSinPhase);
    res = (((vibrationSinConst + (amount * vibrationSinMagnitude >> 0xc)) * vibrationCounter) / vibrationCounterMax);
    vibrationBuf[latestControllerSlotPolled][1] = res;
}

void ResetVibration(void) {
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            vibrationBuf[i][j] = 0;
        }
    }
    vibrationMode = -1;
#if VER_JP
    vibrationDirectSmallMotor = 0;
    vibrationSmallMotor = 0;
    vibrationLargeMotor = 0;
    vibrationDecayTimer = -1;
#endif
}

#if VER_JP
void VibrateDecay(int intensity) {
    vibrationDecayTimer = 24;
    vibrationDecayIntensity = intensity;
    if (intensity < 0) {
        vibrationDecayIntensity = 0;
    }
    if (vibrationDecayIntensity > 100) {
        vibrationDecayIntensity = 100;
    }
}
#endif
