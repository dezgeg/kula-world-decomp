#include "common.h"

extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

extern uint firstGuiTextTexture;
extern TSprite textDrawTemplateSprites1[40];
extern TSprite textDrawTemplateSprites2[40];
extern Texture textures[150];

int textCharIdx;

void InitVariousUiSpriteTemplates(void) {
    int i;

    textCharIdx = 0;
    for (i = 0; i < 40; i++) {
        TSpritePrim(&textDrawTemplateSprites1[i], 0, 0, textures[firstGuiTextTexture].tpage);
        SetSemiTrans(&textDrawTemplateSprites1[i].sprt, textures[firstGuiTextTexture].semitrans);
        SetShadeTex(&textDrawTemplateSprites1[i].sprt, 0);

        TSpritePrim(&textDrawTemplateSprites2[i], 0, 0, textures[firstGuiTextTexture + 1].tpage);
        SetSemiTrans(&textDrawTemplateSprites2[i].sprt, textures[firstGuiTextTexture + 1].semitrans);
        SetShadeTex(&textDrawTemplateSprites2[i].sprt, 0);
    }

    for (i = 0; i < 16; i++) {
        setUV0(&textDrawTemplateSprites1[i].sprt,
               textures[firstGuiTextTexture].u + 12 * i,
               textures[firstGuiTextTexture].v);
        textDrawTemplateSprites1[i].sprt.clut = textures[firstGuiTextTexture].clut;
        textDrawTemplateSprites1[i].sprt.w = 12;
        textDrawTemplateSprites1[i].sprt.h = 16;

        setUV0(&textDrawTemplateSprites2[i].sprt,
               textures[firstGuiTextTexture + 1].u + 12 * i,
               textures[firstGuiTextTexture + 1].v);
        textDrawTemplateSprites2[i].sprt.clut = textures[firstGuiTextTexture + 1].clut;
        textDrawTemplateSprites2[i].sprt.w = 12;
        textDrawTemplateSprites2[i].sprt.h = 16;
    }

    for (i = 16; i < 19; i++) {
        setUV0(&textDrawTemplateSprites1[i].sprt,
               textures[firstGuiTextTexture + 2].u + 4 * (i - 16),
               textures[firstGuiTextTexture + 2].v);
        textDrawTemplateSprites1[i].sprt.clut = textures[firstGuiTextTexture + 2].clut;
        textDrawTemplateSprites1[i].sprt.w = 16;
        textDrawTemplateSprites1[i].sprt.h = 8;

        setUV0(&textDrawTemplateSprites2[i].sprt,
               textures[firstGuiTextTexture + 3].u + 4 * (i - 16),
               textures[firstGuiTextTexture + 3].v);
        textDrawTemplateSprites2[i].sprt.clut = textures[firstGuiTextTexture + 3].clut;
        textDrawTemplateSprites2[i].sprt.w = 16;
        textDrawTemplateSprites2[i].sprt.h = 8;
    }

    for (i = 19; i < 40; i++) {
        TSpritePrim(&textDrawTemplateSprites1[i], 0, 0,
                    textures[firstGuiTextTexture + 4 + (i - 19) * 2].tpage);
        // bug, wrong texture idx used
        SetSemiTrans(&textDrawTemplateSprites1[i].sprt,
                     textures[firstGuiTextTexture].semitrans);
        SetShadeTex(&textDrawTemplateSprites1[i].sprt, 0);
        setUV0(&textDrawTemplateSprites1[i].sprt,
               textures[firstGuiTextTexture + 4 + (i - 19) * 2].u,
               textures[firstGuiTextTexture + 4 + (i - 19) * 2].v);
        textDrawTemplateSprites1[i].sprt.clut =
            textures[firstGuiTextTexture + 4 + (i - 19) * 2].clut;
        // bug, 5 instead of 4
        textDrawTemplateSprites1[i].sprt.w =
            textures[firstGuiTextTexture + 5 + (i - 19) * 2].w;
        // bug, 5 instead of 4
        textDrawTemplateSprites1[i].sprt.h =
            textures[firstGuiTextTexture + 5 + (i - 19) * 2].h;

        TSpritePrim(&textDrawTemplateSprites2[i], 0, 0,
                    textures[firstGuiTextTexture + 5 + (i - 19) * 2].tpage);
        // bug, wrong texture idx used
        SetSemiTrans(&textDrawTemplateSprites2[i].sprt,
                     textures[firstGuiTextTexture + 1].semitrans);
        SetShadeTex(&textDrawTemplateSprites2[i].sprt, 0);
        setUV0(&textDrawTemplateSprites2[i].sprt,
               textures[firstGuiTextTexture + 5 + (i - 19) * 2].u,
               textures[firstGuiTextTexture + 5 + (i - 19) * 2].v);
        textDrawTemplateSprites2[i].sprt.clut =
            textures[firstGuiTextTexture + 5 + (i - 19) * 2].clut;
        textDrawTemplateSprites2[i].sprt.w = textures[firstGuiTextTexture + 5 + (i - 19) * 2].w;
        textDrawTemplateSprites2[i].sprt.h = textures[firstGuiTextTexture + 5 + (i - 19) * 2].h;
    }
}
