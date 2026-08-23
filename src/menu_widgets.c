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

extern void PutDrawAndDispEnvs(void);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering, u_char use24Bit);
extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

extern char S_1_0_4[];
extern char S_Fatal_error_in_jens_2d_eng[];
extern int displayWidth;
extern int gameState;
extern int* MENU_DEFLATED_SPRITES1_PTR;
extern int menuGfxHeight;
extern int menuGfxWidth;
extern int musicVolume;
extern int pauseMenuSprite;
extern int sfxVolume;
extern int whichDrawDispEnv;
extern PrimList primLists[2];
extern Texture textures[150];
extern TSprite bigGuiSprite1[2];
extern TSprite bigGuiSprite2[2];
extern TSprite buttonHelpSprite1[2];
extern TSprite buttonHelpSprite2[2];
extern TSprite menuCursorSprite[2];
extern TSprite musicVolumeWidgetSprite[2];
extern TSprite sfxVolumeWidgetSprite[2];
extern uint firstGuiTexture;

int bigGuiSpriteFade;
int menuCursorSinPhase;
int whichLevelEndSpriteLoaded;
int inflateRetCode;
z_stream zlibStream_a4dd4;

short MENU_CURSOR_MOVE_SPEED[][6] = {
    {28, 29, 21, 26, 0, 0},
    {31, 47, 27, 36, 0, 0},
    {7, 8, 46, 36, 31, 16},
    {7, 8, 46, 36, 31, 16},
    {23, 34, 36, 16, 0, 0},
    {26, 34, 16, 0, 0, 0},
    {36, 23, 34, 36, 16, 0},
    {16, 24, 16, 13, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {12, 8, 0, 0, 0, 0},
};
short MENU_CURSOR_X_POS[][6] = {
    {2, 2, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0},
    {-37, -37, 2, -14, -13, -1},
    {-37, -37, 2, -14, -13, -1},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {2, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
};
short MENU_CURSOR_START_Y_PAUSE_MENU[20] = {
    120, 100, 100, 100, 100, 100, 100, 100, 100, 100,
    100, 14, 100, 100, 80, 100, 90, 85, 120, 70,
};

short MENU_CURSOR_START_Y_MAIN_MENU[20] = {
    130, 130, 130, 130, 130, 130, 120, 143, 131, 120,
    143, 14, 100, 100, 80, 100, 90, 85, 100, 70,
};

void LoadMenuGfx(int menuId) {
    RECT rect;
    short height;
    short width;
    uint unk;
    uint x;
    ushort y;
    int spriteId = menuId * 2 + 50;
    int len;
    int offset;
    int offset2;
    char* buf;
    int dest;

    if (spriteId == whichLevelEndSpriteLoaded || menuId * 2 > MENU_DEFLATED_SPRITES1_PTR[0]) {
        return;
    }

    len = MENU_DEFLATED_SPRITES1_PTR[2 + (spriteId - 50) * 2];
    offset = MENU_DEFLATED_SPRITES1_PTR[1 + (spriteId - 50) * 2];
    buf = (char*)MENU_DEFLATED_SPRITES1_PTR + offset;
    dest = TIM_DECOMP_BUF;
    zlibStream_a4dd4.avail_in = len;
    zlibStream_a4dd4.next_in = buf;
    zlibStream_a4dd4.avail_out = 0x10000;
    zlibStream_a4dd4.next_out = dest;
    inflateRetCode = inflateInit_(&zlibStream_a4dd4, S_1_0_4, 0x38);
    inflateRetCode = inflate(&zlibStream_a4dd4, 4);
    inflateRetCode = inflateEnd(&zlibStream_a4dd4);
    height = *(short*)((char*)TIM_DECOMP_BUF + 0x3E);
    width = *(short*)((char*)TIM_DECOMP_BUF + 0x3C);
    unk = displayWidth - width * 4;
    x = (unk + (unk >> 31)) >> 1;
    if (gameState == 0) {
        y = MENU_CURSOR_START_Y_MAIN_MENU[menuId];
    } else {
        y = MENU_CURSOR_START_Y_PAUSE_MENU[menuId];
    }
    if (height + 94 > 0xff) {
        SetupDisplay(1, 128, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_Fatal_error_in_jens_2d_eng);
        FntPrint("menugfx too high");
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        while (1)
            ;
    }
    rect.x = 704;
    rect.y = 94;
    rect.h = 1;
    rect.w = 16;
    DrawSync(0);
    LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x14));
    DrawSync(0);
    rect.x = 704;
    rect.y = 95;
    rect.w = width;
    rect.h = height;
    LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x40));
    DrawSync(0);
    spriteId++;
    if (spriteId - 50 > MENU_DEFLATED_SPRITES1_PTR[0]) {
        SetupDisplay(1, 0x80, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint(S_Fatal_error_in_jens_2d_eng);
        FntPrint("menugfx nr too big");
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        while (1)
            ;
    }
    len = MENU_DEFLATED_SPRITES1_PTR[2 + (spriteId - 50) * 2];
    offset2 = MENU_DEFLATED_SPRITES1_PTR[1 + (spriteId - 50) * 2];
    buf = (char*)MENU_DEFLATED_SPRITES1_PTR + offset2;
    dest = TIM_DECOMP_BUF;
    zlibStream_a4dd4.avail_in = len;
    zlibStream_a4dd4.next_in = buf;
    zlibStream_a4dd4.avail_out = 0x10000;
    zlibStream_a4dd4.next_out = dest;
    inflateRetCode = inflateInit_(&zlibStream_a4dd4, S_1_0_4, 0x38);
    inflateRetCode = inflate(&zlibStream_a4dd4, 4);
    inflateRetCode = inflateEnd(&zlibStream_a4dd4);
    width = *(short*)((char*)TIM_DECOMP_BUF + 0x3C);
    height = *(short*)((char*)TIM_DECOMP_BUF + 0x3E);
    rect.x = 720;
    rect.y = 94;
    rect.w = 16;
    rect.h = 1;
    LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x14));
    DrawSync(0);
    if (width * 4 < 129) {
        rect.x = 736;
        rect.y = 94;
    } else {
        rect.x = 704;
        rect.y = 175;
    }
    rect.w = width;
    rect.h = height;
    LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x40));
    DrawSync(0);

    TSpritePrim(&bigGuiSprite2[0], 0, 0, GetTPage(0, 2, 704, 94));
    setRGB0(&bigGuiSprite2[0].sprt, 0x80, 0x80, 0x80);
    setXY0(&bigGuiSprite2[0].sprt, x, y);
    SetSemiTrans(&bigGuiSprite2[0].sprt, 2);
    SetShadeTex(&bigGuiSprite2[0].sprt, 0);
    bigGuiSprite2[0].sprt.clut = GetClut(704, 94);
    setWH(&bigGuiSprite2[0].sprt, width * 4, height);
    setUV0(&bigGuiSprite2[0].sprt, 0, 95);

    TSpritePrim(&bigGuiSprite1[0], 0, 0, GetTPage(0, 1, 736, 94));
    setRGB0(&bigGuiSprite1[0].sprt, 0x80, 0x80, 0x80);
    setXY0(&bigGuiSprite1[0].sprt, x, y);
    SetSemiTrans(&bigGuiSprite1[0].sprt, 1);
    SetShadeTex(&bigGuiSprite1[0].sprt, 0);
    bigGuiSprite1[0].sprt.clut = GetClut(720, 94);
    setWH(&bigGuiSprite1[0].sprt, width * 4, height);
    if (width * 4 < 129) {
        setUV0(&bigGuiSprite1[0].sprt, 128, 94);
    } else {
        setUV0(&bigGuiSprite1[0].sprt, 0, 175);
    }
    if (spriteId == 67) {
        buttonHelpSprite2[0] = bigGuiSprite2[0];
        buttonHelpSprite1[0] = bigGuiSprite1[0];

        bigGuiSprite2[0].sprt.h = 43;

        setUV0(&buttonHelpSprite2[0].sprt, 0, 138);
        buttonHelpSprite2[0].sprt.h = height - 43;
        buttonHelpSprite2[0].sprt.y0 = y + 74;
        buttonHelpSprite2[0].sprt.x0 = x;

        bigGuiSprite1[0].sprt.h = 43;
        if (width * 4 < 129) {
            setUV0(&buttonHelpSprite1[0].sprt, 128, 137);
        } else {
            setUV0(&buttonHelpSprite1[0].sprt, 0, 218);
        }
        buttonHelpSprite2[1] = buttonHelpSprite2[0];
        buttonHelpSprite1[0].sprt.h = height - 43;
        buttonHelpSprite1[0].sprt.x0 = x;
        buttonHelpSprite1[0].sprt.y0 = y + 74;
        buttonHelpSprite1[1] = buttonHelpSprite1[0];
    }
    bigGuiSprite2[1] = bigGuiSprite2[0];
    bigGuiSprite1[1] = bigGuiSprite1[0];
    whichLevelEndSpriteLoaded = spriteId - 1;
}

