#include "common.h"

#include <libcd.h>

void StrKickCd(CdlLOC* loc) {
    u_char param = CdlModeSpeed;

    do {
        while (CdControl(CdlSetloc, (u_char*)loc, 0) == 0)
            ;
        while (CdControl(CdlSetmode, &param, 0) == 0)
            ;
        VSync(3);
    } while (CdRead2(CdlModeStream2 | CdlModeSpeed | CdlModeRT) == 0);
}
