#include "common.h"

int replayEnded;
ushort replayPlaybackCurrentButtons;
char* replayPlaybackPtr;
char replayPlaybackRleCounter;
char replayRawButtons;

int InitReplayPlayback(char* param_1) {
    uint rawBtns;

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
