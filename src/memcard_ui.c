#include "common.h"
#include <libmcrd.h>

struct MemcardHdr {
    char magic1;
    char magic2;
    byte iconFlags;
    byte blockNumber;
    ushort title[32];
    byte unused[28];
    byte clut[32];
    byte bitmap[128];
};

// Prototypes
extern int AskSaveOverwrite(void);
extern void DrawPsxButtonBackground(void);
extern void DrawSaveSlotSprites(int isSave);
extern void DrawScoreGraph(void);
extern void DrawStaticUiSprite(short id, short x, short y, short count);
extern void DrawTextCrappyFont(char * str);
extern void DrawTextFancyFont(char * str, short x, short y);
extern void DrawWidgets(int menuId, int cursorPos);
extern void FormatMemcard(void);
extern void FormatTime(int time, char * s, int showPlus);
extern int GetControllerButtons(int slot);
extern int GetControllerStatus(int slot);
extern void InitMemcardUi(void);
extern void InitPsxButtonBackgroundSprites(int);
extern void InitScoreGraph(void * pBuf, int * levelScores, int * unusedScorePtr, int numLevels, int maxScore, int x, int y, int graphWidth, int graphHeight, int isHighscore);
extern void InitSpinningSelectionSprites(void);
extern int LoadSaveSlot(int slot);
extern int LooksLikeAscii(char * param_1);
extern int MemCardUiPart(void);
extern void MusicCheckForLoop(void);
extern void PutDrawAndDispEnvs(void);
extern void ResetTextRenderState(void);
extern void ResetTextVars(void);
extern int SaveMemCard(int slot);
extern void SetBigGuiSpriteVisible(void);
extern void SetSemiTrans(void * p, int enable);
extern void SetShadeTex(void * p, int disable);
extern void SetTextParams(int posX, int posY, int align, int colorR, int colorG, int colorB);
extern void ShowMemCardFullScreenText(char * str);
extern char Sjis2Ascii(int sjis);
extern void SndPlaySfx(int sfx, int tag, SVECTOR * dir, int volume);
extern void SndProcessSpuVoices(void);
extern void TSpritePrim(TSprite * ts, int dfe, int dtd, int tpage);
extern void UpdateMemcardMenuSaveSelectionSprites(int index);
extern int sprintf(char * s, const char * format, ...);

// Variables
int INT_000a5690;
uint mcResult;
int memCardDataValid;
int saveSlot;
long tempMcResult;

extern LINE_F3 LINE_F3_ARRAY_000a49a0[2];
extern LINE_F3 LINE_F3_ARRAY_000a49d0[2];
extern SVECTOR SVECTOR_000a2fac;
extern uint controllerButtons;
extern int curController;
extern struct DIRENTRY direntry;
extern int displayHeight;
extern int displayWidth;
extern long mcCmd;
extern MemcardData memCardData;
extern struct MemcardHdr memcardBuf;
extern int musicShouldLoop;
extern int numCameras;
extern void* otag[2][1][1026];
extern uint prevControllerButtons;
extern PrimList primLists[2];
extern char stringbuf[64];
extern TgiFile * tgi;
extern int whichDrawDispEnv;

extern char S_CHECKING_MEMORY_CARD_PLEASE_WAIT[];
extern char S_FMTd_2[];
extern char S_LOADING_GAME_DO_NOT_REMOVE_MEMORY_CARD[];
extern char S_MEMORY_CARD_IS_FULL_YOU_MUST_REPLACE_ANOTHER_FILE[];
extern char S_NAME[];
extern char S_OR_h_TO_RE_READ_ANOTHER_MEMORY_CARD[];
extern char S_PLEASE_SELECT_FILE_TO_REPLACE[];
extern char S_PRESS_g_TO_CONTINUE_e_TO_CANCEL[];
extern char S_READING_DATA_DO_NOT_REMOVE_MEMORY_CARD[];
extern char S_REPLACING_SELECTED_FILE_DO_NOT_REMOVE_MEMORY_CARD[];
extern char S_SAVING_DO_NOT_REMOVE_MEMORY_CARD[];
extern char S_SIZE_FMTd_BLOCK[];
extern char S_SIZE_FMTd_BLOCKS[];
extern char S_XYZ[];

static inline int TestButton(int button) {
    return controllerButtons & (button & ~prevControllerButtons);
}

