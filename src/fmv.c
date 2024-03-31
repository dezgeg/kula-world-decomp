#include "common.h"

#include <libcd.h>
#include <libgpu.h>
#include <libpress.h>

// Based on: PSX/SAMPLE/CD/MOVIE/TUTO1.C, but seems to have been patched
// for a different VRAM layout (framebuffers are side-by-side).

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

extern uint GetControllerButtons(int slot);
extern int GetControllerStatus(int slot);

extern uint controllerButtons;
extern int curController;
extern uint prevControllerButtons;

// wtf are these types?
extern ushort dispenvScreenX;
extern int dispenvScreenY;

#define MOVIE_WAIT 2000
#define RING_SIZE 32
#define bound(val, n) ((((val) - 1) / (n) + 1) * (n))
#define bound16(val) bound((val), 16)
#define DCT_MODE(is24bit) ((is24bit) ? 3 : 2)
#define VRAMPIX(pixels, is24bit) ((is24bit) ? ((pixels) * 3) / 2 : (pixels))
// Both the even-and-odd frame buffers
#define VRAMPIX_BOTH(pixels, is24bit) ((is24bit) ? (pixels)*3 : (pixels) << 1)

extern int stCdIntrFlag;
extern DECDCTTAB vlc_table;
static DECENV dec;
extern MovieInfo movieInfos[];

int fmvEnded;
int strWidth /* = 0 */;
int strHeight /* = 0 */;
int isFirstSlice;
int isFirstTimeDecEnvInit /* = 1*/;
void* fmvRing /* = ... */;

void* pVlcbuf0 /* = ...*/;
void* pVlcbuf1 /* = ...*/;
void* pImgbuf0 /* = ...*/;
void* pImgbuf1 /* = ...*/;

void StrSetDefDecEnv(DECENV* dec, int x0, int y0, int x1, int y1, MovieInfo* movie);
u_long* StrNext(DECENV* dec, MovieInfo* movie);
void StrInit(CdlLOC* loc, void (*callback)(), MovieInfo* movie);
void StrCallback();
void StrKickCd(CdlLOC* loc);

static inline int GetButtonsFromAnyController() {
    if (GetControllerStatus(curController) != 0) {
        controllerButtons = GetControllerButtons(curController);
    } else {
        controllerButtons = GetControllerButtons((curController + 1) % 2);
    }
    return controllerButtons;
}

static inline int TestButton(int button) {
    return controllerButtons & (button & ~prevControllerButtons);
}

int FmvMainLoop(int movieI) {
    MovieInfo* movie = &movieInfos[movieI];
    extern char S_file_not_found[];
    extern char S_time_out_in_strNext_FMTd[];
    DISPENV disp;
    DRAWENV draw;
    int id;
    CdlFILE file;
    RECT clearRect;
    CdlLOC save_loc;
    int frame_no;
    int controller;
    isFirstSlice = 1;

    if (CdSearchFile(&file, movie->fileName) == 0) {
        printf(S_file_not_found);
        ResetGraph(3);
        StopCallback();
        return 0;
    }

    StrSetDefDecEnv(&dec, VRAMPIX(movie->posX, movie->is24bit), movie->posY,
                    VRAMPIX(movie->posX + movie->scrWidth, movie->is24bit), movie->posY, movie);
    StrInit(&file.pos, StrCallback, movie);
    DecDCTvlcBuild(vlc_table);
    while (StrNextVlc(&dec, movie) == -1) {
        save_loc = file.pos;
        StrKickCd(&save_loc);
    }
    fmvEnded = 0;

    SetDispMask(0);
    setRECT(&clearRect, 0, 0, VRAMPIX_BOTH(movie->scrWidth, movie->is24bit), movie->scrHeight);
    if (movie->is24bit) {
        ClearImage(&clearRect, 0, 0, 0);
    } else {
        ClearImage(&clearRect, 64, 64, 64);
    }

    while (1) {
        DecDCTin(dec.vlcbuf[dec.vlcid], DCT_MODE(movie->is24bit));
        DecDCTout((u_long*)dec.imgbuf[dec.imgid], dec.slice.w * bound16(dec.slice.h) / 2);

        while (StrNextVlc(&dec, movie) == -1) {
            frame_no = StGetBackloc(&save_loc);
            printf(S_time_out_in_strNext_FMTd, frame_no);
            if (frame_no > movie->endFrame || frame_no <= 0)
                save_loc = file.pos;
            StrKickCd(&save_loc);
        }

        StrSync(&dec, 0);

        VSync(0);

        id = dec.rectid ? 0 : 1;
        SetDefDispEnv(&disp, dec.rect[id].x - VRAMPIX(movie->posX, movie->is24bit),
                      dec.rect[id].y - movie->posY, VRAMPIX(movie->scrWidth, movie->is24bit),
                      movie->scrHeight);
        disp.screen.x = dispenvScreenX;
        disp.screen.y = dispenvScreenY + 8;

        if (movie->is24bit) {
            disp.isrgb24 = movie->is24bit;
            disp.disp.w = disp.disp.w * 2 / 3;
        }
        PutDispEnv(&disp);
        SetDispMask(1);

        prevControllerButtons = controllerButtons;
        GetButtonsFromAnyController();
        if (TestButton(PAD_START) || TestButton(PAD_TRIANGLE) || TestButton(PAD_SELECT)) {
            fmvEnded = 1;
        }

        if (fmvEnded == 1) break;
    }

    DecDCToutCallback(0);
    StUnSetRing();
    CdControlB(CdlPause, 0, 0);
    if (fmvEnded == 0) {
        ResetGraph(3);
        StopCallback();
        return 0;
    } else {
        return 1;
    }
}

