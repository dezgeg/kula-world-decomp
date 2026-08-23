#include "common.h"

extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

extern TSprite fontSprites[96];
extern Texture textures[150];

void SetFontEntry(char c, uchar u, uchar v, uchar width, uchar height, uchar x, uchar y, int texIndex) {
    uchar idx = c - 0x20;
    if (idx < 0x60) {
        TSpritePrim(fontSprites + idx, 0, 0, textures[texIndex].tpage);
        setXY0(&fontSprites[idx].sprt, x, y);
        setRGB0(&fontSprites[idx].sprt, 0x80, 0x80, 0x80);
        SetSemiTrans(&fontSprites[idx].sprt, textures[texIndex].semitrans);
        SetShadeTex(&fontSprites[idx].sprt, 0);
        fontSprites[idx].sprt.clut = textures[texIndex].clut;
        fontSprites[idx].sprt.w = width;
        fontSprites[idx].sprt.h = height;
        setUV0(&fontSprites[idx].sprt, textures[texIndex].u + u, textures[texIndex].v + v);
    }
}
