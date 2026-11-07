#include "common.h"

typedef struct Frame {
    short templateIdx;
    short angle;
    short scale;
    short color;
} Frame;

typedef struct ParticleType {
    short numPrims;
    short numFrames;
    short data[0];
} ParticleType;

typedef struct Particle {
    POLY_FT4 prims[2][2];
    SVECTOR pos;
    ParticleType* animData;
    short rotation[2];
    int frame;
    int lightEffectId;
} Particle;

typedef struct JpDemoHintData {
    short startTime;
    short duration;
    short sprite;
} JpDemoHintData;

extern uint Rand(int param_1);
extern void DisableLightEffect(int param_1);
extern void InitLensFlareSprites(void);
extern void InitLevelEndScreen(void);
extern void InitParticles(void);
extern void InitWidgetSprites(void);
extern void ResetTextRenderState(void);
extern void ResetTextVars(void);
extern void SetFontEntry(char c, uchar u, uchar v, uchar width, uchar height, uchar x, uchar y, int texIndex);
extern void SetLightEffectColor(int param_1, uint param_2);
extern void SetTextParams(int posX, int posY, int align, int colorR, int colorG, int colorB);
extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

extern DR_TPAGE jpLabelBackgroundTPage[2];
extern int cameraIndex;
extern int controllerButtons;
extern int demoIndex;
extern int displayWidth;
extern int firstParticleTexture;
extern int gameMode;
extern int gameState;
extern int isPaused;
extern int numCameras;
extern int numParticleTextures;
extern int screenOffsetX;
extern int screenOffsetY;
extern int whichDrawDispEnv;
extern JpDemoHintData jpDemoHintData[][5];
extern long projectionDistance;
extern MATRIX perspMatrixes[2];
extern OT_TYPE otag[2][1][1026];
extern POLY_F4 jpLabelBackgroundPrims[2];
extern PrimList primLists[2];
extern short PARTICLE_TYPE_0[1]; // fake size
extern short PARTICLE_TYPE_1[1]; // fake size
extern short PARTICLE_TYPE_2_6_7[1]; // fake size
extern short PARTICLE_TYPE_3[1]; // fake size
extern short PARTICLE_TYPE_4_5_10[1]; // fake size
extern short PARTICLE_TYPE_8[1]; // fake size
extern short PARTICLE_TYPE_9[1]; // fake size
extern Texture textures[150];
extern TSprite fontSprites[96];
extern TSprite jpDemoHintSprites[2][7];
extern TSprite jpUiLabelSprites[2][2];
extern TSprite textDrawSprites[2][300];
extern uint firstGuiTextTexture;
extern uint firstGuiTexture;

int jpDemoHintAnimTimer;
Particle particles[2][16];
POLY_FT4 particlePolyTemplates[7];
TSprite copycatUiSprites[2][4];
TSprite fontSprites[96];

static byte PARTICLE_COLOR_DATA[7][3];
static byte textColorB;
static byte textColorG;
static byte textColorR;
STATIC_FOR_GP_ACCESS int textCharIdx;
STATIC_FOR_GP_ACCESS short hasParticleType3Active;
static int lastFrameIdWhenTextWasDrawn;
static int numParticles[2];
static int textAlignMode;
static int textPosXEnd;
static int textPosXStart;
static int textPosYCur;
static int textPosYEnd;
static int textPosYStart;
static short* PARTICLE_TYPE_PTRS[12];

short COPYCAT_SPRITE_POSITIONS[] = {
    98, 20 - 4 * VID_NTSC,
    98, 20 - 4 * VID_NTSC,
    28, SCREEN_HEIGHT - 56,
    246, SCREEN_HEIGHT - 56,
    40, SCREEN_HEIGHT - 56,
    246, SCREEN_HEIGHT - 56
};

void VariousInit(void) {
    InitParticles();
    InitLensFlareSprites();
    ResetTextRenderState();
    ResetTextVars();
    InitLevelEndScreen();
#if VER_JP
    jpDemoHintAnimTimer = 0;
#endif
}

#if !VER_EU
void ClearScreen(void) {
    RECT rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = 640;
    rect.h = 240;
    VSync(0);
    DrawSync(0);
    ClearImage(&rect, 0, 0, 0);
    DrawSync(0);
}
#endif

