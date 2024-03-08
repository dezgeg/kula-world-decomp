#include "common.h"

void TexturedSpritePrim(TexturedSprite* prim, int dfe, int dtd, int tpage) {
    SetDrawTPage(&prim->tpage, dfe, dtd, tpage);
    SetSprt(&prim->sprt);
    MargePrim(prim, &prim->sprt);
}
