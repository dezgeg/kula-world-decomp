#include "common.h"

typedef struct PsxButtonBackground {
    TSprite sprites[2][200];
    short buttons[200];
} PsxButtonBackground;

extern uint Rand(int param_1);
extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

int enableGuiBackgroundScroll;
PsxButtonBackground* psxButtonBackgroundPtr;

extern int displayHeight;
extern int displayWidth;
extern int firstLensFlareOrPsxButtonTexture;
extern void* otag[2][1][1026];
extern Texture textures[150];
extern int whichDrawDispEnv;

static int guiBackgroundFadeIn = 0; // .sdata
static int guiBackgroundAnimCounter = 0; // .sdata

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

void DrawPsxButtonBackground(void) {
    int y;
    int x;
    int i;
    uint* ot;
    int b;

    for (i = 0; i < 200; i++) {
        x = (i % 20) * 48 + (rsin(guiBackgroundAnimCounter) * (480 - displayWidth / 2) >> 12) -
            480 + displayWidth / 2;
        y = (i / 20) * 48 + (rcos(guiBackgroundAnimCounter) * (240 - displayHeight / 2) >> 12) -
            240 + displayHeight / 2;
        setXY0(&psxButtonBackgroundPtr->sprites[whichDrawDispEnv][i].sprt, x, y);

        setRGB0(&psxButtonBackgroundPtr->sprites[whichDrawDispEnv][i].sprt, guiBackgroundFadeIn,
                guiBackgroundFadeIn, guiBackgroundFadeIn);

        if (x > -48 && x < displayWidth + 48 && y > -48 && y < displayHeight + 48) {
            addPrim(&otag[whichDrawDispEnv][0][1024] - (psxButtonBackgroundPtr->buttons[i] + 1),
                    &psxButtonBackgroundPtr->sprites[whichDrawDispEnv][i]);
        }
    }

    if (enableGuiBackgroundScroll == 1) {
        guiBackgroundAnimCounter += 9;
    }
    guiBackgroundFadeIn += 6;
    if (guiBackgroundFadeIn > 0x80) {
        guiBackgroundFadeIn = 0x80;
    }
}

void FadePsxButtonBackground(void) {
    guiBackgroundFadeIn -= 20;
    if (guiBackgroundFadeIn < 0) {
        guiBackgroundFadeIn = 0;
    }
}