void InitParticles(void) {
    int i;

    for (i = 0; i < 12; i++) {
        PARTICLE_TYPE_PTRS[i] = NULL;
    }
    PARTICLE_TYPE_PTRS[0] = PARTICLE_TYPE_0;
    PARTICLE_TYPE_PTRS[1] = PARTICLE_TYPE_1;
    PARTICLE_TYPE_PTRS[2] = PARTICLE_TYPE_2_6_7;
    PARTICLE_TYPE_PTRS[3] = PARTICLE_TYPE_3;
    PARTICLE_TYPE_PTRS[4] = PARTICLE_TYPE_4_5_10;
    PARTICLE_TYPE_PTRS[5] = PARTICLE_TYPE_4_5_10;
    PARTICLE_TYPE_PTRS[6] = PARTICLE_TYPE_2_6_7;
    PARTICLE_TYPE_PTRS[7] = PARTICLE_TYPE_2_6_7;
    PARTICLE_TYPE_PTRS[8] = PARTICLE_TYPE_8;
    PARTICLE_TYPE_PTRS[9] = PARTICLE_TYPE_9;
    PARTICLE_TYPE_PTRS[10] = PARTICLE_TYPE_4_5_10;
    PARTICLE_COLOR_DATA[0][0] = 100;
    PARTICLE_COLOR_DATA[0][1] = 50;
    PARTICLE_COLOR_DATA[0][2] = 100;
    PARTICLE_COLOR_DATA[1][0] = 80;
    PARTICLE_COLOR_DATA[1][1] = 80;
    PARTICLE_COLOR_DATA[1][2] = 80;
    PARTICLE_COLOR_DATA[2][0] = 127;
    PARTICLE_COLOR_DATA[2][1] = 60;
    PARTICLE_COLOR_DATA[2][2] = 60;
    PARTICLE_COLOR_DATA[3][0] = 20;
    PARTICLE_COLOR_DATA[3][1] = 20;
    PARTICLE_COLOR_DATA[3][2] = 127;
    PARTICLE_COLOR_DATA[4][0] = 127;
    PARTICLE_COLOR_DATA[4][1] = 127;
    PARTICLE_COLOR_DATA[4][2] = 0;
    PARTICLE_COLOR_DATA[5][0] = 20;
    PARTICLE_COLOR_DATA[5][1] = 20;
    PARTICLE_COLOR_DATA[5][2] = 80;
    PARTICLE_COLOR_DATA[6][0] = 127;
    PARTICLE_COLOR_DATA[6][1] = 127;
    PARTICLE_COLOR_DATA[6][2] = 127;
    for (i = 0; i < numParticleTextures; i++) {
        if (i > 6) {
            break;
        }
        SetPolyFT4(&particlePolyTemplates[i]);
        setRGB0(&particlePolyTemplates[i], 0x80, 0x80, 0x80);
        SetSemiTrans(&particlePolyTemplates[i], textures[firstParticleTexture + i].semitrans);
        setUV4(&particlePolyTemplates[i], textures[firstParticleTexture + i].u,
               textures[firstParticleTexture + i].v,
               textures[firstParticleTexture + i].u + textures[firstParticleTexture + i].w + 0xff,
               textures[firstParticleTexture + i].v, textures[firstParticleTexture + i].u,
               textures[firstParticleTexture + i].v + textures[firstParticleTexture + i].h + 0xff,
               textures[firstParticleTexture + i].u + textures[firstParticleTexture + i].w + 0xff,
               textures[firstParticleTexture + i].v + textures[firstParticleTexture + i].h + 0xff);
        particlePolyTemplates[i].clut = textures[firstParticleTexture + i].clut;
        particlePolyTemplates[i].tpage = textures[firstParticleTexture + i].tpage;
    }
    numParticles[1] = 0;
    numParticles[0] = 0;
}

void AddParticles(int type, SVECTOR* pos, int lightEffectId) {
    if (PARTICLE_TYPE_PTRS[type] != NULL) {
        if (numParticles[cameraIndex] < 16) {
            if (pos->vz < 0 || numCameras == 1) {
                particles[cameraIndex][numParticles[cameraIndex]].animData = (ParticleType*)PARTICLE_TYPE_PTRS[type];
                particles[cameraIndex][numParticles[cameraIndex]].pos.vx = pos->vx;
                particles[cameraIndex][numParticles[cameraIndex]].pos.vy = pos->vy;
                particles[cameraIndex][numParticles[cameraIndex]].pos.vz = pos->vz;
                particles[cameraIndex][numParticles[cameraIndex]].frame = 0;
                particles[cameraIndex][numParticles[cameraIndex]].lightEffectId = lightEffectId;
                particles[cameraIndex][numParticles[cameraIndex]].rotation[0] = 0;
                particles[cameraIndex][numParticles[cameraIndex]].rotation[1] = 0;
                numParticles[cameraIndex]++;
            }
            if (VER_JP && type == 3) {
                hasParticleType3Active = 1;
            }
        }
    }
}

