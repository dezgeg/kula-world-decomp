#include "common.h"
#include "zlib.h"

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

typedef struct SPair {
    short first;
    short second;
} SPair;

typedef struct FramebufferFetch {
    short x;
    short y;
    ushort texel;
} FramebufferFetch;

typedef struct DeflatedEntry {
    int offset;
    int len;
} DeflatedEntry;

typedef struct X {
    char pad1[20];
    char clut[40];
    short w;
    short h;
    char data[1];
} X;

extern int HasCopycatPlayedMinRounds(void);
extern uint Rand(int param_1);
extern void DisableLightEffect(int param_1);
extern void InitLensFlareSprites(void);
extern void InitLevelEndScreen(void);
extern void InitParticles(void);
extern void InitWidgetSprites(void);
extern void LoadJpMemCardMenuGfx(int menuId);
extern void ResetTextRenderState(void);
extern void ResetTextVars(void);
extern void SetFontEntry(char c, uchar u, uchar v, uchar width, uchar height, uchar x, uchar y, int texIndex);
extern void SetLightEffectColor(int param_1, uint param_2);
extern void SetTextParams(int posX, int posY, int align, int colorR, int colorG, int colorB);
extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);
extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

extern DR_TPAGE jpLabelBackgroundTPage[2];
extern int cameraIndex;
extern int controllerButtons;
extern int copycatPlayerScores[2];
extern int curLevel;
extern int curWorld;
extern int curWorld2;
extern int demoIndex;
extern int displayHeight;
extern int displayWidth;
extern int firstLensFlareOrPsxButtonTexture;
extern int firstParticleTexture;
extern int gameMode;
extern int gameState;
extern int gteXScale;
extern int gteYScale;
extern int isFinal;
extern int isNewRecord;
extern int isPaused;
extern int latestPlayerToFinish;
extern int levelEndReason;
extern int levelHasBeenCompletedByPlayer[2];
extern int levelPlayTime[2];
extern int* MENU_DEFLATED_SPRITES2_PTR;
extern int numCameras;
extern int numParticleTextures;
extern int numTimeTrialPlayers;
extern int screenOffsetX;
extern int screenOffsetY;
extern int showingTimeTrialQualifyScreen;
extern int specialLevelType;
extern int timeTrialDifficulty;
extern int totalPlayTime[2];
extern int totalScore;
extern int twoPlayerWhichPlayer;
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
extern SVECTOR lensFlareSamplingPoints[2][16];
extern Texture textures[150];
extern TSprite fontSprites[96];
extern TSprite jpDemoHintSprites[2][7];
extern TSprite jpUiLabelSprites[2][2];
extern TSprite sfxVolumeWidgetSprite[2];
extern TSprite textDrawSprites[2][300];
extern uint firstGuiTextTexture;
extern uint firstGuiTexture;
extern uint prevControllerButtons;

FramebufferFetch framebufferFetches[16];
int bigGuiSpriteFade;
int guiBackgroundFadeIn;
int inflateRetCode;
int jpDemoHintAnimTimer;
int lensFlareScreenX;
int lensFlareScreenY;
int perfectBlinkTimer;
int sunIntensityOnScreen;
Particle particles[2][16];
POLY_FT4 particlePolyTemplates[7];
short lensFlarePositionMultipliers[16];
SPair lensFlareSpriteMidpoints[16];
TSprite bigGuiSprite1[2];
TSprite bigGuiSprite2[2];
TSprite buttonHelpSprite1[2];
TSprite buttonHelpSprite2[2];
TSprite copycatUiSprites[2][4];
TSprite fontSprites[96];
TSprite lensFlareSprites[2][16];
z_stream zlibStream_a4dd4;

static byte PARTICLE_COLOR_DATA[7][3];
static byte textColorB;
static byte textColorG;
static byte textColorR;
STATIC_FOR_GP_ACCESS int textCharIdx;
STATIC_FOR_GP_ACCESS short hasParticleType3Active;
STATIC_FOR_GP_ACCESS TSprite menuCursorSprite[2];
STATIC_FOR_GP_ACCESS TSprite musicVolumeWidgetSprite[2];
static int lastFrameIdWhenTextWasDrawn;
static int numLensFlareSprites;
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

