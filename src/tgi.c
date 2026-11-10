#include "common.h"
#include <string.h>

typedef struct TextureMipLevel {
    short* clutTable; // distance-indexed lookup table (pointing to tgi part 0)
    union {
        short u16;
        struct {
            byte u;
            byte v;
        } u8;
    } uv;
    short tpage;
} TextureMipLevel;

typedef struct CubeTextureMetadata {
    short fullClut;
    short fullUv;
    short fullTpage;
    short pad;
    TextureMipLevel mipmaps[3];
} CubeTextureMetadata;

extern int Square(int val);
extern uint Rand(int param_1);
extern void DisableScreenShake(void);
extern void InitCubeTextureMetadata(void);
extern void InitLasers(void);
extern void InitManySprites(void);
extern void InitPlayerSpecularSprite(void);
extern void InitShadowSprites(void);
extern void InitTurningMotionBlur(void);
extern void LoadImagesFromTgiPart9(short* p);
extern void ParseKeysAndSpecialLevelFromItemData(void);
extern void ProcessLevelData(void);
extern void ResetCheats(void);
extern void ResetVibration(void);
extern void ScanLevelDataForBlinkingEntities(void);
extern void SetSemiTrans(void* p, int enable);
extern void SetShadeTex(void* p, int disable);
extern void SetupDisplay(int isbg, int bgR, int bgG, int bgB, int useDithering, int use24Bit);
extern void SkyboxSinCos(int azimuth, int elevation, short* outX, short* outY, short* outZ);
extern void TweakSkyboxColors(uint param_1);

void RecalcSkyboxes01(int angleOfSomethingI, int angleOfSomethingJ, int countOfSomethingI, int countOfSomethingJ, int param_5);
int RecalcSkyboxes2(void);

extern int firstUnk2Texture;
extern int isFinal;
extern int isInCredits;
extern int lightEffects[32 * 8];
extern int recentlyVisitedCubeFaces[4 * 32];
extern int starfieldSinPhase1;
extern int starfieldSinPhase2;
extern int starfieldSinPhase3;
extern int starfieldTransPhaseX1;
extern int starfieldTransPhaseX2;
extern int starfieldTransPhaseY1;
extern int starfieldTransPhaseY2;
extern int starfieldTransPhaseZ1;
extern int starfieldTransPhaseZ2;
extern int sunglassDisablingState[1];
extern int sunglassSeeEverything[1];
extern int toBeDisabledLightEffects[64];
extern int unkOtagLengths[2][1];
extern SVECTOR starfieldSinVec;
extern Texture textures[NUM_TEXTURES];
extern VECTOR starfieldTrans;
extern void* cubeStates[256 * 16];

int turningMotionBlurPhase;
int turningMotionBlurTable[9 * 7];
DR_TPAGE drTpages1[2][1];
DR_TPAGE drTpages2[2][1];
DR_TPAGE hudDrTpages[2];
CubeTextureMetadata cubeTextureMetadata[1300];
int itemsDispListIdx;
int playerEnemyDispListIdx;
TgiFile* tgi;
int turningMotionBlurEnabled;
short* tgiPart1;
short* tgiPart2;
short* tgiPart3;
short* tgiPart4;
short* tgiPart7;
short* tgiPart8;
short* entityData;
short* levelData;
short numEntities;
int specialLevelType;

int turningMotionBlurTargetLods[7] = { 28, 23, 22, 15, 40, 30, 20 };
int LOD_THRESHOLDS[9] = { 0, 30, 43, 55, 66, 76, 85, 93, 100 }; // percentage
int HIGHSCORE_CUBE_RANDOM_TEXTURES[16] = { 666, 666, 666, 666, 666, 666, 4, 4, 4, 4, 668, 668, 668, 6, 6, 0 };

STATIC_FOR_GP_ACCESS int numPlainTileTextureVariations;
STATIC_FOR_GP_ACCESS int quadSomethingCount;
STATIC_FOR_GP_ACCESS int quadSomethingStartIndex;
STATIC_FOR_GP_ACCESS int recalcSkyboxes;
STATIC_FOR_GP_ACCESS int wasSpecialLevel;
STATIC_FOR_GP_ACCESS short skyboxVertices[3][3];
STATIC_FOR_GP_ACCESS short* tgiPart5;
STATIC_FOR_GP_ACCESS short* tgiPart9;
STATIC_FOR_GP_ACCESS short* tgiPart9;
static int numObjTextures;
static int skyboxMaxAngle1;
static int skyboxMaxAngle2;
static short* tgiEnd;
static short* tgiPart0;
static short* tgiPart10;
static short* tgiPart6;

