#include "common.h"

void TSpritePrim(TSprite* prim, int dfe, int dtd, int tpage) {
    SetDrawTPage(&prim->tpage, dfe, dtd, tpage);
    SetSprt(&prim->sprt);
    MargePrim(prim, &prim->sprt);
}

void TPolyF4Prim(TPolyF4* prim, int dfe, int dtd, int tpage) {
    SetDrawTPage(&prim->tpage, dfe, dtd, tpage);
    setPolyF4(&prim->poly);
    MargePrim(prim, &prim->poly);
}