short LENS_FLARE_COLOR_THRESHOLDS_IN_SPECIAL_LEVEL[2][3] = {
    {0, 0, 0},
    {0, 0, 0},
};

short LENS_FLARE_COLOR_THRESHOLDS_IN_WORLD[13][3] = {
    {24576, 768, 24},
    {24576, 768, 24},
    {24576, 768, 24},
    {24576, 768, 24},
    {23552, 768, 24},
    {24576, 768, 24},
    {-17408, 1504, 47},
    {24576, 768, 24},
    {-17408, 1504, 47},
    {-17408, 1504, 47},
    {11264, 736, 24},
    {24576, 768, 24},
    {24576, 768, 24},
};

SVECTOR SUN_POS_IN_SPECIAL_LEVEL[2] = {
    {0, 4096, 0, 0},
    {0, -4096, 0, 0},
};

SVECTOR SUN_POS_IN_WORLD[13] = {
    {0, 4096, 0, 0},
    {0, 4096, 0, 0},
    {4096, 0, 0, 0},
    {0, 4096, 0, 0},
    {4096, 400, -200, 0},
    {0, 4096, 0, 0},
    {0, 4096, 0, 0},
    {4096, 0, 0, 0},
    {0, 4096, 0, 0},
    {0, 4096, 0, 0},
    {4096, 400, 0, 0},
    {0, 4096, 0, 0},
    {4096, 0, 0, 0},
};


SVECTOR ZERO_SVECTOR_a2ab4 = { 0, 0, 0, 0 };
char S_Fatal_error_in_jens_2d_eng[] = "Fatal error in jens 2d-eng:\n\n";
int whichLevelEndSpriteLoaded = LEVEL_END_GFX_VERY_WELL_DONE;
int gameOverScreenState = 0;
int gameOverScreenFade = 0;
char S_1_0_4[] = "1.0.4"; // hack

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

void InitWidgetSprites(void) {
    TSpritePrim(&musicVolumeWidgetSprite[0], 0, 0, textures[firstGuiTexture + 13].tpage);
    setXY0(&musicVolumeWidgetSprite[0].sprt, 0, 0);
    setUV0(&musicVolumeWidgetSprite[0].sprt, textures[firstGuiTexture + 13].u, textures[firstGuiTexture + 13].v);
    setRGB0(&musicVolumeWidgetSprite[0].sprt, 0x80, 0x80, 0x80);
    SetSemiTrans(&musicVolumeWidgetSprite[0].sprt, textures[firstGuiTexture + 13].semitrans);
    SetShadeTex(&musicVolumeWidgetSprite[0].sprt, 0);
    musicVolumeWidgetSprite[0].sprt.clut = textures[firstGuiTexture + 13].clut;
    musicVolumeWidgetSprite[0].sprt.w = textures[firstGuiTexture + 13].w;
    musicVolumeWidgetSprite[0].sprt.h = textures[firstGuiTexture + 13].h;

    musicVolumeWidgetSprite[1] = musicVolumeWidgetSprite[0];

    sfxVolumeWidgetSprite[1] = sfxVolumeWidgetSprite[0] = musicVolumeWidgetSprite[1];

    TSpritePrim(&menuCursorSprite[0], 0, 0, textures[firstGuiTexture + 13].tpage);
    setXY0(&menuCursorSprite[0].sprt, 0, 0);
    setUV0(&menuCursorSprite[0].sprt, textures[firstGuiTexture + 13].u, textures[firstGuiTexture + 13].v);
    setRGB0(&menuCursorSprite[0].sprt, 0x80, 0x80, 0x80);
    SetSemiTrans(&menuCursorSprite[0].sprt, textures[firstGuiTexture + 13].semitrans);
    SetShadeTex(&menuCursorSprite[0].sprt, 0);
    menuCursorSprite[0].sprt.clut = textures[firstGuiTexture + 13].clut;
    menuCursorSprite[0].sprt.w = textures[firstGuiTexture + 13].w;
    menuCursorSprite[0].sprt.h = textures[firstGuiTexture + 13].h;

    menuCursorSprite[1] = menuCursorSprite[0];
}

