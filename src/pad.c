#include "common.h"

#include <LIBPAD.H>

extern int isDemoMode;

extern int vibrationEnabled;
extern int vibrationMode;
extern int vibrationSinPhase;
extern int vibrationCounter;
extern int vibrationSinConst;
extern int vibrationSinMagnitude;
extern int vibrationAngleIncrement;
extern int vibrationCounterMax;
extern char* vibrationSeqPtr;
extern u_char padVibrationModeEntered[];
extern u_char vibrationBuf[][2];

extern int latestControllerSlotPolled;
extern u_char padData[2][34];

extern void ResetVibration();

extern u_char VIBRATION_ACT_ALIGN[];
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
    return (~*(short *)&padData[slot][2]) & 0xffff;
  }
    return 0;
}

int GetControllerStatus(int slot) {
  unsigned typeMsb;

  if (padData[slot][0] == 0) {
    typeMsb = padData[slot][1] >> 4;
    if (typeMsb == 4 || typeMsb == 5 || typeMsb == 7)
        return 1;
  }
  return 0;
}

// unused
int Return1(void) {
    return 1;
}

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
  // HACK: don't use GP-relative for these variables
  __asm__(".extern isDemoMode,999");
}

void Vibrate98(int seq) {
  if (vibrationEnabled) {
    vibrationMode = 98;
    vibrationSeqPtr = VIBRATION_SEQS[seq];
  }
}

#if 0
void UpdateVibration(void) {
  int amount;
  int res;
  int phi;
  int phi2;

  if (vibrationEnabled == 0) {
    return;
  }
  if (isDemoMode != 1) {
      if (vibrationMode != 99) {
        if (vibrationMode < 100) {
          if (vibrationMode == 98) {
            if (*vibrationSeqPtr != -1) {
                vibrationBuf[latestControllerSlotPolled][0] = *vibrationSeqPtr++;
                vibrationBuf[latestControllerSlotPolled][1] = *vibrationSeqPtr++;
                return;
            }
          } else {
              return;
          }
        } else {
          if (vibrationMode == 100) {
              vibrationCounter++;
              if (vibrationCounterMax < vibrationCounter) {
                  vibrationCounter = vibrationCounterMax;
              }
          } else if (vibrationMode == 101) {
             vibrationCounter--;
             if (vibrationCounter < 0) {
               vibrationMode = -1;
               return;
             }
          }

          phi = vibrationSinPhase + vibrationAngleIncrement;
          if (phi < 0) {
            phi2 = phi + 0xfff;
          } else {
              phi2 = phi;
          }
          vibrationSinPhase = phi + (phi2 >> 0xc) * -0x1000;
          amount = rsin(vibrationSinPhase);
          res = (((vibrationSinConst + (amount * vibrationSinMagnitude >> 0xc)) *
                         vibrationCounter) / vibrationCounterMax);
          vibrationBuf[latestControllerSlotPolled][1] = res;
          return;
        }
      } else {
        vibrationCounter = vibrationCounter + -1;
        if (vibrationCounter > -1) {
            return;
        }
      }
  }
  vibrationMode = -1;
  vibrationBuf[latestControllerSlotPolled][0] = 0;
  vibrationBuf[latestControllerSlotPolled][1] = 0;
}
#else
void UpdateVibration(void) {
  int amount;
  int res;
  int phi;
  int phi2;

  if (vibrationEnabled == 0) {
    return;
  }
  if (isDemoMode != 1) {
    switch(vibrationMode) {
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
            vibrationCounter = vibrationCounter + -1;
            if (vibrationCounter > -1) {
                return;
            }
            vibrationMode = -1;
            vibrationBuf[latestControllerSlotPolled][0] = 0;
            vibrationBuf[latestControllerSlotPolled][1] = 0;
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
    phi = vibrationSinPhase + vibrationAngleIncrement;
    if (phi < 0) {
    phi2 = phi + 0xfff;
    } else {
        phi2 = phi;
    }
    vibrationSinPhase = phi + (phi2 >> 0xc) * -0x1000;
    amount = rsin(vibrationSinPhase);
    res = (((vibrationSinConst + (amount * vibrationSinMagnitude >> 0xc)) *
                vibrationCounter) / vibrationCounterMax);
    vibrationBuf[latestControllerSlotPolled][1] = res;
    return;
  }
}
#endif
