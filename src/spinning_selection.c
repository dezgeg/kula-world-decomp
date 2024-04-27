#include "common.h"

extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

extern uint firstGuiTexture;
extern TSprite spinningSelectionSprite1[2];
extern TSprite spinningSelectionSprite2[2];
extern Texture textures[150];

void InitSpinningSelectionSprites(void) {
    TSpritePrim(&spinningSelectionSprite1[0], 0, 0, textures[firstGuiTexture + 13].tpage);
    setXY0(&spinningSelectionSprite1[0].sprt, 0, 0);
    setUV0(&spinningSelectionSprite1[0].sprt, textures[firstGuiTexture + 13].u, textures[firstGuiTexture + 13].v);
    setRGB0(&spinningSelectionSprite1[0].sprt, 128, 128, 128);
    SetSemiTrans(&spinningSelectionSprite1[0].sprt, textures[firstGuiTexture + 13].semitrans);
    SetShadeTex(&spinningSelectionSprite1[0].sprt, 0);
    spinningSelectionSprite1[0].sprt.clut = textures[firstGuiTexture + 13].clut;
    spinningSelectionSprite1[0].sprt.w = textures[firstGuiTexture + 13].w;
    spinningSelectionSprite1[0].sprt.h = textures[firstGuiTexture + 13].h;

    spinningSelectionSprite1[1] = spinningSelectionSprite1[0];
    spinningSelectionSprite2[0] = spinningSelectionSprite1[1];
    spinningSelectionSprite2[1] = spinningSelectionSprite2[0];
}