void InitLensFlareSprite(short posMul, short textureIdx, int scaleHeight) {
    if (numLensFlareSprites < 16) {
        lensFlarePositionMultipliers[numLensFlareSprites] = posMul;
        TSpritePrim(&lensFlareSprites[0][numLensFlareSprites], 0, 0, textures[textureIdx].tpage);
        setUV0(&lensFlareSprites[0][numLensFlareSprites].sprt,
                textures[textureIdx].u,
                textures[textureIdx].v);
        setRGB0(&lensFlareSprites[0][numLensFlareSprites].sprt, 0x80, 0x80, 0x80);
        SetSemiTrans(&lensFlareSprites[0][numLensFlareSprites].sprt, textures[textureIdx].semitrans);
        SetShadeTex(&lensFlareSprites[0][numLensFlareSprites].sprt, 0);
        lensFlareSprites[0][numLensFlareSprites].sprt.clut = textures[textureIdx].clut;
        lensFlareSprites[0][numLensFlareSprites].sprt.w = textures[textureIdx].w;
        lensFlareSprites[0][numLensFlareSprites].sprt.h = textures[textureIdx].h;

        lensFlareSprites[1][numLensFlareSprites] = lensFlareSprites[0][numLensFlareSprites];

        lensFlareSpriteMidpoints[numLensFlareSprites].first = lensFlareSprites[0][numLensFlareSprites].sprt.w / 2;
        if (scaleHeight) {
            lensFlareSpriteMidpoints[numLensFlareSprites].second = (lensFlareSprites[0][numLensFlareSprites].sprt.h * 1.236) / 2.0;
        } else {
            lensFlareSpriteMidpoints[numLensFlareSprites].second = lensFlareSprites[0][numLensFlareSprites].sprt.h / 2;
        }
        numLensFlareSprites++;
    }
}

void InitLensFlareSprites(void) {
    int a;
    int i;
    int m23 = 23;
    int m80 = 80;

    numLensFlareSprites = 0;
    lensFlareScreenX = 0;
    lensFlareScreenY = 0;

    InitLensFlareSprite(-0x830, firstLensFlareOrPsxButtonTexture, 0);
    InitLensFlareSprite(0, firstParticleTexture, 0);
    InitLensFlareSprite(0x1000, firstLensFlareOrPsxButtonTexture, 0);
    InitLensFlareSprite(0x1db0, firstLensFlareOrPsxButtonTexture + 1, 0);
    InitLensFlareSprite(0x21ca, firstLensFlareOrPsxButtonTexture + 2, 0);
    InitLensFlareSprite(0x564, firstLensFlareOrPsxButtonTexture + 5, 0);
    InitLensFlareSprite(0x9ec, firstLensFlareOrPsxButtonTexture + 5, 0);
    InitLensFlareSprite(0xd70, firstLensFlareOrPsxButtonTexture + 5, 0);
    InitLensFlareSprite(0x1284, firstLensFlareOrPsxButtonTexture + 5, 0);
    InitLensFlareSprite(0x13c4, firstLensFlareOrPsxButtonTexture + 3, 1);
    InitLensFlareSprite(0x193c, firstLensFlareOrPsxButtonTexture + 3, 1);
    InitLensFlareSprite(0x1720, firstLensFlareOrPsxButtonTexture + 4, 1);
    InitLensFlareSprite(0x794, firstLensFlareOrPsxButtonTexture + 4, 1);
    InitLensFlareSprite(0xb18, firstLensFlareOrPsxButtonTexture + 4, 1);
    InitLensFlareSprite(0x1162, firstLensFlareOrPsxButtonTexture + 4, 1);
    InitLensFlareSprite(0x15a4, firstLensFlareOrPsxButtonTexture + 4, 1);

    lensFlareSamplingPoints[0][0].vx = 0;
    lensFlareSamplingPoints[0][0].vy = 0;
    lensFlareSamplingPoints[0][0].vz = 0;
    lensFlareSamplingPoints[1][0] = lensFlareSamplingPoints[0][0];
    for (i = 1; i < 16; i++) {
        a = 300 + (i << 12) / 15;
        lensFlareSamplingPoints[0][i].vx = (m23 * rsin(a)) >> 12;
        lensFlareSamplingPoints[0][i].vy = (m23 * rcos(a)) >> 12;
        lensFlareSamplingPoints[0][i].vz = 0;
        lensFlareSamplingPoints[1][i].vx = (m80 * rsin(a)) >> 12;
        lensFlareSamplingPoints[1][i].vy = (m80 * rcos(a)) >> 12;
        lensFlareSamplingPoints[1][i].vz = 0;
    }
}

