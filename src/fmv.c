#include "common.h"

#include <libcd.h>

// Based on: PSX/SAMPLE/CD/MOVIE/TUTO1.C

typedef struct {
    char* fileName;
    int is24bit;
    int startFrame;
    int endFrame;
    int posX;
    int posY;
    int scrWidth;
    int scrHeight;
} MovieInfo;

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

#define MOVIE_WAIT 2000
#define VRAMPIX(pixels, is24bit)  ((is24bit) ? ((pixels) * 3) / 2 : (pixels))
// weird variant? maybe the original doesn't work for PAL?
#define VRAMPIX2(pixels, is24bit) ((is24bit) ? (pixels) * 3 : (pixels) << 1)

int fmvEnded;
int strWidth /* = 0*/;
int strHeight /* = 0*/;

u_long* StrNext(DECENV* dec, MovieInfo* movie) {
    u_long* addr;
    StHEADER* sector;
    int cnt = MOVIE_WAIT;

    while (StGetNext((u_long**)&addr, (u_long**)&sector)) {
        if (--cnt == 0) return (0);
    }

    if ((int)sector->frameCount >= movie->endFrame) {
        fmvEnded = 1;
    }

    if (strWidth != sector->width || strHeight != sector->height) {
        RECT rect;
        setRECT(&rect, 0, 0, VRAMPIX2(movie->scrWidth, movie->is24bit), movie->scrHeight);
        if (movie->is24bit) {
            ClearImage(&rect, 0, 0, 0);
        } else {
            ClearImage(&rect, 64, 64, 64);
        }

        strWidth = sector->width;
        strHeight = sector->height;
    }
    dec->rect[0].w = dec->rect[1].w = VRAMPIX(strWidth, movie->is24bit);
    dec->rect[0].h = dec->rect[1].h = strHeight;
    dec->slice.h = strHeight;

    return (addr);
}

void StrSync(DECENV* dec, int mode) {
    extern char S_time_out_in_decoding[];
    volatile u_long cnt = WAIT_TIME;

    while (dec->isdone == 0) {
        if (--cnt == 0) {
            printf(S_time_out_in_decoding);
            dec->isdone = 1;
            dec->rectid = dec->rectid ? 0 : 1;
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
