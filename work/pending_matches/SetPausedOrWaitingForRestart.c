#include "common.h"

extern void HandlePauseModeRotationEffect(Player *player);
short isPausedOrWaitingForRestart;
extern Player thePlayer;

void SetPausedOrWaitingForRestart(void) {
    isPausedOrWaitingForRestart = 1;
    HandlePauseModeRotationEffect(&thePlayer);
}