void DrawLensFlares(void) {
    MATRIX matrix;
    short screenCoords[VER_JP ? 4 : 10]; // probably fake
    long sz;
    int i;
    int j;
    int sy;
    int sx;
    int halfWidth;
    int halfHeight;
    int texel;

    sunIntensityOnScreen = 0;
    halfWidth = displayWidth / 2;
    halfHeight = displayHeight / 2;

    matrix = perspMatrixes[cameraIndex];
    matrix.t[0] = matrix.t[0] - 0x2000 >> 4;
    matrix.t[1] = matrix.t[1] - 0x2000 >> 4;
    matrix.t[2] = matrix.t[2] - 0x2000 >> 4;
    for (i = 0; i < 3; i++) {
        matrix.m[0][i] = matrix.m[0][i] * gteXScale >> 12;
        matrix.m[1][i] = matrix.m[1][i] * gteYScale >> 12;
    }
    matrix.t[0] = matrix.t[0] * gteXScale >> 12;
    matrix.t[1] = matrix.t[1] * gteYScale >> 12;
    SetRotMatrix(&matrix);
    SetTransMatrix(&matrix);

    if (specialLevelType == 0) {
        sz = RotTransPers(&SUN_POS_IN_WORLD[VER_JP ? curWorld2 : curWorld], (long*)&screenCoords[0], (long*)&screenCoords[2], (long*)&screenCoords[2]);
        if (sz << 2 > 0) {
            sx = screenCoords[0];
            sy = screenCoords[1];
            for (i = 0; i < 16; i++) {
                framebufferFetches[i].x = lensFlareSamplingPoints[0][i].vx + lensFlareScreenX;
                framebufferFetches[i].y = lensFlareSamplingPoints[0][i].vy + lensFlareScreenY;
                if ((sx + lensFlareSamplingPoints[1][i].vx & 0x7fff) < screenOffsetX &&
                    (sy + lensFlareSamplingPoints[1][i].vy & 0x7fff) < screenOffsetY) {
                    texel = framebufferFetches[i].texel;
                    if ((texel & 0x7c00) >= LENS_FLARE_COLOR_THRESHOLDS_IN_WORLD[VER_JP ? curWorld2 : curWorld][0] &&
                        (texel & 0x3e0) >= LENS_FLARE_COLOR_THRESHOLDS_IN_WORLD[VER_JP ? curWorld2 : curWorld][1] &&
                        (texel & 0x1f) >= LENS_FLARE_COLOR_THRESHOLDS_IN_WORLD[VER_JP ? curWorld2 : curWorld][2]) {
                        sunIntensityOnScreen += 5;
                    }
                }
            }
            lensFlareScreenX = sx;
            lensFlareScreenY = sy;
            if (sunIntensityOnScreen > 0) {
                for (i = 0; i < numLensFlareSprites; i++) {
                    if (lensFlarePositionMultipliers[i] != 0) {
                        setRGB0(&lensFlareSprites[whichDrawDispEnv][i].sprt,
                                sunIntensityOnScreen, sunIntensityOnScreen, sunIntensityOnScreen);
                        lensFlareSprites[whichDrawDispEnv][i].sprt.x0 = (sx + (lensFlarePositionMultipliers[i] * (halfWidth - sx) >> 12)) - lensFlareSpriteMidpoints[i].first;
                        lensFlareSprites[whichDrawDispEnv][i].sprt.y0 = (sy + (lensFlarePositionMultipliers[i] * (halfHeight - sy) >> 12)) - lensFlareSpriteMidpoints[i].second;
                        addPrim(&otag[whichDrawDispEnv][cameraIndex][1], &lensFlareSprites[whichDrawDispEnv][i]);
                    }
                }
            }
        }
    }
    if (specialLevelType == 1) {
        for (j = 0; j < 2; j++) {
            if (sunIntensityOnScreen > 0) {
                return;
            }
            sz = RotTransPers(&SUN_POS_IN_SPECIAL_LEVEL[j], (long*)&screenCoords[0], (long*)&screenCoords[2], (long*)&screenCoords[2]);
            if (0 < sz << 2) {
                sx = screenCoords[0];
                sy = screenCoords[1];
                for (i = 0; i < 16; i++) {
                    framebufferFetches[i].x = lensFlareSamplingPoints[0][i].vx + lensFlareScreenX;
                    framebufferFetches[i].y = lensFlareSamplingPoints[0][i].vy + lensFlareScreenY;
                    if ((sx + lensFlareSamplingPoints[1][i].vx & 0x7fff) < screenOffsetX &&
                        (sy + lensFlareSamplingPoints[1][i].vy & 0x7fff) < screenOffsetY) {
                        texel = framebufferFetches[i].texel;
                        if ((texel & 0x7c00) >= LENS_FLARE_COLOR_THRESHOLDS_IN_SPECIAL_LEVEL[j][0] &&
                            (texel & 0x3e0) >= LENS_FLARE_COLOR_THRESHOLDS_IN_SPECIAL_LEVEL[j][1] &&
                            (texel & 0x1f) >= LENS_FLARE_COLOR_THRESHOLDS_IN_SPECIAL_LEVEL[j][2]) {
                            sunIntensityOnScreen += 5;
                        }
                    }
                }
                lensFlareScreenX = sx;
                lensFlareScreenY = sy;
                if (sunIntensityOnScreen > 0) {
                    for (i = 0; i < numLensFlareSprites; i++) {
                        setRGB0(&lensFlareSprites[whichDrawDispEnv][i].sprt,
                                sunIntensityOnScreen, sunIntensityOnScreen, sunIntensityOnScreen);
                        lensFlareSprites[whichDrawDispEnv][i].sprt.x0 = (sx + (lensFlarePositionMultipliers[i] * (halfWidth - sx) >> 12)) - lensFlareSpriteMidpoints[i].first;
                        lensFlareSprites[whichDrawDispEnv][i].sprt.y0 = (sy + (lensFlarePositionMultipliers[i] * (halfHeight - sy) >> 12)) - lensFlareSpriteMidpoints[i].second;
                        if (lensFlarePositionMultipliers[i] != 0) {
                            addPrim(&otag[whichDrawDispEnv][cameraIndex][1], &lensFlareSprites[whichDrawDispEnv][i]);
                        } else {
                            setRGB0(&lensFlareSprites[whichDrawDispEnv][i].sprt,
                                sunIntensityOnScreen, sunIntensityOnScreen, sunIntensityOnScreen);
                            addPrim(&otag[whichDrawDispEnv][cameraIndex][1024], &lensFlareSprites[whichDrawDispEnv][i]);
                        }
                    }
                }
            }
        }
    }
}

