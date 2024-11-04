#include "common.h"

#include <LIBSIO.H>

extern int GetControllerButtons(int slot);
extern void PutDrawAndDispEnvs(void);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering, u_char use24Bit);

extern int copycatModeStartingPlayer;
extern int gameMode;
extern int levelHasBeenCompletedByPlayer[2];
extern int levelPlaytimesInThisWorld[15];
extern int levelScores[150];
extern int specialLevelType;
extern int timeTrialAtEndOfWorld;

char* sioWritePtr;
int byteCountToReceiveFromSio;
int curLevel;
int curWorld;
int curWorld2;
int debugBonusLevels;
int gameState;
int gotSioData;
int highestLevelReached;
int isDemoMode;
int isFinal;
int latestPlayerToFinish;
int levelAfterBonusLevel;
int levelEndReason;
int levelHasBeenCompletedByPlayer[2];
int levelPlayTime[2];
int levelScore;
int loadNewWorld;
int numTimeTrialPlayers;
int startingPlayerForThisLevel;
int totalPlayTime[2];
int totalScore;
int twoPlayerWhichPlayer;
int vsyncCounter;
int whichDrawDispEnv;
short numFruits;
uint fruitsCollectedBitmask;
uint savedFruitsCollectedBitmask;

// https://decomp.me/scratch/LaYxz
INCLUDE_ASM("asm/nonmatchings/main", main);

void SioRecvVsyncCallback(void) {
    vsyncCounter = vsyncCounter + 1;
}

INCLUDE_ASM("asm/nonmatchings/main", MainGameLoop);

void LevelCompletedOrDied(void) {
    int i;
    int j;
    int penalty;
    int t;

    if (gameMode == 0 && gotSioData == 0) {
        if (loadNewWorld == 0) {
            if (levelEndReason < 0 && specialLevelType == 0) {
                penalty = ((isFinal == 0 ? curWorld2 * 15 : curWorld2 * 2) + curLevel + 1) * 50;
                if (penalty > 5000) {
                    penalty = 5000;
                }
                totalScore -= levelScore + penalty;
                fruitsCollectedBitmask = savedFruitsCollectedBitmask;
            } else {
                if (savedFruitsCollectedBitmask != fruitsCollectedBitmask) {
                    numFruits++;
                }
                totalScore += levelScore;
                savedFruitsCollectedBitmask = fruitsCollectedBitmask;
            }
            if (specialLevelType == 0) {
                levelScores[curWorld2 * 15 + curLevel] = totalScore;
            } else {
                t = levelAfterBonusLevel;
                if (t >= 15) {
                    t = 15;
                }
                if (t < 0) {
                    t = 0;
                }
                levelScores[curWorld2 * 15 + t - 1] = totalScore;
            }
        }
        totalPlayTime[0] += levelPlayTime[0] / 50;
    }
    if (gameMode != 2 || gotSioData != 0 || gameState == 4) {
        return;
    }
    latestPlayerToFinish = twoPlayerWhichPlayer;
    if (loadNewWorld) {
        return;
    }
    if (levelEndReason < 0 && specialLevelType == 0) {
        fruitsCollectedBitmask = savedFruitsCollectedBitmask;
        if (numTimeTrialPlayers == 2) {
            totalPlayTime[twoPlayerWhichPlayer] += 4;
            if (levelHasBeenCompletedByPlayer[0] == 0 && levelHasBeenCompletedByPlayer[1] == 0)
                twoPlayerWhichPlayer = (twoPlayerWhichPlayer + 1) % 2;
            }
    } else {
        if (savedFruitsCollectedBitmask != fruitsCollectedBitmask) {
            numFruits++;
        }
        savedFruitsCollectedBitmask = fruitsCollectedBitmask;
        levelHasBeenCompletedByPlayer[twoPlayerWhichPlayer] = 1;
        if (numTimeTrialPlayers == 1) {
            j = levelPlayTime[0] / 50;
            if (levelPlayTime[0] < 0) {
                j--;
            }
            if (timeTrialAtEndOfWorld == 0 || j < levelPlaytimesInThisWorld[curLevel]) {
                levelPlaytimesInThisWorld[curLevel % 15] = j;
            }
            totalPlayTime[twoPlayerWhichPlayer] = 0;
            for (i = 0; i < 15; i++) {
                totalPlayTime[twoPlayerWhichPlayer] += levelPlaytimesInThisWorld[i];
            }
        } else {
            totalPlayTime[twoPlayerWhichPlayer] += levelPlayTime[twoPlayerWhichPlayer] / 50;
            if (levelPlayTime[twoPlayerWhichPlayer] < 0) {
                totalPlayTime[twoPlayerWhichPlayer] -= 1;
            }

            if (levelHasBeenCompletedByPlayer[0] == 1) {
                if (levelHasBeenCompletedByPlayer[1] != 1) {
                    twoPlayerWhichPlayer = (twoPlayerWhichPlayer + 1) % 2;
                }
            } else if (levelHasBeenCompletedByPlayer[1] == 1) {
                twoPlayerWhichPlayer = (twoPlayerWhichPlayer + 1) % 2;
            }
        }
    }
    if (totalScore >= 0) {
        return;
    }
    gameMode = 0;
    numTimeTrialPlayers = 0;
    if (isDemoMode == 1) {
        return;
    }

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 15; j++) {
            levelScores[i*15+j] = -1;
        }
    }
}

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
        if ((i & 0xff) == 0 || i == byteCountToReceiveFromSio - 1) {
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
