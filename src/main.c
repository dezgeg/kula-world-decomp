#include "common.h"

#include <LIBSIO.H>
#include "zlib.h"

extern int GetControllerButtons(int slot);
extern int MainGameLoop(void);
extern int ParseGGI(void* ggi); // hack
extern uint ReadDataFile(int world, int filetype, void* buf);
extern void InitCopycatUiTextures(void);
extern void InitFont(void);
extern void InitHighscores(void);
extern void InitSpinningSelectionSprites(void);
extern void InitStuff(void);
extern void InitVariousUiSpriteTemplates(void);
extern void LevelInit(void);
extern void LoadingScreen(void);
extern void LoadSaveFromMemoryCard(void);
extern void LoadWarningTim(void);
extern void Noop(void);
extern void ParseLevelDataFromTgi(void);
extern void ParseTGI(void* tgiBuf); // hack
extern void PlayBonusMusic(void);
extern void PlayMusic(int world);
extern void PutDrawAndDispEnvs(void);
extern void ResetCopycatMode(int param_1);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering, u_char use24Bit);
extern void SndInitFromSfxFile(void * file, int length); // hack
extern void SndMuteAllVoices(void);
extern void SndMuteVoiceByTag(int tag);
extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);
extern void SwitchFromBonusToNormalMusic(void);
extern void VariousInit(void);
extern void WriteToDevkit(int param_1);

extern int copycatModeStartingPlayer;
extern int DEMO_LEVELS[3];
extern int dispenvScreenX;
extern int dispenvScreenY;
extern int drawCopyright;
extern int gameMode;
extern int isPaused;
extern int levelHasBeenCompletedByPlayer[2];
extern int levelPlaytimesInThisWorld[15];
extern int levelScores[150];
extern int loadingIsComplete;
extern int specialLevelType;
extern int timeTrialAtEndOfWorld;
extern SVECTOR ZERO_SVECTOR_a2f04;

char* sioWritePtr;
int byteCountToReceiveFromSio;
int curFileLength;
int curLevel;
int curWorld2;
int curWorld;
int debugBonusLevels;
int demoIndex;
int devkitAltBoot;
int displayHeight;
int displayModeHasBeenSet;
int displayWidth;
int fruitsCollectedBitmask;
int gameState;
int gotSioData;
int highestLevelReached;
int inflateRetCode1;
int inflateRetCode2;
int inflateRetCode3;
int isDemoMode;
int isFinal;
int latestPlayerToFinish;
int levelAfterBonusLevel;
int levelEndReason;
int levelHasBeenCompletedByPlayer[2];
int levelPlayTime[2];
int levelScore;
int loadNewWorld;
int loadedInitialSave;
int mainGameLoopRetVal;
int numTimeTrialPlayers;
int prevLevelEndReason;
int savedFruitsCollectedBitmask;
int skipNextLoad;
int startingPlayerForThisLevel;
int totalPlayTime[2];
int totalScore;
int twoPlayerWhichPlayer;
int vsyncCounter;
int whichDrawDispEnv;
short numFruits;

static z_stream levelLoadZlibStream;

