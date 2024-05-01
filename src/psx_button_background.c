#include "common.h"

typedef struct PsxButtonBackground {
    TSprite sprites[2][200];
    short buttons[200];
} PsxButtonBackground;

extern uint Rand(int param_1);
extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

int enableGuiBackgroundScroll;
int guiBackgroundFadeIn;
PsxButtonBackground* psxButtonBackgroundPtr;

extern int firstLensFlareOrPsxButtonTexture;
extern Texture textures[150];

void InitPsxButtonBackgroundSprites(int param_1) {
    uint rng;
    int relativeTex;
    int i;

    psxButtonBackgroundPtr = 0x1ef120;
    for (i = 0; i < 200; i++) {
        rng = Rand(4);
        psxButtonBackgroundPtr->buttons[i] = rng;
        relativeTex = rng + 6;
        TSpritePrim(&psxButtonBackgroundPtr->sprites[0][i], 0, 0,
                    textures[firstLensFlareOrPsxButtonTexture + relativeTex].tpage);
        SetSemiTrans(&psxButtonBackgroundPtr->sprites[0][i].sprt,
                     textures[firstLensFlareOrPsxButtonTexture + relativeTex].semitrans);
        SetShadeTex(&psxButtonBackgroundPtr->sprites[0][i].sprt, 0);
        setRGB0(&psxButtonBackgroundPtr->sprites[0][i].sprt, guiBackgroundFadeIn,
                guiBackgroundFadeIn, guiBackgroundFadeIn);

        psxButtonBackgroundPtr->sprites[0][i].sprt.clut =
            textures[firstLensFlareOrPsxButtonTexture + relativeTex].clut;
        psxButtonBackgroundPtr->sprites[0][i].sprt.w =
            textures[firstLensFlareOrPsxButtonTexture + relativeTex].w;
        psxButtonBackgroundPtr->sprites[0][i].sprt.h =
            textures[firstLensFlareOrPsxButtonTexture + relativeTex].h;
        setUV0(&psxButtonBackgroundPtr->sprites[0][i].sprt,
               textures[firstLensFlareOrPsxButtonTexture + relativeTex].u,
               textures[firstLensFlareOrPsxButtonTexture + relativeTex].v);
        psxButtonBackgroundPtr->sprites[1][i] = psxButtonBackgroundPtr->sprites[0][i];
    }

    guiBackgroundFadeIn = 0;
    switch (param_1) {
        case 0:
            enableGuiBackgroundScroll = 0;
            break;
        case 1:
            enableGuiBackgroundScroll = 1;
            break;
    }
}
