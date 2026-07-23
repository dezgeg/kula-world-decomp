#include "common.h"
#include "zlib.h"

typedef struct DeflatedEntry {
    int offset;
    int len;
} DeflatedEntry;

typedef struct DeflatedSprites {
    int count;
    DeflatedEntry entries[1];
} DeflatedSprites;

typedef struct X {
    char pad1[20];
    char clut[40];
    short w;
    short h;
    char data[1];
} X;

extern void TSpritePrim(TSprite * ts, int dfe, int dtd, int tpage);
extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);

extern byte menuGfxBuf[32768];
extern DeflatedSprites* D_00074C4C;
extern DeflatedSprites* deflatedSprites;
extern int copycatPlayerScores[2];
extern int curLevel;
extern int curWorld;
extern int DAT_000a3374;
extern int displayWidth;
extern int gameMode;
extern int isFinal;
extern int levelEndReason;
extern int levelHasBeenCompletedByPlayer[2];
extern int levelPlayTime[2];
extern int* MENU_DEFLATED_SPRITES2_PTR;
extern int numTimeTrialPlayers;
extern int specialLevelType;
extern int timeTrialDifficulty;
extern int totalPlayTime[2];
extern int totalScore;
extern int twoPlayerWhichPlayer;
extern int whichDrawDispEnv;
extern PrimList primLists[2];
extern TSprite bigGuiSprite1[2];
extern TSprite bigGuiSprite2[2];
extern uint prevControllerButtons;

int bigGuiSpriteFade;
z_stream zlibStream_a4dd4;
int inflateRetCode;

SVECTOR SVECTOR_000a2ab4 = { 0, 0, 0, 0 };
char S_Fatal_error_in_jens_2d_eng[] = "Fatal error in jens 2d-eng:\n\n";
int whichLevelEndSpriteLoaded = 34;
int gameOverScreenState = 0;
int gameOverScreenFade = 0;
char S_1_0_4[] = "1.0.4"; // hack

void InitLevelEndScreen(void) {
    whichLevelEndSpriteLoaded = 48;
    gameOverScreenState = 0;
    bigGuiSpriteFade = 0;
}

// https://decomp.me/scratch/DYXGq
INCLUDE_ASM("asm/nonmatchings/level_end_screen", LoadLevelEndReasonGfx);

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
    TSprite* pTVar1;
    TSprite* pTVar2;
    PrimList* pPVar3;

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
            SndPlaySfx(SFX_GAME_OVER, 0, &SVECTOR_000a2ab4, 8000);
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
