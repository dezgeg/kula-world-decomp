#include "common.h"

int dispModeChangeNewWidth;
int dispModeChangeState;

void UnusedSetDisplayModeChange(int param_1) {
    dispModeChangeNewWidth = param_1;
    dispModeChangeState = 1;
}
