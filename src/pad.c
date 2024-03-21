#include "common.h"

#include <LIBPAD.H>

u_char VIBRATION_ACT_ALIGN[] = { 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, };
int vibrationEnabled = 1;

extern int isDemoMode;

int vibrationMode;
int vibrationSinPhase;
int vibrationCounter;
int vibrationSinConst;
int vibrationSinMagnitude;
int vibrationAngleIncrement;
int vibrationCounterMax;
int latestControllerSlotPolled;
char* vibrationSeqPtr;

extern u_char padVibrationModeEntered[];
extern u_char vibrationBuf[][2];

extern u_char padData[2][34];

extern void ResetVibration();

extern char* VIBRATION_SEQS[];

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
