#include "common.h"

int vsyncCounter;

INCLUDE_ASM("asm/nonmatchings/main", main);

void SioRecvVsyncCallback(void) {
    vsyncCounter = vsyncCounter + 1;
}

INCLUDE_ASM("asm/nonmatchings/main", MainGameLoop);

INCLUDE_ASM("asm/nonmatchings/main", LevelCompletedOrDied);

INCLUDE_ASM("asm/nonmatchings/main", DecideNextLevel);

INCLUDE_ASM("asm/nonmatchings/main", ReceiveBufFromSio);