void ProcessAndRenderParticles(void) {
    long sxy;
    long dummy;
    int i;
    int j;
    int lightFlag;
    char colorR;
    char colorG;
    char colorB;
    Frame* fd;

    int sx;
    int sy;

    int scaledCosVal;
    int scaledSinVal;
    int scale;
    int z;
    int clipped;
    int off;
    int t;

#if VER_JP
    hasParticleType3Active = 0;
#endif
    for (i = 0; i < numParticles[cameraIndex]; i++) {
        colorB = 0;
        colorG = 0;
        colorR = 0;
        lightFlag = -1;
        while (particles[cameraIndex][i].frame >= particles[cameraIndex][i].animData->numFrames) {
            DisableLightEffect(particles[cameraIndex][i].lightEffectId);
            numParticles[cameraIndex]--;

            for (j = i; j < numParticles[cameraIndex]; j++) {
                particles[cameraIndex][j].pos = particles[cameraIndex][j + 1].pos;
                particles[cameraIndex][j].animData = particles[cameraIndex][j + 1].animData;
                particles[cameraIndex][j].frame = particles[cameraIndex][j + 1].frame;
                particles[cameraIndex][j].lightEffectId = particles[cameraIndex][j + 1].lightEffectId;
                particles[cameraIndex][j].rotation[0] = particles[cameraIndex][j + 1].rotation[0];
                particles[cameraIndex][j].rotation[1] = particles[cameraIndex][j + 1].rotation[1];
            }
            if (i >= numParticles[cameraIndex]) {
                return;
            }
        }
        if (i >= numParticles[cameraIndex]) {
            return;
        }

        if (VER_JP && particles[cameraIndex][i].animData == (ParticleType*)PARTICLE_TYPE_PTRS[3]) {
            hasParticleType3Active = 1;
        }

        // pArr[0] - template idx
        // pArr[1] - rotation angle (<0 -> set to random)
        // pArr[2] - scale (if negative some shear(?) effect)
        fd = (Frame*)particles[cameraIndex][i].animData;
        fd = (Frame*)((short*)fd + 2 + (((ParticleType*)fd)->numPrims * 4 * particles[cameraIndex][i].frame));
        for (j = 0; j < ((ParticleType*)particles[cameraIndex][i].animData)->numPrims; j++) {
            scale = abs(fd->scale);
            if (particles[cameraIndex][i].pos.vz < 0) {
                particles[cameraIndex][i].prims[whichDrawDispEnv][j] = particlePolyTemplates[fd->templateIdx];
                setRGB0(&particles[cameraIndex][i].prims[whichDrawDispEnv][j], fd->color, fd->color, fd->color);
                sx = particles[cameraIndex][i].pos.vx;
                sy = particles[cameraIndex][i].pos.vy;
                if (fd->angle < 0) {
                    particles[cameraIndex][i].rotation[j] = Rand(0x1000);
                    scaledCosVal = scale * rcos(particles[cameraIndex][i].rotation[j]) >> 18;
                    scaledSinVal = scale * rsin(particles[cameraIndex][i].rotation[j]) >> 18;
                } else {
                    scaledCosVal = scale * rcos(fd->angle + particles[cameraIndex][i].rotation[j]) >> 18;
                    scaledSinVal = scale * rsin(fd->angle + particles[cameraIndex][i].rotation[j]) >> 18;

                }

                if (fd->scale >= 0) {
                    setXY4(&particles[cameraIndex][i].prims[whichDrawDispEnv][j],
                        sx + scaledCosVal, sy + scaledSinVal,
                        sx - scaledSinVal, sy + scaledCosVal,
                        sx + scaledSinVal, sy - scaledCosVal,
                        sx - scaledCosVal, sy - scaledSinVal);

                } else {
                    setXY4(&particles[cameraIndex][i].prims[whichDrawDispEnv][j],
                        sx + scaledCosVal, sy + scaledSinVal,
                        sx + scaledCosVal - scaledSinVal, sy + scaledCosVal + scaledSinVal,
                        sx, sy,
                        sx - scaledSinVal, sy + scaledCosVal);
                }
                addPrim(&primLists[whichDrawDispEnv].main, &particles[cameraIndex][i].prims[whichDrawDispEnv][j]);
            } else {
                SetRotMatrix(&perspMatrixes[cameraIndex]);
                SetTransMatrix(&perspMatrixes[cameraIndex]);
                z = RotTransPers(&particles[cameraIndex][i].pos, &sxy, &dummy, &dummy) << 2;

                if (z > 120) {
                    sx = (short)sxy;
                    sy = sxy >> 16;
                    if (fd->angle < 0) {
                        particles[cameraIndex][i].rotation[j] = Rand(0x1000);
                        t = rcos(particles[cameraIndex][i].rotation[j]) >> 16;
                        if (t == 0) {
                            t = 1;
                        }
                        scaledCosVal = (scale * projectionDistance) / t;
                        t = rsin(particles[cameraIndex][i].rotation[j]) >> 16;
                        if (t == 0) {
                            t = 1;
                        }
                        scaledSinVal = (scale * projectionDistance) / t;
                    } else {
                        scaledCosVal = ((scale * projectionDistance) / z) * rcos(fd->angle + particles[cameraIndex][i].rotation[j]) >> 16;
                        scaledSinVal = ((scale * projectionDistance) / z) * rsin(fd->angle + particles[cameraIndex][i].rotation[j]) >> 16;
                    }
                    clipped = screenOffsetX <= (sx + scaledCosVal & 0x7fff) && screenOffsetY <= (sy + scaledSinVal & 0x7fff) &&
                            screenOffsetX <= (sx - scaledSinVal & 0x7fff) && screenOffsetY <= (sy + scaledCosVal & 0x7fff) &&
                            screenOffsetX <= (sx + scaledSinVal & 0x7fff) && screenOffsetY <= (sy - scaledCosVal & 0x7fff) &&
                            screenOffsetX <= (sx - scaledCosVal & 0x7fff) && screenOffsetY <= (sy - scaledSinVal & 0x7fff);
                    if (!clipped) {
                        lightFlag = 0;
                        colorR += (fd->color * PARTICLE_COLOR_DATA[fd->templateIdx][0]) >> 9;
                        colorG += (fd->color * PARTICLE_COLOR_DATA[fd->templateIdx][1]) >> 9;
                        colorB += (fd->color * PARTICLE_COLOR_DATA[fd->templateIdx][2]) >> 9;
                        particles[cameraIndex][i].prims[whichDrawDispEnv][j] = particlePolyTemplates[fd->templateIdx];
                        setRGB0(&particles[cameraIndex][i].prims[whichDrawDispEnv][j],fd->color,fd->color,fd->color);
                        if (fd->scale >= 0) {
                            setXY4(&particles[cameraIndex][i].prims[whichDrawDispEnv][j],
                                sx + scaledCosVal, sy + scaledSinVal,
                                sx - scaledSinVal, sy + scaledCosVal,
                                sx + scaledSinVal, sy - scaledCosVal,
                                sx - scaledCosVal, sy - scaledSinVal);
                        } else {
                            setXY4(&particles[cameraIndex][i].prims[whichDrawDispEnv][j],
                                sx + scaledCosVal,  sy + scaledSinVal,
                                sx + scaledCosVal - scaledSinVal, sy + scaledCosVal + scaledSinVal,
                                sx, sy,
                                sx - scaledSinVal, sy + scaledCosVal);
                        }
                        addPrim(&otag[whichDrawDispEnv][cameraIndex][0], &particles[cameraIndex][i].prims[whichDrawDispEnv][j]);
                    }
                }
            }
            fd++;
        }
        if (lightFlag == 0) {
            SetLightEffectColor(particles[cameraIndex][i].lightEffectId, (colorR << 16) + (colorG << 8) | colorB);
        }

        particles[cameraIndex][i].frame++;
    }
}

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

