#include "common.h"

#include <libapi.h>

extern uint* savedSp;
extern uint* unusedInitStackParamEnd;
extern uint* unusedInitStackParamStart;

void UnusedInitStack(uint end, uint start) {
    uint* endMasked = end & 0xfffffff;
    uint* startMasked = start & 0xfffffff;
    uint* p;
    int val;

    unusedInitStackParamEnd = endMasked;
    unusedInitStackParamStart = startMasked;

    p = (int*)(GetSp() & 0xfffffff);
    val = 0x12345678;
    savedSp = p;

    while (endMasked <= --p) {
        *p = val++;
    }
}

int UnusedValidateStack(void) {
    uint val;
    uint* p = unusedInitStackParamEnd;

    val = (savedSp - unusedInitStackParamEnd) + 0x12345678;
    while (p <= unusedInitStackParamStart && *p++ == --val)
        ;

    return unusedInitStackParamStart - p;
}
