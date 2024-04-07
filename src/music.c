#include "common.h"

#include <libcd.h>
#include <libgpu.h>

extern void Noop(void);
extern void Noop2(void);

int musicCounter;
int musicCurSector;
int musicEndSector;
int musicStartSector;
int musicUnkAlwaysZero1;
int musicUnkAlwaysZero2;
int savedMusicCurSector;
int savedMusicEndSector;
int savedMusicStartSector;

extern CdlFILTER musicCdlFilter;
extern CdlLOC musicCdlLoc;
extern CdlLOC musicCurLoc;

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