void StrSetDefDecEnv(DECENV* dec, int x0, int y0, int x1, int y1, MovieInfo* movie) {
    if (isFirstTimeDecEnvInit == 1) {
        dec->vlcbuf[0] = pVlcbuf0;
        dec->vlcbuf[1] = pVlcbuf1;
        dec->vlcid = 0;

        dec->imgbuf[0] = pImgbuf0;
        dec->imgbuf[1] = pImgbuf1;
        dec->imgid = 0;
        dec->rectid = 0;
        dec->isdone = 0;
        isFirstTimeDecEnvInit = 0;
    }

    dec->rect[0].x = x0;
    dec->rect[0].y = y0;
    dec->rect[1].x = x1;
    dec->rect[1].y = y1;
    dec->slice.w = VRAMPIX(16, movie->is24bit);
    dec->is24bit = movie->is24bit;

    if (dec->rectid == 0) {
        dec->slice.x = x0;
        dec->slice.y = y0;
    } else {
        dec->slice.x = x1;
        dec->slice.y = y1;
    }
}

void StrInit(CdlLOC* loc, void (*callback)(), MovieInfo* movie) {
    DecDCTReset(0);
    DecDCToutCallback(callback);
    StSetRing(fmvRing, RING_SIZE);
    StSetStream(movie->is24bit, movie->startFrame, 0xffffffff, 0, 0);
    StrKickCd(loc);
}

void StrCallback() {
    int mod;
    int id;
    RECT snap_rect;

    if (dec.is24bit) {
        if (stCdIntrFlag) {
            StCdInterrupt();
            stCdIntrFlag = 0;
        }
    }
    id = dec.imgid;
    snap_rect = dec.slice;

    dec.imgid = dec.imgid ? 0 : 1;

    if (isFirstSlice && (mod = dec.rect[dec.rectid].w % dec.slice.w)) {
        dec.slice.x += mod;
        isFirstSlice = 0;
    } else {
        dec.slice.x += dec.slice.w;
    }

    if (dec.slice.x < dec.rect[dec.rectid].x + dec.rect[dec.rectid].w) {
        DecDCTout((u_long*)dec.imgbuf[dec.imgid], dec.slice.w * bound16(dec.slice.h) / 2);
    } else {
        dec.isdone = 1;
        isFirstSlice = 1;

        dec.rectid = dec.rectid ? 0 : 1;
        dec.slice.x = dec.rect[dec.rectid].x;
        dec.slice.y = dec.rect[dec.rectid].y;
    }
    LoadImage(&snap_rect, (u_long*)dec.imgbuf[id]);
}

int StrNextVlc(DECENV* dec, MovieInfo* movie) {
    int cnt;
    u_long* next;

    for (cnt = 2000; cnt != 0; cnt--) {
        next = StrNext(dec, movie);
        if (next) {
            dec->vlcid = dec->vlcid ? 0 : 1;
            DecDCTvlc2(next, dec->vlcbuf[dec->vlcid], vlc_table);
            StFreeRing(next);
            return 0;
        }
    }
    return -1;
}

u_long* StrNext(DECENV* dec, MovieInfo* movie) {
    u_long* addr;
    StHEADER* sector;
    int cnt = MOVIE_WAIT;

    while (StGetNext((u_long**)&addr, (u_long**)&sector)) {
        if (--cnt == 0) {
            return (0);
        }
    }

    if ((int)sector->frameCount >= movie->endFrame) {
        fmvEnded = 1;
    }

    if (strWidth != sector->width || strHeight != sector->height) {
        RECT rect;
        setRECT(&rect, 0, 0, VRAMPIX_BOTH(movie->scrWidth, movie->is24bit), movie->scrHeight);
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