void DrawWidgets(int menuId, int cursorPos) {
    if (menuId == 2 || menuId == 3) {
        musicVolumeWidgetSprite[whichDrawDispEnv].sprt.x0 = displayWidth / 2 + (musicVolume - 6) * 5 + 2;
        sfxVolumeWidgetSprite[whichDrawDispEnv].sprt.x0 = displayWidth / 2 + (sfxVolume - 6) * 5 + 2;
        if (gameState == 0) {
            musicVolumeWidgetSprite[whichDrawDispEnv].sprt.y0 = MENU_CURSOR_START_Y_MAIN_MENU[menuId] + 20;
            sfxVolumeWidgetSprite[whichDrawDispEnv].sprt.y0 = MENU_CURSOR_START_Y_MAIN_MENU[menuId];
        } else {
            musicVolumeWidgetSprite[whichDrawDispEnv].sprt.y0 = MENU_CURSOR_START_Y_PAUSE_MENU[menuId] + 20;
            sfxVolumeWidgetSprite[whichDrawDispEnv].sprt.y0 = MENU_CURSOR_START_Y_PAUSE_MENU[menuId];
        }
        addPrim(&primLists[whichDrawDispEnv].main, &musicVolumeWidgetSprite[whichDrawDispEnv]);
        addPrim(&primLists[whichDrawDispEnv].main, &sfxVolumeWidgetSprite[whichDrawDispEnv]);
    }
    if (menuId < 8 || menuId == 14) {
        menuCursorSinPhase = (menuCursorSinPhase + 100 - MENU_CURSOR_MOVE_SPEED[menuId][cursorPos]) & 0xfff;
        menuCursorSprite[whichDrawDispEnv].sprt.x0 = displayWidth / 2 +
            MENU_CURSOR_X_POS[menuId][cursorPos] +
            (MENU_CURSOR_MOVE_SPEED[menuId][cursorPos] * rsin(menuCursorSinPhase) >> 12) -
            textures[firstGuiTexture + 13].w / 2;
        if (gameState == 0) {
            menuCursorSprite[whichDrawDispEnv].sprt.y0 = MENU_CURSOR_START_Y_MAIN_MENU[menuId] + cursorPos * 20;
        } else {
            menuCursorSprite[whichDrawDispEnv].sprt.y0 = MENU_CURSOR_START_Y_PAUSE_MENU[menuId] + cursorPos * 20;
        }
        if (menuId == 6) {
            menuCursorSprite[whichDrawDispEnv].sprt.y0 += 8;
        }
        if (menuId == 14) {
            menuCursorSprite[whichDrawDispEnv].sprt.y0 += 60;
        }
        setRGB0(&menuCursorSprite[whichDrawDispEnv].sprt, 0x80, 0x80, 0x80);
        addPrim(&primLists[whichDrawDispEnv].main, &menuCursorSprite[whichDrawDispEnv]);
    }
    LoadMenuGfx(menuId);
    if (whichLevelEndSpriteLoaded > 49) {
        addPrim(&primLists[whichDrawDispEnv].main, &bigGuiSprite1[whichDrawDispEnv]);
        if (menuId != 11) {
            addPrim(&primLists[whichDrawDispEnv].main, &bigGuiSprite2[whichDrawDispEnv]);
        }
        if (menuId == 8) {
            addPrim(&primLists[whichDrawDispEnv].main, &buttonHelpSprite1[whichDrawDispEnv]);
            addPrim(&primLists[whichDrawDispEnv].main, &buttonHelpSprite2[whichDrawDispEnv]);
        }
    }
}

void DrawMenuCursorSprite(int x, int y) {
    int color;
    int m = 20;

    menuCursorSinPhase = (menuCursorSinPhase + 80) & 0xfff;
    menuCursorSprite[whichDrawDispEnv].sprt.x0 = x + (m * rsin(menuCursorSinPhase) >> 12) - textures[firstGuiTexture + 13].w / 2;
    menuCursorSprite[whichDrawDispEnv].sprt.y0 = y;
    color = bigGuiSpriteFade - y / 2;
    if (color < 0) {
        color = 0;
    }
    if (color > 0x80) {
        color = 0x80;
    }
    setRGB0(&menuCursorSprite[whichDrawDispEnv].sprt, color, color, color);
    addPrim(&primLists[whichDrawDispEnv].main, &menuCursorSprite[whichDrawDispEnv]);
}
