#include "common.h"

typedef struct CheatState {
    int activated;
    int toggled;
    int preventActivation;
    int unused;
} CheatState;

uint buttonBuf[8];
uint* buttonsPtr;

extern CheatState cheatState[9];

void ResetCheats(void) {
    int i, j;

    for (i = 0; i < 9; i++) {
        int* p = &cheatState[i];
        for (j = 0; j < 4; j++) {
            p[j] = 0;
        }
    }
    for (i = 0; i < 8; i++) {
        buttonBuf[i] = 0;
    }
    buttonsPtr = buttonBuf;
}
