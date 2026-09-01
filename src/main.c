#include "common.h"

#include <LIBSIO.H>
#include "zlib.h"

typedef struct FramebufferFetch {
    short x;
    short y;
    short texel;
} FramebufferFetch;

extern int GetControllerButtons(int slot);
extern int GetControllerStatus(int slot);
extern int GetReadyScreen(void);
extern int MainGameLoop(void);
extern int ParseGGI(void* ggi); // hack
extern uint ReadDataFile(int world, int filetype, void* buf);
extern void AddDrChangePrims(void);
extern void DecideNextLevel(void);
extern void DrawBigGuiSprite(int param_1);
extern void DrawHud(void);
extern void DrawLensFlares(int unknown);
extern void DrawLevelScoreSummary(void);
extern void DrawTextCrappyFont(char* str);
extern void HandleCheats(void);
extern void HighScoreUi(int unk);
extern void InitAllDigitSprites(void);
extern void InitCopycatUiTextures(void);
extern void InitFont(void);
extern void InitHighscores(void);
extern void InitParticles(void);
extern void InitReplayPlayback(char* param_1);
extern void InitReplaySaving(void);
extern void InitSpinningSelectionSprites(void);
extern void InitStuff(void);
extern void InitVariousUiSpriteTemplates(void);
extern void LevelInit(void);
extern void LevelCompletedOrDied(void);
extern void LoadingScreen(void);
extern void LoadSaveFromMemoryCard(void);
extern void LoadWarningTim(void);
extern void MainGameVSyncCallback(void);
extern void MusicCheckForLoop(void);
extern void Noop(void);
extern void ParseLevelDataFromTgi(void);
extern void ParseTGI(void* tgiBuf); // hack
extern void PauseOrMainMenu(void);
extern void PlayBonusMusic(void);
extern void PlayMusic(int world);
extern void ProcessAndRenderParticles(void);
extern void ProcessPlayer(void);
extern void ProcessScreenShake(void);
extern void PutDrawAndDispEnvs(void);
extern void QuitToMainMenu(void);
extern void ReceiveBufFromSio(void);
extern void RenderBackground(void);
extern void RenderEverythingElseAndProcessSomeStuff(void);
extern void RenderPlayerOrCopycatLabels(int sprite, char color);
extern void ResetCopycatMode(int param_1);
extern void ResetLethargyEffect(void);
extern void ResetTextRenderState(void);
extern void ResetTextVars(void);
extern void ResetVibration(void);
extern void SavePointMenu(void);
extern void SetPausedOrWaitingForRestart(void);
extern void SetTextParams(int posX, int posY, int align, int colorR, int colorG, int colorB);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering, u_char use24Bit);
extern void ShowEndingFmv(int fmvI);
extern void SndInitFromSfxFile(void* file, int length); // hack
extern void SndMuteAllTaggedVoices(void);
extern void SndMuteAllVoices(void);
extern void SndMuteVoiceByTag(int tag);
extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);
extern void SndProcessSpuVoices(void);
extern void SwitchFromBonusToNormalMusic(void);
extern void UnusedProcessDisplayModeChange(void);
extern void UpdateScoreAtEndOfLevel(void);
extern void UpdateVibration(void);
extern void VariousInit(void);
extern void WaitForVBlank(void);
extern void WriteToDevkit(int param_1);

extern FramebufferFetch framebufferFetches[16];
extern int cheated;
extern int copycatModeStartingPlayer;
extern int copycatPlayerScores[2];
extern int curController;
extern int curMenu;
extern int cursorPosInMenu[8];
extern int dispenvScreenX;
extern int dispenvScreenY;
extern int drawBonusWidget;
extern int drawCopycatWidgets;
extern int drawCopyright;
extern int drawFruitWidgets;
extern int drawGeometryAndObjects;
extern int drawHourglassWidget;
extern int drawKeyWidget;
extern int drawScoreWidget;
extern int drawTimeAttackWidgets;
extern int drawTimerPausedWidget;
extern int gameMode;
extern int gotVsync;
extern int halfFps;
extern int INT_ARRAY_000a51b0[2];
extern int isPaused;
extern int lethargyMode;
extern int levelHasBeenCompletedByPlayer[2];
extern int levelPlayTime[2];
extern int levelPlaytimesInThisWorld[15];
extern int levelScores[150];
extern int levelScoreSummaryConfirmed;
extern int levelTimeLeft;
extern int loadingIsComplete;
extern int savePointMenuConfirmed;
extern int sioCheatActivated;
extern int specialLevelType;
extern int timeTrialAtEndOfWorld;
extern int TIME_TRIAL_PAR_TIMES[150];
extern int unkOtagLengths[2][1];
extern int unkUnused;
extern int wasPausedPreviousFrame;
extern PrimList primLists[2];
extern short numEntities;
extern short SHORT_ARRAY_ARRAY_ARRAY_000d4678[8][8][8];
extern SVECTOR ZERO_SVECTOR_a2f04;
extern TgiFile* tgi;
extern uint controllerButtons;
extern uint prevControllerButtons;
extern OT_TYPE otag[2][1][1026];
extern void* renderedPrimsBuf;
extern void* REPLAY_INPUT_PTRS;
extern OT_TYPE unkOtag[2][1][32][64];