void* ParseTGI(TgiFile* tgiBuf) {
    int i;
    int j;
    short* p;
    RECT rect;

    tgi = tgiBuf;
    tgiPart0 = tgiBuf + 1;
    tgiPart1 = tgiPart0 + tgiBuf->part0Words;
    tgiPart2 = tgiPart1 + tgiBuf->part1Words;
    tgiPart3 = tgiPart2 + tgiBuf->part2Words;
    tgiPart4 = tgiPart3 + tgiBuf->part3Words;
    tgiPart5 = tgiPart4 + tgiBuf->part4Words;
    tgiPart6 = tgiPart5 + tgiBuf->part5Words;
    tgiPart7 = tgiPart6 + tgiBuf->part6Words;
    tgiPart8 = tgiPart7 + tgiBuf->part7Words;
    tgiPart9 = tgiPart8 + tgiBuf->part8Words;
    tgiPart10 = tgiPart9 + tgiBuf->part9Words;
    tgiEnd = tgiPart10 + tgiBuf->part10Words;
    InitTurningMotionBlur();
    InitCubeTextureMetadata();

    p = tgiPart10;
    DrawSync(0);
    while (p < tgiEnd) {
        rect.x = *p++;
        rect.y = *p++;
        rect.w = *p++;
        rect.h = *p++;
        LoadImage(&rect, p);
        DrawSync(0);
        p += rect.w * rect.h;
    }
    recalcSkyboxes = 1;
    if (tgi->skyboxFlag == 1025) {
        LoadImagesFromTgiPart9(tgiPart9 + 0x3f00 / 2);
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 1; j++) {
            SetDrawTPage(&drTpages1[i][j], 0, 1, GetTPage(0, 0, 0, 0));
            SetDrawTPage(&drTpages2[i][j], 0, 1, GetTPage(0, 0, 0, 0));
        }
        SetDrawTPage(&hudDrTpages[i], 0, 0, GetTPage(0, 0, 0, 0));
    }
    return tgiPart9 + 0x3f00 / 2;
}

void InitCubeTextureMetadata(void) {
    short* pPart6;
    int i;
    int j;
    int mipmap;
    short* p;
    int p0;
    short* de;

    int tx;
    int ty;
    int mx;
    int my;

    for (p = tgiPart5; p < tgiPart6; p += 2) {
        p0 = p[0];
        for (i = 0; i < 3; i++) {
            for (j = 0; j < p[1]; j++) {
                pPart6 = &tgiPart6[20 * (p0 + j)];
                de = &cubeTextureMetadata[p0 * 3 + i * p[1] + j];
                *de++ = pPart6[i]; // fullClut
                tx = pPart6[3];
                ty = pPart6[4];
                mx = (tx % 64) * 2;
                my = ty % 256;
                *de++ = my << 8 | mx; // fullUv
                *de++ = GetTPage(1, 0, tx, ty); // fullTpage
                *de++ = 0; // pad
                pPart6 += 5;
                for (mipmap = 0; mipmap < 3; pPart6 += 5, mipmap++) {
                    *(int*)de = &tgiPart0[pPart6[i] * tgi->numDepthClueCluts]; // clutTable
                    de += 2;
                    tx = pPart6[3];
                    ty = pPart6[4];
                    mx = (tx % 64) * 4;
                    my = ty % 256;
                    switch (mipmap) {
                        case 0:
                            mx += 0x1f;
                            break;
                        case 1:
                            mx += 0xf;
                            break;
                        case 2:
                            mx += 0x7;
                            break;
                    }
                    *de++ = my << 8 | mx; // uv
                    *de++ = GetTPage(0, 0, tx, ty); // tpage
                }
            }
        }
    }
}