void main(void) {
    extern char S_FMTx[];
    extern char S_rescue_session_saved_as_psx_cube_pad_rescue_pad[];
    extern char S_to_replay[];
    extern char S_replay[];
    extern char S_1_0_4_2[];

    int i;
    int j;

    isDemoMode = 0;
    dispenvScreenX = 0;
    dispenvScreenY = 18;
    InitStuff();
    Noop();
    if (devkitAltBoot == 1) {
        WriteToDevkit(1);
        SetupDisplay(1, 0, 0x80, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_rescue_session_saved_as_psx_cube_pad_rescue_pad);
        FntPrint(S_to_replay);
        FntPrint(S_replay);
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        while (1)
            ;
    }
    isPaused = 1;
    loadNewWorld = 1;
    CdInit();
                                        /* sfx */
    curFileLength = ReadDataFile(curWorld,1,0xFD000);
    SndInitFromSfxFile(0xFD000,curFileLength);
    LoadWarningTim();
                                        /* piracy screen music */
    SndPlaySfx(0x6f,0x378,&ZERO_SVECTOR_a2f04,8000);
                                        /* ggi */
    curFileLength = ReadDataFile(curWorld,3,0xFD000);
    i = ParseGGI(0xFD000);
    printf(S_FMTx, i);
    InitFont();
    InitVariousUiSpriteTemplates();
    InitHighscores();
    InitCopycatUiTextures();
    InitSpinningSelectionSprites();
    fruitsCollectedBitmask = 0;
    savedFruitsCollectedBitmask = 0;
    levelScore = 0;
    totalScore = 0;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 15; j++) {
            levelScores[i * 15 + j] = -1;
        }
    }
    levelEndReason = 0;
    prevLevelEndReason = 0;
    do {
        if (isDemoMode == 1) {
            curLevel = DEMO_LEVELS[demoIndex] % 15;
            if (curWorld !=  DEMO_LEVELS[demoIndex] / 15) {
                loadNewWorld = 1;
            }
            curWorld =  DEMO_LEVELS[demoIndex] / 15;
            demoIndex = (demoIndex + 1) % 3;
        }
        if (loadNewWorld && !skipNextLoad) {
            LoadingScreen();
            if (!loadedInitialSave) {
                LoadSaveFromMemoryCard();
                loadedInitialSave = 1;
            } else {
                /* Loading screen music */
                SndPlaySfx(111,888,&ZERO_SVECTOR_a2f04,8000);
            }
            /* .tgi */
            curFileLength = ReadDataFile(curWorld,0,0x132000);
            if (displayModeHasBeenSet == 0) {
                displayWidth = 320;
                displayHeight = 256;
                SetupDisplay(1, 0, 0, 0, 0, 0);
                displayModeHasBeenSet = 1;
                VSync(0);
                VSync(0);
            }
            ParseTGI(0x132000);
            loadingIsComplete = 888;
            curWorld2 = curWorld;
            /* .pak */
            curFileLength = ReadDataFile(curWorld,2,0x156000);
            PlayMusic(curWorld);
            loadNewWorld = 0;
            VSyncCallback(NULL);
            SndMuteVoiceByTag(888);
            loadingIsComplete = -1;
        }
        skipNextLoad = 0;
        if (curLevel >= *(int* )0x156000) {
            curLevel = *(int* )0x156000 - 1;
        }
        if (curLevel < 0) {
            curLevel = 0;
        }
        if (gotSioData != 0) {
            if (gotSioData == 1) {
                levelLoadZlibStream.avail_in = byteCountToReceiveFromSio;
                levelLoadZlibStream.next_in = 0x15D800;
                levelLoadZlibStream.avail_out = 0x60000;
                levelLoadZlibStream.next_out = 0x15E000;
                inflateRetCode1 = inflateInit_(&levelLoadZlibStream,S_1_0_4_2,0x38);
                inflateRetCode2 = inflate(&levelLoadZlibStream,4);
                inflateRetCode3 = inflateEnd(&levelLoadZlibStream);
                if (inflateRetCode1 < 0 || inflateRetCode2 < 0 || inflateRetCode3 < 0) {
                    gotSioData = 0;
                }
            } else {
                memcpy(0x15E000, 0x650000, 0x66b000 - 0x650000);
            }
            if (gotSioData == 0) goto loadFromPak;
        } else {
loadFromPak:
            levelLoadZlibStream.avail_in = *(uint *)(0x156008 + curLevel * 8);
            levelLoadZlibStream.next_in = 0x156000 + *(int *)(0x156004 + curLevel * 8);
            levelLoadZlibStream.avail_out = 0x60000;
            levelLoadZlibStream.next_out = 0x15E000;
            inflateRetCode1 = inflateInit_(&levelLoadZlibStream,S_1_0_4_2,0x38);
            inflateRetCode2 = inflate(&levelLoadZlibStream,4);
            inflateRetCode3 = inflateEnd(&levelLoadZlibStream);
        }
        if (specialLevelType != 0) {
            SwitchFromBonusToNormalMusic();
        }
        ParseLevelDataFromTgi();
        LevelInit();
        if (gameMode == 1 && mainGameLoopRetVal < 1) {
            copycatModeStartingPlayer ^= 1;
            ResetCopycatMode(copycatModeStartingPlayer);
        }
        VariousInit();
        if (gameState != 0 && gameState != 4 &&
             (gameMode == 0 || (gameMode == 2 && numTimeTrialPlayers == 1))) {
                                        /* level begin sfx (but also done in other places?) */
            SndPlaySfx(30,0,&ZERO_SVECTOR_a2f04,7000);
        }
        if (specialLevelType != 0) {
            PlayBonusMusic();
        }
        mainGameLoopRetVal = MainGameLoop();
        SndMuteAllVoices();
        drawCopyright = 0;
        DrawSync(0);
        VSync(0);
    } while(1);
}

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
            levelScores[i * 15 + j] = -1;
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
        } else {
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

    SetupDisplay(1, 128, 0, 0, 0, 0);
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