char* sioWritePtr;
int buttonSaveReplayMode;
int byteCountToReceiveFromSio;
int curFileLength;
int dispModeChangeState;
int fruitsCollectedBitmask;
int inflateRetCode1;
int inflateRetCode2;
int inflateRetCode3;
int inGetReadyScreen;
int isDemoMode;
int latestPlayerToFinish;
int levelAfterBonusLevel;
int levelEndReason;
int levelHasBeenCompletedByPlayer[2];
int levelPlayTime[2];
int levelScore;
int mainGameLoopRetVal;
int menuIdleTimer;
int numTimeTrialPlayers;
int prevLevelEndReason;
int savedFruitsCollectedBitmask;
int startingPlayerForThisLevel;
int totalPlayTime[2];
int totalScore;
int twoPlayerWhichPlayer;
int whichDrawDispEnv;
short numFruits;

static z_stream levelLoadZlibStream;

int D_000A2E54[4] = { 0, 0, 0, 0 };
int numCameras = 1;
int cameraIndex = 0;
int displayWidth = 320;
int gteXScale = 4096;
int displayHeight = 256;
int gteYScale = 4096;
long projectionDistance = 160;
int screenOffsetX = 320;
int screenOffsetY = 256;
int devkitAltBoot = 0;
int gotSioData = 0;
int prevLevelTimeLeft = 0;
int numKeysRemaining = 0;
int D_000A2E98 = 0;
int highestLevelReached = 30;
int curLevel = 0;
int curWorld = 0;
int curWorld2 = 0;
int loadNewWorld = 1;
int skipNextLoad = 0;
int loadedInitialSave = 0;
int displayModeHasBeenSet = 0;
int unusedFrameCounter = 0;
int gameState = 0;
int demoTimer = 400;
int demoIndex = 0;
int D_000A2ECC = 0;
int debugBonusLevels = 0;
int debugDisableTimer = 0;
int finalUnlocked = 0;
int isFinal = 0;
int musicShouldLoop = 1;

int D_000A2EE4[6] = { 0, 0, 0, 0, 0, 0 };
short copycatNewOrCopyMoves = 0;
short D_000A2EFE = 0;
int timeTrialDifficulty = 0;
SVECTOR ZERO_SVECTOR_a2f04 = { 0, 0, 0, 0 };
int DEMO_LEVELS[] = { 4, 19, 41 };
int TIME_TRIAL_PAR_TIMES[] = {
    1, 6, 5, 12, 3,
    16, 19, 9, 8, 14,
    12, 13, 10, 5, 22,
    12, 14, 12, 16, 16,
    1, 3, 9, 14, 7,
    12, 7, 8, 8, 13,
    3, 7, 5, 13, 14,
    5, 14, 12, 7, 13,
    18, 23, 23, 21, 27,
    16, 16, 6, 16, 15,
    5, 23, 8, 19, 7,
    20, 11, 18, 45, 27,
    1, 13, 12, 32, 26,
    18, 19, 24, 33, 18,
    25, 14, 16, 26, 31,
    15, 18, 3, 8, 12,
    9, 46, 14, 23, 18,
    13, 4, 15, 17, 21,
    11, 18, 27, 15, 39,
    20, 19, 41, 30, 50,
    30, 39, 30, 16, 21,
    28, 16, 25, 20, 17,
    34, 35, 13, 8, 18,
    20, 6, 10, 22, 9,
    7, 19, 18, 22, 6,
    31, 32, 29, 18, 20,
    32, 20, 36, 24, 26,
    17, 35, 34, 34, 10,
    16, 55, 44, 51, 35,
    29, 32, 54, 40, 18,
};

