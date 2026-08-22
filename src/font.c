#include "common.h"

extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

extern TSprite fontSprites[96];
extern Texture textures[150];

void SetFontEntry(char c, uchar u, uchar v, uchar width, uchar height, uchar x, uchar y, int texIndex) {
    SPRT* sprt;
    uchar idx = c - 0x20;
    if (idx < 0x60) {
        TSpritePrim(fontSprites + idx, 0, 0, textures[texIndex].tpage);
        sprt = &fontSprites[idx].sprt;
        sprt->x0 = x;
        sprt->y0 = y;
        sprt->r0 = 0x80;
        sprt->g0 = 0x80;
        sprt->b0 = 0x80;
        SetSemiTrans(&fontSprites[idx].sprt, textures[texIndex].semitrans);
        SetShadeTex(&fontSprites[idx].sprt, 0);
        fontSprites[idx].sprt.clut = textures[texIndex].clut;
        fontSprites[idx].sprt.w = width;
        fontSprites[idx].sprt.h = height;
        sprt->u0 = textures[texIndex].u + u;
        sprt->v0 = textures[texIndex].v + v;
    }
}
