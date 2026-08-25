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

extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);
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
int whichLevelEndSpriteLoaded = LEVEL_END_GFX_VERY_WELL_DONE;
int gameOverScreenState = 0;
int gameOverScreenFade = 0;
char S_1_0_4[] = "1.0.4"; // hack

void InitLevelEndScreen(void) {
    whichLevelEndSpriteLoaded = LEVEL_END_GFX_NONE;
    gameOverScreenState = 0;
    bigGuiSpriteFade = 0;
}

void LoadLevelEndReasonGfx() {
    int whichGfx;
    short x0;
    short y0;
    RECT rect;
    short w;
    short h;
    int len;
    int off;
    char* ptr;
    void* dest;
    int playTime;

    whichGfx = -levelEndReason * 2;
    if (levelEndReason >= 0) {
        whichGfx = LEVEL_END_GFX_WELL_DONE;
        if (gameMode == 2) {
            playTime = levelPlayTime[twoPlayerWhichPlayer] + timeTrialDifficulty * 50;
            if (playTime < 1) {
                whichGfx = LEVEL_END_GFX_VERY_WELL_DONE;
            }
            if (playTime > 1000) {
                whichGfx = LEVEL_END_GFX_LEVEL_CLEARED;
            }
        }
    } else {
        if (totalScore < 0 && specialLevelType == 0 && gameMode == 0) {
            if (gameOverScreenState == 0) {
                gameOverScreenState = 1;
            }
            if (gameOverScreenState == 3) {
                whichGfx = LEVEL_END_GFX_GAME_OVER;
            }
        }
    }
    if (gameMode == 1 && curLevel == 0 && (copycatPlayerScores[0] > 5 || copycatPlayerScores[1] > 5) && levelEndReason < 0) {
        whichGfx = LEVEL_END_GFX_PLAYER_2_WON;
        if (copycatPlayerScores[1] < copycatPlayerScores[0]) {
            whichGfx = LEVEL_END_GFX_PLAYER_1_WON;
        }
        if (copycatPlayerScores[0] == copycatPlayerScores[1]) {
            whichGfx = LEVEL_END_GFX_DRAW;
        }
    }
    if (gameMode == 0 && isFinal == 1 && levelEndReason > 0 && curWorld == 10) {
        whichGfx = LEVEL_END_GFX_FINAL_COMPLETE;
    }
    if (gameMode == 2) {
        if (levelEndReason > 0 && curLevel == 0 && totalScore >= 0 && DAT_000a3374 == 1 && numTimeTrialPlayers == 1) {
            whichGfx = LEVEL_END_GFX_QUALIFIED;
            if (totalPlayTime[0] > 0) {
                whichGfx = LEVEL_END_GFX_FAILED_TO_QUALIFY;
            }
        }
        if (gameMode == 2 && numTimeTrialPlayers == 2 && curLevel == 14 && levelEndReason > 0 && levelHasBeenCompletedByPlayer[0] == 1 && levelHasBeenCompletedByPlayer[1] == 1) {
            whichGfx = LEVEL_END_GFX_PLAYER_2_WON;
            if (totalPlayTime[0] < totalPlayTime[1]) {
                whichGfx = LEVEL_END_GFX_PLAYER_1_WON;
            }
            if (totalPlayTime[0] == totalPlayTime[1]) {
                whichGfx = LEVEL_END_GFX_DRAW;
            }
        }
    }
    if (levelEndReason == 0) {
        whichGfx = LEVEL_END_GFX_LOADING;
    }

    if (whichGfx == whichLevelEndSpriteLoaded || whichGfx > MENU_DEFLATED_SPRITES2_PTR[0] - 2 || (levelEndReason == -10 && whichGfx == LEVEL_END_GFX_WELL_DONE)) {
        return;
    }
    if (whichGfx == LEVEL_END_GFX_LOADING) {
        bigGuiSpriteFade = 0;
    }
    len = MENU_DEFLATED_SPRITES2_PTR[2 + whichGfx * 2];
    off = MENU_DEFLATED_SPRITES2_PTR[1 + whichGfx * 2];
    ptr = (char*)MENU_DEFLATED_SPRITES2_PTR + off;
    dest = TIM_DECOMP_BUF;
    zlibStream_a4dd4.avail_in = len;
    zlibStream_a4dd4.next_in = ptr;
    zlibStream_a4dd4.avail_out = 0x10000;
    zlibStream_a4dd4.next_out = dest;
    inflateRetCode = inflateInit_(&zlibStream_a4dd4, S_1_0_4, 0x38);
    inflateRetCode = inflate(&zlibStream_a4dd4, 4);
    inflateRetCode = inflateEnd(&zlibStream_a4dd4);

    w = *(short*)((char*)TIM_DECOMP_BUF + 0x3c);
    h = *(short*)((char*)TIM_DECOMP_BUF + 0x3e);
    x0 = (displayWidth - 4 * w) / 2;
    if (whichGfx < LEVEL_END_GFX_GAME_OVER) {
        y0 = 5;
        if (gameMode == 1 || (gameMode == 2 && numTimeTrialPlayers == 1)) {
            y0 = 20;
        }
    } else {
        y0 = 50;
    }
    if (whichGfx == LEVEL_END_GFX_FINAL_COMPLETE) {
        y0 = 50;
    }
    if (whichGfx == LEVEL_END_GFX_LOADING) {
        y0 = 60;
    }
    if (whichGfx == LEVEL_END_GFX_QUALIFIED || whichGfx == LEVEL_END_GFX_FAILED_TO_QUALIFY) {
        y0 = 10;
    }
    if (h + 94 < 0x100) {
        rect.x = 704;
        rect.y = 94;
        rect.h = 1;
        rect.w = 16;
        DrawSync(0);
        LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x14));
        DrawSync(0);
        rect.x = 704;
        rect.y = 95;
        rect.w = w;
        rect.h = h;
        LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x40));
        DrawSync(0);
        whichGfx++;
        if (whichGfx <= MENU_DEFLATED_SPRITES2_PTR[0] - 1) {
            len = MENU_DEFLATED_SPRITES2_PTR[2 + whichGfx * 2];
            off = MENU_DEFLATED_SPRITES2_PTR[1 + whichGfx * 2];
            ptr = (char*)MENU_DEFLATED_SPRITES2_PTR + off;
            dest = TIM_DECOMP_BUF;
            zlibStream_a4dd4.avail_in = len;
            zlibStream_a4dd4.next_in = ptr;
            zlibStream_a4dd4.avail_out = 0x10000;
            zlibStream_a4dd4.next_out = dest;

            inflateRetCode = inflateInit_(&zlibStream_a4dd4, S_1_0_4, 0x38);
            inflateRetCode = inflate(&zlibStream_a4dd4, 4);
            inflateRetCode = inflateEnd(&zlibStream_a4dd4);
            w = *(short*)((char*)TIM_DECOMP_BUF + 0x3c);
            h = *(short*)((char*)TIM_DECOMP_BUF + 0x3e);
            rect.x = 720;
            rect.y = 94;
            rect.w = 16;
            rect.h = 1;
            LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x14));
            DrawSync(0);
            if (w * 4 < 0x81) {
                rect.x = 736;
                rect.y = 94;
            } else {
                rect.x = 704;
                rect.y = 175;
            }
            rect.w = w;
            rect.h = h;
            LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x40));
            DrawSync(0);

            TSpritePrim(bigGuiSprite2, 0, 0, GetTPage(0, 2, 704, 94));
            setXY0(&bigGuiSprite2[0].sprt, x0, y0);
            SetSemiTrans(&bigGuiSprite2[0].sprt, 2);
            SetShadeTex(&bigGuiSprite2[0].sprt, 0);
            bigGuiSprite2[0].sprt.clut = GetClut(704, 94);
            setWH(&bigGuiSprite2[0].sprt, w * 4, h);
            setUV0(&bigGuiSprite2[0].sprt, 0, 0x5f);
            bigGuiSprite2[1] = bigGuiSprite2[0];

            TSpritePrim(bigGuiSprite1, 0, 0, GetTPage(0, 1, 736, 94));
            setXY0(&bigGuiSprite1[0].sprt, x0, y0);
            SetSemiTrans(&bigGuiSprite1[0].sprt, 1);
            SetShadeTex(&bigGuiSprite1[0].sprt, 0);
            bigGuiSprite1[0].sprt.clut = GetClut(720, 94);
            setWH(&bigGuiSprite1[0].sprt, w * 4, h);
            if (w * 4 < 129) {
                setUV0(&bigGuiSprite1[0].sprt, 128, 94);
            } else {
                setUV0(&bigGuiSprite1[0].sprt, 0, 175);
            }
            bigGuiSprite1[1] = bigGuiSprite1[0];
            whichLevelEndSpriteLoaded = whichGfx - 1;
        }
    }
}

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
    if (gameOverScreenState == 1 && gameOverScreenFade >= 0x80) {
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
