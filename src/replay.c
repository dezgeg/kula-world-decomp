#include "common.h"

extern int displayHeight;
extern int displayWidth;

int randomSeed = 1;

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

#ifndef SKIP_UNUSED_CODE
void UnusedLoadFullScreenPicture(void* param_1) {
    int halfPaddingY;
    int halfPaddingX;
    int height;
    uint heightAndWidth;
    int width;
    RECT rect;

    heightAndWidth = *(uint*)(param_1 + 0x10);
    width = heightAndWidth & 0xffff;
    height = heightAndWidth >> 0x10;
    halfPaddingX = (displayWidth - width) / 2;
    halfPaddingY = (displayHeight - height) / 2;
    DrawSync(0);

    rect.x = 0;
    rect.y = 0;
    rect.w = displayWidth * 2;
    rect.h = displayHeight;
    ClearImage(&rect, 0, 0, 0);

    rect.x = halfPaddingX;
    rect.y = halfPaddingY;
    rect.w = width;
    rect.h = height;
    LoadImage(&rect, param_1 + 0x14);

    rect.x = displayWidth + halfPaddingX;
    rect.y = halfPaddingY;
    rect.w = width;
    rect.h = height;
    LoadImage(&rect, param_1 + 0x14);

    DrawSync(0);
}
#endif

void Srand(int seed) {
    randomSeed = seed;
}

int Rand(unsigned v) {
    int seed = randomSeed * 0x41c64e6d + 12345;
    randomSeed = seed;
    return (v * (seed & 0xFFFF)) >> 16;
}
