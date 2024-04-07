#include "common.h"

#include <libcd.h>

extern void Noop(void);
extern void Noop2(void);

int musicCounter;
int musicCurSector;
int musicEndSector;
int musicUnkAlwaysZero1;
int musicUnkAlwaysZero2;

extern CdlFILTER musicCdlFilter;
extern CdlLOC musicCdlLoc;
extern CdlLOC musicCurLoc;

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