void GetHighscoreCubeStyle(ushort* pTpage, ushort* pClut, byte* pU, byte* pV, int* pSemitrans) {
    int variation;
    int texture;
    CubeTextureMetadata* md;

    variation = HIGHSCORE_CUBE_RANDOM_TEXTURES[Rand(16)];
    *pSemitrans = 0;
    switch (variation) {
        default:
            texture = variation;
            break;
        case 6:
            texture = 6;
            *pSemitrans = 1;
            break;
        case 666:
            texture = tgi->numFixedTextures + Rand(tgi->numPlainTileTextureVariationsNormalLevel);
            break;
        case 668:
            texture = tgi->numFixedTextures + tgi->numPlainTileTextureVariationsNormalLevel + 2;
            break;
    }
    md = &cubeTextureMetadata[tgiPart5[texture * 2] * 3 + tgiPart5[texture * 2 + 1]];
    *pTpage = md->mipmaps[0].tpage | 0x20;
    *pClut = *md->mipmaps[0].clutTable;
    *pU = md->mipmaps[0].uv.u8.u - 0x1f;
    *pV = md->mipmaps[0].uv.u8.v;
}

void LoadImagesFromTgiPart9(short* p) {
    int i;
    int w;
    int h;
    RECT rect;

    for (i = 0; i < 24; i++) {
        rect.x = *p++;
        rect.y = *p++;
        w = *p++;
        rect.w = w;
        h = *p++;
        rect.h = h;
        LoadImage(&rect, p);
        DrawSync(0);
        p += w * h;
    }
    rect.x = *p++;
    rect.y = *p++;
    rect.w = *p++;
    rect.h = *p++;
    LoadImage(&rect, p);
    DrawSync(0);
}

void InitTurningMotionBlur(void) {
    int i;
    int j;
    int fromTgi;
    int fromConst;
    int* tgip;
    int* constp;

    constp = &turningMotionBlurTargetLods[0];
    tgip = &tgi->lodDistance[0];

    for (i = 0; i < 7; i++) {
        fromTgi = *tgip++;
        fromConst = *constp++;
        for (j = 0; j < 9; j++) {
            turningMotionBlurTable[j * 7 + i] = fromTgi + ((fromConst - fromTgi) * LOD_THRESHOLDS[j]) / 100;
        }
    }
}

#if VER_JP
void ParseLevelDataFromTgi(int isCredits) {
#else
void ParseLevelDataFromTgi(void) {
    const int isCredits = 0;
#endif
    int i;
    int j;
    int* ptr;
    int* ptr2;

    levelData = LEVEL_DATA_BUF;
    entityData = ENTITY_DATA_BUF;
    numEntities = *(short*)ENTITY_COUNT_ADDR;

    InitLasers();
    ParseKeysAndSpecialLevelFromItemData();
    ResetVibration();
    DisableScreenShake();

    for (i = 0; i < 39304; i++) {
        ((short*)CUBE_GRID_BUF)[i] = -1;
    }

    ptr = cubeStates;
    for (i = 0; i < 4096; i++) {
        *ptr++ = -1;
    }

    if (specialLevelType == 1) {
        numPlainTileTextureVariations = tgi->numPlainTileTextureVariationsSpecialLevel;
        numObjTextures = tgi->numObjTexturesSpecialLevel;
        quadSomethingStartIndex = tgi->numFixedTextures + tgi->numPlainTileTextureVariationsNormalLevel + tgi->numObjTexturesNormalLevel;
    } else {
        numPlainTileTextureVariations = tgi->numPlainTileTextureVariationsNormalLevel;
        numObjTextures = tgi->numObjTexturesNormalLevel;
        quadSomethingStartIndex = 0;
    }

    quadSomethingCount = tgi->numFixedTextures + numPlainTileTextureVariations + numObjTextures;

    ProcessLevelData();
    InitManySprites();
    ResetCheats();

    for (i = 0; i < 32; i++) {
        recentlyVisitedCubeFaces[4 * i] = (void*)-1;
    }

    itemsDispListIdx = 0;
    playerEnemyDispListIdx = 0;

    for (i = 0; i < 1; i++) {
        sunglassSeeEverything[i] = 0;
    }

    for (i = 0; i < 1; i++) {
        sunglassDisablingState[i] = 0;
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 1; j++) {
            unkOtagLengths[i][j] = 32;
        }
    }

    for (i = 0; i < 32; i++) {
        lightEffects[8 * i] = 0xffffffff;
    }

    InitShadowSprites();
#if VER_EU
    InitPlayerSpecularSprite();
#endif
    ScanLevelDataForBlinkingEntities();

    for (i = 0; i < 64; i++) {
        toBeDisabledLightEffects[i] = -1;
    }

    turningMotionBlurEnabled = 0;
    for (i = 0; i < 7; i++) {
        tgi->lodDistance[i] = turningMotionBlurTable[0 * 7 + i];
    }

#if VER_JP
    isInCredits = 0;
#endif

    if (tgi->skyboxFlag == 1025) {
        if (specialLevelType != wasSpecialLevel || recalcSkyboxes == 1) {
            switch (specialLevelType) {
                case 0:
                    RecalcSkyboxes01(3, 5, 16, 5, 40);
                    break;
                case 1:
                    RecalcSkyboxes01(4, 3, 16, VELOCITY(10), 36);
                    break;
                case 2:
                    RecalcSkyboxes2();
                    break;
            }
            recalcSkyboxes = 0;
        }
    }

    if (isFinal == 1) {
        if (!isCredits) {
            TweakSkyboxColors(0x787878);
        }
    }

    if (tgi->skyboxFlag == 1025) {
        if (specialLevelType == 2) {
            SetupDisplay(1, 0, 0, 0, 0, 0);
        } else {
            SetupDisplay(0, 0, 0, 0, 0, 0);
        }
    } else {
        if (specialLevelType == 1) {
            SetupDisplay(1, tgi->specialLevelBgColor[0], tgi->specialLevelBgColor[1], tgi->specialLevelBgColor[2], 0, 0);
        } else {
            SetupDisplay(1, tgi->normalLevelBgColor[0], tgi->normalLevelBgColor[1], tgi->normalLevelBgColor[2], 0, 0);
        }
    }
}

