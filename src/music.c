#include "common.h"

#include <libcd.h>
#include <libgpu.h>

typedef struct Music {
    char* filename;
    ushort chan;
    short sectors;
} Music;

extern void Noop(void);
extern void Noop2(void);
extern void PutDrawAndDispEnvs(void);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering,
                         u_char use24Bit);
extern void SndSetMusicVolume(void);

short bonusMusicIndex = 0;
short playingBonusMusic = 0;

CdlFILTER musicCdlFilter;
int bonusMusicSearchAttempt;
int musicCounter;
int musicCurSector;
int musicEndSector;
int musicSearchAttempt;
int musicStartSector;
int musicUnkAlwaysZero1;
int musicUnkAlwaysZero2;
int savedMusicCurSector;
int savedMusicEndSector;
int savedMusicStartSector;
int savedMusicXaChan;

extern CdlFILE musicCdlfile;
extern CdlLOC musicBonusLoc;
extern CdlLOC musicCdlLoc;
extern CdlLOC savedMusicCdlLoc;
extern Music BONUS_MUSICS[3];
extern Music MUSICS[11];
extern int musicCdMode;

extern int whichDrawDispEnv;

void PlayMusic(int world) {
    extern char S_File_error[];
    char dummy[8];
    char* filename;

    Noop2();
    SndSetMusicVolume();
    musicCounter = 50;
    musicSearchAttempt = 0;
    while (CdSearchFile(&musicCdlfile, MUSICS[world].filename) == NULL) {
        if (musicSearchAttempt >= 10) break;
        CdInit();
        musicSearchAttempt++;
    }
    if (musicSearchAttempt >= 10) {
        filename = MUSICS[world].filename;
        VSyncCallback(NULL);
        SetupDisplay(1, 0x80, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_File_error);
        FntPrint("could not find music \n");
        FntPrint(filename);
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        do {
        } while (1);
    }
    musicStartSector = CdPosToInt(&musicCdlfile.pos);
    musicEndSector = musicStartSector + MUSICS[world].sectors * 4;
    CdReadSync(0, 0);
    while (CdControlB(CdlSetmode, &musicCdMode, 0) == 0)
        ;
    CdReadSync(0, 0);
    CdIntToPos(musicStartSector, &musicCdlLoc);
    CdReadSync(0, 0);
    musicCdlFilter.file = 1;
    musicCdlFilter.chan = MUSICS[world].chan;
    while (CdControlB(CdlSetfilter, &musicCdlFilter, dummy) == 0)
        ;
    if (CdControl(CdlReadS, &musicCdlLoc, dummy) == 0) {
        filename = MUSICS[world].filename;
        VSyncCallback(NULL);
        SetupDisplay(1, 0x80, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_File_error);
        FntPrint("could not start playing \n");
        FntPrint(filename);
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        do {
        } while (1);
    }
    Noop();
    SpuSetReverb(1);
    playingBonusMusic = 0;
}

void PlayBonusMusic(void) {
    extern char S_File_error[];
    char dummy[8];
    char* filename;

    Noop2();
    SndSetMusicVolume();
    if (bonusMusicIndex < 0) {
        bonusMusicIndex = 0;
    }
    musicCounter = 50;
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

    if (bonusMusicSearchAttempt >= 10) {
        filename = BONUS_MUSICS[bonusMusicIndex].filename;
        VSyncCallback(NULL);
        SetupDisplay(1, 0x80, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_File_error);
        FntPrint("could not find music \n");
        FntPrint(filename);
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        do {
        } while (1);
    }

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
        VSyncCallback(NULL);
        SetupDisplay(1, 0x80, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_File_error);
        FntPrint("could not start playing \n");
        FntPrint("bonus song");
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        do {
        } while (1);
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
    musicCounter = 50;
    savedMusicStartSector = musicStartSector;
    savedMusicEndSector = musicEndSector;
    savedMusicCurSector = musicCurSector;
    savedMusicXaChan = musicCdlFilter.chan;
    while (CdControlB(CdlPause, 0, dummy) == 0)
        ;
    CdReadSync(0, 0);
    Noop();
    SpuSetReverb(1);
}

void SwitchFromBonusToNormalMusic(void) {
    extern char S_File_error[];
    char dummy[8];

    Noop2();
    SndSetMusicVolume();
    musicCdlLoc = savedMusicCdlLoc;
    // XXX: Why?
    *(volatile int*)&musicCurSector = savedMusicCurSector;
    musicCounter = 50;
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
        VSyncCallback(NULL);
        SetupDisplay(1, 0x80, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_File_error);
        FntPrint("could not start playing \n");
        FntPrint("resumed song after bonus");
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        do {
        } while (1);
    }
    SndSetMusicVolume();
    Noop();
    SpuSetReverb(1);
    playingBonusMusic = 0;
}

void UnusedDebugPrintMusicVariables(void) {
    FntPrint("CD INFO\nSTARTPOS %d\nENDPOS %d\nCURPOS %d\n",
            musicStartSector, musicEndSector, musicCurSector);
    FntPrint("\nSTARTPOS_BU %d\nENDPOS_BU %d\nCURPOS_BU %d\n\n",
            savedMusicStartSector, savedMusicEndSector, savedMusicCurSector);
}

static CdlLOC musicCurLoc = { 0, 0, 0, 0 };
static char PAD[8] = { 0 }; // hack

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
