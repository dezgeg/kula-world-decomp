#include "common.h"

extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

static TSprite menuCursorSprite[2];
static TSprite musicVolumeWidgetSprite[2];

extern uint firstGuiTexture;
extern TSprite sfxVolumeWidgetSprite[2];
extern Texture textures[150];

void InitWidgetSprites(void) {
    TSpritePrim(&musicVolumeWidgetSprite[0], 0, 0, textures[firstGuiTexture + 13].tpage);
    setXY0(&musicVolumeWidgetSprite[0].sprt, 0, 0);
    setUV0(&musicVolumeWidgetSprite[0].sprt, textures[firstGuiTexture + 13].u, textures[firstGuiTexture + 13].v);
    setRGB0(&musicVolumeWidgetSprite[0].sprt, 0x80, 0x80, 0x80);
    SetSemiTrans(&musicVolumeWidgetSprite[0].sprt, textures[firstGuiTexture + 13].semitrans);
    SetShadeTex(&musicVolumeWidgetSprite[0].sprt, 0);
    musicVolumeWidgetSprite[0].sprt.clut = textures[firstGuiTexture + 13].clut;
    setWH(&musicVolumeWidgetSprite[0].sprt, textures[firstGuiTexture + 13].w, textures[firstGuiTexture + 13].h);

    musicVolumeWidgetSprite[1] = musicVolumeWidgetSprite[0];

    sfxVolumeWidgetSprite[0] = musicVolumeWidgetSprite[1];
    sfxVolumeWidgetSprite[1] = sfxVolumeWidgetSprite[0];

    TSpritePrim(&menuCursorSprite[0], 0, 0, textures[firstGuiTexture + 13].tpage);
    setXY0(&menuCursorSprite[0].sprt, 0, 0);
    setUV0(&menuCursorSprite[0].sprt, textures[firstGuiTexture + 13].u, textures[firstGuiTexture + 13].v);
    setRGB0(&menuCursorSprite[0].sprt, 0x80, 0x80, 0x80);
    SetSemiTrans(&menuCursorSprite[0].sprt, textures[firstGuiTexture + 13].semitrans);
    SetShadeTex(&menuCursorSprite[0].sprt, 0);
    menuCursorSprite[0].sprt.clut = textures[firstGuiTexture + 13].clut;
    setWH(&menuCursorSprite[0].sprt, textures[firstGuiTexture + 13].w, textures[firstGuiTexture + 13].h);

    menuCursorSprite[1] = menuCursorSprite[0];
}
