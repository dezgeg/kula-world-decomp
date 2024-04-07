#include "common.h"

#include <libcd.h>
#include <libgpu.h>

extern void Noop(void);
extern void Noop2(void);
extern void PutDrawAndDispEnvs(void);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering,
                         u_char use24Bit);
extern void SndSetMusicVolume(void);

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
short playingBonusMusic;

extern CdlFILTER musicCdlFilter;
extern CdlLOC musicBonusLoc;
extern CdlLOC musicCdlLoc;
extern CdlLOC musicCurLoc;
extern CdlLOC savedMusicCdlLoc;
extern int musicCdMode;

extern int whichDrawDispEnv;

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
    extern char S_could_not_start_playing[];
    extern char S_resumed_song_after_bonus[];
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
    musicCdlFilter.chan = (u_char)savedMusicXaChan;
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
        FntPrint(S_could_not_start_playing);
        FntPrint(S_resumed_song_after_bonus);
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
    extern char S_CD_INFO_STARTPOS_FMTd_ENDPOS_FMTd_CURPOS_FMTd[];
    extern char S_STARTPOS_BU_FMTd_ENDPOS_BU_FMTd_CURPOS_BU_FMTd[];
    FntPrint(S_CD_INFO_STARTPOS_FMTd_ENDPOS_FMTd_CURPOS_FMTd, musicStartSector, musicEndSector,
             musicCurSector);
    FntPrint(S_STARTPOS_BU_FMTd_ENDPOS_BU_FMTd_CURPOS_BU_FMTd, savedMusicStartSector,
             savedMusicEndSector, savedMusicCurSector);
}

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
