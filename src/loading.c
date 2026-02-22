#include "common.h"

#include <libcd.h>
#include "zlib.h"

extern void PutDrawAndDispEnvs(void);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering,
                         u_char use24Bit);
extern void Noop(void);
extern void Noop2(void);
extern void UnusedLoadFullScreenPicture(void* param_1);
extern void LoadingScreenVSyncCallback(void);

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

char S_KULA_KULA_PIC_PAK_1[] = "\\KULA\\KULA_PIC.PAK;1";
struct {
    short hack;
} skipFirstLoadingScreen = {1}; // XXX: padding somehow wrong

Music BONUS_MUSICS[] = {
    {"\\XA\\MUSIC_1.XA;1", 0, 3335},
    {"\\XA\\MUSIC_2.XA;1", 1, 5474},
    {"\\XA\\MUSIC_0.XA;1", 0, 3279},
};

int musicUnkAlwaysZero1 = 0;
int musicUnkAlwaysZero2 = 0;
int musicCdMode = 0x78;

Music MUSICS[] = {
    {"\\XA\\MUSIC_1.XA;1", 1, 4533},
    {"\\XA\\MUSIC_3.XA;1", 2, 6844},
    {"\\XA\\MUSIC_2.XA;1", 2, 5646},
    {"\\XA\\MUSIC_2.XA;1", 3, 5786},
    {"\\XA\\MUSIC_3.XA;1", 3, 7151},
    {"\\XA\\MUSIC_2.XA;1", 0, 5178},
    {"\\XA\\MUSIC_3.XA;1", 1, 5979},
    {"\\XA\\MUSIC_3.XA;1", 0, 5897},
    {"\\XA\\MUSIC_1.XA;1", 2, 4705},
    {"\\XA\\MUSIC_1.XA;1", 3, 4776},
    {"\\SAMPLE.XA;1", 0, 0},
};

char S_File_error[] = "File error:\n\n";

int sizeOfSfxFile;
int unusedReadErrorCode;

extern char filenameBuf[24];
extern int displayHeight;
extern int displayWidth;
extern int gameMode;
extern int isFinal;
extern int levelEndReason;
extern int whichDrawDispEnv;
extern void* otag[2][1][1026];

static z_stream zlibStream_a4b80;
extern char fileBuf[];

void UnusedFileError(char* str1, char* str2) {
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
}

uint ReadDataFile(int world, int filetype, void* buf) {
    int i;
    int j;
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

    if (!CdSearchFile(&cdlfile, filenameBuf)) {
        VSyncCallback(NULL);
        SetupDisplay(1, 0x80, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_File_error);
        FntPrint("could not find ");
        FntPrint(filenameBuf);
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        while (1)
            ;
    }
    if (!CdControl(CdlSeekL, &cdlfile, 0)) {
        VSyncCallback(NULL);
        SetupDisplay(1, 0x80, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_File_error);
        FntPrint("seek error ");
        FntPrint(filenameBuf);
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        while (1)
            ;
    }
    if (!CdRead((cdlfile.size >> 11) + 1, buf, 0x80)) {
        VSyncCallback(NULL);
        SetupDisplay(1, 0x80, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_File_error);
        FntPrint("read error ");
        FntPrint(filenameBuf);
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        while (1)
            ;
    }
    CdReadSync(0, 0);
    if (filetype == 1) {
        sizeOfSfxFile = cdlfile.size;
    }
    return cdlfile.size;
}

uint UnusedReadKulaPicPak(void* unknown, char* buf) {
    CdlFILE cdlfile;

    Noop2();
    if (CdSearchFile(&cdlfile, S_KULA_KULA_PIC_PAK_1) == 0) {
        unusedReadErrorCode = 2;
    }
    while (CdControl(CdlSeekL, &cdlfile, 0) == 0) {
        unusedReadErrorCode = 3;
    }
    while (CdRead((cdlfile.size >> 11) + 1, buf, 0x80) == 0) {
        unusedReadErrorCode = 4;
    }
    CdReadSync(0, 0);
    Noop();
    return cdlfile.size;
}

void UnusedInflateSomething(int idx, int* data) {
    zlibStream_a4b80.avail_in = data[2 + 2 * idx];
    zlibStream_a4b80.next_in = (char*)data + data[1 + 2 * idx];
    zlibStream_a4b80.avail_out = 0x60000;
    zlibStream_a4b80.next_out = 0xfd000;
    inflateInit_(&zlibStream_a4b80, "1.0.4", 0x38);
    inflate(&zlibStream_a4b80, 4);
    inflateEnd(&zlibStream_a4b80);
    VSync(0);
    UnusedLoadFullScreenPicture(0xfd000);
}

void LoadWarningTim(void) {
    RECT rect = {0, 0, 640, 256};
    char filename[24] = "\\HIRO\\WARNING.TIM;1";
    CdlFILE cdlfile;

    displayWidth = 640;
    displayHeight = 256;
    SetupDisplay(0, 0, 96, 0, 0, 0);
    whichDrawDispEnv = 0;
    PutDrawAndDispEnvs();
    if (!CdSearchFile(&cdlfile, filename)) {
        VSyncCallback(NULL);
        SetupDisplay(1, 0x80, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_File_error);
        FntPrint("could not find ");
        FntPrint(filename);
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        while (1)
            ;
    }
    if (!CdControl(CdlSeekL, &cdlfile, 0)) {
        VSyncCallback(NULL);
        SetupDisplay(1, 0x80, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_File_error);
        FntPrint("seek error ");
        FntPrint(filename);
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        while (1)
            ;
    }
    if (!CdRead((cdlfile.size >> 11) + 1, 0xfd000, 0x80)) {
        VSyncCallback(NULL);
        SetupDisplay(1, 0x80, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_File_error);
        FntPrint("read error ");
        FntPrint(filename);
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        while (1)
            ;
    }
    CdReadSync(0, 0);
    zlibStream_a4b80.next_in = 0xfd000;
    zlibStream_a4b80.avail_out = 0x60000;
    zlibStream_a4b80.next_out = 0x132000;
    zlibStream_a4b80.avail_in = cdlfile.size;
    inflateInit_(&zlibStream_a4b80, "1.0.4", 0x38);
    inflate(&zlibStream_a4b80, 4);
    inflateEnd(&zlibStream_a4b80);
    LoadImage(&rect, 0x132014);
    DrawSync(0);
}

void LoadingScreen(void) {
    int i;

    if (!skipFirstLoadingScreen.hack) {
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
        skipFirstLoadingScreen.hack = 0;
    }
}
