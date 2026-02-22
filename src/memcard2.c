#include "common.h"

#include <libmcrd.h>

extern void ShowMemCardFullScreenText(char* str);

int INT_000a5690;
int memCardDataValid;
long mcResult;
long tempMcResult;

extern DrawDisp drawdisp[2];
extern Highscore highscores[6];
extern MemcardData memCardData;
extern char* SAVE_ICON_TIM_PTR;
extern int curLevel;
extern int curWorld;
extern int dispenvScreenX;
extern int dispenvScreenY;
extern int finalUnlocked;
extern int fruitsCollectedBitmask;
extern int gameMode;
extern int highestLevelReached;
extern int highscoreLevelScores[6][150];
extern int isFinal;
extern int levelScores[150];
extern int musicVolume;
extern int savedFruitsCollectedBitmask;
extern int sfxVolume;
extern int timeTrialDifficulty;
extern int totalPlayTime[2];
extern int totalScore;
extern int vibrationEnabled;
extern long mcCmd;
extern short numFruits;
extern short turnDelayEnabled;

static inline void SetDispScreen(ushort x, ushort y) {
    drawdisp[0].disp.screen.x = drawdisp[1].disp.screen.x = x;
    drawdisp[0].disp.screen.y = drawdisp[1].disp.screen.y = y;
}

static inline void ClearMemCardData() {
    uint i;
    char* p = &memCardData;
    for (i = 0; i < 8196; i++) {
        (*p++) = 0;
    }
}

void LoadHighscoresFromMemcardData(void) {
    int i;
    int j;

    if (highscores[0].score < memCardData.highscore0.score) {
        highscores[0].score = memCardData.highscore0.score;
        highscores[0].time = memCardData.highscore0.time;
        highscores[0].fruits = memCardData.highscore0.fruits;
        highscores[0].level = memCardData.highscore0.level;
        for (i = 0; i < 8; i++) {
            highscores[0].name[i] = memCardData.highscore0.name[i];
        }

        for (j = 0; j < 150; j++) {
            highscoreLevelScores[0][j] = memCardData.highscore0LevelScores[j];
        }
    }

    if (highscores[0].score > memCardData.highscore0.score) {
        memCardData.highscore0.score = highscores[0].score;
        memCardData.highscore0.time = highscores[0].time;
        memCardData.highscore0.fruits = highscores[0].fruits;
        memCardData.highscore0.level = highscores[0].level;

        for (i = 0; i < 8; i++) {
            memCardData.highscore0.name[i] = highscores[0].name[i];
        }

        for (j = 0; j < 150; j++) {
            memCardData.highscore0LevelScores[j] = highscoreLevelScores[0][j];
        }
    }

    for (i = 0; i < 4; i++) {
        if (memCardData.saveslots[i].valid == 1) {
            highscores[i + 1].score = memCardData.saveslots[i].score;
            highscores[i + 1].time = memCardData.saveslots[i].playtime;
            highscores[i + 1].fruits = memCardData.saveslots[i].fruits;

            if (memCardData.saveslots[i].isFinal == 0) {
                highscores[i + 1].level =
                    memCardData.saveslots[i].curWorld * 15 + memCardData.saveslots[i].curLevel + 1;
                for (j = 0; j < 150; j++) {
                    highscoreLevelScores[i + 1][j] = memCardData.saveslots[i].levelScores[j];
                }
            } else {
                highscores[i + 1].level =
                    memCardData.saveslots[i].curWorld * 2 + memCardData.saveslots[i].curLevel + 151;
                for (j = 0; j < 150; j++) {
                    highscoreLevelScores[i + 1][j] = 0;
                }
            }
        } else {
            highscores[i + 1].score = 0;
            highscores[i + 1].time = 0;
            highscores[i + 1].fruits = 0;
            highscores[i + 1].level = 0;
            for (j = 0; j < 150; j++) {
                highscoreLevelScores[i + 1][j] = 0;
            }
        }
    }
}

void FormatMemcard(void) {
    extern char S_FORMATTING_MEMORY_CARD_PLEASE_WAIT[];
    MemCardAccept(0);
    MemCardSync(0, &mcCmd, &tempMcResult);
    if (tempMcResult == McErrNotFormat) {
        ShowMemCardFullScreenText(S_FORMATTING_MEMORY_CARD_PLEASE_WAIT);
        tempMcResult = MemCardFormat(0);
    }
    mcResult = tempMcResult;
}

