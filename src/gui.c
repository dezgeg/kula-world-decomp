#include "common.h"

unsigned firstGuiTexture;
int drawTimerPausedWidget;

extern Texture textures[];

extern TSprite titleSprite[2];
extern TSprite titleSprite_[2];  // hack to make gcc not create induction variable of sprt
__asm__(".set titleSprite_, titleSprite");

void InitTitleSprite(void) {
    int i;
    SPRT* sprt;
    for (i = 0; i < 2; i++) {
        TSpritePrim(&titleSprite[i], 0, 0, textures[firstGuiTexture + 16].tpage);
        sprt = &titleSprite_[i].sprt;
        setRGB0(sprt, 128, 128, 128);
        sprt->x0 = 38;
        sprt->y0 = 25;

        SetSemiTrans(&titleSprite_[i].sprt, textures[firstGuiTexture + 16].semitrans);
        SetShadeTex(&titleSprite_[i].sprt, 0);
        titleSprite[i].sprt.clut = textures[firstGuiTexture + 16].clut;
        titleSprite[i].sprt.u0 = textures[firstGuiTexture + 16].u;
        titleSprite[i].sprt.v0 = textures[firstGuiTexture + 16].v;
        titleSprite[i].sprt.w = textures[firstGuiTexture + 16].w;
        titleSprite[i].sprt.h = textures[firstGuiTexture + 16].h;
    }
}

extern TSprite copyrightSprite[2];
extern TSprite copyrightSprite_[2];  // hack to make gcc not create induction variable of sprt
__asm__(".set copyrightSprite_, copyrightSprite");

void InitCopyrightSprite(void) {
    int i;
    SPRT* sprt;
    for (i = 0; i < 2; i++) {
        TSpritePrim(&copyrightSprite[i], 0, 0, textures[firstGuiTexture + 17].tpage);
        sprt = &copyrightSprite_[i].sprt;
        setRGB0(sprt, 128, 128, 128);
        sprt->x0 = 46;
        sprt->y0 = 218;

        SetSemiTrans(&copyrightSprite_[i].sprt, textures[firstGuiTexture + 17].semitrans);
        SetShadeTex(&copyrightSprite_[i].sprt, 0);
        copyrightSprite[i].sprt.clut = textures[firstGuiTexture + 17].clut;
        copyrightSprite[i].sprt.u0 = textures[firstGuiTexture + 17].u;
        copyrightSprite[i].sprt.v0 = textures[firstGuiTexture + 17].v;
        copyrightSprite[i].sprt.w = textures[firstGuiTexture + 17].w;
        copyrightSprite[i].sprt.h = textures[firstGuiTexture + 17].h;
    }
}

extern TSprite timerPausedSprite[2];
extern TSprite timerPausedSprite_[2];  // hack to make gcc not create induction variable of sprt
__asm__(".set timerPausedSprite_, timerPausedSprite");

void InitTimerPausedSprite(void) {
    int i;
    SPRT* sprt;

    drawTimerPausedWidget = 0;
    for (i = 0; i < 2; i++) {
        TSpritePrim(&timerPausedSprite[i], 0, 0, textures[firstGuiTexture + 19].tpage);
        sprt = &timerPausedSprite_[i].sprt;
        setRGB0(sprt, 128, 128, 128);
        sprt->x0 = 140;
        sprt->y0 = 6;

        SetSemiTrans(&timerPausedSprite_[i].sprt, textures[firstGuiTexture + 19].semitrans);
        SetShadeTex(&timerPausedSprite_[i].sprt, 0);
        timerPausedSprite[i].sprt.clut = textures[firstGuiTexture + 19].clut;
        timerPausedSprite[i].sprt.u0 = textures[firstGuiTexture + 19].u;
        timerPausedSprite[i].sprt.v0 = textures[firstGuiTexture + 19].v;
        timerPausedSprite[i].sprt.w = textures[firstGuiTexture + 19].w;
        timerPausedSprite[i].sprt.h = textures[firstGuiTexture + 19].h;
    }
}