static int RecalcSkyboxes01_angleMul1;
static int RecalcSkyboxes01_angleMul2;
static int RecalcSkyboxes01_countOfSomethingJ;
static int RecalcSkyboxes01_countOfSomethingI;
static int RecalcSkyboxes01_param5;
static int RecalcSkyboxes01_anglesProduct;
static int RecalcSkyboxes01_param5Times2;
static int RecalcSkyboxes01_maxAngle1Copy;
static int RecalcSkyboxes01_maxAngle2Plus1;
static int RecalcSkyboxes01_countsProduct;
static int RecalcSkyboxes01_sizeMidBlk;
static int RecalcSkyboxes01_maxAnglesProduct;
static int RecalcSkyboxes01_maxAnglesProduct2;
static int RecalcSkyboxes01_sizePolyMap;
static int RecalcSkyboxes01_sizeVertices;
static int RecalcSkyboxes01_sizePolys;
#define angleMul1 RecalcSkyboxes01_angleMul1
#define angleMul2 RecalcSkyboxes01_angleMul2
#define countOfSomethingJ RecalcSkyboxes01_countOfSomethingJ
#define countOfSomethingI RecalcSkyboxes01_countOfSomethingI
#define param5 RecalcSkyboxes01_param5
#define anglesProduct RecalcSkyboxes01_anglesProduct
#define param5Times2 RecalcSkyboxes01_param5Times2
#define maxAngle1Copy RecalcSkyboxes01_maxAngle1Copy
#define maxAngle2Plus1 RecalcSkyboxes01_maxAngle2Plus1
#define countsProduct RecalcSkyboxes01_countsProduct
#define sizeMidBlk RecalcSkyboxes01_sizeMidBlk
#define maxAnglesProduct RecalcSkyboxes01_maxAnglesProduct
#define maxAnglesProduct2 RecalcSkyboxes01_maxAnglesProduct2
#define sizePolyMap RecalcSkyboxes01_sizePolyMap
#define sizeVertices RecalcSkyboxes01_sizeVertices
#define sizePolys RecalcSkyboxes01_sizePolys
void RecalcSkyboxes01(int angleOfSomethingI, int angleOfSomethingJ, int countI, int countJ, int param_5) {
    short* ptr;
    int i;
    int j;
    int maxDist;
    int l;
    int s;
    int u;
    int v;
    int w;
    int k;
    short* texcoords;

    angleMul1 = angleOfSomethingI;
    angleMul2 = angleOfSomethingJ;
    countOfSomethingJ = countI;
    countOfSomethingI = countJ;
    param5 = param_5;
    anglesProduct = angleOfSomethingI * angleOfSomethingJ;
    param5Times2 = param_5 * 2;
    skyboxMaxAngle1 = countI * angleOfSomethingI;
    maxAngle1Copy = skyboxMaxAngle1;
    skyboxMaxAngle2 = countJ * angleOfSomethingJ;
    maxAngle2Plus1 = skyboxMaxAngle2 + 1;
    countsProduct = countI * countJ;
    sizeMidBlk = (countsProduct + 2) * 8;
    maxAnglesProduct = skyboxMaxAngle1 * skyboxMaxAngle2;
    maxAnglesProduct2 = skyboxMaxAngle1 * maxAngle2Plus1;
    sizePolyMap = maxAnglesProduct2;
    sizeVertices = (maxAnglesProduct * 20) / 3;
    if ((maxAnglesProduct2 & 0x1f) > 0) {
        sizePolyMap = (maxAnglesProduct2 + 32) & -32;
    }
    sizePolys = maxAnglesProduct2 * param5Times2;

    ptr = (short*)SKYBOX_BUF;

    for (i = 0; i < countOfSomethingI; i++) {
        for (j = 0; j < countOfSomethingJ; j++) {
            SkyboxSinCos(j * angleMul1 - 1, i * angleMul2 - 1, &skyboxVertices[0][0], &skyboxVertices[0][1], &skyboxVertices[0][2]);
            SkyboxSinCos((j + 1) * angleMul1, i * angleMul2 - 1, &skyboxVertices[1][0], &skyboxVertices[1][1], &skyboxVertices[1][2]);
            SkyboxSinCos(j * angleMul1 - 1, (i + 1) * angleMul2, &skyboxVertices[2][0], &skyboxVertices[2][1], &skyboxVertices[2][2]);
            SkyboxSinCos((j + 1) * angleMul1, (i + 1) * angleMul2, &skyboxVertices[3][0], &skyboxVertices[3][1], &skyboxVertices[3][2]);

            // this overruns the array
            for (k = 0; k < 3; k++) {
                *ptr++ = skyboxVertices[4][k] = (((skyboxVertices[0][k] + skyboxVertices[1][k]) + skyboxVertices[2][k]) + skyboxVertices[3][k]) / 4;
            }

            maxDist = 0;
            for (k = 0; k < 4; k++) {
                s = SquareRoot0(Square(skyboxVertices[4][0] - skyboxVertices[k][0]) +
                                Square(skyboxVertices[4][1] - skyboxVertices[k][1]) +
                                Square(skyboxVertices[4][2] - skyboxVertices[k][2]));
                if (maxDist < s)
                    maxDist = s;
            }
            *ptr++ = maxDist;
        }
    }

    *(int*)&ptr[0] = 0;
    *(int*)&ptr[2] = 0;
    *(int*)&ptr[4] = -1;
    *(int*)&ptr[6] = -1;
    ptr += 8;

    if (param_5 == 36) {
        for (i = 0; i < countOfSomethingI; i++) {
            for (j = 0; j < countOfSomethingJ; j++) {
                for (k = 0; k < angleMul1; k++) {
                    for (l = 0; l < (angleMul2 / 3); l++) {
                        SkyboxSinCos(j * angleMul1 + k, i * angleMul2 + l * 3, &ptr[0], &ptr[1], &ptr[6]);
                        SkyboxSinCos(j * angleMul1 + k, i * angleMul2 + l * 3 + 1, &ptr[2], &ptr[3], &ptr[7]);
                        SkyboxSinCos(j * angleMul1 + k, i * angleMul2 + l * 3 + 2, &ptr[4], &ptr[5], &ptr[8]);
                        ptr += 10;
                    }
                }
            }
        }
    } else {
        for (i = 0; i < countOfSomethingI; i++) {
            for (j = 0; j < countOfSomethingJ; j++) {
                for (k = 0; k < angleMul2; k++) {
                    for (l = 0; l < (angleMul1 / 3); l++) {
                        SkyboxSinCos(j * angleMul1 + l * 3, i * angleMul2 + k, &ptr[0], &ptr[1], &ptr[6]);
                        SkyboxSinCos(j * angleMul1 + l * 3 + 1, i * angleMul2 + k, &ptr[2], &ptr[3], &ptr[7]);
                        SkyboxSinCos(j * angleMul1 + l * 3 + 2, i * angleMul2 + k, &ptr[4], &ptr[5], &ptr[8]);
                        ptr += 10;
                    }
                }
            }
        }
    }

    ptr += sizePolyMap / 2;
    if (param5 == 36) {
        POLY_G4* pu = (unsigned char*)ptr;
        for (i = 0; i < maxAnglesProduct2 * 2; pu++, i++) {
            setPolyG4(pu);
        }
    } else {
        texcoords = tgiPart9;
        ptr += (maxAngle1Copy * param5Times2) / 2;
        for (i = 0; i < 24; i++) {
            for (j = 0; j < 48; j++) {
                setPolyFT4((POLY_FT4*)ptr);
                SetShadeTex((POLY_FT4*)ptr, 0);
                SetSemiTrans((POLY_FT4*)ptr, 0);
                setRGB0((POLY_FT4*)ptr, 128, 128, 128);
                ((POLY_FT4*)(ptr))->clut = GetClut(texcoords[0], texcoords[1]);
                ((POLY_FT4*)(ptr))->tpage = GetTPage(1, 0, texcoords[2], texcoords[3]);

                u = texcoords[4];
                v = texcoords[5];
                w = texcoords[6];
                *(u_short*)&((POLY_FT4*)(ptr))->u0 = (v << 8) | u;
                *(u_short*)&((POLY_FT4*)(ptr))->u1 = (v << 8) | (u + w);
                *(u_short*)&((POLY_FT4*)(ptr))->u2 = ((v + 15) << 8) | u;
                *(u_short*)&((POLY_FT4*)(ptr))->u3 = ((v + 15) << 8) | (u + w);
                ptr += 20;
                texcoords += 7;

                for (k = 0; k < 1; k++) {
                    memcpy(ptr, ptr - 20, 40);
                    ptr += 20;
                }
            }
        }
    }
}