void InitLevelEndScreen(void) {
    whichLevelEndSpriteLoaded = LEVEL_END_GFX_NONE;
    gameOverScreenState = 0;
    bigGuiSpriteFade = 0;
}

void LoadLevelEndReasonGfx() {
    int whichGfx;
    short x0;
    short y0;
    RECT rect;
    short w;
    short h;
    int len;
    int off;
    int off2;
    char* ptr;
    void* dest;
    int playTime;

    whichGfx = -levelEndReason * 2;
    if (levelEndReason >= 0) {
        whichGfx = LEVEL_END_GFX_WELL_DONE;
        if (VER_JP && gameMode == 0 && isNewRecord == 1) {
            whichGfx = 38;
        }
        if (gameMode == 2) {
            playTime = levelPlayTime[VER_EU ? twoPlayerWhichPlayer : latestPlayerToFinish] + timeTrialDifficulty * FPS;
            if (playTime < 1) {
                whichGfx = LEVEL_END_GFX_VERY_WELL_DONE;
            }
            if (playTime > 20 * FPS) {
                whichGfx = LEVEL_END_GFX_LEVEL_CLEARED;
            }
        }
    } else {
        if (totalScore < 0 && specialLevelType == 0 && gameMode == 0) {
            if (gameOverScreenState == 0) {
                gameOverScreenState = 1;
            }
            if (gameOverScreenState == 3) {
                whichGfx = LEVEL_END_GFX_GAME_OVER;
            }
        }
    }
    if (gameMode == 1 && curLevel == 0 && (copycatPlayerScores[0] > 5 || copycatPlayerScores[1] > 5) && levelEndReason < 0) {
        whichGfx = LEVEL_END_GFX_PLAYER_2_WON;
        if (copycatPlayerScores[1] < copycatPlayerScores[0]) {
            whichGfx = LEVEL_END_GFX_PLAYER_1_WON;
        }
        if (copycatPlayerScores[0] == copycatPlayerScores[1]) {
            whichGfx = LEVEL_END_GFX_DRAW;
        }
    }
    if (gameMode == 0 && isFinal == 1 && levelEndReason > 0 && curWorld == 10) {
        whichGfx = LEVEL_END_GFX_FINAL_COMPLETE;
    }
    if (gameMode == 2) {
        if (levelEndReason > 0 && curLevel == 0 && totalScore >= 0 && showingTimeTrialQualifyScreen == 1 && numTimeTrialPlayers == 1) {
            whichGfx = LEVEL_END_GFX_QUALIFIED;
            if (totalPlayTime[0] > 0) {
                whichGfx = LEVEL_END_GFX_FAILED_TO_QUALIFY;
            }
        }
        if (gameMode == 2 && numTimeTrialPlayers == 2 && curLevel == 14 && levelEndReason > 0 && levelHasBeenCompletedByPlayer[0] == 1 && levelHasBeenCompletedByPlayer[1] == 1) {
            whichGfx = LEVEL_END_GFX_PLAYER_2_WON;
            if (totalPlayTime[0] < totalPlayTime[1]) {
                whichGfx = LEVEL_END_GFX_PLAYER_1_WON;
            }
            if (totalPlayTime[0] == totalPlayTime[1]) {
                whichGfx = LEVEL_END_GFX_DRAW;
            }
        }
    }
    if (levelEndReason == 0) {
        whichGfx = LEVEL_END_GFX_LOADING;
    }

    if (whichGfx == whichLevelEndSpriteLoaded || whichGfx > MENU_DEFLATED_SPRITES2_PTR[0] - 2 || (levelEndReason == LEVEL_END_TIME_TRIAL_QUIT && whichGfx == LEVEL_END_GFX_WELL_DONE)) {
        return;
    }
    if (whichGfx == LEVEL_END_GFX_LOADING) {
        bigGuiSpriteFade = 0;
    }
    if (VER_JP && whichGfx == 38) {
        perfectBlinkTimer = 0;
    }
    len = MENU_DEFLATED_SPRITES2_PTR[2 + whichGfx * 2];
    off = MENU_DEFLATED_SPRITES2_PTR[1 + whichGfx * 2];
    ptr = (char*)MENU_DEFLATED_SPRITES2_PTR + off;
    dest = TIM_DECOMP_BUF;
    zlibStream_a4dd4.avail_in = len;
    zlibStream_a4dd4.next_in = ptr;
    zlibStream_a4dd4.avail_out = 0x10000;
    zlibStream_a4dd4.next_out = dest;
    inflateRetCode = inflateInit_(&zlibStream_a4dd4, S_1_0_4, 0x38);
    inflateRetCode = inflate(&zlibStream_a4dd4, 4);
    inflateRetCode = inflateEnd(&zlibStream_a4dd4);

    w = *(short*)((char*)TIM_DECOMP_BUF + 0x3c);
    h = *(short*)((char*)TIM_DECOMP_BUF + 0x3e);
    x0 = (displayWidth - 4 * w) / 2;
    if (whichGfx < LEVEL_END_GFX_GAME_OVER) {
        y0 = 5 - 2 * VID_NTSC;
        if (gameMode == 1 || (gameMode == 2 && numTimeTrialPlayers == 1)) {
            y0 = 20 - 2 * VID_NTSC;
        }
    } else {
        y0 = 50;
    }
    if (whichGfx == LEVEL_END_GFX_FINAL_COMPLETE) {
        y0 = 50;
    }
    if (whichGfx == LEVEL_END_GFX_LOADING) {
        y0 = 60;
    }
    if (whichGfx == LEVEL_END_GFX_QUALIFIED || whichGfx == LEVEL_END_GFX_FAILED_TO_QUALIFY) {
        y0 = 10;
    }
    if (h + 94 < 0x100) {
        rect.x = 704;
        rect.y = 94;
        rect.h = 1;
        rect.w = 16;
        DrawSync(0);
        LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x14));
        DrawSync(0);
        rect.x = 704;
        rect.y = 95;
        rect.w = w;
        rect.h = h;
        LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x40));
        DrawSync(0);
        whichGfx++;
        if (whichGfx <= MENU_DEFLATED_SPRITES2_PTR[0] - 1) {
            len = MENU_DEFLATED_SPRITES2_PTR[2 + whichGfx * 2];
            off2 = MENU_DEFLATED_SPRITES2_PTR[1 + whichGfx * 2];
            ptr = (char*)MENU_DEFLATED_SPRITES2_PTR + off2;
            dest = TIM_DECOMP_BUF;
            zlibStream_a4dd4.avail_in = len;
            zlibStream_a4dd4.next_in = ptr;
            zlibStream_a4dd4.avail_out = 0x10000;
            zlibStream_a4dd4.next_out = dest;

            inflateRetCode = inflateInit_(&zlibStream_a4dd4, S_1_0_4, 0x38);
            inflateRetCode = inflate(&zlibStream_a4dd4, 4);
            inflateRetCode = inflateEnd(&zlibStream_a4dd4);
            w = *(short*)((char*)TIM_DECOMP_BUF + 0x3c);
            h = *(short*)((char*)TIM_DECOMP_BUF + 0x3e);
            rect.x = 720;
            rect.y = 94;
            rect.w = 16;
            rect.h = 1;
            LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x14));
            DrawSync(0);
            if (w * 4 < 0x81) {
                rect.x = 736;
                rect.y = 94;
            } else {
                rect.x = 704;
                rect.y = 175;
            }
            rect.w = w;
            rect.h = h;
            LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x40));
            DrawSync(0);

            TSpritePrim(bigGuiSprite2, 0, 0, GetTPage(0, 2, 704, 94));
            setXY0(&bigGuiSprite2[0].sprt, x0, y0);
            SetSemiTrans(&bigGuiSprite2[0].sprt, 2);
            SetShadeTex(&bigGuiSprite2[0].sprt, 0);
            bigGuiSprite2[0].sprt.clut = GetClut(704, 94);
            bigGuiSprite2[0].sprt.w = w * 4;
            bigGuiSprite2[0].sprt.h = h;
            setUV0(&bigGuiSprite2[0].sprt, 0, 0x5f);
            bigGuiSprite2[1] = bigGuiSprite2[0];

            TSpritePrim(bigGuiSprite1, 0, 0, GetTPage(0, 1, 736, 94));
            setXY0(&bigGuiSprite1[0].sprt, x0, y0);
            SetSemiTrans(&bigGuiSprite1[0].sprt, 1);
            SetShadeTex(&bigGuiSprite1[0].sprt, 0);
            bigGuiSprite1[0].sprt.clut = GetClut(720, 94);
            bigGuiSprite1[0].sprt.w = w * 4;
            bigGuiSprite1[0].sprt.h = h;
            if (w * 4 < 129) {
                setUV0(&bigGuiSprite1[0].sprt, 128, 94);
            } else {
                setUV0(&bigGuiSprite1[0].sprt, 0, 175);
            }
            bigGuiSprite1[1] = bigGuiSprite1[0];
            whichLevelEndSpriteLoaded = whichGfx - 1;
        }
    }
}

