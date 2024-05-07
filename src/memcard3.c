#include "common.h"

#include <libmcrd.h>

extern void LoadHighscoresFromMemcardData(void);

int memCardDataValid;
long mcResult;
long tempMcResult;

extern int dispenvScreenX;
extern int dispenvScreenY;
extern DrawDisp drawdisp[2];
extern int finalUnlocked;
extern int highestLevelReached;
extern long mcCmd;
extern MemcardData memCardData;
extern int musicVolume;
extern int sfxVolume;
extern short turnDelayEnabled;
extern int vibrationEnabled;

static inline void SetDispScreen(ushort x, ushort y) {
    drawdisp[0].disp.screen.x = drawdisp[1].disp.screen.x = x;
    drawdisp[0].disp.screen.y = drawdisp[1].disp.screen.y = y;
}

void LoadSaveFromMemoryCard(void){
    extern char S_BESCES_01000KULA[];
    uint i;
    char* p = &memCardData;

    for (i = 0; i < 8196; i++) {
        *p++ = 0;
    }
    MemCardAccept(0);
    MemCardSync(0, &mcCmd, &tempMcResult);
    if ((tempMcResult == 0) || (tempMcResult == McErrNewCard)) {
        tempMcResult = McErrCardNotExist;
        if (MemCardReadFile(0, S_BESCES_01000KULA, (unsigned long*)&memCardData, 0, 0x1000)) {
            MemCardSync(0, &mcCmd, &tempMcResult);
        }
        if (tempMcResult == 0) {
            if (memCardData.arcadeModeDone == McErrCardNotExist) {
                finalUnlocked = McErrCardNotExist;
            }
            if (highestLevelReached < memCardData.highestLevelReached) {
                highestLevelReached = (int)memCardData.highestLevelReached;
            }
            turnDelayEnabled = memCardData.turnDelayEnabled;
            vibrationEnabled = memCardData.vibrationEnabled;
            musicVolume = memCardData.musicVolume;
            sfxVolume = memCardData.sfxVolume;
            dispenvScreenX = memCardData.screenX;
            dispenvScreenY = memCardData.screenY;
            LoadHighscoresFromMemcardData();
            memCardDataValid = 1;
            SetDispScreen(dispenvScreenX, dispenvScreenY);
            mcResult = tempMcResult;
        }
    }
}