#undef angleMul1
#undef angleMul2
#undef countOfSomethingJ
#undef countOfSomethingI
#undef param5
#undef anglesProduct
#undef param5Times2
#undef maxAngle1Copy
#undef maxAngle2Plus1
#undef countsProduct
#undef sizeMidBlk
#undef maxAnglesProduct
#undef maxAnglesProduct2
#undef sizePolyMap
#undef sizeVertices
#undef sizePolys
int RecalcSkyboxes2(void) {
    short* dst;
    short* ptr;
    short* ptr2;
    int x;
    int i;
    int y;
    int z;
    int j;
    int k;
    int l;
    int *u1, *u2;
    u32* p_u32;
    int clut;
#if VER_EU
    int dummy[16];
#endif

    starfieldSinPhase1 = 0x200;
    starfieldSinPhase2 = 0;
    starfieldSinPhase3 = 0x400;
    starfieldSinVec.vx = 0;
    starfieldSinVec.vy = 0;
    starfieldSinVec.vz = 0;

    starfieldTransPhaseX1 = 0x200;
    starfieldTransPhaseX2 = 0x400;
    starfieldTransPhaseY1 = 0;
    starfieldTransPhaseY2 = 0x400;
    starfieldTransPhaseZ1 = 0;
    starfieldTransPhaseZ2 = 0;
    starfieldTrans.vx = 0;
    starfieldTrans.vy = 0;
    starfieldTrans.vz = 0;

    ptr = (short*)SKYBOX_BUF;
    for (z = 0; z < 0x3332; z += 0x3332) {
        for (y = 0; y < 0x3332; y += 0x3332) {
            for (x = 0; x < 0x3332; x += 0x3332) {
                ptr[0] = x + 0x1999;
                ptr[1] = y + 0x1999;
                ptr[2] = z + 0x1999;
                ptr[3] = 0;
                ptr += 4;
                for (j = 0; j < 16; j++) {
                    ptr[0] = x + Rand(0x3332);
                    ptr[1] = y + Rand(0x3332);
                    ptr[2] = x + Rand(0x3332);
                    ptr[3] = y + Rand(0x3332);
                    ptr[4] = x + Rand(0x3332);
                    ptr[5] = y + Rand(0x3332);
                    ptr[6] = z + Rand(0x3332);
                    ptr[7] = z + Rand(0x3332);
                    ptr[8] = z + Rand(0x3332);
                    ptr[9] = 0;
                    ptr += 10;
                }
            }
        }
    }

    *(int*)((char*)SKYBOX_BUF + 0xa8e8) = SquareRoot0(0x7adb853);

    dst = (short*)((char*)SKYBOX_BUF + 0xa8ec);
    for (z = -0x7FFD; z < 0x7FFD; z += 0x3332) {
        for (y = -0x7FFD; y < 0x7FFD; y += 0x3332) {
            for (x = -0x7FFD; x < 0x7FFD; x += 0x3332) {
                ptr = (short*)SKYBOX_BUF;
                for (i = 0; i < 1; i++) {
                    dst[0] = ptr[0] + x;
                    dst[1] = ptr[1] + y;
                    dst[2] = ptr[2] + z;
                    dst[3] = 0;
                    ptr += 4;
                    dst += 4;
                    for (j = 0; j < 16; j++) {
                        dst[0] = ptr[0] + x;
                        dst[1] = ptr[1] + y;
                        dst[2] = ptr[2] + x;
                        dst[3] = ptr[3] + y;
                        dst[4] = ptr[4] + x;
                        dst[5] = ptr[5] + y;
                        dst[6] = ptr[6] + z;
                        dst[7] = ptr[7] + z;
                        dst[8] = ptr[8] + z;
                        dst[9] = 0;
                        ptr += 10;
                        dst += 10;
                    }
                    for (j = 0; j < 9; j++) {
                        dst[j] = 0x666;
                    }
                    do {
                        dst[9] = -1;
                        dst += 10;
                    } while (0);
                }
            }
        }
    }

    dst[0] = 0;
    dst[1] = 0;
    dst[2] = 0;
    dst[3] = -1;
    dst += 4;

    u1 = (int*)((char*)SKYBOX_BUF + 0xaa3c);
    p_u32 = (int*)((char*)SKYBOX_BUF + 0xaa48);
    for (i = 0; i < 125; i++) {
        u1[-2] = p_u32[0];
        u1[-1] = u1[4];
        u1 += 87;
        p_u32 += 87;
    }

    p_u32 = (u32*)SKYBOX_BUF;
    clut = textures[firstUnk2Texture].clut;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 1; j++) {
            for (k = 0; k < 1081; k++) {
                if (k == 0) {
                    SetDrawTPage((DR_TPAGE*)p_u32, 0, 0, textures[firstUnk2Texture].tpage);
                    p_u32[0] = ((u32)(p_u32 + 5) & 0xFFFFFF) | 0x01000000;
                } else {
                    p_u32[0] = ((u32)(p_u32 + 5) & 0xFFFFFF) | 0x04000000;
                    if (textures[firstUnk2Texture].semitrans == 0) {
                        p_u32[1] = 0x64808080;
                    } else {
                        p_u32[1] = 0x66808080;
                    }
                    p_u32[3] = (u32)clut << 16;
                    p_u32[4] = 0x00030004;
                }
                p_u32 += 5;
            }
        }
    }

    return (u32)dst - (u32)SKYBOX_BUF;
}

