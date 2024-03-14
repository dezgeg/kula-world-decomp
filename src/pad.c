#include "common.h"

#include <LIBPAD.H>

extern int latestControllerSlotPolled;
extern int vibrationEnabled;
extern u_char padData[2][34];
extern u_char padVibrationModeEntered[];
extern u_char vibrationBuf[][2];

extern void ResetVibration();

extern u_char VIBRATION_ACT_ALIGN[];

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

int Return1(void) {
    return 1;
}

void VibrateDirectlyToBuf(char value) {
  if (vibrationEnabled != 0) {
    vibrationBuf[latestControllerSlotPolled][0] = value;
  }
}
