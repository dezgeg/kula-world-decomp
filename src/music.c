#include "common.h"
#include <libcd.h>
#include <libgpu.h>

extern void Noop(void);
extern void Noop2(void);
extern void PutDrawAndDispEnvs(void);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering, u_char use24Bit);
extern void SndSetMusicVolume(void);

extern char S_File_error[];
extern Music BONUS_MUSICS[3];
extern Music MUSICS[11];
extern int musicCdMode;
extern int whichDrawDispEnv;

short playingBonusMusic = 0;
short bonusMusicIndex = 0;

CdlFILTER musicCdlFilter;
CdlFILE musicCdlfile;
CdlLOC musicBonusLoc;
CdlLOC musicCdlLoc;
CdlLOC savedMusicCdlLoc;
int musicCounter;
int musicCurSector;
int musicEndSector;
int musicStartSector;
int musicUnkAlwaysZero1;
int musicUnkAlwaysZero2;
int savedMusicCurSector;
int savedMusicEndSector;
int savedMusicStartSector;
int savedMusicXaChan;

static int bonusMusicSearchAttempt;
static int musicSearchAttempt;

// FIXME: this file should be merged with loading.c and use single definition
static inline void FileError(char* str1, char* str2) {
#if VER_US
    int dummy;
    (void)&dummy;
    VSync(3);
#else
    VSyncCallback(NULL);
    SetupDisplay(1, 0x80, 0, 0, 0, 0);
    FntFlush(-1);
    DrawSync(0);
    whichDrawDispEnv = 0;
    PutDrawAndDispEnvs();
    FntPrint(S_File_error);
    FntPrint(str1);
    FntPrint(str2);
    FntFlush(-1);
    whichDrawDispEnv = 1;
    PutDrawAndDispEnvs();
    while (1)
        ;
#endif
}

#if VER_US
#define IF_WHILE while
#define FileError(a,b) VSync(3)
#define VSYNC_3() VSync(3)
#else
#define IF_WHILE if
#define VSYNC_3() /* nop */
#endif

void PlayMusic(int world) {
#if VER_US
    int pad[2];
    int musicSearchAttempt;
#endif
    char dummy[8];

    Noop2();
    SndSetMusicVolume();
    musicCounter = FPS;
    musicSearchAttempt = 0;
    while (CdSearchFile(&musicCdlfile, MUSICS[world].filename) == NULL) {
        if (musicSearchAttempt >= 10)
            break;
        CdInit();
        musicSearchAttempt++;
    }
#if !VER_US
    if (musicSearchAttempt >= 10) {
        FileError("could not find music \n", MUSICS[world].filename);
    }
#endif
    musicStartSector = CdPosToInt(&musicCdlfile.pos);
    musicEndSector = musicStartSector + MUSICS[world].sectors * 4;
    CdReadSync(0, 0);
    while (CdControlB(CdlSetmode, &musicCdMode, 0) == 0) {
        VSYNC_3();
    }
    VSYNC_3();
#if !VER_US
    CdReadSync(0, 0);
#endif
    CdIntToPos(musicStartSector, &musicCdlLoc);
    CdReadSync(0, 0);
    musicCdlFilter.file = 1;
    musicCdlFilter.chan = MUSICS[world].chan;
    while (CdControlB(CdlSetfilter, &musicCdlFilter, dummy) == 0) {
        VSYNC_3();
    }
    IF_WHILE (CdControl(CdlReadS, &musicCdlLoc, dummy) == 0) {
        FileError("could not start playing \n", MUSICS[world].filename);
    }
#if VER_US
    savedMusicCdlLoc = musicCdlLoc;
    savedMusicStartSector = musicStartSector;
    savedMusicEndSector = musicEndSector;
    savedMusicCurSector = musicStartSector;
    savedMusicXaChan = musicCdlFilter.chan;
#endif
    Noop();
    SpuSetReverb(1);
    playingBonusMusic = 0;
}

