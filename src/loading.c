#include "common.h"
#include <libcd.h>
#include <libgpu.h>
#include "zlib.h"

extern void LoadingScreenVSyncCallback(void);
extern void Noop2(void);
extern void Noop2(void);
extern void Noop(void);
extern void Noop(void);
extern void PutDrawAndDispEnvs(void);
extern void PutDrawAndDispEnvs(void);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering, u_char use24Bit);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering, u_char use24Bit);
extern void SndSetMusicVolume(void);
extern void UnusedLoadFullScreenPicture(void* param_1);

extern char filenameBuf[24];
extern int displayHeight;
extern int displayWidth;
extern int gameMode;
extern int isFinal;
extern int levelEndReason;
extern int musicCdMode;
extern int whichDrawDispEnv;
extern OT_TYPE otag[2][1][1026];
extern void* warningImageCompressedPtr;

CdlFILE musicCdlfile;
CdlFILTER musicCdlFilter;
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
int sizeOfSfxFile;
int unusedReadErrorCode;

static int bonusMusicSearchAttempt;
static int musicSearchAttempt;

char* LEVEL_DIRS[] = {
    "\\HIRO\\HIRO",
    "\\HILLS\\HILLS",
    "\\INCA\\INCA",
    "\\ARCTIC\\ARCTIC",
    "\\COWBOY\\COWBOY",
    "\\FIELD\\FIELD",
    "\\ATLANT\\ATLANT",
    "\\HAZE\\HAZE",
    "\\MARS\\MARS",
    "\\HELL\\HELL",
    "\\HIRO\\COPYCAT",
};

char* EXTENSIONS[] = {
    ".TGI;1",
    ".SFX;1",
    ".PAK;1",
    ".GGI;1",
    "FI.PAK;1",
};

#if !VER_EU
char PIC_PAK_FILENAME[] = {"\\KULA\\PIC.PAK;1"};
#else
char PIC_PAK_FILENAME[24] = "\\KULA\\KULA_PIC.PAK;1";
#endif
short skipFirstLoadingScreen = 1;

Music BONUS_MUSICS[] = {
    {"\\XA\\MUSIC_1.XA;1", 0, VER_EU ? 3335 : 3197},
    {"\\XA\\MUSIC_2.XA;1", 1, VER_EU ? 5474 : 5379},
    {"\\XA\\MUSIC_0.XA;1", 0, VER_EU ? 3279 : 3239},
};

int musicUnkAlwaysZero1 = 0;
int musicUnkAlwaysZero2 = 0;
int musicCdMode = 0x78;

Music MUSICS[] = {
    {"\\XA\\MUSIC_1.XA;1", 1, VER_EU ? 4533 : 4441},
    {"\\XA\\MUSIC_3.XA;1", 2, VER_EU ? 6844 : 6751},
    {"\\XA\\MUSIC_2.XA;1", 2, VER_EU ? 5646 : 5556},
    {"\\XA\\MUSIC_2.XA;1", 3, VER_EU ? 5786 : 5683},
    {"\\XA\\MUSIC_3.XA;1", 3, VER_EU ? 7151 : 7057},
    {"\\XA\\MUSIC_2.XA;1", 0, VER_EU ? 5178 : 5084},
    {"\\XA\\MUSIC_3.XA;1", 1, VER_EU ? 5979 : 5885},
    {"\\XA\\MUSIC_3.XA;1", 0, VER_EU ? 5897 : 5803},
    {"\\XA\\MUSIC_1.XA;1", 2, VER_EU ? 4705 : 4612},
    {"\\XA\\MUSIC_1.XA;1", 3, VER_EU ? 4776 : 4674},
    {"\\SAMPLE.XA;1", 0, 0},
};

char S_File_error[] = "File error:\n\n";
#if !VER_EU
char S_could_not_find[] = "could not find ";
char S_seek_error[] = "seek error ";
char S_read_error[] = "read error ";
#endif