int LoadSaveMenu(void) {
    int i, j, k;
    int j2, k2;
    int j3, k3;
    int maxScore, maxScore2, maxScore3;
    int autorepeatCounter;
    int slot;

    autorepeatCounter = 0;
    prevControllerButtons = controllerButtons;

    for (i = 0; i < 1; i++) {
        ClearOTagR(&otag[0][i][0], 1026);
        ClearOTagR(&otag[1][i][0], 1026);
    }

    ClearOTagR(&primLists[0].main, 4);
    ClearOTagR(&primLists[1].main, 4);

    DrawSync(0);
    VSync(0);

    InitMemcardUi();
    InitPsxButtonBackgroundSprites(0);
    InitSpinningSelectionSprites();
    LoadSaveSlot(-1);

    for (i = 0;; i++) {
        if (memCardData.saveslots[(saveSlot + i) % 4].valid != 0) {
            break;
        }
        if (i >= 4)
            break;
    }
    saveSlot = (saveSlot + i) % 4;

    maxScore = 0;
    for (k = 0; k < 4; k++) {
        if (memCardData.saveslots[k].valid == 1) {
            for (j = 0; j < 150; j++) {
                if (maxScore < memCardData.saveslots[k].levelScores[j]) {
                    maxScore = memCardData.saveslots[k].levelScores[j];
                }
            }
        }
    }

    InitScoreGraph((void *)0x179000, memCardData.saveslots[saveSlot].levelScores, (int *)-1, 150, maxScore, 16, 135, 280, 90, 0);

    if (!TestButton(PAD_TRIANGLE)) {
        do {
            DrawSync(0);
            VSync(0);
            whichDrawDispEnv = !whichDrawDispEnv;
            PutDrawAndDispEnvs();

            for (i = 0; i < numCameras; i++) {
                ClearOTagR(&otag[whichDrawDispEnv][i][0], 1026);
            }
            ClearOTagR(&primLists[whichDrawDispEnv].main, 4);

            ResetTextRenderState();

             for (i = 0; i < numCameras; i++) {
                DrawOTag(&otag[!whichDrawDispEnv][i][tgi->skyboxFlag]);
            }
            DrawOTag(&primLists[!whichDrawDispEnv].gui3);

            SndProcessSpuVoices();
            prevControllerButtons = controllerButtons;
            if (GetControllerStatus(curController) != 0) {
                controllerButtons = GetControllerButtons(curController);
            } else {
                controllerButtons = GetControllerButtons((curController + 1) % 2);
            }

            if (musicShouldLoop == 1) {
                MusicCheckForLoop();
            }

            if (controllerButtons == prevControllerButtons) {
                autorepeatCounter++;
            } else {
                autorepeatCounter = 0;
            }
            if (autorepeatCounter > 20) {
                autorepeatCounter -= 5;
                prevControllerButtons = 0;
            }

            SetBigGuiSpriteVisible();
            ResetTextVars();
            DrawPsxButtonBackground();

            if (memCardDataValid == 1) {
                if (TestButton(PAD_R)) {
                    i = saveSlot;
                    do {
                        saveSlot = (saveSlot + 1) % 4;
                    } while (memCardData.saveslots[saveSlot].valid == 0);

                    if (saveSlot != i) {
                        SndPlaySfx(109, 0, &SVECTOR_000a2fac, 8000);
                        DrawSync(0);
                        maxScore2 = 0;
                        for (k2 = 0; k2 < 4; k2++) {
                            if (memCardData.saveslots[k2].valid == 1) {
                                for (j2 = 0; j2 < 150; j2++) {
                                    if (maxScore2 < memCardData.saveslots[k2].levelScores[j2]) {
                                        maxScore2 = memCardData.saveslots[k2].levelScores[j2];
                                    }
                                }
                            }
                        }
                        InitScoreGraph((void *)0x179000, memCardData.saveslots[saveSlot].levelScores, (int *)-1, 150, maxScore2, 16, 135, 280, 90, 0);
                    }
                }

                if (TestButton(PAD_L)) {
                    i = saveSlot;
                    do {
                        saveSlot--;
                        if (saveSlot < 0)
                            saveSlot = 3;
                    } while (memCardData.saveslots[saveSlot].valid == 0);

                    if (saveSlot != i) {
                        SndPlaySfx(109, 0, &SVECTOR_000a2fac, 8000);
                        DrawSync(0);
                        maxScore3 = 0;
                        for (k3 = 0; k3 < 4; k3++) {
                            if (memCardData.saveslots[k3].valid == 1) {
                                for (j3 = 0; j3 < 150; j3++) {
                                    if (maxScore3 < memCardData.saveslots[k3].levelScores[j3]) {
                                        maxScore3 = memCardData.saveslots[k3].levelScores[j3];
                                    }
                                }
                            }
                        }
                        InitScoreGraph((void *)0x179000, memCardData.saveslots[saveSlot].levelScores, (int *)-1, 150, maxScore3, 16, 135, 280, 90, 0);
                    }
                }

                if (TestButton(PAD_CROSS)) {
                    SndPlaySfx(109, 0, &SVECTOR_000a2fac, 8000);
                    ShowMemCardFullScreenText(S_LOADING_GAME_DO_NOT_REMOVE_MEMORY_CARD);
                    if (LoadSaveSlot(saveSlot) == 1) {
                        return 1;
                    }
                }

                UpdateMemcardMenuSaveSelectionSprites(saveSlot);
                if (memCardData.saveslots[saveSlot].isFinal == 0 && memCardData.saveslots[saveSlot].gameMode == 0) {
                    DrawScoreGraph();
                }
                InitMemcardUi();
                DrawSaveSlotSprites(0);

                if (memCardData.saveslots[saveSlot].isFinal == 0) {
                    sprintf(stringbuf, S_FMTd_2, memCardData.saveslots[saveSlot].curWorld * 15 + memCardData.saveslots[saveSlot].curLevel + 1);
                } else {
                    sprintf(stringbuf, S_FMTd_2, memCardData.saveslots[saveSlot].curWorld * 2 + memCardData.saveslots[saveSlot].curLevel + 151);
                }
                DrawTextFancyFont(stringbuf, 110, 89);
                DrawStaticUiSprite(3, 20, 89, 0);
                DrawStaticUiSprite(11, 20, 108, 0);

                if (memCardData.saveslots[saveSlot].gameMode == 0) {
                    sprintf(stringbuf, S_FMTd_2, memCardData.saveslots[saveSlot].score);
                    DrawTextFancyFont(stringbuf, 280, 89);
                    DrawStaticUiSprite(5, 130, 89, 0);
                    DrawStaticUiSprite(12, 100, 108, 0);
                } else {
                    FormatTime(memCardData.saveslots[saveSlot].score, stringbuf, 1);
                    DrawTextFancyFont(stringbuf, 280, 89);
                    DrawStaticUiSprite(1, 130, 89, 0);
                    DrawStaticUiSprite(10, 100, 108, 0);
                }
            } else {
                ResetTextRenderState();
                SetTextParams(displayWidth / 2, 100, 1, 128, 128, 128);
                switch (mcResult) {
                case McErrCardInvalid:
                    DrawWidgets(13, 0);
                    break;
                case McErrNotFormat:
                    DrawWidgets(15, 0);
                    break;
                case McErrFileNotExist:
                    DrawWidgets(15, 0);
                    break;
                case McErrCardNotExist:
                    DrawWidgets(12, 0);
                    break;
                }

                if (TestButton(PAD_CROSS)) {
                    SndPlaySfx(109, 0, &SVECTOR_000a2fac, 8000);
                    ShowMemCardFullScreenText(S_READING_DATA_DO_NOT_REMOVE_MEMORY_CARD);
                    LoadSaveSlot(-1);
                    for (i = 0; ; i++) {
                        if (memCardData.saveslots[(saveSlot + i) % 4].valid != 0) break;
                        if (i >= 4)
                            break;
                    }
                    saveSlot = (saveSlot + i) % 4;
                }
            }
        } while (!TestButton(PAD_TRIANGLE));
    }

    prevControllerButtons = -1;
    return 0;
}

