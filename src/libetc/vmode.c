#include "common.h"

#include <STDDEF.H>

#include <LIBETC.H>

extern long D_000A1FA8;

#if 1
long SetVideoMode(long mode) {
    long prev = D_000A1FA8;
    D_000A1FA8 = mode;
    return prev;
}
#else
INCLUDE_ASM("asm/nonmatchings/libetc/vmode", SetVideoMode);
#endif

#if 1
long GetVideoMode(void) { return D_000A1FA8; }
#else
INCLUDE_ASM("asm/nonmatchings/libetc/vmode", GetVideoMode);
#endif
