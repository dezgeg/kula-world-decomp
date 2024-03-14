#include "common.h"

#include <LIBPAD.H>

extern int latestControllerSlotPolled;
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
