#include "common.h"

extern void LoadLevelEndReasonGfx(int unused);
extern void SndPlaySfx(int sfx, int tag, SVECTOR * dir, int volume);

extern PrimList primLists[2];
extern SVECTOR SVECTOR_000a2ab4;
extern TSprite bigGuiSprite1[2];
extern TSprite bigGuiSprite2[2];
extern int gameMode;
extern int levelEndReason;
extern int specialLevelType;
extern int totalScore;
extern int whichDrawDispEnv;
extern uint prevControllerButtons;

int bigGuiSpriteFade;
int gameOverScreenFade;
int gameOverScreenState;

void FadeOutBigGuiSprite(void) {
    bigGuiSpriteFade -= 20;
    if (bigGuiSpriteFade < 0) {
        bigGuiSpriteFade = 0;
    }
}

void SetBigGuiSpriteVisible(void) {
    bigGuiSpriteFade = 0x100;
}

void FullyFinishLevelEndScreenFades(void) {
    bigGuiSpriteFade = 0x100;
    gameOverScreenFade = 0x80;
    if (totalScore < 0 && specialLevelType == 0 && gameMode == 0) {
        gameOverScreenState = 3;
    }
}

void DrawBigGuiSprite(int param_1) {
    TSprite *pTVar1;
    TSprite *pTVar2;
    PrimList *pPVar3;

    LoadLevelEndReasonGfx(0);
    if (param_1 == 2) {
        /* 2 at beginning of load screen */
        bigGuiSpriteFade = 0;
        gameOverScreenFade = 0;
    }
    if (bigGuiSpriteFade < 0x100) {
        bigGuiSpriteFade += 3;
        if (gameMode != 1) {
            prevControllerButtons = -1;
        }
    }
    if (gameOverScreenState == 1 || gameOverScreenState == 2) {
        prevControllerButtons = 0xffffffff;
    }
    if (gameOverScreenState == 3 && gameOverScreenFade < 0x80) {
        prevControllerButtons = 0xffffffff;
    }

    if (gameOverScreenState < 2) {
        gameOverScreenFade = bigGuiSpriteFade - bigGuiSprite1[whichDrawDispEnv].sprt.y0 / 2;
    } else if (gameOverScreenState == 2) {
        gameOverScreenFade -= 3;
    } else {
        gameOverScreenFade += 3;
    }
    if (gameOverScreenFade < 0) {
        gameOverScreenFade = 0;
        if (gameOverScreenState == 2) {
            gameOverScreenState = 3;
            SndPlaySfx(110,0,&SVECTOR_000a2ab4,8000);
        }
    }
    if (gameOverScreenFade > 0x80) {
        gameOverScreenFade = 0x80;
    }
    if (gameOverScreenState == 1 && gameOverScreenFade > 0x7f) {
        gameOverScreenState = 2;
    }
    if (param_1 == 0 && (levelEndReason > -9 || gameOverScreenState == 3)) {
        setRGB0(&bigGuiSprite1[whichDrawDispEnv].sprt, gameOverScreenFade, gameOverScreenFade, gameOverScreenFade);
        setRGB0(&bigGuiSprite2[whichDrawDispEnv].sprt, gameOverScreenFade, gameOverScreenFade, gameOverScreenFade);
        addPrim(&primLists[whichDrawDispEnv].main, &bigGuiSprite1[whichDrawDispEnv]);
        addPrim(&primLists[whichDrawDispEnv].main, &bigGuiSprite2[whichDrawDispEnv]);
    }
    if (param_1 == 1) {
        if (bigGuiSpriteFade > 0x80) {
            bigGuiSpriteFade = 0x80;
        }
        setRGB0(&bigGuiSprite1[whichDrawDispEnv].sprt, bigGuiSpriteFade, bigGuiSpriteFade, bigGuiSpriteFade);
        setRGB0(&bigGuiSprite2[whichDrawDispEnv].sprt, bigGuiSpriteFade, bigGuiSpriteFade, bigGuiSpriteFade);
        DrawPrim(&bigGuiSprite1[whichDrawDispEnv]);
        DrawPrim(&bigGuiSprite2[whichDrawDispEnv]);
    }
}

INCLUDE_ASM("asm/nonmatchings/level_end_screen2", LoadMenuGfx);
