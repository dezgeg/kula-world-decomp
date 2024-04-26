#include "common.h"

#include <libcd.h>
#include "zlib.h"

extern void PutDrawAndDispEnvs(void);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering,
                         u_char use24Bit);
extern void Noop(void);
extern void Noop2(void);
extern void UnusedLoadFullScreenPicture(void* param_1);

int sizeOfSfxFile;
int unusedReadErrorCode;

extern char filenameBuf[24];
extern int gameMode;
extern int isFinal;
extern int whichDrawDispEnv;
extern char* EXTENSIONS[5];
extern char* LEVEL_DIRS[11];

static z_stream zlibStream_a4b80;
extern char fileBuf[];

void UnusedFileError(char* str1, char* str2) {
    extern char S_File_error[];
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
    extern char S_File_error[];
    extern char S_could_not_find[];
    extern char S_seek_error[];
    extern char S_read_error[];
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
        FntPrint(S_could_not_find);
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
        FntPrint(S_seek_error);
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
        FntPrint(S_read_error);
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
    extern char S_KULA_KULA_PIC_PAK_1[];
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
    extern char S_1_0_4_3[];

    zlibStream_a4b80.avail_in = data[2 + 2 * idx];
    zlibStream_a4b80.next_in = (char*)data + data[1 + 2 * idx];
    zlibStream_a4b80.avail_out = 0x60000;
    zlibStream_a4b80.next_out = 0xfd000;
    inflateInit_(&zlibStream_a4b80, S_1_0_4_3, 0x38);
    inflate(&zlibStream_a4b80, 4);
    inflateEnd(&zlibStream_a4b80);
    VSync(0);
    UnusedLoadFullScreenPicture(0xfd000);
}
