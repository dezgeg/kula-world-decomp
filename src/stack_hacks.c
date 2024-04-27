#include "common.h"

#include <libapi.h>

extern uint savedSp;
extern uint unusedInitStackParamEnd;
extern uint unusedInitStackParamStart;

void UnusedInitStack(uint end, uint start) {
    uint endMasked = end & 0xfffffff;
    int* p;
    int val;

    unusedInitStackParamEnd = endMasked;
    unusedInitStackParamStart = start & 0xfffffff;

    p = (int*)(GetSp() & 0xfffffff);
    val = 0x12345678;
    savedSp = (uint)p;

    while (endMasked <= --p) {
        *p = val++;
    }
}
