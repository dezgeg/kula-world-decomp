#include "common.h"

extern PrimList primLists[2];
extern Texture textures[150];
extern int gameMode;
extern int whichDrawDispEnv;
extern uint firstGuiTextTexture;
extern int controllerButtons;
extern int isPaused;
extern int gameState;

TSprite copycatUiSprites[2][4];

short COPYCAT_SPRITE_POSITIONS[] = {
    98, 20 - 4 * VID_NTSC,
    98, 20 - 4 * VID_NTSC,
    28, SCREEN_HEIGHT - 56,
    246, SCREEN_HEIGHT - 56,
    40, SCREEN_HEIGHT - 56,
    246, SCREEN_HEIGHT - 56
};

void InitCopycatUiTextures(void) {
    int i;
    int tex;
    for (i = 0; i < 4; i++) {
        tex = i + 40;
        TSpritePrim(&copycatUiSprites[0][i], 0, 0, textures[firstGuiTextTexture + tex].tpage);
        setXY0(&copycatUiSprites[0][i].sprt, COPYCAT_SPRITE_POSITIONS[i * 2], COPYCAT_SPRITE_POSITIONS[i * 2 + 1]);
        setRGB0(&copycatUiSprites[0][i].sprt, 0x80, 0x80, 0x80);
        SetSemiTrans(&copycatUiSprites[0][i].sprt, textures[firstGuiTextTexture + tex].semitrans);
        SetShadeTex(&copycatUiSprites[0][i].sprt, 0);
        copycatUiSprites[0][i].sprt.clut = textures[firstGuiTextTexture + tex].clut;
        copycatUiSprites[0][i].sprt.w = textures[firstGuiTextTexture + tex].w;
        copycatUiSprites[0][i].sprt.h = textures[firstGuiTextTexture + tex].h;
        setUV0(&copycatUiSprites[0][i].sprt, textures[firstGuiTextTexture + tex].u, textures[firstGuiTextTexture + tex].v);
        copycatUiSprites[1][i] = copycatUiSprites[0][i];
    }
}

void RenderPlayerOrCopycatLabels(int sprite, u_char color) {
#if !VER_EU
    if ((controllerButtons & PAD_SELECT) && isPaused == 1) {
        if (gameState != 0) {
            return;
        }
    }
#endif
    if (sprite > 1) {
        setXY0(&copycatUiSprites[whichDrawDispEnv][sprite].sprt,
               COPYCAT_SPRITE_POSITIONS[gameMode * 4 + (sprite - 2) * 2],
               COPYCAT_SPRITE_POSITIONS[gameMode * 4 + (sprite - 2) * 2 + 1]);
    }
    setRGB0(&copycatUiSprites[whichDrawDispEnv][sprite].sprt, color, color, color);
    addPrim(&primLists[whichDrawDispEnv].gui2, &copycatUiSprites[whichDrawDispEnv][sprite]);
}