static z_stream zlibStream_a4b80;
static z_stream unusedZlibStream;

void FileError(char* str1, char* str2) {
#if !VER_EU
    int pad[2];
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

#if !VER_EU
#define IF_WHILE while
#define FileError(a,b) VSync(3)
#define VSYNC_3() VSync(3)
#else
#define IF_WHILE if
#define VSYNC_3() (void)0
#endif

uint ReadDataFile(int world, int filetype, void* buf) {
    int i;
    int j;
#if !VER_EU
    int pad[2];
#endif
    CdlFILE cdlfile;

    if (gameMode == 1 && filetype == 2) {
        world = 10;
    }
    if (gameMode == 0 && isFinal == 1 && filetype == 2) {
        filetype = 4;
    }

    for (i = 0; LEVEL_DIRS[world][i] != '\0'; i++) {
        filenameBuf[i] = LEVEL_DIRS[world][i];
    }
    for (j = 0; EXTENSIONS[filetype][j] != '\0'; j++) {
        filenameBuf[i + j] = EXTENSIONS[filetype][j];
    }
    filenameBuf[i + j] = '\0';

    IF_WHILE (!CdSearchFile(&cdlfile, filenameBuf)) {
        FileError("could not find ", filenameBuf);
    }
    IF_WHILE (!CdControl(CdlSeekL, &cdlfile, 0)) {
        FileError("seek error ", filenameBuf);
    }
    IF_WHILE (!CdRead((cdlfile.size >> 11) + 1, buf, 0x80)) {
        FileError("read error ", filenameBuf);
    }
    CdReadSync(0, 0);
    if (filetype == 1) {
        sizeOfSfxFile = cdlfile.size;
    }
    return cdlfile.size;
}

#ifndef SKIP_UNUSED_CODE
uint UnusedReadKulaPicPak(void* unknown, char* buf) {
    CdlFILE cdlfile;

#if VER_JP
    printf("LOAD PIC!!!!!!!!!!!!\n");
#else
    Noop2();
#endif
    IF_WHILE (CdSearchFile(&cdlfile, PIC_PAK_FILENAME) == 0) {
#if VER_JP
        VSync(3);
        printf("error 2\n");
#else
        unusedReadErrorCode = 2;
#endif
    }
    while (CdControl(CdlSeekL, &cdlfile, 0) == 0) {
#if VER_JP
        VSync(3);
        printf("error 3\n");
#else
        unusedReadErrorCode = 3;
#endif
    }
    while (CdRead((cdlfile.size >> 11) + 1, buf, 0x80) == 0) {
#if VER_JP
        VSync(3);
        printf("error 4\n");
#else
        unusedReadErrorCode = 4;
#endif
    }
    CdReadSync(0, 0);
#if VER_JP
    printf("Loaded Pic\n");
#else
    Noop();
#endif
    return cdlfile.size;
}

void UnusedInflateSomething(int idx, int* data) {
#if !VER_EU
    DrawSync(0);
    VSync(0);
#endif
    zlibStream_a4b80.avail_in = data[2 + 2 * idx];
    zlibStream_a4b80.next_in = (char*)data + data[1 + 2 * idx];
    zlibStream_a4b80.avail_out = 0x60000;
    zlibStream_a4b80.next_out = FILE_BUF;
    inflateInit_(&zlibStream_a4b80, "1.0.4", 0x38);
    inflate(&zlibStream_a4b80, 4);
    inflateEnd(&zlibStream_a4b80);
#if !VER_EU
    DrawSync(0);
#endif
    VSync(0);
    UnusedLoadFullScreenPicture(FILE_BUF);
}
#endif

void LoadWarningTim(void) {
#if VER_JP
    // embedded in binary
    RECT rect = {0, 0, 320, 240};
    RECT rect2 = {320, 0, 320, 240};

    whichDrawDispEnv = 0;
    PutDrawAndDispEnvs();
    zlibStream_a4b80.avail_in = 0x91d;
    zlibStream_a4b80.next_in = warningImageCompressedPtr;
    zlibStream_a4b80.avail_out = 0x60000;
    zlibStream_a4b80.next_out = TGI_FILE_BUF;
#else
    // load from disc (also do SetupDisplay)
#if VER_US
    RECT rect = {0, 0, 480, 240};
    char filename[24] = "\\HIRO\\SCEI.TIM;1";
#else
    RECT rect = {0, 0, 640, 256};
    char filename[24] = "\\HIRO\\WARNING.TIM;1";
#endif
    CdlFILE cdlfile;

    displayWidth = VER_US ? 320 : 640;
    displayHeight = VER_US ? 240 : 256;
    SetupDisplay(0, 0, 96, 0, 0, VID_NTSC);
    whichDrawDispEnv = 0;
    PutDrawAndDispEnvs();
    IF_WHILE (!CdSearchFile(&cdlfile, filename)) {
        FileError("could not find ", filename);
    }
    IF_WHILE (!CdControl(CdlSeekL, &cdlfile, 0)) {
        FileError("seek error ", filename);
    }
    IF_WHILE (!CdRead((cdlfile.size >> 11) + 1, (u_long*)FILE_BUF, 0x80)) {
        FileError("read error ", filename);
    }
    CdReadSync(0, 0);
    zlibStream_a4b80.next_in = FILE_BUF;
    zlibStream_a4b80.avail_out = 0x60000;
    zlibStream_a4b80.next_out = TGI_FILE_BUF;
    zlibStream_a4b80.avail_in = cdlfile.size;
#endif

    inflateInit_(&zlibStream_a4b80, "1.0.4", 0x38);
    inflate(&zlibStream_a4b80, 4);
    inflateEnd(&zlibStream_a4b80);
    LoadImage(&rect, (u_long*)((char*)TGI_FILE_BUF + 0x14));
    DrawSync(0);
#if VER_JP
    LoadImage(&rect2, (u_long*)((char*)TGI_FILE_BUF + 0x14));
    DrawSync(0);
#endif
}

void LoadingScreen(void) {
    int i;

    if (!skipFirstLoadingScreen) {
        DrawSync(0);
        DrawBigGuiSprite(2);
        for (i = 0; i < 1; i++) {
            ClearOTagR(&otag[0][i][0], 1026);
            ClearOTagR(&otag[1][i][0], 1026);
        }
        DrawSync(0);
        levelEndReason = 0;
        SetupDisplay(0, 0, 0, 0, 0, 0);
        DrawBigGuiSprite(1);
        InitPsxButtonBackgroundSprites(1);
        DrawSync(0);
        VSync(0);
        VSyncCallback(LoadingScreenVSyncCallback);
    } else {
        skipFirstLoadingScreen = 0;
    }
}

#if !VER_EU
int unusedLoading1 = 1;
#endif
short playingBonusMusic = 0;
short bonusMusicIndex = 0;
#if !VER_EU
char S_could_not_start_playing[] = "could not start playing \n";
char S_bonus_song[] = "bonus song";
char S_resumed_song_after_bonus[] = "resumed song after bonus";
#endif

void PlayMusic(int world) {
#if !VER_EU
    int pad[2];
    int musicSearchAttempt;
#endif
    char dummy[8];

    Noop2();
    SndSetMusicVolume();
    musicCounter = FPS;
    musicSearchAttempt = 0;
    while (CdSearchFile(&musicCdlfile, MUSICS[world].filename) == NULL) {
        if (musicSearchAttempt >= (VER_JP ? 20 : 10))
            break;
        CdInit();
        musicSearchAttempt++;
        if (VER_JP) {
            VSync(2);
        }
    }
#if VER_EU
    if (musicSearchAttempt >= 10) {
        FileError("could not find music \n", MUSICS[world].filename);
    }
#elif VER_JP
    if (musicSearchAttempt >= 20) {
        printf("could not find music %s \n", MUSICS[world].filename);
    }
#endif
    musicStartSector = CdPosToInt(&musicCdlfile.pos);
    musicEndSector = musicStartSector + MUSICS[world].sectors * 4;
    CdReadSync(0, 0);
    while (CdControlB(CdlSetmode, &musicCdMode, 0) == 0) {
        VSYNC_3();
    }
    VSYNC_3();
#if VER_EU
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
#if !VER_EU
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
#if !VER_EU
    int pad[2];
    int bonusMusicSearchAttempt;
#endif
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
        if (bonusMusicSearchAttempt >= (VER_JP ? 20 : 10)) {
            break;
        }
        CdInit();
        bonusMusicSearchAttempt++;
        if (VER_JP) {
            VSync(2);
        }
    }
#if VER_EU
    if (bonusMusicSearchAttempt >= 10) {
        FileError("could not find music \n", BONUS_MUSICS[bonusMusicIndex].filename);
    }
#elif VER_JP
    if (VER_JP && bonusMusicSearchAttempt >= 20) {
        printf("could not find music %s \n", BONUS_MUSICS[bonusMusicIndex].filename);
    }
#endif

    musicStartSector = CdPosToInt(&musicCdlfile.pos);
    musicEndSector = musicStartSector + BONUS_MUSICS[bonusMusicIndex].sectors * 4;
    CdReadSync(0, 0);
    while (CdControlB(CdlSetmode, &musicCdMode, 0) == 0) {
        VSYNC_3();
    }
    VSYNC_3();
#if VER_EU
    CdReadSync(0, 0);
#endif

    CdIntToPos(musicStartSector, &musicCdlLoc);
    CdReadSync(0, 0);
    musicCdlFilter.file = 1;
    musicCdlFilter.chan = BONUS_MUSICS[bonusMusicIndex].chan;
    while (CdControlB(CdlSetfilter, &musicCdlFilter, dummy) == 0) {
        VSYNC_3();
    }
    IF_WHILE (CdControlB(CdlReadS, &musicCdlLoc, dummy) == 0) {
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
    musicCounter = FPS;
    savedMusicStartSector = musicStartSector;
    savedMusicEndSector = musicEndSector;
    savedMusicCurSector = musicCurSector;
    savedMusicXaChan = musicCdlFilter.chan;
    savedMusicCdlLoc = musicCdlLoc;
    while (CdControlB(CdlPause, 0, dummy) == 0) {
        VER_JP ? CdInit() : VSYNC_3();
    }
    CdReadSync(0, 0);
    Noop();
    SpuSetReverb(1);
#if !VER_EU
    playingBonusMusic = 1;
#endif
}

void SwitchFromBonusToNormalMusic(void) {
#if !VER_EU
    int pad[2];
#endif
    char dummy[8];

#if !VER_EU
    if (playingBonusMusic != 1) {
        return;
    }
#endif
    Noop2();
    SndSetMusicVolume();
    musicCurSector = savedMusicCurSector;
    musicStartSector = savedMusicStartSector;
    musicEndSector = savedMusicEndSector;
    musicCdlLoc = savedMusicCdlLoc;
    musicCounter = FPS;
    CdIntToPos(musicCurSector, &musicBonusLoc);
    musicCdlFilter.chan = savedMusicXaChan;
#if VER_EU
    CdReadSync(0, 0);
#endif
    while (CdControlB(CdlSetmode, &musicCdMode, 0) == 0) {
        VSYNC_3();
    }
#if VER_EU
    CdReadSync(0, 0);
#endif
    do {
        VSYNC_3();
    } while (CdControlB(CdlSetfilter, &musicCdlFilter, dummy) == 0);
    IF_WHILE (CdControlB(CdlReadS, &musicBonusLoc, dummy) == 0) {
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
