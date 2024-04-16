#include "common.h"

int replayEnded;
short replayPlaybackCurrentButtons;
char* replayPlaybackPtr;
char replayPlaybackRleCounter;
char replayRawButtons;

int InitReplayPlayback(char* param_1) {
    uchar rawBtns;

    replayEnded = 0;
    replayPlaybackPtr = param_1 + 21;
    replayPlaybackRleCounter = param_1[20];
    replayPlaybackPtr = param_1 + 22;
    rawBtns = param_1[21];
    replayRawButtons = param_1[21];
    replayPlaybackCurrentButtons = ((rawBtns << 4) & 0xf0) | ((rawBtns << 5) & 0x200) |
                                   ((rawBtns << 6) & 0x800) | ((rawBtns << 8) & 0x4000);
    return *(int*)(param_1 + 16);
}

int GetButtonsFromReplay(void) {
    uchar rawBtns;

    if (!replayEnded) {
        if (replayPlaybackRleCounter != 0) {
            replayPlaybackRleCounter--;
        } else {
            replayPlaybackRleCounter = *replayPlaybackPtr++ - 1;
            rawBtns = *replayPlaybackPtr++;
            replayRawButtons = rawBtns;
            if (rawBtns == 0xff) {
                replayEnded = 1;
                return 0;
            }
            replayPlaybackCurrentButtons = ((rawBtns << 4) & 0xf0) | ((rawBtns << 5) & 0x200) |
                                           ((rawBtns << 6) & 0x800) | ((rawBtns << 8) & 0x4000);
        }
        return replayPlaybackCurrentButtons;
    }
    return 0;
}
