#include "common.h"

#include <string.h>

extern char S_[];
extern char S__2[];
extern char S_FMTd_FMTdFMTd[];
extern char S_FMTd_FMTdFMTd_FMTdFMTd[];

INCLUDE_ASM("asm/nonmatchings/level_end", UpdateScoreAtEndOfLevel);

void FormatTime(int time, char* s, int showPlus) {
    if (time < 0) {
        s += sprintf(s, S_);
    }
    if (time > 0 && showPlus > 0) {
        s += sprintf(s, S__2);
    }

    if (abs(time) > 3600) {
        sprintf(s, S_FMTd_FMTdFMTd_FMTdFMTd, abs(time) / 3600,
                abs(time) / 600 + (abs(time) / 3600) * -6, abs(time) / 60 + (abs(time) / 600) * -10,
                abs(time) / 10 + (abs(time) / 60) * -6, abs(time) % 10);
    } else {
        sprintf(s, S_FMTd_FMTdFMTd, abs(time) / 60, abs(time) / 10 + (abs(time) / 60) * -6,
                abs(time) % 10);
    }
}

INCLUDE_ASM("asm/nonmatchings/level_end", DrawLevelScoreSummary);