int Square(int param_1) {
    return param_1 * param_1;
}

void SkyboxSinCos(int azimuth, int elevation, short* outX, short* outY, short* outZ) {
    int elevationAngle;
    int xzRadius;
    int sinAzimuth;
    int cosAzimuth;
    int x;
    int y;

    if (azimuth < 0) {
        azimuth = azimuth + skyboxMaxAngle1;
    }
    if (skyboxMaxAngle1 <= azimuth) {
        azimuth = azimuth - skyboxMaxAngle1;
    }
    if (elevation < 0) {
        elevation = 0;
    }
    elevationAngle = elevation << 11;
    if (skyboxMaxAngle2 <= elevation) {
        elevation = skyboxMaxAngle2 - 1;
        elevationAngle = elevation << 11;
    }
    xzRadius = rcos(elevationAngle / (skyboxMaxAngle2 - 1) + 0x400);
    cosAzimuth = rcos((azimuth << 12) / skyboxMaxAngle1);
    cosAzimuth = cosAzimuth * xzRadius;
    if (cosAzimuth < 0) {
        cosAzimuth = cosAzimuth + 0xfff;
    }
    x = cosAzimuth >> 12;
    y = rsin(elevationAngle / (skyboxMaxAngle2 - 1) + 0x400);
    sinAzimuth = rsin((azimuth << 12) / skyboxMaxAngle1);
    sinAzimuth = sinAzimuth * xzRadius;
    if (sinAzimuth < 0) {
        sinAzimuth = sinAzimuth + 0xfff;
    }

    *outX = x;
    *outY = y;
    *outZ = sinAzimuth >> 12;
}