void PlayBonusMusic(void) {
    char dummy[8];

    Noop2();
    SndSetMusicVolume();
    if (bonusMusicIndex < 0) {
        bonusMusicIndex = 0;
    }
    musicCounter = FPS;
    if (playingBonusMusic == 0) {
        savedMusicCdlLoc = musicCdlLoc;
        savedMusicStartSector = musicStartSector;
        savedMusicEndSector = musicEndSector;
        savedMusicCurSector = musicCurSector;
        savedMusicXaChan = musicCdlFilter.chan;
    }
    bonusMusicSearchAttempt = 0;
    while (CdSearchFile(&musicCdlfile, BONUS_MUSICS[bonusMusicIndex].filename) == NULL) {
        if (bonusMusicSearchAttempt >= 10) {
            break;
        }
        CdInit();
        bonusMusicSearchAttempt++;
    }

#if !VER_US
    if (bonusMusicSearchAttempt >= 10) {
        FileError("could not find music \n", BONUS_MUSICS[bonusMusicIndex].filename);
    }
#endif

    musicStartSector = CdPosToInt(&musicCdlfile.pos);
    musicEndSector = musicStartSector + BONUS_MUSICS[bonusMusicIndex].sectors * 4;
    CdReadSync(0, 0);
    while (CdControlB(CdlSetmode, &musicCdMode, 0) == 0)
        ;
    CdReadSync(0, 0);
    CdIntToPos(musicStartSector, &musicCdlLoc);
    CdReadSync(0, 0);
    musicCdlFilter.file = 1;
    musicCdlFilter.chan = BONUS_MUSICS[bonusMusicIndex].chan;
    while (CdControlB(CdlSetfilter, &musicCdlFilter, dummy) == 0)
        ;
    if (CdControlB(CdlReadS, &musicCdlLoc, dummy) == 0) {
        FileError("could not start playing \n", "bonus song");
    }
    Noop();
    SpuSetReverb(1);
    playingBonusMusic = 1;
    bonusMusicIndex = (bonusMusicIndex + 1) % 3;
}

void MusicPause(void) {
    char dummy[8];

    Noop2();
    savedMusicCdlLoc = musicCdlLoc;
    musicCounter = FPS;
    savedMusicStartSector = musicStartSector;
    savedMusicEndSector = musicEndSector;
    savedMusicCurSector = musicCurSector;
    savedMusicXaChan = musicCdlFilter.chan;
    while (CdControlB(CdlPause, 0, dummy) == 0) {
        VSYNC_3();
    }
    CdReadSync(0, 0);
    Noop();
    SpuSetReverb(1);
#if VER_US
    playingBonusMusic = 1;
#endif
}

void SwitchFromBonusToNormalMusic(void) {
    char dummy[8];

#if VER_US
    if (playingBonusMusic != 1) {
        return;
    }
#endif
    Noop2();
    SndSetMusicVolume();
    musicCdlLoc = savedMusicCdlLoc;
    // XXX: Why?
    *(volatile int*)&musicCurSector = savedMusicCurSector;
    musicCounter = FPS;
    musicStartSector = savedMusicStartSector;
    musicEndSector = savedMusicEndSector;
    CdIntToPos(musicCurSector, &musicBonusLoc);
    musicCdlFilter.chan = savedMusicXaChan;
    CdReadSync(0, 0);
    while (CdControlB(CdlSetmode, &musicCdMode, 0) == 0)
        ;
    CdReadSync(0, 0);
    while (CdControlB(CdlSetfilter, &musicCdlFilter, dummy) == 0)
        ;
    if (CdControlB(CdlReadS, &musicBonusLoc, dummy) == 0) {
        FileError("could not start playing \n", "resumed song after bonus");
    }
    SndSetMusicVolume();
    Noop();
    SpuSetReverb(1);
    playingBonusMusic = 0;
}

#ifndef SKIP_UNUSED_CODE
void UnusedDebugPrintMusicVariables(void) {
    FntPrint("CD INFO\nSTARTPOS %d\nENDPOS %d\nCURPOS %d\n",
            musicStartSector, musicEndSector, musicCurSector);
    FntPrint("\nSTARTPOS_BU %d\nENDPOS_BU %d\nCURPOS_BU %d\n\n",
            savedMusicStartSector, savedMusicEndSector, savedMusicCurSector);
}
#endif

// XXX: These variables have to be here in order to match
static CdlLOC musicCurLoc = { 0, 0, 0, 0 };
static char unusedA36F4[8] = { 0 };

void MusicCheckForLoop(void) {
    musicCounter--;
    if (musicCounter < 1 && musicUnkAlwaysZero1 + musicUnkAlwaysZero2 == 0) {
        musicCounter = 32;
        CdControl(CdlGetlocL, 0, &musicCurLoc);
        musicCurSector = CdPosToInt(&musicCurLoc);
        if (musicEndSector - 75 <= musicCurSector) {
            Noop2();
            CdControl(CdlReadS, &musicCdlLoc, 0);
            CdControlF(CdlSetfilter, &musicCdlFilter);
            Noop();
        }
    }
}
