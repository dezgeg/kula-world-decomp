#include "common.h"

#include <libcd.h>

extern void PutDrawAndDispEnvs(void);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering,
                         u_char use24Bit);

int sizeOfSfxFile;

extern char filenameBuf[24];
extern int gameMode;
extern int isFinal;
extern int whichDrawDispEnv;
extern char* EXTENSIONS[5];
extern char* LEVEL_DIRS[11];

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