static inline int TestButton(uint button) {
    return controllerButtons & (button & ~prevControllerButtons);
}

void main(void) {
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
        FntPrint("rescue session saved as:\n\\psx\\cube\\pad\\rescue.pad\n\n");
        FntPrint("to replay:\n");
        FntPrint("replay!\n\n");
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
    curFileLength = ReadDataFile(curWorld, 1, FILE_BUF);
    SndInitFromSfxFile(FILE_BUF, curFileLength);
    LoadWarningTim();
    SndPlaySfx(SFX_LOADING, 888, &ZERO_SVECTOR_a2f04, 8000);
    /* ggi */
    curFileLength = ReadDataFile(curWorld, 3, FILE_BUF);
    i = ParseGGI(FILE_BUF);
    printf("%x\n", i);
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
            if (curWorld != DEMO_LEVELS[demoIndex] / 15) {
                loadNewWorld = 1;
            }
            curWorld = DEMO_LEVELS[demoIndex] / 15;
            demoIndex = (demoIndex + 1) % 3;
        }
        if (loadNewWorld && !skipNextLoad) {
            LoadingScreen();
            if (!loadedInitialSave) {
                LoadSaveFromMemoryCard();
                loadedInitialSave = 1;
            } else {
                /* Loading screen music */
                SndPlaySfx(SFX_LOADING, 888, &ZERO_SVECTOR_a2f04, 8000);
            }
            /* .tgi */
            curFileLength = ReadDataFile(curWorld, 0, TGI_FILE_BUF);
            if (displayModeHasBeenSet == 0) {
                displayWidth = 320;
                displayHeight = 256;
                SetupDisplay(1, 0, 0, 0, 0, 0);
                displayModeHasBeenSet = 1;
                VSync(0);
                VSync(0);
            }
            ParseTGI(TGI_FILE_BUF);
            loadingIsComplete = 888;
            curWorld2 = curWorld;
            /* .pak */
            curFileLength = ReadDataFile(curWorld, 2, LEVEL_PAK_BUF);
            PlayMusic(curWorld);
            loadNewWorld = 0;
            VSyncCallback(NULL);
            SndMuteVoiceByTag(888);
            loadingIsComplete = -1;
        }
        skipNextLoad = 0;
        if (curLevel >= *(int*)LEVEL_PAK_BUF) {
            curLevel = *(int*)LEVEL_PAK_BUF - 1;
        }
        if (curLevel < 0) {
            curLevel = 0;
        }
        if (gotSioData != 0) {
            if (gotSioData == 1) {
                levelLoadZlibStream.avail_in = byteCountToReceiveFromSio;
                levelLoadZlibStream.next_in = SIO_DATA_BUF;
                levelLoadZlibStream.avail_out = 0x60000;
                levelLoadZlibStream.next_out = LEVEL_DATA_BUF;
                inflateRetCode1 = inflateInit_(&levelLoadZlibStream, "1.0.4", 0x38);
                inflateRetCode2 = inflate(&levelLoadZlibStream, 4);
                inflateRetCode3 = inflateEnd(&levelLoadZlibStream);
                if (inflateRetCode1 < 0 || inflateRetCode2 < 0 || inflateRetCode3 < 0) {
                    gotSioData = 0;
                }
            } else {
                memcpy(LEVEL_DATA_BUF, 0x650000, 0x66b000 - 0x650000);
            }
            if (gotSioData == 0)
                goto loadFromPak;
        } else {
        loadFromPak:
            levelLoadZlibStream.avail_in = *(uint*)((char*)LEVEL_PAK_BUF + 8 + curLevel * 8);
            levelLoadZlibStream.next_in = (char*)LEVEL_PAK_BUF + *(int*)((char*)LEVEL_PAK_BUF + 4 + curLevel * 8);
            levelLoadZlibStream.avail_out = 0x60000;
            levelLoadZlibStream.next_out = LEVEL_DATA_BUF;
            inflateRetCode1 = inflateInit_(&levelLoadZlibStream, "1.0.4", 0x38);
            inflateRetCode2 = inflate(&levelLoadZlibStream, 4);
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
        if (gameState != 0 && gameState != 4 && (gameMode == 0 || (gameMode == 2 && numTimeTrialPlayers == 1))) {
            /* level begin sfx (but also done in other places?) */
            SndPlaySfx(SFX_LEVEL_LOAD, 0, &ZERO_SVECTOR_a2f04, 7000);
        }
        if (specialLevelType != 0) {
            PlayBonusMusic();
        }
        mainGameLoopRetVal = MainGameLoop();
        SndMuteAllVoices();
        drawCopyright = 0;
        DrawSync(0);
        VSync(0);
    } while (1);
}

