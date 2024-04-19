#include "common.h"

extern short COPYCAT_SPRITE_POSITIONS[];
extern TSprite copycatUiSprites[2][4];
extern uint firstGuiTextTexture;
extern Texture textures[150];

void InitCopycatUiTextures(void) {
    int i;
    int tex;
    SPRT* sprt;
    for (i = 0; i < 4; i++) {
        // XXX: ugly permuter mess
        short (*arr1)[2] = &COPYCAT_SPRITE_POSITIONS[0];
        short (*arr2)[2] = &COPYCAT_SPRITE_POSITIONS[0] + 1;

        tex = i + 40;
        TSpritePrim(&copycatUiSprites[0][i], 0, 0, textures[firstGuiTextTexture + tex].tpage);
        sprt = &copycatUiSprites[0][i].sprt;
        sprt->x0 = arr1[i][0];
        sprt->y0 = arr2[i][0];
        sprt->r0 = 0x80;
        sprt->g0 = 0x80;
        sprt->b0 = 0x80;
        SetSemiTrans(&copycatUiSprites[0][i].sprt, textures[firstGuiTextTexture + tex].semitrans);
        SetShadeTex(&copycatUiSprites[0][i].sprt, 0);
        copycatUiSprites[0][i].sprt.clut = textures[firstGuiTextTexture + tex].clut;
        copycatUiSprites[0][i].sprt.w = textures[firstGuiTextTexture + tex].w;
        copycatUiSprites[0][i].sprt.h = textures[firstGuiTextTexture + tex].h;
        sprt->u0 = textures[firstGuiTextTexture + tex].u;
        sprt->v0 = textures[firstGuiTextTexture + tex].v;
        copycatUiSprites[1][i] = copycatUiSprites[0][i];
    }
}
