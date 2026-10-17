#include "common.h"

#include <libapi.h>

extern uint* savedSp[];
extern uint* unusedInitStackParamEnd[];
extern uint* unusedInitStackParamStart[];

#ifndef SKIP_UNUSED_CODE
void UnusedInitStack(uint end, uint start) {
    uint* endMasked;
    uint* p;
    int val;

    unusedInitStackParamEnd[0] = endMasked = (uint*)(end & 0xfffffff);
    unusedInitStackParamStart[0] = (uint*)(start & 0xfffffff);

    p = (int*)(GetSp() & 0xfffffff);
    val = 0x12345678;
    savedSp[0] = p;

    while (endMasked <= --p) {
        *p = val++;
    }
}

int UnusedValidateStack(void) {
    uint* p = unusedInitStackParamEnd[0];
    uint val = (savedSp[0] - unusedInitStackParamEnd[0]) + 0x12345678;

    while (p <= unusedInitStackParamStart[0]) {
        if (*p++ != --val) {
            break;
        }
    }

    return unusedInitStackParamStart[0] - p;
}
#endif
