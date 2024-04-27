#include "common.h"

extern PrimList primLists[2];
extern TSprite copycatUiSprites[2][4];
extern Texture textures[150];
extern int gameMode;
extern int whichDrawDispEnv;
extern short COPYCAT_SPRITE_POSITIONS[];
extern uint firstGuiTextTexture;

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

void RenderPlayerOrCopycatLabels(int sprite, u_char color) {
    if (sprite > 1) {
        // XXX: not too sure about this... maybe the symbol does start earlier
        setXY0(&copycatUiSprites[whichDrawDispEnv][sprite].sprt,
               COPYCAT_SPRITE_POSITIONS[sprite * 2 + gameMode * 4 - 4],
               COPYCAT_SPRITE_POSITIONS[sprite * 2 + gameMode * 4 + 1 - 4]);
    }
    setRGB0(&copycatUiSprites[whichDrawDispEnv][sprite].sprt, color, color, color);
    addPrim(&primLists[whichDrawDispEnv].gui2, &copycatUiSprites[whichDrawDispEnv][sprite]);
}
