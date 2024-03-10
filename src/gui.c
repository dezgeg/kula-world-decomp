#include "common.h"

extern unsigned firstGuiTexture;
extern TSprite copyrightSprite[2];
extern Texture textures[];

extern TSprite copyrightSprite_[2]; // hack to make gcc not create induction variable of sprt
__asm__(".set copyrightSprite_, copyrightSprite");

void InitCopyrightSprite(void)
{
    int i;
    SPRT* sprt;
    for (i = 0; i < 2; i++) {
        TSpritePrim(&copyrightSprite[i], 0, 0, textures[firstGuiTexture + 17].tpage);
        sprt = &copyrightSprite_[i].sprt;
        setRGB0(sprt, 128,128,128);
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