// XXX: must be here to match
int vsyncCounter = 0;
int unusedRenderPhase = 666;

void SioRecvVsyncCallback(void) {
    vsyncCounter++;
}

int MainGameLoop(void) {
    int i;
    int j;
    int k;
    int temp;
    int controllerStatuses[2];
    int pad[16];
    RECT rect;
    int tex;
    int end;

    inGetReadyScreen = 0;
    end = 0;
    InitAllDigitSprites();
    if (curWorld2 * 15 + curLevel > highestLevelReached && curLevel < 15) {
        highestLevelReached = curWorld2 * 15 + curLevel;
    }
    if (gameMode == 1) {
        twoPlayerWhichPlayer = curController;
    }
    if (gameMode == 2) {
        if (numTimeTrialPlayers == 1) {
            levelPlayTime[twoPlayerWhichPlayer] = (-TIME_TRIAL_PAR_TIMES[curWorld2 * 15 + curLevel] - timeTrialDifficulty) * 50;
        } else {
            levelPlayTime[twoPlayerWhichPlayer] = -TIME_TRIAL_PAR_TIMES[curWorld2 * 15 + curLevel] * 50;
        }
    } else {
        levelPlayTime[twoPlayerWhichPlayer] = 0;
    }
    if (gameMode == 2 && numTimeTrialPlayers == 2) {
        inGetReadyScreen = 1;
    }
    savePointMenuConfirmed = 0;
    levelScoreSummaryConfirmed = 0;
    demoTimer = 400;
    menuIdleTimer = 0;
    for (i = 0; i < 2; i++) {
        controllerStatuses[i] = GetControllerStatus(i);
    }
    whichDrawDispEnv = 0;
    for (i = 0; i < 1; i++) {
        ClearOTagR(otag[whichDrawDispEnv][i], 1026);
    }
    ClearOTagR(&primLists[whichDrawDispEnv].main, 4);
    dispModeChangeState = 0;
    buttonSaveReplayMode = 0;
    if (isDemoMode == 1) {
        buttonSaveReplayMode = 2;
    }
    switch (buttonSaveReplayMode) {
        case 1:
            InitReplaySaving();
            break;
        case 2:
            InitReplayPlayback((&REPLAY_INPUT_PTRS)[(demoIndex + 2) % 3]);
            break;
    }
    VSyncCallback(MainGameVSyncCallback);
    gotVsync = 0;
    while (!end) {
        DrawSync(0);
        rect.w = 1;
        rect.h = 1;
        for (i = 0; i < 16; i++) {
            rect.x = framebufferFetches[i].x + (whichDrawDispEnv == 0 ? displayWidth : 0);
            rect.y = framebufferFetches[i].y;
            StoreImage(&rect, &tex);
            DrawSync(0);
            framebufferFetches[i].texel = tex & 0x7fff;
        }
        if (lethargyMode != 0 || halfFps == 1) {
            VSync(2);
        } else {
            WaitForVBlank();
        }
        whichDrawDispEnv = !whichDrawDispEnv;
        renderedPrimsBuf = (char*)RENDERED_PRIMS_BUF + whichDrawDispEnv * 0xc000;
        UnusedProcessDisplayModeChange();
        PutDrawAndDispEnvs();
        SetGeomScreen(projectionDistance);

        for (i = 0; i < numCameras; i++) {
            ClearOTagR(otag[whichDrawDispEnv][i], 1026);
        }
        ClearOTagR(&primLists[whichDrawDispEnv].main, 4);
        for (i = 0; i < numCameras; i++) {
            for (j = unkOtagLengths[whichDrawDispEnv][i]; j > 0;) {
                j--;
                ClearOTagR(&unkOtag[whichDrawDispEnv][i][j][0], 64);
                DrawSync(0);
            }
            unkOtagLengths[whichDrawDispEnv][i] = 0;
        }
        ResetTextRenderState();
        ResetTextVars();
        unkUnused = 0;
        unusedRenderPhase = 0;
        for (i = 0; i < numCameras; i++) {
            DrawOTag(&otag[!whichDrawDispEnv][i][tgi->skyboxFlag]);
        }
        unusedRenderPhase = 1;
        DrawOTag(&primLists[!whichDrawDispEnv].gui3);
        unusedRenderPhase = 2;
        SndProcessSpuVoices();
        prevControllerButtons = controllerButtons;
        if (gameMode == 2) {
            curController = twoPlayerWhichPlayer;
        }
        if (GetControllerStatus(curController) != 0) {
            temp = curController;
        } else {
            temp = (curController + 1) % 2;
        }
        controllerButtons = GetControllerButtons(temp);
        HandleCheats();
        UpdateVibration();
        ProcessScreenShake();
        if (controllerStatuses[0] != GetControllerStatus(0) || controllerStatuses[1] != GetControllerStatus(1)) {
            if (levelEndReason == 0) {
                if (!isPaused && !isDemoMode && !inGetReadyScreen) {
                    isPaused = 1;
                }
            }
            for (i = 0; i < 2; i++) {
                controllerStatuses[i] = GetControllerStatus(i);
            }
        }
        if (GetControllerStatus(0) == 0 && GetControllerStatus(1) == 0) {
            if (levelEndReason == 0 && !isPaused && !isDemoMode && !inGetReadyScreen) {
                isPaused = 1;
                SndMuteAllTaggedVoices();
            }
            if (isPaused == 1 && gameState != 0) {
                SetTextParams(displayWidth / 2 + 10, 180, 1, 0x80, 0x80, 0x80);
            } else {
                SetTextParams(displayWidth / 2 + 10, 127, 1, 0x80, 0x80, 0x80);
            }
            DrawTextCrappyFont("NO CONTROLLER!\n");
        }
        if (isDemoMode == 1) {
            SetTextParams(displayWidth / 2, 116, 1, 0x80, 0x80, 0x80);
            DrawTextCrappyFont("DEMO MODE\n");
        }
        if (gameMode != 2 || numTimeTrialPlayers != 2) {
            inGetReadyScreen = 0;
        }
        if (controllerButtons & PAD_SELECT && isPaused == 1 && gameState != 0) {
            drawHourglassWidget = 0;
            drawTimerPausedWidget = 0;
            drawKeyWidget = 0;
            drawScoreWidget = 0;
            drawFruitWidgets = 0;
            drawBonusWidget = 0;
            drawTimeAttackWidgets = 0;
            drawCopycatWidgets = 0;
            drawGeometryAndObjects = 0;
        }
        if (!isPaused && levelEndReason == 0 && !inGetReadyScreen) {
            ProcessPlayer();
        } else {
            SetPausedOrWaitingForRestart();
        }
        RenderBackground();
        RenderEverythingElseAndProcessSomeStuff();
        AddDrChangePrims();
        ProcessAndRenderParticles();
        DrawLensFlares(0);
        if (prevControllerButtons == controllerButtons) {
            menuIdleTimer++;
        } else {
            menuIdleTimer = 0;
        }
        if (gameState != 0) {
            DrawHud();
        }
        if (isDemoMode == 1 && gameState == 0 && demoTimer >= 0) {
            demoTimer--;
            if (demoTimer <= 0) {
                controllerButtons |= PAD_CROSS;
            }
        }
        MusicCheckForLoop();
        if (isPaused == 1 && drawGeometryAndObjects == 1) {
            PauseOrMainMenu();
        }
        if (sioCheatActivated == 1 && _sio_control(0, 0, 0) & 2) {
            gotSioData = 0;
            VSyncCallback(SioRecvVsyncCallback);
            if ((_sio_control(0, 4, 0) & 0xff) == 'K') {
                vsyncCounter = 0;
                do {
                    if (_sio_control(0, 0, 0) & 2)
                        break;
                } while (vsyncCounter < 51);
                if (vsyncCounter < 51) {
                    byteCountToReceiveFromSio = _sio_control(0, 4, 0) & 0xff;
                    vsyncCounter = 0;
                    while ((_sio_control(0, 0, 0) & 2) == 0)
                        ;
                    if (vsyncCounter > 50)
                        break;
                    byteCountToReceiveFromSio += (_sio_control(0, 4, 0) & 0xff) * 0x100;
                    ReceiveBufFromSio();
                    if (gameState != 0) {
                        isPaused = 0;
                        levelEndReason = LEVEL_END_EXIT;
                    }
                }
            }
            VSyncCallback(0);
            VSyncCallback(MainGameVSyncCallback);
            gotVsync = 0;
        }
        if (inGetReadyScreen == 1 && gameMode == 2 && gameState != 0) {
            inGetReadyScreen = GetReadyScreen();
        }
        if (levelTimeLeft < 1000 && levelTimeLeft % 50 > 25 && prevLevelTimeLeft % 50 < 25 &&
                levelTimeLeft > 0 && !isPaused && levelEndReason == 0) {
            if (levelTimeLeft % 100 < 50) {
                SndPlaySfx(SFX_HOURGLASS_TICK, 0, &ZERO_SVECTOR_a2f04, 8000 - 10 * (levelTimeLeft - 250));
            } else {
                SndPlaySfx(SFX_HOURGLASS_TICK, 37000, &ZERO_SVECTOR_a2f04, 8000 - 10 * (levelTimeLeft - 250));
            }
        }
        prevLevelTimeLeft = levelTimeLeft;
        if (levelEndReason != 0) {
            if (prevLevelEndReason != 0 && levelScoreSummaryConfirmed == 1) {
                if ((levelEndReason > 0 || specialLevelType >= 1) &&
                        !isDemoMode && !gotSioData &&
                        (gameMode == 0 || (gameMode == 2 && numTimeTrialPlayers == 1)) &&
                        curLevel == (curLevel / 5) * 5 &&
                        (gameMode != 2 || (curLevel == 0 && !timeTrialAtEndOfWorld)) &&
                        curLevel <= 14 &&
                        levelEndReason != LEVEL_END_HIDDEN_EXIT && !cheated && !debugBonusLevels &&
                        (!isFinal || curWorld2 == 4)) {
                    SavePointMenu();
                } else {
                    savePointMenuConfirmed = 1;
                    controllerButtons = PAD_CROSS;
                    prevControllerButtons = 0;
                }
            }
            drawHourglassWidget = 0;
            drawTimerPausedWidget = 0;
            drawKeyWidget = 0;
            drawScoreWidget = 0;
            drawFruitWidgets = 0;
            drawBonusWidget = 0;
            drawTimeAttackWidgets = 0;
            drawCopycatWidgets = 0;
            drawGeometryAndObjects = 1;
        } else {
            drawHourglassWidget = 0;
            drawTimerPausedWidget = 0;
            drawKeyWidget = 0;
            drawScoreWidget = 0;
            drawFruitWidgets = 0;
            drawBonusWidget = 0;
            drawTimeAttackWidgets = 0;
            drawCopycatWidgets = 0;
            drawGeometryAndObjects = 1;
            if (gameMode == 0) {
                drawHourglassWidget = 1;
                drawKeyWidget = 1;
                drawScoreWidget = 1;
                drawFruitWidgets = 1;
                if (isFinal == 1) {
                    drawFruitWidgets = 0;
                }
            }
            if (gameMode == 1) {
                drawHourglassWidget = 0;
                drawKeyWidget = 0;
                drawScoreWidget = 0;
                drawFruitWidgets = 0;
            }
            if (gameMode == 2) {
                drawHourglassWidget = 1;
                drawTimeAttackWidgets = 1;
                drawKeyWidget = 1;
            }
            if (specialLevelType == 1) {
                drawKeyWidget = 0;
                drawFruitWidgets = 1;
                drawBonusWidget = 1;
            }
            if (specialLevelType == 2) {
                drawFruitWidgets = 0;
            }
        }
        if (gameMode == 1 && gameState != 0) {
            if (!isPaused && levelEndReason == 0) {
                if (copycatNewOrCopyMoves == 1) {
                    RenderPlayerOrCopycatLabels(1, 0x80);
                } else {
                    RenderPlayerOrCopycatLabels(0, 0x80);
                }
            }
            if (twoPlayerWhichPlayer == 0) {
                RenderPlayerOrCopycatLabels(2, 0x80);
                RenderPlayerOrCopycatLabels(3, 0x50);
            } else {
                RenderPlayerOrCopycatLabels(2, 0x50);
                RenderPlayerOrCopycatLabels(3, 0x80);
            }
            drawCopycatWidgets = 1;
        }
        if (levelEndReason == 0) {
            if (gameState != 0 && !isPaused && !inGetReadyScreen) {
                levelPlayTime[twoPlayerWhichPlayer]++;
                if (lethargyMode != 0) {
                    levelPlayTime[twoPlayerWhichPlayer]++;
                }
            }
            if (levelEndReason == 0 && gameMode == 2 && gameState != 0 && numTimeTrialPlayers == 2) {
                if (twoPlayerWhichPlayer == 0) {
                    RenderPlayerOrCopycatLabels(2, 0x80);
                    RenderPlayerOrCopycatLabels(3, 0x50);
                } else {
                    RenderPlayerOrCopycatLabels(2, 0x50);
                    RenderPlayerOrCopycatLabels(3, 0x80);
                }
            }
        }
        FntFlush(-1);
        unusedFrameCounter++;
        if (!isDemoMode && menuIdleTimer >= 2000 && controllerButtons == 0 && gameState == 0) {
            wasPausedPreviousFrame = 0;
            gameMode = 0;
            screenOffsetY = displayHeight;
            totalPlayTime[0] = 0;
            numCameras = 1;
            isPaused = 0;
            cursorPosInMenu[curMenu] = 0;
            curMenu = 0;
            gameState = 1;
            InitAllDigitSprites();
            isDemoMode = 1;
            levelScore = 0;
            totalScore = 0;
            for (i = 0; i < 150; i++) {
                levelScores[i] = -1;
            }
            return -1;
        }
        if (isDemoMode == 1 && (GetControllerButtons(0) != 0 || GetControllerButtons(1) != 0)) {
            QuitToMainMenu();
        }
        if (levelEndReason != 0) {
            if ((TestButton(PAD_CROSS) || TestButton(PAD_TRIANGLE) || TestButton(PAD_SQUARE) ||
                    TestButton(PAD_CIRCLE)|| TestButton(PAD_START)) && savePointMenuConfirmed == 1) {
                end = 1;
            }
        }
        if (gameState > 1) {
            end = 1;
        }
        temp = levelEndReason; // XXX: ugly
        if (temp == 0) {
            if (loadNewWorld) {
                end = 1;
            }
        } else {
            if (prevLevelEndReason == 0) {
                if (temp < 0) {
                    if (specialLevelType > 0) {
                        levelScore = 0;
                    }
                    if (gameMode == 1) {
                        copycatPlayerScores[(curController + 1) % 2]++;
                    }
                }
                LevelCompletedOrDied();
                DecideNextLevel();
                SndMuteAllTaggedVoices();
                ResetLethargyEffect();
                ResetVibration();
                InitParticles();
                UpdateScoreAtEndOfLevel();
            }
            if (levelScoreSummaryConfirmed == 0 && levelEndReason >= LEVEL_END_TIME_TRIAL_QUIT) {
                DrawBigGuiSprite(0);
                DrawLevelScoreSummary();
            }
            goto skip;
        }
        if (end) {
            LevelCompletedOrDied();
            DecideNextLevel();
        }
    skip:
        prevLevelEndReason = levelEndReason;
    }
    VSyncCallback(0);
    if (levelHasBeenCompletedByPlayer[0] == 1 && levelHasBeenCompletedByPlayer[1] == 1) {
        for (i = 0; i < numTimeTrialPlayers; i++) {
            levelHasBeenCompletedByPlayer[i] = 0;
        }
    }
    if (gameMode == 0) {
        if (totalScore < 0 || curWorld >= 10) {
            if (isDemoMode != 1) {
                if (!isFinal) {
                    if (curWorld >= 10) {
                        ShowEndingFmv(0);
                        ResetCallback();
                    }
                    HighScoreUi(1);
                }
                if (curWorld >= 10) {
                    finalUnlocked = 1;
                    curWorld = 0;
                    totalScore = -1;
                }
                for (i = 0; i < 10; i++) {
                    for (k = 0; k < 15; k++) {
                        levelScores[i * 15 + k] = -1;
                    }
                }
            }
        }
    }
    if (curWorld >= 10 && loadNewWorld == 1) {
        i = totalPlayTime[0];
        if (gameMode == 2 && numTimeTrialPlayers == 1 && i + timeTrialDifficulty / 50 < 1) {
            ShowEndingFmv(0);
            ResetCallback();
        }
        finalUnlocked = 1;
        gameMode = 0;
        curWorld = 0;
        totalScore = -1;
    }
    if (totalScore < 0) {
        if (isDemoMode == 1) {
            isDemoMode = 0;
        }
        if (gotSioData == 0) {
            curLevel = 0;
            if (curWorld2 != 0 || gameMode == 1 || isFinal == 1) {
                curWorld = 0;
                loadNewWorld = 1;
            }
            isFinal = 0;
        }
        gameState = 4;
        totalScore = 0;
        fruitsCollectedBitmask = 0;
        savedFruitsCollectedBitmask = 0;
        isPaused = 1;
    }
    if (gameState == 4) {
        gameState = 0;
        gameMode = 0;
    }
    levelScore = 0;
    levelPlayTime[twoPlayerWhichPlayer] = 0;
    return levelEndReason;
}

