#include "common.h"

#include <libcd.h>
#include <libgpu.h>

typedef struct Music {
    char* file;
    ushort chan;
    short sectors;
} Music;

extern void Noop(void);
extern void Noop2(void);
extern void PutDrawAndDispEnvs(void);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering,
                         u_char use24Bit);
extern void SndSetMusicVolume(void);

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
short bonusMusicIndex;
short playingBonusMusic;

extern CdlFILE musicCdlfile;
extern CdlLOC musicBonusLoc;
extern CdlLOC musicCdlLoc;
extern CdlLOC musicCurLoc;
extern CdlLOC savedMusicCdlLoc;
extern Music BONUS_MUSICS[3];
extern Music MUSICS[11];
extern int musicCdMode;

extern int whichDrawDispEnv;

void PlayMusic(int world) {
    extern char S_File_error[];
    extern char S_could_not_find_music[];
    extern char S_could_not_start_playing[];
    char dummy[8];
    char* filename;

    Noop2();
    SndSetMusicVolume();
    musicCounter = 50;
    musicSearchAttempt = 0;
    while (CdSearchFile(&musicCdlfile, MUSICS[world].file) == NULL) {
        if (musicSearchAttempt >= 10) break;
        CdInit();
        musicSearchAttempt++;
    }
    if (musicSearchAttempt >= 10) {
        filename = MUSICS[world].file;
        VSyncCallback(NULL);
        SetupDisplay(1, 0x80, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_File_error);
        FntPrint(S_could_not_find_music);
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
        filename = MUSICS[world].file;
        VSyncCallback(NULL);
        SetupDisplay(1, 0x80, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_File_error);
        FntPrint(S_could_not_start_playing);
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
    extern char S_could_not_find_music[];
    extern char S_could_not_start_playing[];
    extern char S_bonus_song[];
    char dummy[8];
    char* file;

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
    while (CdSearchFile(&musicCdlfile, BONUS_MUSICS[bonusMusicIndex].file) == NULL) {
        if (bonusMusicSearchAttempt >= 10) {
            break;
        }
        CdInit();
        bonusMusicSearchAttempt++;
    }

    if (bonusMusicSearchAttempt >= 10) {
        file = BONUS_MUSICS[bonusMusicIndex].file;
        VSyncCallback(NULL);
        SetupDisplay(1, 0x80, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_File_error);
        FntPrint(S_could_not_find_music);
        FntPrint(file);
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
        FntPrint(S_could_not_start_playing);
        FntPrint(S_bonus_song);
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
