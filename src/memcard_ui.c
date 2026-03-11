#include "common.h"
#include <libmcrd.h>

// Prototypes
extern void DrawPsxButtonBackground(void);
extern void DrawSaveSlotSprites(int isSave);
extern void DrawScoreGraph(void);
extern void DrawStaticUiSprite(short id, short x, short y, short count);
extern void DrawTextFancyFont(char * str, short x, short y);
extern void DrawWidgets(int menuId, int cursorPos);
extern void FormatTime(int time, char * s, int showPlus);
extern int GetControllerButtons(int slot);
extern int GetControllerStatus(int slot);
extern void InitMemcardUi(void);
extern void InitPsxButtonBackgroundSprites(int);
extern void InitScoreGraph(void * pBuf, int * levelScores, int * unusedScorePtr, int numLevels, int maxScore, int x, int y, int graphWidth, int graphHeight, int isHighscore);
extern void InitSpinningSelectionSprites(void);
extern int LoadSaveSlot(int slot);
extern void MusicCheckForLoop(void);
extern void PutDrawAndDispEnvs(void);
extern void ResetTextRenderState(void);
extern void ResetTextVars(void);
extern void SetBigGuiSpriteVisible(void);
extern void SetTextParams(int posX, int posY, int align, int colorR, int colorG, int colorB);
extern void ShowMemCardFullScreenText(char * str);
extern void SndPlaySfx(int sfx, int tag, SVECTOR * dir, int volume);
extern void SndProcessSpuVoices(void);
extern void UpdateMemcardMenuSaveSelectionSprites(int index);
extern int sprintf(char * s, const char * format, ...);

// Variables
uint mcResult;
int memCardDataValid;
int saveSlot;

extern char S_LOADING_GAME_DO_NOT_REMOVE_MEMORY_CARD[];
extern char S_READING_DATA_DO_NOT_REMOVE_MEMORY_CARD[];
extern char S_FMTd_2[]; // "%d \n"

extern SVECTOR SVECTOR_000a2fac;
extern uint controllerButtons;
extern int curController;
extern int displayWidth;
extern MemcardData memCardData;
extern int musicShouldLoop;
extern int numCameras;
extern void* otag[2][1][1026];
extern uint prevControllerButtons;
extern PrimList primLists[2];
extern char stringbuf[64];
extern TgiFile * tgi;
extern int whichDrawDispEnv;

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


INCLUDE_ASM("asm/nonmatchings/memcard_ui", MemCardUi);

INCLUDE_ASM("asm/nonmatchings/memcard_ui", MemCardUiPart);
