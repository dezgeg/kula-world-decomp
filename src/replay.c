#include "common.h"

static int replayEnded;
static short replayPlaybackCurrentButtons;
static char* replayPlaybackPtr;
static char replayPlaybackRleCounter;
static char replayRawButtons;

int InitReplayPlayback(char* param_1) {
    replayEnded = 0;
    replayPlaybackPtr = param_1 + 20;
    replayPlaybackRleCounter = *replayPlaybackPtr++;
    replayRawButtons = *replayPlaybackPtr++;
    replayPlaybackCurrentButtons = ((replayRawButtons << 4) & 0xf0) | ((replayRawButtons << 5) & 0x200) |
                                   ((replayRawButtons << 6) & 0x800) | ((replayRawButtons << 8) & 0x4000);
    return *(int*)(param_1 + 16);
}

int GetButtonsFromReplay(void) {
    if (!replayEnded) {
        if (replayPlaybackRleCounter != 0) {
            replayPlaybackRleCounter--;
        } else {
            replayPlaybackRleCounter = *replayPlaybackPtr++;
            replayPlaybackRleCounter--;
            replayRawButtons = *replayPlaybackPtr++;
            if (replayRawButtons == 0xff) {
                replayEnded = 1;
                return 0;
            }
            replayPlaybackCurrentButtons = ((replayRawButtons << 4) & 0xf0) | ((replayRawButtons << 5) & 0x200) |
                                           ((replayRawButtons << 6) & 0x800) | ((replayRawButtons << 8) & 0x4000);
        }
        return replayPlaybackCurrentButtons;
    }
    return 0;
}