int MemCardUi(void) {
    int i, j, k;
    int k2, j2;
    int k3, j3;
    int maxScore, maxScore2, maxScore3;
    int cursorPos;
    int idleTimer;
    int res;
    int status;

    idleTimer = 0;
    INT_000a5690 = 0;
    cursorPos = 1;
    LoadSaveSlot(-1);
    if (memCardDataValid == 0 && mcResult != McErrFileNotExist) {
        INT_000a5690 = 1;
    } else {
        INT_000a5690 = 0;
    }

    prevControllerButtons = controllerButtons;

    for (i = 0; i < 1; i++) {
        ClearOTagR(&otag[0][i][0], 1026);
        ClearOTagR(&otag[1][i][0], 1026);
    }

    ClearOTagR(&primLists[0].main, 4);
    ClearOTagR(&primLists[1].main, 4);
    DrawSync(0);
    VSync(0);

    InitMemcardUi();
    InitPsxButtonBackgroundSprites(0);
    InitSpinningSelectionSprites();

    maxScore = 0;
    for (k = 0; k < 4; k++) {
        if (memCardData.saveslots[k].valid == 1) {
            for (j = 0; j < 150; j++) {
                if (maxScore < memCardData.saveslots[k].levelScores[j]) {
                    maxScore = memCardData.saveslots[k].levelScores[j];
                }
            }
        }
    }

    InitScoreGraph((void *)0x179000, memCardData.saveslots[saveSlot].levelScores, (int *)-1, 150, maxScore, 16, 135, 280, 90, 0);
    if (TestButton(PAD_TRIANGLE)) {
        prevControllerButtons = -1;
        return 0;
    }
    do {
        DrawSync(0);
        VSync(0);
        whichDrawDispEnv = !whichDrawDispEnv;

        PutDrawAndDispEnvs();

        for (i = 0; i < numCameras; i++) {
            ClearOTagR(&otag[whichDrawDispEnv][i][0], 1026);
        }

        ClearOTagR(&primLists[whichDrawDispEnv].main, 4);
        ResetTextRenderState();

        for (i = 0; i < numCameras; i++) {
            DrawOTag(&otag[!whichDrawDispEnv][i][tgi->skyboxFlag]);
        }

        DrawOTag(&primLists[!whichDrawDispEnv].gui3);
        SndProcessSpuVoices();

        prevControllerButtons = controllerButtons;
        status = GetControllerStatus(curController);
        if (status) {
            controllerButtons = GetControllerButtons(curController);
        } else {
            controllerButtons = GetControllerButtons((curController + 1) % 2);
        }

        if (musicShouldLoop == 1)
            MusicCheckForLoop();

        if (controllerButtons == prevControllerButtons) {
            idleTimer++;
        } else {
            idleTimer = 0;
        }

        if (idleTimer > 20) {
            idleTimer -= 5;
            prevControllerButtons = 0;
        }

        SetBigGuiSpriteVisible();
        ResetTextVars();
        DrawPsxButtonBackground();

        if (INT_000a5690 == 0) {
            if (TestButton(PAD_R)) {
                SndPlaySfx(0x6d, 0, &SVECTOR_000a2fac, 8000);
                saveSlot = (saveSlot + 1) % 4;

                DrawSync(0);
                maxScore3 = 0;
                for (j3 = 0; j3 < 4; j3++) {
                    if (memCardData.saveslots[j3].valid == 1) {
                        for (k3 = 0; k3 < 150; k3++) {
                            if (maxScore3 < memCardData.saveslots[j3].levelScores[k3]) {
                                maxScore3 = memCardData.saveslots[j3].levelScores[k3];
                            }
                        }
                    }
                }
                InitScoreGraph((void *)0x179000, memCardData.saveslots[saveSlot].levelScores, (int *)-1, 150, maxScore3, 16, 135, 280, 90, 0);
            }

            if (TestButton(PAD_L)) {
                SndPlaySfx(0x6d, 0, &SVECTOR_000a2fac, 8000);
                saveSlot--;
                if (saveSlot < 0)
                    saveSlot = 3;

                DrawSync(0);
                maxScore2 = 0;
                for (k2 = 0; k2 < 4; k2++) {
                    if (memCardData.saveslots[k2].valid == 1) {
                        for (j2 = 0; j2 < 150; j2++) {
                            if (maxScore2 < memCardData.saveslots[k2].levelScores[j2]) {
                                maxScore2 = memCardData.saveslots[k2].levelScores[j2];
                            }
                        }
                    }
                }
                InitScoreGraph((void *)0x179000, memCardData.saveslots[saveSlot].levelScores, (int *)-1, 150, maxScore2, 16, 135, 280, 90, 0);
            }

            UpdateMemcardMenuSaveSelectionSprites(saveSlot);

            if (memCardData.saveslots[saveSlot].valid == 1 && memCardData.saveslots[saveSlot].isFinal == 0 && memCardData.saveslots[saveSlot].gameMode == 0) {
                DrawScoreGraph();
            }

            InitMemcardUi();
            DrawSaveSlotSprites(1);

            if (memCardData.saveslots[saveSlot].valid == 1) {
                if (memCardData.saveslots[saveSlot].isFinal == 0) {
                    sprintf(stringbuf, S_FMTd_2, memCardData.saveslots[saveSlot].curWorld * 15 + memCardData.saveslots[saveSlot].curLevel + 1);
                } else {
                    sprintf(stringbuf, S_FMTd_2, memCardData.saveslots[saveSlot].curWorld * 2 + memCardData.saveslots[saveSlot].curLevel + 151);
                }
                DrawTextFancyFont(stringbuf, 110, 89);
                DrawStaticUiSprite(3, 20, 89, 0);
                DrawStaticUiSprite(11, 20, 108, 0);

                if (memCardData.saveslots[saveSlot].gameMode == 0) {
                    sprintf(stringbuf, S_FMTd_2, memCardData.saveslots[saveSlot].score);
                    DrawTextFancyFont(stringbuf, 280, 89);
                    DrawStaticUiSprite(5, 130, 89, 0);
                    DrawStaticUiSprite(12, 100, 108, 0);
                } else {
                    FormatTime(memCardData.saveslots[saveSlot].score, stringbuf, 1);
                    DrawTextFancyFont(stringbuf, 280, 89);
                    DrawStaticUiSprite(1, 130, 89, 0);
                    DrawStaticUiSprite(10, 100, 108, 0);
                }
            }

            if (TestButton(PAD_CROSS)) {
                SndPlaySfx(0x6d, 0, &SVECTOR_000a2fac, 8000);
                if (memCardData.saveslots[saveSlot].valid == 0) {
                    ShowMemCardFullScreenText(S_SAVING_DO_NOT_REMOVE_MEMORY_CARD);
                    if (SaveMemCard(saveSlot) == 1) {
                        return 1;
                    }
                } else {
                    INT_000a5690 = 1;
                    mcResult = 666;
                }
            }
        } else {
            ResetTextRenderState();
            SetTextParams(displayWidth / 2, 100, 1, 128, 128, 128);

            switch (mcResult) {
            case McErrCardNotExist:
                DrawWidgets(12, 0);
                break;
            case McErrCardInvalid:
                DrawWidgets(13, 0);
                break;
            case McErrNewCard:
                INT_000a5690 = 0;
                ShowMemCardFullScreenText(S_READING_DATA_DO_NOT_REMOVE_MEMORY_CARD);
                LoadSaveSlot(-1);
                if (memCardDataValid == 0 && mcResult != McErrFileNotExist)
                    INT_000a5690 = 1;
                else
                    INT_000a5690 = 0;
                break;
            case McErrNotFormat:
                DrawWidgets(14, cursorPos);
                if (TestButton(PAD_U) && cursorPos == 1) {
                    SndPlaySfx(0x6d, 0, &SVECTOR_000a2fac, 8000);
                    cursorPos = 0;
                }
                if (TestButton(PAD_D) && cursorPos == 0) {
                    SndPlaySfx(0x6d, 0, &SVECTOR_000a2fac, 8000);
                    cursorPos = 1;
                }
                if (TestButton(PAD_CROSS)) {
                    if (cursorPos == 0) {
                        ShowMemCardFullScreenText(S_CHECKING_MEMORY_CARD_PLEASE_WAIT);
                        FormatMemcard();
                        LoadSaveSlot(-1);
                        if (memCardDataValid == 0 && mcResult != McErrFileNotExist)
                            INT_000a5690 = 1;
                        else
                            INT_000a5690 = 0;
                        cursorPos = 1;
                        prevControllerButtons = -1;
                    } else if (cursorPos == 1) {
                        prevControllerButtons = -1;
                        return 0;
                    }
                }
                break;
            case McErrBlockFull:
                DrawWidgets(16, 0);
                if (TestButton(PAD_CROSS)) {
                    SndPlaySfx(0x6d, 0, &SVECTOR_000a2fac, 8000);
                    switch (MemCardUiPart()) {
                        case 1:
                            ShowMemCardFullScreenText(S_REPLACING_SELECTED_FILE_DO_NOT_REMOVE_MEMORY_CARD);
                            if (SaveMemCard(saveSlot) == 1) {
                                return 1;
                            }
                            INT_000a5690 = 0;
                            ShowMemCardFullScreenText(S_READING_DATA_DO_NOT_REMOVE_MEMORY_CARD);
                            LoadSaveSlot(-1);
                            if (memCardDataValid == 0 && mcResult != McErrFileNotExist)
                                INT_000a5690 = 1;
                            else
                                INT_000a5690 = 0;
                            break;
                        case 0:
                            INT_000a5690 = 0;
                            ShowMemCardFullScreenText(S_READING_DATA_DO_NOT_REMOVE_MEMORY_CARD);
                            LoadSaveSlot(-1);
                            if (memCardDataValid == 0 && mcResult != McErrFileNotExist)
                                INT_000a5690 = 1;
                            else
                                INT_000a5690 = 0;
                            break;
                        case -1:
                            prevControllerButtons = -1;
                            return 0;
                    }
                }
                if (TestButton(PAD_CIRCLE)) {
                    INT_000a5690 = 0;
                    ShowMemCardFullScreenText(S_READING_DATA_DO_NOT_REMOVE_MEMORY_CARD);
                    LoadSaveSlot(-1);
                    if (memCardDataValid == 0 && mcResult != McErrFileNotExist)
                        INT_000a5690 = 1;
                    else
                        INT_000a5690 = 0;
                }
                break;
            case 666:
                DrawWidgets(17, 0);
                if (TestButton(PAD_CROSS)) {
                    SndPlaySfx(0x6d, 0, &SVECTOR_000a2fac, 8000);
                    ShowMemCardFullScreenText(S_SAVING_DO_NOT_REMOVE_MEMORY_CARD);
                    if (SaveMemCard(saveSlot) == 1) {
                        return 1;
                    }
                }
                if (TestButton(PAD_TRIANGLE)) {
                    mcResult = 0;
                    INT_000a5690 = 0;
                    prevControllerButtons = -1;
                }
                break;
            }

            if (TestButton(PAD_CROSS) && mcResult) {
                SndPlaySfx(0x6d, 0, &SVECTOR_000a2fac, 8000);
                INT_000a5690 = 0;
                ShowMemCardFullScreenText(S_READING_DATA_DO_NOT_REMOVE_MEMORY_CARD);
                LoadSaveSlot(-1);
                if (memCardDataValid == 0 && mcResult != McErrFileNotExist)
                    INT_000a5690 = 1;
                else
                    INT_000a5690 = 0;
            }
        }
    } while (!TestButton(PAD_TRIANGLE));

    prevControllerButtons = -1;
    return 0;
}

