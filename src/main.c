#include "common.h"

#include <LIBSIO.H>

extern int GetControllerButtons(int slot);
extern void PutDrawAndDispEnvs(void);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering, u_char use24Bit);

extern int copycatModeStartingPlayer;
extern int gameMode;
extern int levelHasBeenCompletedByPlayer[2];
extern int specialLevelType;
extern int timeTrialAtEndOfWorld;

char* sioWritePtr;
int byteCountToReceiveFromSio;
int curLevel;
int curWorld;
int debugBonusLevels;
int gotSioData;
int highestLevelReached;
int levelAfterBonusLevel;
int levelEndReason;
int loadNewWorld;
int numTimeTrialPlayers;
int startingPlayerForThisLevel;
int twoPlayerWhichPlayer;
int vsyncCounter;
int whichDrawDispEnv;
uint fruitsCollectedBitmask;
uint savedFruitsCollectedBitmask;

// https://decomp.me/scratch/LaYxz
INCLUDE_ASM("asm/nonmatchings/main", main);

void SioRecvVsyncCallback(void) {
    vsyncCounter = vsyncCounter + 1;
}

INCLUDE_ASM("asm/nonmatchings/main", MainGameLoop);

// https://decomp.me/scratch/HBwQp
INCLUDE_ASM("asm/nonmatchings/main", LevelCompletedOrDied);

void DecideNextLevel(void) {
    int bVar1;
    int i;

    if (gotSioData != 0) {
        return;
    }
    if (gameMode == 0 && specialLevelType == 0 && levelEndReason > 0) {
        if (levelEndReason == 3) {
            curLevel++;
            if (curLevel == 15) {
                curLevel = *(int*)0x156000;
            }
            levelAfterBonusLevel = curLevel;
            curLevel = 18;
        }
        else {
            curLevel++;
            if ((savedFruitsCollectedBitmask & 0x1f) == 0x1f) {
                fruitsCollectedBitmask = 0;
                savedFruitsCollectedBitmask = 0;
                levelAfterBonusLevel = curLevel;
                if (curLevel == 15 && debugBonusLevels == 0) {
                    levelAfterBonusLevel = *(int*)0x156000;
                }
                curLevel = ((curLevel - 1) / 5) % 3 + 15;
            } else {
                if (curLevel == 15 && debugBonusLevels == 0) {
                    curLevel = *(int*)0x156000;
                }
            }
        }
    }
    if (gameMode == 1 && specialLevelType == 0 && levelEndReason < 0) {
        if (copycatModeStartingPlayer == 1) {
            curLevel++;
            curWorld++;
            if (curWorld > highestLevelReached / 15) {
                curWorld = 0;
            }
            loadNewWorld = 1;
        }
        if (curWorld > 9) {
            curLevel = 0;
            curWorld = 0;
        }
    }
    if (gameMode == 2 && specialLevelType == 0 && levelEndReason > 0) {
        bVar1 = 0;
        for (i = 0; i < numTimeTrialPlayers; i++) {
            if (levelHasBeenCompletedByPlayer[i] != 1) {
                bVar1 = 1;
            }
        }
        if (!bVar1) {
            curLevel++;
            if (curLevel == 15) {
                curLevel = *(int*)0x156000;
            }
            if (numTimeTrialPlayers == 2) {
                startingPlayerForThisLevel = (startingPlayerForThisLevel + 1) % 2;
            }
            twoPlayerWhichPlayer = startingPlayerForThisLevel;
            if (savedFruitsCollectedBitmask == 0x1f) {
                savedFruitsCollectedBitmask = 0;
                fruitsCollectedBitmask = 0;
            }
        }
    }
    if (specialLevelType > 0) {
        if (curLevel < *(int*)0x156000 && debugBonusLevels == 0) {
            if (((savedFruitsCollectedBitmask & 0x1f) == 0x1f)) {
                fruitsCollectedBitmask = 0;
                savedFruitsCollectedBitmask = 0;
                if (levelAfterBonusLevel > 5) {
                    if (levelAfterBonusLevel > 10) {
                        curLevel = 17;
                    } else {
                        curLevel = 16;
                    }
                } else {
                    curLevel = 15;
                }
            } else {
                curLevel = levelAfterBonusLevel;
            }
        }
        if (specialLevelType > 0) {
            if (curLevel >= *(int*)0x156000) goto LAB_00042f60;
            if (debugBonusLevels == 2) {
                if (levelEndReason > 0) {
                    curLevel++;
                    if (curLevel > 17)
                        curLevel = *(int*)0x156000;
                }
            }
        }
    }
    if (curLevel < *(int*)0x156000) {
        return;
    }
LAB_00042f60:
    switch (debugBonusLevels) {
        case 0:
            curLevel = 0;
            break;
        case 1:
            curLevel = *(int*)0x156000 - 1;
            break;
        case 2:
            curLevel = 15;
            break;
    }
    if (timeTrialAtEndOfWorld == 0 || gameMode != 2) {
        curWorld++;
    }
    loadNewWorld = 1;
}

void ReceiveBufFromSio(void) {
    extern char S_recived_FMTd[];
    extern char S_of_FMTd_bytes[];

    int i;

    SetupDisplay(1,128,0,0,0,0);
    FntFlush(-1);
    DrawSync(0);
    whichDrawDispEnv = 0;
    PutDrawAndDispEnvs();
    sioWritePtr = 0x0015d800;
    for (i = 0; i < byteCountToReceiveFromSio; i++) {
        vsyncCounter = 0;
        while ((_sio_control(0,0,0) & 2) == 0) {
            if (vsyncCounter > 50) {
                gotSioData = 0;
                SetupDisplay(0,0,0,0,0,0);
                return;
            }
        }
        *sioWritePtr++ = _sio_control(0,4,0);
        if (((i & 0xff) == 0) || i == byteCountToReceiveFromSio - 1) {
            FntPrint(S_recived_FMTd,i + 1);
            FntPrint(S_of_FMTd_bytes,byteCountToReceiveFromSio);
            FntFlush(-1);
            whichDrawDispEnv = !whichDrawDispEnv;
            PutDrawAndDispEnvs();
            if (GetControllerButtons(0) & PAD_SELECT) {
                gotSioData = 0;
                SetupDisplay(0,0,0,0,0,0);
                return;
            }
        }
    }
    gotSioData = 1;
    SetupDisplay(0,0,0,0,0,0);
}