void FadeOutBigGuiSprite(void) {
    bigGuiSpriteFade -= 20;
    if (bigGuiSpriteFade < 0) {
        bigGuiSpriteFade = 0;
    }
}

void SetBigGuiSpriteVisible(void) {
    bigGuiSpriteFade = 0x100;
}

void FullyFinishLevelEndScreenFades(void) {
    bigGuiSpriteFade = 0x100;
    gameOverScreenFade = 0x80;
    if (totalScore < 0 && specialLevelType == 0 && gameMode == 0) {
        gameOverScreenState = 3;
    }
}

void DrawBigGuiSprite(int param_1) {
    LoadLevelEndReasonGfx(0);
    if (param_1 == 2) {
        /* 2 at beginning of load screen */
        bigGuiSpriteFade = 0;
        gameOverScreenFade = 0;
    }
    if (bigGuiSpriteFade < 0x100) {
        bigGuiSpriteFade += 3;
        if (gameMode != 1) {
            prevControllerButtons = -1;
        }
    }
    if (gameOverScreenState == 1 || gameOverScreenState == 2) {
        prevControllerButtons = 0xffffffff;
    }
    if (gameOverScreenState == 3 && gameOverScreenFade < 0x80) {
        prevControllerButtons = 0xffffffff;
    }

    if (gameOverScreenState < 2) {
        gameOverScreenFade = bigGuiSpriteFade - bigGuiSprite1[whichDrawDispEnv].sprt.y0 / 2;
    } else if (gameOverScreenState == 2) {
        gameOverScreenFade -= 3;
    } else {
        gameOverScreenFade += 3;
    }
    if (gameOverScreenFade < 0) {
        gameOverScreenFade = 0;
        if (gameOverScreenState == 2) {
            gameOverScreenState = 3;
            SndPlaySfx(SFX_GAME_OVER, 0, &ZERO_SVECTOR_a2ab4, 8000);
        }
    }
    if (gameOverScreenFade > 0x80) {
        gameOverScreenFade = 0x80;
    }
    if (gameOverScreenState == 1 && gameOverScreenFade >= 0x80) {
        gameOverScreenState = 2;
    }
    if (param_1 == 0 && (levelEndReason > LEVEL_END_GAME_OVER || gameOverScreenState == 3)) {
        setRGB0(&bigGuiSprite1[whichDrawDispEnv].sprt, gameOverScreenFade, gameOverScreenFade, gameOverScreenFade);
        setRGB0(&bigGuiSprite2[whichDrawDispEnv].sprt, gameOverScreenFade, gameOverScreenFade, gameOverScreenFade);
        addPrim(&primLists[whichDrawDispEnv].main, &bigGuiSprite1[whichDrawDispEnv]);
        addPrim(&primLists[whichDrawDispEnv].main, &bigGuiSprite2[whichDrawDispEnv]);
    }
    if (param_1 == 1) {
        if (bigGuiSpriteFade > 0x80) {
            bigGuiSpriteFade = 0x80;
        }
        setRGB0(&bigGuiSprite1[whichDrawDispEnv].sprt, bigGuiSpriteFade, bigGuiSpriteFade, bigGuiSpriteFade);
        setRGB0(&bigGuiSprite2[whichDrawDispEnv].sprt, bigGuiSpriteFade, bigGuiSpriteFade, bigGuiSpriteFade);
        DrawPrim(&bigGuiSprite1[whichDrawDispEnv]);
        DrawPrim(&bigGuiSprite2[whichDrawDispEnv]);
    }
}