int MemCardUiPart(void) {
    short filecount;
    int cursorX;
    int cursorY;
    int keyRepeatTimer;
    RECT rect;
    unsigned int i;
    int idx;
    int k;
    TSprite fileSprites[16][2];
    char rawName[15][24];
    char fileNames[15][36];
    char fileSizes[16];
    int files;
    char* ptr;

    keyRepeatTimer = 0;
    cursorX = 0;
    cursorY = 0;
    files = 1;
    filecount = 0;

    ptr = &memCardData;
    for (i = 0; i < 0x2004; i++) {
        *ptr++ = 0;
    }

    MemCardAccept(0);
    MemCardSync(0, &mcCmd, &tempMcResult);
    if (tempMcResult != 0) {
        return 0;
    }

    for (idx = 0; idx < 15; idx++) {
        if (files < 1) break;
        MemCardGetDirentry(0, S_XYZ, &direntry, &files, idx, 1);

        if (MemCardReadFile(0, direntry.name, (u_long*)&memcardBuf, 0, 256) == 1 && files > 0) {
            MemCardSync(0, &mcCmd, &tempMcResult);
            fileSizes[filecount] = memcardBuf.blockNumber;

            if (fileSizes[filecount] == 0)
                continue;
            for (k = 0; k < 22; k++) {
                rawName[filecount][k] = ((char*)&direntry)[k];
                if (rawName[filecount][k] == 0) break;
            }

            DrawSync(0);
            rect.x = 704;
            rect.y = filecount + 150;
            rect.w = 16;
            rect.h = 1;
            LoadImage(&rect, memcardBuf.clut);

            rect.x = filecount * 4 + 704;
            rect.y = 118;
            rect.w = 4;
            rect.h = 16;
            LoadImage(&rect, memcardBuf.bitmap);
            DrawSync(0);

            TSpritePrim(&fileSprites[filecount][0], 0, 0, GetTPage(0, 1, 704, 118));

            setRGB0(&fileSprites[filecount][0].sprt, 0x60, 0x60, 0x60);
            setXY0(&fileSprites[filecount][0].sprt, (filecount % 3) * 24 + 124, (filecount / 3) * 24 + 50);

            SetSemiTrans(&fileSprites[filecount][0].sprt, 0);
            SetShadeTex(&fileSprites[filecount][0].sprt, 0);

            fileSprites[filecount][0].sprt.clut = GetClut(704, filecount + 150);
            fileSprites[filecount][0].sprt.w = 16;
            fileSprites[filecount][0].sprt.h = 16;

            setUV0(&fileSprites[filecount][0].sprt, ((filecount * 4 + 704) % 64) * 4, 118);

            fileSprites[filecount][1] = fileSprites[filecount][0];

            if (LooksLikeAscii(memcardBuf.title)) {
                for (k = 0; k < 32; k++) {
                    char bVar1 = ((char*)&memcardBuf)[4 + k];
                    fileNames[filecount][k] = bVar1;
                    if (bVar1 == 0) break;
                    if (bVar1 >= 'a' && bVar1 <= 'z') {
                        fileNames[filecount][k] = bVar1 + 0xe0;
                    }
                }
            } else {
                for (k = 0; k < 32; k++) {
                    char cVar2 = Sjis2Ascii(((unsigned short*)&memcardBuf)[2 + k]);
                    fileNames[filecount][k] = cVar2;
                    if (cVar2 == 0) break;
                    if (cVar2 >= 'a' && cVar2 <= 'z') {
                        fileNames[filecount][k] = cVar2 + 0xe0;
                    }
                }
            }

            fileNames[filecount][k] = '\n';
            fileNames[filecount][k + 1] = 0;
            filecount++;
        }
    }
    prevControllerButtons = controllerButtons;

    for (idx = 0; idx < 1; idx++) {
        ClearOTagR(&otag[0][idx][0], 1026);
        ClearOTagR(&otag[1][idx][0], 1026);
    }

    ClearOTagR(&primLists[0].main, 4);
    ClearOTagR(&primLists[1].main, 4);
    DrawSync(0);
    VSync(0);

    if (!TestButton(PAD_TRIANGLE)) {
        do {
            DrawSync(0);
            VSync(0);
            whichDrawDispEnv = !whichDrawDispEnv;

            PutDrawAndDispEnvs();

            for (idx = 0; idx < numCameras; idx++) {
                ClearOTagR(&otag[whichDrawDispEnv][idx][0], 1026);
            }
            ClearOTagR(&primLists[whichDrawDispEnv].main, 4);
            ResetTextRenderState();

            for (idx = 0; idx < numCameras; idx++) {
                DrawOTag(&otag[!whichDrawDispEnv][idx][tgi->skyboxFlag]);
            }
            DrawOTag(&primLists[!whichDrawDispEnv].gui3);
            SndProcessSpuVoices();

            prevControllerButtons = controllerButtons;
            if (GetControllerStatus(curController)) {
                controllerButtons = GetControllerButtons(curController);
            } else {
                controllerButtons = GetControllerButtons((curController + 1) % 2);
            }

            if (musicShouldLoop == 1) MusicCheckForLoop();

            if (controllerButtons == prevControllerButtons)
                keyRepeatTimer++;
            else
                keyRepeatTimer = 0;

            if (keyRepeatTimer > 20) {
                keyRepeatTimer -= 5;
                prevControllerButtons = 0;
            }

            SetBigGuiSpriteVisible();
            ResetTextVars();
            DrawPsxButtonBackground();

            if (TestButton(PAD_CROSS)) {
                SndPlaySfx(0x6d, 0, &SVECTOR_000a2fac, 8000);
                if (AskSaveOverwrite()) {
                    idx = MemCardDeleteFile(0, rawName[cursorY * 3 + cursorX]);
                    if (idx == 0) {
                        memCardDataValid = 0;
                        return 1;
                    }
                    mcResult = idx;
                    return 0;
                }
                DrawSync(0);
                VSync(0);
            }

            if (TestButton(PAD_U)) {
                setRGB0(&fileSprites[3 * cursorY + cursorX][0].sprt, 0x60, 0x60, 0x60);
                setRGB0(&fileSprites[3 * cursorY + cursorX][1].sprt, 0x60, 0x60, 0x60);
                SndPlaySfx(0x6d, 0, &SVECTOR_000a2fac, 8000);
                cursorY--;
                if (cursorY < 0) {
                    cursorY = 4;
                }
                while (cursorY * 3 + cursorX >= filecount) {
                    cursorY--;
                }
            }

            if (TestButton(PAD_D)) {
                setRGB0(&fileSprites[3 * cursorY + cursorX][0].sprt, 0x60, 0x60, 0x60);
                setRGB0(&fileSprites[3 * cursorY + cursorX][1].sprt, 0x60, 0x60, 0x60);
                SndPlaySfx(0x6d, 0, &SVECTOR_000a2fac, 8000);
                cursorY = (cursorY + 1) % 5;
                if (cursorY * 3 + cursorX >= filecount) {
                    cursorY = 0;
                }
            }

            if (TestButton(PAD_L)) {
                setRGB0(&fileSprites[3 * cursorY + cursorX][0].sprt, 0x60, 0x60, 0x60);
                setRGB0(&fileSprites[3 * cursorY + cursorX][1].sprt, 0x60, 0x60, 0x60);
                SndPlaySfx(0x6d, 0, &SVECTOR_000a2fac, 8000);
                cursorX--;
                if (cursorX < 0) {
                    cursorX = 2;
                    while (cursorY * 3 + cursorX >= filecount) {
                        cursorX--;
                    }
                }
            }

            if (TestButton(PAD_R)) {
                setRGB0(&fileSprites[3 * cursorY + cursorX][0].sprt, 0x60, 0x60, 0x60);
                setRGB0(&fileSprites[3 * cursorY + cursorX][1].sprt, 0x60, 0x60, 0x60);
                SndPlaySfx(0x6d, 0, &SVECTOR_000a2fac, 8000);
                cursorX = (cursorX + 1) % 3;
                if (cursorY * 3 + cursorX >= filecount) {
                    cursorX = 0;
                }
            }

            if (TestButton(PAD_TRIANGLE)) {
                DrawSync(0);
                VSync(0);
                prevControllerButtons = -1;
                return -1;
            }

            if (TestButton(PAD_CIRCLE)) break;

            SetTextParams(displayWidth / 2, displayHeight / 2 - 105, 1, 128, 128, 128);
            DrawTextCrappyFont(S_MEMORY_CARD_IS_FULL_YOU_MUST_REPLACE_ANOTHER_FILE);

            SetTextParams(20, displayHeight / 2 + 40, 0, 128, 128, 128);
            DrawTextCrappyFont(S_NAME);
            SetTextParams(60, displayHeight / 2 + 40, 0, 128, 128, 128);
            DrawTextCrappyFont(fileNames[cursorY * 3 + cursorX]);

            SetTextParams(20, displayHeight / 2 + 52, 0, 128, 128, 128);
            if (fileSizes[cursorY * 3 + cursorX] == 1) {
                sprintf(stringbuf, S_SIZE_FMTd_BLOCK, fileSizes[cursorY * 3 + cursorX]);
            } else {
                sprintf(stringbuf, S_SIZE_FMTd_BLOCKS, fileSizes[cursorY * 3 + cursorX]);
            }
            DrawTextCrappyFont(stringbuf);

            SetTextParams(displayWidth / 2, displayHeight / 2 + 70, 1, 128, 128, 128);
            DrawTextCrappyFont(S_PLEASE_SELECT_FILE_TO_REPLACE);
            DrawTextCrappyFont(S_PRESS_g_TO_CONTINUE_e_TO_CANCEL);
            DrawTextCrappyFont(S_OR_h_TO_RE_READ_ANOTHER_MEMORY_CARD);

            setRGB0(&fileSprites[3 * cursorY + cursorX][0].sprt, 0xc0, 0xc0, 0xc0);
            setRGB0(&fileSprites[3 * cursorY + cursorX][1].sprt, 0xc0, 0xc0, 0xc0);

            for (idx = 0; idx < filecount; idx++) {
                addPrim(&primLists[whichDrawDispEnv].main, &fileSprites[idx][whichDrawDispEnv]);
            }

            SetLineF3(&LINE_F3_ARRAY_000a49a0[whichDrawDispEnv]);
            SetLineF3(&LINE_F3_ARRAY_000a49d0[whichDrawDispEnv]);
            setRGB0(&LINE_F3_ARRAY_000a49a0[whichDrawDispEnv], 255, 0, 0);
            setXY3(&LINE_F3_ARRAY_000a49a0[whichDrawDispEnv], cursorX * 24 + 123, cursorY * 24 + 49,
                          cursorX * 24 + 140, cursorY * 24 + 49,
                          cursorX * 24 + 140, cursorY * 24 + 66);

            setRGB0(&LINE_F3_ARRAY_000a49d0[whichDrawDispEnv], 255, 0, 0);
            setXY3(&LINE_F3_ARRAY_000a49d0[whichDrawDispEnv], cursorX * 24 + 140, cursorY * 24 + 66,
                          cursorX * 24 + 123, cursorY * 24 + 66,
                          cursorX * 24 + 123, cursorY * 24 + 49);

            addPrim(&primLists[whichDrawDispEnv].main, &LINE_F3_ARRAY_000a49a0[whichDrawDispEnv]);
            addPrim(&primLists[whichDrawDispEnv].main, &LINE_F3_ARRAY_000a49d0[whichDrawDispEnv]);
        } while (!TestButton(PAD_TRIANGLE));
    }

    DrawSync(0);
    VSync(0);
    prevControllerButtons = -1;
    return 0;
}