void InitFont(void) {
    int i;

    InitWidgetSprites();
    SetTextParams(displayWidth >> 1, 0, 1, 128, 128, 128);
    for (i = 0; i < 128; i++) {
        SetFontEntry(i, 0, 0, 0, 0, 0, 0, firstGuiTexture + 9);
    }
    SetFontEntry('e', 0, 0, 11, 11, 0, 2, firstGuiTexture + 10);
    SetFontEntry('f', 12, 0, 11, 11, 0, 2, firstGuiTexture + 10);
    SetFontEntry('g', 24, 0, 11, 11, 0, 2, firstGuiTexture + 10);
    SetFontEntry('h', 36, 0, 11, 11, 0, 2, firstGuiTexture + 10);
    SetFontEntry('A', 0, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('B', 8, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('C', 16, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('D', 24, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('E', 32, 0, 6, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('F', 40, 0, 6, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('G', 48, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('H', 56, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('I', 64, 0, 4, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('J', 72, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('K', 80, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('L', 88, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('M', 96, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('N', 104, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('O', 112, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('P', 120, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('Q', 128, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('R', 136, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('S', 144, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('T', 152, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('U', 160, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('V', 168, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('W', 176, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('X', 184, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('Y', 192, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('Z', 200, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('0', 232, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('1', 240, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('2', 248, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('3', 0, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('4', 8, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('5', 16, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('6', 24, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('7', 32, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('8', 40, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('9', 48, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('.', 56, 12, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(',', 64, 12, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(':', 72, 12, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(';', 80, 12, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('-', 88, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('+', 96, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('\'', 104, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('`', 104, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('!', 112, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('?', 120, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('\"', 128, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('#', 136, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('%', 144, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('&', 152, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('/', 160, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('\\', 168, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('(', 176, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(')', 184, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('[', 176, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(']', 184, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('{', 176, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('}', 184, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('=', 192, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('>', 200, 12, 12, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('<', 212, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('|', 220, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(' ', 0, 0, 4, 0, 0, 2, firstGuiTexture + 9);
}

void ResetTextRenderState(void) {
    lastFrameIdWhenTextWasDrawn = -1;
}

void SetTextParams(int posX, int posY, int align, int colorR, int colorG, int colorB) {
    if (-1 < align) {
        textAlignMode = align;
    }
    if (posX != -1) {
        textPosXStart = posX;
    }
    if (posY != -1) {
        textPosYStart = posY;
        textPosYEnd = posY;
    }
    if (colorR > 0x80) {
        colorR = 0x80;
    }
    if (colorG > 0x80) {
        colorG = 0x80;
    }
    if (colorB > 0x80) {
        colorB = 0x80;
    }
    if (colorR >= 0) {
        textColorR = (byte)colorR;
    }
    if (colorG >= 0) {
        textColorG = (byte)colorG;
    }
    if (colorB >= 0) {
        textColorB = (byte)colorB;
    }
}

void DrawTextCrappyFont(char* str) {
    int i;
    int j;
    int width;
    int lineStartIndex;

    if (whichDrawDispEnv != lastFrameIdWhenTextWasDrawn) {
        lastFrameIdWhenTextWasDrawn = whichDrawDispEnv;
        textPosXEnd = textPosXStart;
        textPosYCur = textPosYStart;
        textPosYEnd = textPosYStart;
    }

    textPosYCur = textPosYEnd;
    lineStartIndex = 0;
    for (width = i = 0; str[i]; i++) {
        if (VER_EU || str[i] - 0x20 >= 0)
            width += fontSprites[str[i] - 0x20].sprt.w + 1;
        if (str[i] == '\n' || str[i] == 0) {
            if (textAlignMode == 0) {
                textPosXEnd = textPosXStart;
            }
            if (textAlignMode == 1) {
                textPosXEnd = textPosXStart - width / 2;
            }
            if (textAlignMode == 2) {
                textPosXEnd = textPosXStart - width;
            }

            for (j = lineStartIndex; str[j] != '\n' && str[j]; j++) {
                if (fontSprites[str[j] - 0x20].sprt.h != 0 && str[j] - 0x20 >= 0) {
                    if (textCharIdx + 1 >= 400) {
                        return;
                    }
                    memcpy(&textDrawSprites[whichDrawDispEnv][textCharIdx], &fontSprites[str[j] - 0x20], sizeof(TSprite));
                    if (str[j] != '|') {
                        setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, textColorR, textColorG, textColorB);
                    } else {
                        setRGB0(&textDrawSprites[whichDrawDispEnv][textCharIdx].sprt, textColorR, 0, 0);
                    }
                    textDrawSprites[whichDrawDispEnv][textCharIdx].sprt.x0 += textPosXEnd;
                    textDrawSprites[whichDrawDispEnv][textCharIdx].sprt.y0 += textPosYCur;
                    addPrim(&primLists[whichDrawDispEnv].main, &textDrawSprites[whichDrawDispEnv][textCharIdx]);
                    textCharIdx++;
                }
                textPosXEnd += fontSprites[str[j] - 0x20].sprt.w + 1;
            }
            width = 0;
            lineStartIndex = i + 1;
            textPosYCur += 11;
        }
    }
    textPosYEnd = textPosYCur;
}