void LevelCompletedOrDied(void) {
    int i;
    int j;
    int penalty;
    int t;

    if (gameMode == 0 && !gotSioData) {
        if (!loadNewWorld) {
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
    if (gameMode != 2 || gotSioData || gameState == 4) {
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
    int unfinished;
    int i;

    if (gotSioData) {
        return;
    }
    if (gameMode == 0 && specialLevelType == 0 && levelEndReason > 0) {
        if (levelEndReason == LEVEL_END_HIDDEN_EXIT) {
            curLevel++;
            if (curLevel == 15) {
                curLevel = *(int*)LEVEL_PAK_BUF;
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
                    levelAfterBonusLevel = *(int*)LEVEL_PAK_BUF;
                }
                curLevel = ((curLevel - 1) / 5) % 3 + 15;
            } else {
                if (curLevel == 15 && debugBonusLevels == 0) {
                    curLevel = *(int*)LEVEL_PAK_BUF;
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
        if (curWorld >= 10) {
            curLevel = 0;
            curWorld = 0;
        }
    }
    if (gameMode == 2 && specialLevelType == 0 && levelEndReason > 0) {
        unfinished = 0;
        for (i = 0; i < numTimeTrialPlayers; i++) {
            if (levelHasBeenCompletedByPlayer[i] != 1) {
                unfinished = 1;
            }
        }
        if (!unfinished) {
            curLevel++;
            if (curLevel == 15) {
                curLevel = *(int*)LEVEL_PAK_BUF;
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
        if (curLevel < *(int*)LEVEL_PAK_BUF && debugBonusLevels == 0) {
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
            if (curLevel >= *(int*)LEVEL_PAK_BUF)
                goto LAB_00042f60;
            if (debugBonusLevels == 2) {
                if (levelEndReason > 0) {
                    curLevel++;
                    if (curLevel > 17)
                        curLevel = *(int*)LEVEL_PAK_BUF;
                }
            }
        }
    }
    if (curLevel < *(int*)LEVEL_PAK_BUF) {
        return;
    }
LAB_00042f60:
    switch (debugBonusLevels) {
        case 0:
            curLevel = 0;
            break;
        case 1:
            curLevel = *(int*)LEVEL_PAK_BUF - 1;
            break;
        case 2:
            curLevel = 15;
            break;
    }
    if (!timeTrialAtEndOfWorld || gameMode != 2) {
        curWorld++;
    }
    loadNewWorld = 1;
}

void ReceiveBufFromSio(void) {
    int i;

    SetupDisplay(1, 128, 0, 0, 0, 0);
    FntFlush(-1);
    DrawSync(0);
    whichDrawDispEnv = 0;
    PutDrawAndDispEnvs();
    sioWritePtr = SIO_DATA_BUF;
    for (i = 0; i < byteCountToReceiveFromSio; i++) {
        vsyncCounter = 0;
        while ((_sio_control(0, 0, 0) & 2) == 0) {
            if (vsyncCounter > 50) {
                gotSioData = 0;
                SetupDisplay(0, 0, 0, 0, 0, 0);
                return;
            }
        }
        *sioWritePtr++ = _sio_control(0, 4, 0);
        if ((i & 0xff) == 0 || i == byteCountToReceiveFromSio - 1) {
            FntPrint("recived %d\n", i + 1);
            FntPrint("of %d bytes\n", byteCountToReceiveFromSio);
            FntFlush(-1);
            whichDrawDispEnv = !whichDrawDispEnv;
            PutDrawAndDispEnvs();
            if (GetControllerButtons(0) & PAD_SELECT) {
                gotSioData = 0;
                SetupDisplay(0, 0, 0, 0, 0, 0);
                return;
            }
        }
    }
    gotSioData = 1;
    SetupDisplay(0, 0, 0, 0, 0, 0);
}
