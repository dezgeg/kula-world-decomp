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
extern void TSpritePrim(TSprite * ts, int dfe, int dtd, int tpage);

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
char S_menugfx_too_high[] = "menugfx too high"; // hack
char S_menugfx_nr_too_big[] = "menugfx nr too big"; // hack

INCLUDE_ASM("asm/nonmatchings/menu_widgets", LoadMenuGfx);

void DrawWidgets(int menuId, int cursorPos) {
    if (menuId == 2 || menuId == 3) {
        musicVolumeWidgetSprite[whichDrawDispEnv].sprt.x0 = displayWidth / 2 + (musicVolume - 6) * 5 + 2;
        sfxVolumeWidgetSprite[whichDrawDispEnv].sprt.x0 = displayWidth / 2 + (sfxVolume - 6) * 5 + 2;
        if (gameState == 0) {
            musicVolumeWidgetSprite[whichDrawDispEnv].sprt.y0 = MENU_CURSOR_START_Y_MAIN_MENU[menuId] + 20;
            sfxVolumeWidgetSprite[whichDrawDispEnv].sprt.y0 = MENU_CURSOR_START_Y_MAIN_MENU[menuId];
        } else {
            musicVolumeWidgetSprite[whichDrawDispEnv].sprt.y0 =  MENU_CURSOR_START_Y_PAUSE_MENU[menuId] + 20;
            sfxVolumeWidgetSprite[whichDrawDispEnv].sprt.y0 =  MENU_CURSOR_START_Y_PAUSE_MENU[menuId];
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
