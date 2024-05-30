#include "common.h"

extern void LoadMenuGfx(int menuId);

int bigGuiSpriteFade;
int menuCursorSinPhase;
int whichLevelEndSpriteLoaded;

extern short MENU_CURSOR_MOVE_SPEED[15][6];
extern short MENU_CURSOR_START_Y_MAIN_MENU[20];
extern short MENU_CURSOR_START_Y_PAUSE_MENU[20];
extern short MENU_CURSOR_X_POS[12][6];
extern PrimList primLists[2];
extern TSprite bigGuiSprite1[2];
extern TSprite bigGuiSprite2[2];
extern TSprite buttonHelpSprite1[2];
extern TSprite buttonHelpSprite2[2];
extern TSprite menuCursorSprite[2];
extern TSprite musicVolumeWidgetSprite[2];
extern TSprite sfxVolumeWidgetSprite[2];
extern Texture textures[150];
extern int displayWidth;
extern int gameState;
extern int musicVolume;
extern int sfxVolume;
extern int whichDrawDispEnv;
extern uint firstGuiTexture;

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
    if (color > 0x80 ) {
        color = 0x80;
    }
    setRGB0(&menuCursorSprite[whichDrawDispEnv].sprt, color, color, color);
    addPrim(&primLists[whichDrawDispEnv].main, &menuCursorSprite[whichDrawDispEnv]);
}
