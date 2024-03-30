#include "common.h"

#include <libcd.h>

// Based on: PSX/SAMPLE/CD/MOVIE/TUTO1.C

typedef struct {
    u_long* vlcbuf[2];
    int vlcid;
    u_short* imgbuf[2];
    int imgid;
    RECT rect[2];
    int rectid;
    RECT slice;
    int isdone;
    int is24bit;
} DECENV;

void StrSync(DECENV* dec, int mode) {
    extern char S_time_out_in_decoding[];
    volatile u_long cnt = WAIT_TIME;

    while (dec->isdone == 0) {
        if (--cnt == 0) {
            printf(S_time_out_in_decoding);
            dec->isdone = 1;
            dec->rectid = dec->rectid? 0: 1;
            dec->slice.x = dec->rect[dec->rectid].x;
            dec->slice.y = dec->rect[dec->rectid].y;
        }
    }
    dec->isdone = 0;
}

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
