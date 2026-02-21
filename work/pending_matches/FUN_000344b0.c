#include "common.h"

int FUN_000344b0(int a0, int a1) {
    if ((a0 == 1 || a0 == 4) && (a1 == 1 || a1 == 4)) return 1;
    if ((a0 == 2 || a0 == 3) && (a1 == 2 || a1 == 3)) return 1;
    if ((a0 == 0 || a0 == 5) && (a1 == 0 || a1 == 5)) return 1;
    return 0;
}
