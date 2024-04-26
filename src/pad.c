#include "common.h"

#include <LIBPAD.H>

extern int isDemoMode;

static unsigned char VIBRATION_ACT_ALIGN[6] = { 0x00, 0x01, 0xff, 0xff, 0xff, 0xff };
int vibrationEnabled = 1;

// sbss offset 0x94420..0x94490
static int latestControllerSlotPolled;
static u_char padData[2][34];
static u_char vibrationBuf[2][2];
static u_char padVibrationModeEntered[];
static int vibrationMode;
static int vibrationSinConst;
static int vibrationSinMagnitude;
static int vibrationSinPhase;
static int vibrationAngleIncrement;
static int vibrationCounterMax;
static int vibrationCounter;
static char* vibrationSeqPtr;

extern void ResetVibration();

static char VIBRATION_SEQ_DATA[];

static char* VIBRATION_SEQS[] = {
    &VIBRATION_SEQ_DATA,
    &VIBRATION_SEQ_DATA[0] + 90,
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
static char PAD[] = { 0x00 }; // hack

int GetControllerButtons(int slot) {
    int status;

    latestControllerSlotPolled = slot;

    status = GetControllerStatus(slot);
    if (status != 0) {
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

    if (padData[slot][0] == 0) {
        typeMsb = padData[slot][1] >> 4;
        if (typeMsb == 4 || typeMsb == 5 || typeMsb == 7) return 1;
    }
    return 0;
}

// unused
int Return1(void) { return 1; }

// unused
void VibrateDirectlyToBuf(char value) {
    if (vibrationEnabled) {
        vibrationBuf[latestControllerSlotPolled][0] = value;
    }
}

void Vibrate100(int constant, int magnitude, int angleIncrement, int max) {
    if (vibrationEnabled) {
        vibrationBuf[latestControllerSlotPolled][0] = 0;
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
    res = (((vibrationSinConst + (amount * vibrationSinMagnitude >> 0xc)) * vibrationCounter) /
           vibrationCounterMax);
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
}
