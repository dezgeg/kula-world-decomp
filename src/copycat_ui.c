#include "common.h"

typedef struct JpDemoHintData {
    short startTime;
    short duration;
    short sprite;
} JpDemoHintData;

extern DR_TPAGE jpLabelBackgroundTPage[2];
extern int controllerButtons;
extern int demoIndex;
extern int displayWidth;
extern int gameMode;
extern int gameState;
extern int isPaused;
extern int whichDrawDispEnv;
extern JpDemoHintData jpDemoHintData[][5];
extern POLY_F4 jpLabelBackgroundPrims[2];
extern PrimList primLists[2];
extern Texture textures[150];
extern TSprite jpDemoHintSprites[2][7];
extern TSprite jpUiLabelSprites[2][2];
extern uint firstGuiTextTexture;

int jpDemoHintAnimTimer;
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
        tex = i + (VER_JP ? 50 : 40);
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

#if VER_JP
void DrawJpLabelBackground(int tex, int y) {
    SetDrawTPage(&jpLabelBackgroundTPage[whichDrawDispEnv], 0, 0, GetTPage(0, 2, 0, 0));
    setPolyF4(&jpLabelBackgroundPrims[whichDrawDispEnv]);
    setSemiTrans(&jpLabelBackgroundPrims[whichDrawDispEnv], 1);
    setRGB0(&jpLabelBackgroundPrims[whichDrawDispEnv], 0xB0, 0xB0, 0xB0);
    tex += 54;
    setXY4(&jpLabelBackgroundPrims[whichDrawDispEnv],
           displayWidth / 2 - textures[firstGuiTextTexture + tex].w / 2 - 1, y - 1,
           displayWidth / 2 + textures[firstGuiTextTexture + tex].w / 2 + 1, y - 1,
           displayWidth / 2 - textures[firstGuiTextTexture + tex].w / 2 - 1, y + 11,
           displayWidth / 2 + textures[firstGuiTextTexture + tex].w / 2 + 1, y + 11);
    addPrim(&primLists[whichDrawDispEnv].main, &jpLabelBackgroundPrims[whichDrawDispEnv]);
    addPrim(&primLists[whichDrawDispEnv].main, &jpLabelBackgroundTPage[whichDrawDispEnv]);
}

void InitJpUiLabels() {
    int i;
    int tex;
    for (i = 0; i < 2; i++) {
        tex = i + 54;
        TSpritePrim(&jpUiLabelSprites[0][i], 0, 0, textures[firstGuiTextTexture + tex].tpage);
        setXY0(&jpUiLabelSprites[0][i].sprt, COPYCAT_SPRITE_POSITIONS[i * 2], COPYCAT_SPRITE_POSITIONS[i * 2 + 1]);
        setRGB0(&jpUiLabelSprites[0][i].sprt, 0x80, 0x80, 0x80);
        SetSemiTrans(&jpUiLabelSprites[0][i].sprt, textures[firstGuiTextTexture + tex].semitrans);
        SetShadeTex(&jpUiLabelSprites[0][i].sprt, 0);
        jpUiLabelSprites[0][i].sprt.clut = textures[firstGuiTextTexture + tex].clut;
        jpUiLabelSprites[0][i].sprt.w = textures[firstGuiTextTexture + tex].w;
        jpUiLabelSprites[0][i].sprt.h = textures[firstGuiTextTexture + tex].h;
        setUV0(&jpUiLabelSprites[0][i].sprt, textures[firstGuiTextTexture + tex].u, textures[firstGuiTextTexture + tex].v);
        jpUiLabelSprites[1][i] = jpUiLabelSprites[0][i];
    }
}

void DrawJpUiLabels(int sprite, int y) {
    int hack;
    if ((controllerButtons & PAD_SELECT) && isPaused == 1) {
        if (gameState != 0) {
            return;
        }
    }
    setXY0(&jpUiLabelSprites[whichDrawDispEnv][sprite].sprt,
           displayWidth / 2 - textures[firstGuiTextTexture + (hack = sprite + 54)].w / 2,
           y);
    addPrim(&primLists[whichDrawDispEnv].main, &jpUiLabelSprites[whichDrawDispEnv][sprite]);
}

void InitJpDemoHints() {
    int i;
    int tex;
    for (i = 0; i < 7; i++) {
        tex = i + 56;
        TSpritePrim(&jpDemoHintSprites[0][i], 0, 0, textures[firstGuiTextTexture + tex].tpage);
        setXY0(&jpDemoHintSprites[0][i].sprt, COPYCAT_SPRITE_POSITIONS[i * 2], COPYCAT_SPRITE_POSITIONS[i * 2 + 1]);
        setRGB0(&jpDemoHintSprites[0][i].sprt, 0x80, 0x80, 0x80);
        SetSemiTrans(&jpDemoHintSprites[0][i].sprt, textures[firstGuiTextTexture + tex].semitrans);
        SetShadeTex(&jpDemoHintSprites[0][i].sprt, 0);
        jpDemoHintSprites[0][i].sprt.clut = textures[firstGuiTextTexture + tex].clut;
        jpDemoHintSprites[0][i].sprt.w = textures[firstGuiTextTexture + tex].w;
        jpDemoHintSprites[0][i].sprt.h = textures[firstGuiTextTexture + tex].h;
        setUV0(&jpDemoHintSprites[0][i].sprt, textures[firstGuiTextTexture + tex].u, textures[firstGuiTextTexture + tex].v);
        jpDemoHintSprites[1][i] = jpDemoHintSprites[0][i];
    }
}

void DrawJpDemoHints() {
    int i;
    int hack;
    int alpha;
    int elapsed;

    jpDemoHintAnimTimer++;
    for (i = 0; i < 5; i++) {
        if (jpDemoHintData[demoIndex][i].startTime * FPS < jpDemoHintAnimTimer &&
                jpDemoHintAnimTimer < (jpDemoHintData[demoIndex][i].startTime + jpDemoHintData[demoIndex][i].duration) * FPS &&
                jpDemoHintData[demoIndex][i].startTime >= 0) {
            alpha = 128;
            elapsed = jpDemoHintAnimTimer - jpDemoHintData[demoIndex][i].startTime * FPS;

            if (elapsed < 40) {
                alpha = (elapsed * 128) / 40;
            }
            elapsed = (jpDemoHintData[demoIndex][i].startTime + jpDemoHintData[demoIndex][i].duration) * FPS - jpDemoHintAnimTimer;
            if (elapsed < 15) {
                alpha = (elapsed * 128) / 15;
            }

            setRGB0(&jpDemoHintSprites[whichDrawDispEnv][jpDemoHintData[demoIndex][i].sprite].sprt, alpha, alpha, alpha);
            setXY0(&jpDemoHintSprites[whichDrawDispEnv][jpDemoHintData[demoIndex][i].sprite].sprt,
                   displayWidth / 2 - textures[firstGuiTextTexture + (hack = jpDemoHintData[demoIndex][i].sprite + 56)].w / 2,
                   70);
            addPrim(&primLists[whichDrawDispEnv].gui2, &jpDemoHintSprites[whichDrawDispEnv][jpDemoHintData[demoIndex][i].sprite]);
        }
    }
}
#endif