int LoadSaveSlot(uint slot) {
    extern char S_BESCES_01000KULA[];
    int i;

    MemCardAccept(0);
    MemCardSync(0, &mcCmd, &tempMcResult);

    if (tempMcResult == McErrNewCard || (tempMcResult == 0 && memCardDataValid == 0)) {
        ClearMemCardData();
        if (MemCardReadFile(0, S_BESCES_01000KULA, &memCardData, 0, 0x1000) == McErrCardNotExist) {
            MemCardSync(0, &mcCmd, &tempMcResult);
            if (tempMcResult == 0) {
                memCardDataValid = 1;
            } else {
                memCardDataValid = 0;
                ClearMemCardData();
            }
            mcResult = tempMcResult;
        }
    } else if (tempMcResult != 0) {
        ClearMemCardData();
        memCardDataValid = 0;
        mcResult = tempMcResult;
    }

    if (memCardDataValid == 0) {
        return 0;
    }

    LoadHighscoresFromMemcardData();
    if (memCardData.arcadeModeDone == 1) {
        finalUnlocked = memCardData.arcadeModeDone;
    }
    if (memCardData.highestLevelReached > highestLevelReached) {
        highestLevelReached = memCardData.highestLevelReached;
    }
    if (slot < 4) {
        musicVolume = memCardData.musicVolume;
        sfxVolume = memCardData.sfxVolume;
        dispenvScreenX = memCardData.screenX;
        dispenvScreenY = memCardData.screenY;
        turnDelayEnabled = memCardData.turnDelayEnabled;
        vibrationEnabled = memCardData.vibrationEnabled;
        for (i = 0; i < 150; i++) {
            levelScores[i] = memCardData.saveslots[slot].levelScores[i];
        }
        gameMode = memCardData.saveslots[slot].gameMode;
        if (gameMode == 0) {
            totalScore = memCardData.saveslots[slot].score;
            totalPlayTime[0] = memCardData.saveslots[slot].playtime;
        } else {
            totalPlayTime[0] = memCardData.saveslots[slot].score;
        }
        curLevel = memCardData.saveslots[slot].curLevel;
        timeTrialDifficulty = memCardData.saveslots[slot].timeTrialDifficulty;
        curWorld = memCardData.saveslots[slot].curWorld;
        savedFruitsCollectedBitmask = memCardData.saveslots[slot].fruitBitmask;
        numFruits = memCardData.saveslots[slot].fruits;
        isFinal = memCardData.saveslots[slot].isFinal;
        fruitsCollectedBitmask = savedFruitsCollectedBitmask;
    }
    return 1;
}

extern char S_KULA_WORLD[];
extern char S_BESCES_01000KULA[16];

int SaveMemCard(uint slot) {
    int ret;
    int i;
    char buf[32];

    MemCardAccept(0);
    MemCardSync(0, &mcCmd, &tempMcResult);
    if (tempMcResult == 0) {
        memCardData.header.magic[0] = 'S';
        memCardData.header.magic[1] = 'C';
        memCardData.header.iconFlags = 0x11;
        memCardData.header.blockNumber = 1;
        sprintf(buf, S_KULA_WORLD);
        for (i = 0; i < 32; i++) {
            memCardData.header.title[i] = Ascii2Sjis(buf[i]);
        }
        for (i = 0; i < 28; i++) {
            memCardData.header.unused[i] = 0;
        }
        for (i = 0; i < 16; i++) {
            memCardData.header.iconClut[i] = ((short*)(SAVE_ICON_TIM_PTR + 20))[i];
        }
        for (i = 0; i < 128; i++) {
            memCardData.header.iconBitmap[i] = (SAVE_ICON_TIM_PTR + 64)[i];
        }

        if (finalUnlocked == 1 || (curWorld == 10 && curLevel == 0)) {
            memCardData.arcadeModeDone = 1;
        }
        if (highestLevelReached > memCardData.highestLevelReached) {
            memCardData.highestLevelReached = highestLevelReached;
        }
        memCardData.turnDelayEnabled = turnDelayEnabled;
        memCardData.vibrationEnabled = vibrationEnabled;
        memCardData.musicVolume = musicVolume;
        memCardData.sfxVolume = sfxVolume;
        if (slot < 4) {
            memCardData.screenX = dispenvScreenX;
            memCardData.screenY = dispenvScreenY;
            for (i = 0; i < 150; i++) {
                memCardData.saveslots[slot].levelScores[i] = levelScores[i];
            }
            if (gameMode == 0) {
                memCardData.saveslots[slot].playtime = totalPlayTime[0];
                memCardData.saveslots[slot].score = totalScore;
            } else {
                memCardData.saveslots[slot].playtime = totalPlayTime[0];
                memCardData.saveslots[slot].score = totalPlayTime[0];
            }
            memCardData.saveslots[slot].valid = 1;
            memCardData.saveslots[slot].timeTrialDifficulty = timeTrialDifficulty;
            memCardData.saveslots[slot].curLevel = curLevel;
            memCardData.saveslots[slot].curWorld = curWorld;
            memCardData.saveslots[slot].fruitBitmask = savedFruitsCollectedBitmask;
            memCardData.saveslots[slot].fruits = numFruits;
            memCardData.saveslots[slot].isFinal = isFinal;
            memCardData.saveslots[slot].gameMode = gameMode;
        }
        ret = MemCardCreateFile(0,S_BESCES_01000KULA,1);
        if (ret == 0 || ret == McErrAlreadyExist) {
            if (MemCardWriteFile(0,S_BESCES_01000KULA, (long*)&memCardData,0,0x1000) == McErrCardNotExist) {
                MemCardSync(0,&mcCmd,&tempMcResult);
                if (tempMcResult != 0) {
                    INT_000a5690 = 1;
                    mcResult = tempMcResult;
                    return 0;
                }
            }
        } else {
            INT_000a5690 = 1;
            mcResult = ret;
            return 0;
        }
    }  else {
            INT_000a5690 = 1;
            mcResult = tempMcResult;
            return 0;
    }
    LoadHighscoresFromMemcardData();

    return 1;
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
