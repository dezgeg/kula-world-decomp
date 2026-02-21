#include "common.h"

extern short *ggiPart2DepthCueingLookup;

int GetShortFromGgiPart2(int param_1) {
    if (param_1 >= 4096) {
        return 4096;
    }
    return ggiPart2DepthCueingLookup[param_1];
}
