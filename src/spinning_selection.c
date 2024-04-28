#include "common.h"

extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

extern uint firstGuiTexture;
extern PrimList primLists[2];
extern TSprite spinningSelectionSprite1[2];
extern TSprite spinningSelectionSprite2[2];
extern Texture textures[150];
extern int whichDrawDispEnv;

int spinningSelectionPhase;

void InitSpinningSelectionSprites(void) {
    TSpritePrim(&spinningSelectionSprite1[0], 0, 0, textures[firstGuiTexture + 13].tpage);
    setXY0(&spinningSelectionSprite1[0].sprt, 0, 0);
    setUV0(&spinningSelectionSprite1[0].sprt,
           textures[firstGuiTexture + 13].u,
           textures[firstGuiTexture + 13].v);
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

void UpdateMemcardMenuSaveSelectionSprites(int slot) {
    spinningSelectionPhase = spinningSelectionPhase + 60 & 0xfff;
    spinningSelectionSprite1[whichDrawDispEnv].sprt.x0 =
        55 + slot * 70 + ((rsin(spinningSelectionPhase) * 10) >> 12) - textures[firstGuiTexture + 13].w / 2;
    spinningSelectionPhase = spinningSelectionPhase + 60 & 0xfff;
    spinningSelectionSprite1[whichDrawDispEnv].sprt.y0 =
        64 + (rcos(spinningSelectionPhase) * 10 >> 12) - textures[firstGuiTexture + 13].w / 2;
    addPrim(&primLists[whichDrawDispEnv].main, &spinningSelectionSprite1[whichDrawDispEnv]);

    spinningSelectionSprite2[whichDrawDispEnv].sprt.x0 =
        55 + slot * 70 + (rsin(spinningSelectionPhase + 0x800 & 0xfff) * 10 >> 12) - textures[firstGuiTexture + 13].w / 2;
    spinningSelectionSprite2[whichDrawDispEnv].sprt.y0 =
        64 + (rcos(spinningSelectionPhase + 0x800 & 0xfff) * 10 >> 12) - textures[firstGuiTexture + 13].w / 2;
    addPrim(&primLists[whichDrawDispEnv].main, &spinningSelectionSprite2[whichDrawDispEnv]);
}

void Update2PlayerLevelSelectionSprites(int x, int y) {
    spinningSelectionPhase = spinningSelectionPhase + 60 & 0xfff;
    spinningSelectionSprite1[whichDrawDispEnv].sprt.x0 =
        x + ((rsin(spinningSelectionPhase) * 7) >> 12) - textures[firstGuiTexture + 13].w / 2;
    spinningSelectionPhase = spinningSelectionPhase + 60 & 0xfff;
    spinningSelectionSprite1[whichDrawDispEnv].sprt.y0 =
        y + (rcos(spinningSelectionPhase) * 7 >> 12) - textures[firstGuiTexture + 13].w / 2;
    addPrim(&primLists[whichDrawDispEnv].main, &spinningSelectionSprite1[whichDrawDispEnv]);

    spinningSelectionSprite2[whichDrawDispEnv].sprt.x0 =
        x + (rsin(spinningSelectionPhase + 0x800 & 0xfff) * 7 >> 12) - textures[firstGuiTexture + 13].w / 2;
    spinningSelectionSprite2[whichDrawDispEnv].sprt.y0 =
        y + (rcos(spinningSelectionPhase + 0x800 & 0xfff) * 7 >> 12) - textures[firstGuiTexture + 13].w / 2;
    addPrim(&primLists[whichDrawDispEnv].main, &spinningSelectionSprite2[whichDrawDispEnv]);
}
