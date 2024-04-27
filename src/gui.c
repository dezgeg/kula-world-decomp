#include "common.h"

unsigned firstGuiTexture;
int drawTimerPausedWidget;

extern Texture textures[];

extern TSprite titleSprite[2];
extern TSprite copyrightSprite[2];
extern TSprite timerPausedSprite[2];

void InitTitleSprite(void) {
    int i;
    for (i = 0; i < 2; i++) {
        TSpritePrim(&titleSprite[i], 0, 0, textures[firstGuiTexture + 16].tpage);
        setRGB0(&titleSprite[i].sprt, 128, 128, 128);
        setXY0(&titleSprite[i].sprt, 38, 25);
        SetSemiTrans(&titleSprite[i].sprt, textures[firstGuiTexture + 16].semitrans);
        SetShadeTex(&titleSprite[i].sprt, 0);
        titleSprite[i].sprt.clut = textures[firstGuiTexture + 16].clut;
        titleSprite[i].sprt.u0 = textures[firstGuiTexture + 16].u;
        titleSprite[i].sprt.v0 = textures[firstGuiTexture + 16].v;
        titleSprite[i].sprt.w = textures[firstGuiTexture + 16].w;
        titleSprite[i].sprt.h = textures[firstGuiTexture + 16].h;
    }
}

void InitCopyrightSprite(void) {
    int i;
    for (i = 0; i < 2; i++) {
        TSpritePrim(&copyrightSprite[i], 0, 0, textures[firstGuiTexture + 17].tpage);
        setRGB0(&copyrightSprite[i].sprt, 128, 128, 128);
        setXY0(&copyrightSprite[i].sprt, 46, 218);
        SetSemiTrans(&copyrightSprite[i].sprt, textures[firstGuiTexture + 17].semitrans);
        SetShadeTex(&copyrightSprite[i].sprt, 0);
        copyrightSprite[i].sprt.clut = textures[firstGuiTexture + 17].clut;
        copyrightSprite[i].sprt.u0 = textures[firstGuiTexture + 17].u;
        copyrightSprite[i].sprt.v0 = textures[firstGuiTexture + 17].v;
        copyrightSprite[i].sprt.w = textures[firstGuiTexture + 17].w;
        copyrightSprite[i].sprt.h = textures[firstGuiTexture + 17].h;
    }
}

void InitTimerPausedSprite(void) {
    int i;

    drawTimerPausedWidget = 0;
    for (i = 0; i < 2; i++) {
        TSpritePrim(&timerPausedSprite[i], 0, 0, textures[firstGuiTexture + 19].tpage);
        setRGB0(&timerPausedSprite[i].sprt, 128, 128, 128);
        setXY0(&timerPausedSprite[i].sprt, 140, 6);
        SetSemiTrans(&timerPausedSprite[i].sprt, textures[firstGuiTexture + 19].semitrans);
        SetShadeTex(&timerPausedSprite[i].sprt, 0);
        timerPausedSprite[i].sprt.clut = textures[firstGuiTexture + 19].clut;
        timerPausedSprite[i].sprt.u0 = textures[firstGuiTexture + 19].u;
        timerPausedSprite[i].sprt.v0 = textures[firstGuiTexture + 19].v;
        timerPausedSprite[i].sprt.w = textures[firstGuiTexture + 19].w;
        timerPausedSprite[i].sprt.h = textures[firstGuiTexture + 19].h;
    }
}
