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

typedef struct P {
    ulong* p0;
    ulong* p1;
} P;

typedef struct FaceData {
    void** pointerInsideCubeState;
    short texIdx;
    short dir;
    short x, y, z;
    short textureRotation;
    int flags;
    int color;
} FaceData;

typedef void (*QuadFunc)(Quad* quad, int width, int x, int y, int z, int textureRotation);

typedef struct {
    short entityType;
    short pad2;
    short counter;
    char pad[250];
} CrumblingBlockEntity;

typedef struct FlashingEntity {
    short entityType;
    short cubeType;
    short initState;
    short state;
    short counter;
    short pad[120];
    short x;
    short y;
    short z;
} FlashingEntity;

extern int FlipDir(int dir);
extern int GetFaceTypeAtRelativeToDir(int x, int y, int z, int dirIndex);
extern int GetFaceTypeOfBlockType(int blockType);
extern int Square(int val);
extern uint Rand(int param_1);
extern void DirFunc0(Quad* quad, int width, int x, int y, int z, int textureRotation);
extern void DirFunc1(Quad* quad, int width, int x, int y, int z, int textureRotation);
extern void DirFunc2(Quad* quad, int width, int x, int y, int z, int textureRotation);
extern void DirFunc3(Quad* quad, int width, int x, int y, int z, int textureRotation);
extern void DirFunc4(Quad* quad, int width, int x, int y, int z, int textureRotation);
extern void DirFunc5(Quad* quad, int width, int x, int y, int z, int textureRotation);
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
extern void ProcessMovingPlatforms2(void);
extern void ResetCheats(void);
extern void ResetVibration(void);
extern void ScanLevelDataForBlinkingEntities(void);
extern void ScanLevelDataForRetractableSpikes(void);
extern void SetSemiTrans(void* p, int enable);
extern void SetShadeTex(void* p, int disable);
extern void SetupDisplay(int isbg, int bgR, int bgG, int bgB, int useDithering, int use24Bit);
extern void SkyboxSinCos(int azimuth, int elevation, short* outX, short* outY, short* outZ);
extern void TweakSkyboxColors(uint param_1);

int CoordHash(int x, int y, int z, int dir, int div, int mod);
int GetRandomTextureRotation(void);
int RecalcSkyboxes2(void);
void AddQuadToAnimatedTextureChain(AnimatedTextureChain* chain, Quad** quadPtr, int initAnimFrame1, int initAnimFrame2);
void InitLasers2(void);
void ProcessCubesIntoFaces(void);
void RecalcSkyboxes01(int angleOfSomethingI, int angleOfSomethingJ, int countOfSomethingI, int countOfSomethingJ, int param_5);
void ResolveAnimatedTextureChain(AnimatedTextureChain* textureChain);
void ScanLevelDataForMovingBlocks1(void);
void SetFaceData(int index, void** pointerInsideCubeState, int texIdx, int flags, int dir, int xFine, int yFine, int zFine, int textureRotation, int color);

extern byte BONUS_BLOCK_ANIM_FLAG_DATA0[60];
extern byte BONUS_BLOCK_ANIM_FLAG_DATA0_END[1];
extern byte BONUS_BLOCK_ANIM_FLAG_DATA1[56];
extern byte FIRE_BLOCK_ANIM_FLAG_DATA[52];
extern byte FIRE_BLOCK_ANIM_FLAG_DATA_END[];
extern byte INVIS_BLOCK_ANIM_FLAG_DATA[124];
extern byte INVIS_BLOCK_ANIM_FLAG_DATA_END[];
extern byte LASER_INTENSITY_DATA[1]; // fake size
extern byte LASER_INTENSITY_DATA_END[1]; // fake size
extern int bonusBlockTextureChain[];
extern int crumblingSpecialBlockTextureChain[];
extern int cubeStates[16 * 256];
extern int curWorld;
extern int fireBlockTextureChain[];
extern int firstUnk2Texture;
extern int gameMode;
extern int invisBlockVisibility[6];
extern int invisibleBlockTextureChain[];
extern int isFinal;
extern int isInCredits;
extern int laserDrawBuffer[4096];
extern int lightEffects[32 * 8];
extern int numKeysInLevel;
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
extern POLY_FT4 shadowPrims[2][1][2][16];
extern POLY_FT4 specularPrims[2][1][NUM_SPECULAR_PRIMS];
extern P shadowPrimPtrs[2][1][2][16];
extern P specularPrimPtrs[2][1][NUM_SPECULAR_PRIMS];
extern short flashingBlockEntityIndexes[64];
extern SVECTOR starfieldSinVec;
extern Texture textures[NUM_TEXTURES];
extern uint firstGuiTexture;
extern uint INVIS_BLOCK_ANIM_COLOR_DATA[1]; // fake size
extern uint INVIS_BLOCK_ANIM_COLOR_DATA_END[];
extern uint laserData0[1]; // fake size
extern uint laserData1[1]; // fake size
extern uint laserData2[1]; // fake size
extern uint laserData3[1]; // fake size
extern uint NUM_TEXTURE_ANIM_FRAMES;
extern uint TEXTURE_ANIM_DATA[1]; // fake size
extern VECTOR starfieldTrans;


CubeTextureMetadata cubeTextureMetadata[1300];
DR_TPAGE drTpages1[2][1];
DR_TPAGE drTpages2[2][1];
DR_TPAGE hudDrTpages[2];
int itemsDispListIdx;
int numCrumblingBlocks;
int numCubesInLevelTmp;
int numCubesRemainingInLevel[5];
int numFlashingBlocks;
int numMovingPlatforms;
int numRetractableSpikes;
int numTotalQuads;
int playerEnemyDispListIdx;
int retractingSpikeData[64 * 2];
int specialLevelType;
int turningMotionBlurEnabled;
int turningMotionBlurPhase;
int turningMotionBlurTable[9 * 7];
short crumblingBlockEntityIndexes[64];
short* entityData;
short* levelData;
short movingBlockEntityIndexes[16];
short numEntities;
short* tgiPart1;
short* tgiPart2;
short* tgiPart3;
short* tgiPart4;
short* tgiPart7;
short* tgiPart8;
TgiFile* tgi;
void** levelExitQuadPPtr;

STATIC_FOR_GP_ACCESS int numPlainTileTextureVariations;
STATIC_FOR_GP_ACCESS int quadSomethingCount;
STATIC_FOR_GP_ACCESS int quadSomethingStartIndex;
STATIC_FOR_GP_ACCESS int recalcSkyboxes;
STATIC_FOR_GP_ACCESS int wasSpecialLevel;
STATIC_FOR_GP_ACCESS short skyboxVertices[3][3];
STATIC_FOR_GP_ACCESS short* tgiPart5;
STATIC_FOR_GP_ACCESS short* tgiPart9;
static int cubeCounter;
static int numberOfCubeFaces;
static int numObjTextures;
static int skyboxMaxAngle1;
static int skyboxMaxAngle2;
static short* tgiEnd;
static short* tgiPart0;
static short* tgiPart10;
static short* tgiPart6;

short MOVING_BLOCK_TEXTURE_DATA[3][4][12] = {
    {
        { 3, 1, 0, 0, 3, 1, 3, 1, 0, 0, 3, 1 },
        { 1, 3, -1, 666, 1, 3, 1, 3, 0, 0, 1, 1 },
        { -2, 1, -1, 666, -2, 1, -2, 1, -1, 666, -2, 1 },
        { 1, 1, 0, 0, 1, 1, 1, 1, -1, 666, 1, 3 },
    },
    {
        { 3, 0, 3, 0, 0, 0, 0, 0, 3, 0, 3, 0 },
        { 1, 0, 1, 0, -1, 666, 0, 0, 1, 0, 1, 0 },
        { -2, 0, -2, 0, -1, 666, -1, 666, -2, 0, -2, 0 },
        { 1, 2, 1, 2, 0, 0, -1, 666, 1, 2, 1, 2 },
    },
    {
        { 0, 0, 3, 1, 3, 0, 3, 0, 3, 1, 0, 0 },
        { 0, 0, 1, 3, 1, 0, 1, 2, 1, 1, -1, 666 },
        { -1, 666, -2, 1, -2, 0, -2, 0, -2, 1, -1, 666 },
        { -1, 666, 1, 1, 1, 2, 1, 0, 1, 3, 0, 0 },
    },
};
int dummyLevelExitQuad = 0x666;
void* dummyLevelExitQuadPtr = &dummyLevelExitQuad;
FaceData* faceDataPtr = (FaceData*)FACE_DATA_BUF;
int primBufUsage = 0;
int maxPrimBufUsage = 0;
QuadFunc QUAD_FUNC_PTRS[6] = {
    DirFunc0,
    DirFunc1,
    DirFunc2,
    DirFunc3,
    DirFunc4,
    DirFunc5,
};
int turningMotionBlurTargetLods[7] = { 28, 23, 22, 15, 40, 30, 20 };
int LOD_THRESHOLDS[9] = { 0, 30, 43, 55, 66, 76, 85, 93, 100 }; // percentage
int HIGHSCORE_CUBE_RANDOM_TEXTURES[16] = { 666, 666, 666, 666, 666, 666, 4, 4, 4, 4, 668, 668, 668, 6, 6, 0 };

#define IE_ID 1
#define IE_VARIANT 3
#define IE_STATE 4
#define IE_UNK8 8
#define IE_COUNTER 15
#define IE_UNK16 16
#define QUAD_BASE ((Quad*)QUAD_BUF)

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

void ProcessLevelData(void) {
    int i;

    numberOfCubeFaces = 0;
    numCubesInLevelTmp = 0;
    cubeCounter = 0;

    InitAnimatedTextureChain(fireBlockTextureChain, 0x80,
            FIRE_BLOCK_ANIM_FLAG_DATA_END - FIRE_BLOCK_ANIM_FLAG_DATA, 0,
            FIRE_BLOCK_ANIM_FLAG_DATA, NULL,
            NULL, NULL, NULL, NULL);
    InitAnimatedTextureChain(invisibleBlockTextureChain, 0x200,
            INVIS_BLOCK_ANIM_FLAG_DATA_END - INVIS_BLOCK_ANIM_FLAG_DATA, INVIS_BLOCK_ANIM_COLOR_DATA_END - INVIS_BLOCK_ANIM_COLOR_DATA,
            INVIS_BLOCK_ANIM_FLAG_DATA, NULL,
            INVIS_BLOCK_ANIM_COLOR_DATA, NULL, NULL, NULL);

    if (specialLevelType == 1) {
        uint* animData = TEXTURE_ANIM_DATA;
        InitAnimatedTextureChain(crumblingSpecialBlockTextureChain, 0x300,
                0, NUM_TEXTURE_ANIM_FRAMES,
                NULL, NULL,
                animData + ((curWorld % 2) * 2) * NUM_TEXTURE_ANIM_FRAMES, animData + ((curWorld % 2) * 2 + 1) * NUM_TEXTURE_ANIM_FRAMES, NULL, NULL);
        InitAnimatedTextureChain(bonusBlockTextureChain, 0x300,
                BONUS_BLOCK_ANIM_FLAG_DATA0_END - BONUS_BLOCK_ANIM_FLAG_DATA0, NUM_TEXTURE_ANIM_FRAMES,
                BONUS_BLOCK_ANIM_FLAG_DATA0, BONUS_BLOCK_ANIM_FLAG_DATA1,
                animData + ((curWorld % 2) * 2) * NUM_TEXTURE_ANIM_FRAMES, animData + ((curWorld % 2) * 2 + 1) * NUM_TEXTURE_ANIM_FRAMES, NULL, NULL);
    }

    ProcessCubesIntoFaces();
    ScanLevelDataForMovingBlocks1();
    InitLasers2();
    CopyQuadData();
    ScanLevelDataForCrumblingBlocks();
    ScanLevelDataForFlashingBlocks();
    ProcessMovingPlatforms2();

    ResolveAnimatedTextureChain(fireBlockTextureChain);
    ResolveAnimatedTextureChain(invisibleBlockTextureChain);
    if (specialLevelType == 1) {
        ResolveAnimatedTextureChain(crumblingSpecialBlockTextureChain);
        ResolveAnimatedTextureChain(bonusBlockTextureChain);
    }

    ScanLevelDataForRetractableSpikes();

    numCubesRemainingInLevel[0] = numCubesInLevelTmp;
    for (i = 1; i < 4; i++) {
        numCubesRemainingInLevel[i] = 0;
    }
}

void ProcessCubesIntoFaces(void) {
    // FIXME: eliminate as many local variables as possible
    int x;
    int y;
    int z;
    int xFine;
    int yFine;
    int zFine;
    int dir;

    int textureIdx;
    int itemType;
    int style;
    int newCube;
    int blockType;
    int type;
    int relType;
    int isNonempty;
    int flags;
    int textureRotation;

    int csptr;

    levelExitQuadPPtr = &dummyLevelExitQuadPtr;
    for (z = 1; z < 33; z++) {
        for (y = 1; y < 33; y++) {
            for (x = 1; x < 33; x++) {
                blockType = levelData[x * 34 * 34 + y * 34 + z];
                style = GetFaceTypeOfBlockType(blockType);
                if (style == 0 || style == 1 || style == 2 || style == 3 || style == 4 || style == 6 || style == 7) {
                    dir = newCube = isNonempty = 0;
                    csptr = (int)cubeStates;
                    xFine = x << 9;
                    yFine = y << 9;
                    zFine = z << 9;
                    for (; dir < 6; dir++, csptr += 4) {
                        if (style == 0 && blockType >= 5) {
                            type = entityData[((blockType - 5) * 128 + 1) + dir * 16];
                            if (type == OBJ_PLAYER_SPAWN_PAUSED) {
                                type = OBJ_TIMER_PAUSE;
                            }
                            if (type >= OBJ_SLOW_STAR) {
                                // enemy
                                type = 0;
                            }
                        } else {
                            type = style;
                            if (style >= 5) {
                                type = -style;
                            }
                        }
                        relType = GetFaceTypeAtRelativeToDir(x, y, z, dir);
                        switch (type) {
                            case OBJ_INVISIBLE_PATCH:
                                if (!((relType >= 0 && relType <= 1) || (relType >= 2 && relType <= 3) || relType == 4)) {
                                    // 6 == invisible
                                    SetFaceData(numberOfCubeFaces++, (void**)(cubeCounter * 64 + csptr), quadSomethingStartIndex + 6, 0x100,
                                                dir, xFine, yFine, zFine, 0, 0x808080);
                                    AddQuadToAnimatedTextureChain(invisibleBlockTextureChain, (Quad**)(cubeCounter * 64 + csptr), -1,
                                                                  CoordHash(xFine, yFine, zFine, dir, 0x40,
                                                                            INVIS_BLOCK_ANIM_COLOR_DATA_END - INVIS_BLOCK_ANIM_COLOR_DATA));
                                    isNonempty = 1;
                                    ((char*)&cubeStates[cubeCounter * 16 + 12])[dir] = 0;
                                }
                                break;
                            case -OBJ_CRUMBLING_BLOCK:
                                flags = 0xc; // QF_BACKFACE_CULL | QF_GOURAUD
                                if (!((relType >= 0 && relType <= 1) || relType == 2 || relType == 4 || relType == 6)) {
                                    flags = 0xd; // QF_ACTIVE | QF_BACKFACE_CULL | QF_GOURAUD
                                }
                                if (specialLevelType == 1) {
                                    AddQuadToAnimatedTextureChain(crumblingSpecialBlockTextureChain, (Quad**)(cubeCounter * 64 + csptr), -1,
                                                                  CoordHash(xFine, yFine, zFine, dir, 0x40, NUM_TEXTURE_ANIM_FRAMES));
                                    flags &= ~8; // QF_BACKFACE_CULL
                                }
                                if (specialLevelType == 2) {
                                    flags |= 2; // QF_SEMITRANSPARENT
                                }
                                // 4 == crumbling
                                SetFaceData(numberOfCubeFaces++, (void**)(cubeCounter * 64 + csptr), quadSomethingStartIndex + 4, flags, dir,
                                            xFine, yFine, zFine, 0, 0x808080);
                                isNonempty = 1;
                                newCube = 1;
                                ((char*)&cubeStates[cubeCounter * 16 + 12])[dir] = 0;
                                break;

                            case -OBJ_FLASHING_BLOCK:
                                flags = 0xd; // QF_ACTIVE | QF_BACKFACE_CULL | QF_GOURAUD
                                textureIdx = quadSomethingStartIndex + tgi->numFixedTextures;
                                textureRotation = GetRandomTextureRotation();
                                if (specialLevelType == 1) {
                                    AddQuadToAnimatedTextureChain(bonusBlockTextureChain, (Quad**)(cubeCounter * 64 + csptr), -1,
                                                                  CoordHash(xFine, yFine, zFine, dir, 0x40, NUM_TEXTURE_ANIM_FRAMES));
                                    flags = 5; // QF_ACTIVE | QF_BACKFACE_CULL
                                }
                                SetFaceData(numberOfCubeFaces++, (void**)(cubeCounter * 64 + csptr), textureIdx, flags, dir, xFine, yFine,
                                            zFine, textureRotation, 0xa0a0a0);
                                isNonempty = 1;
                                newCube = 1;
                                ((char*)&cubeStates[cubeCounter * 16 + 12])[dir] = 0;
                                break;

                            default:
                                if (!((relType >= 0 && relType < 2) || relType == 2 || relType == 4)) {
                                    flags = 0xd; // QF_ACTIVE | QF_BACKFACE_CULL | QF_GOURAUD
                                    textureRotation = 0;
                                    textureIdx = quadSomethingStartIndex + tgi->numFixedTextures;
                                    if (type == 0) {
                                        textureIdx += Rand(numPlainTileTextureVariations);
                                        textureRotation = GetRandomTextureRotation();
                                    } else {
                                        textureIdx += numPlainTileTextureVariations + type;
                                        if (type > 19) {
                                            // item or enemy
                                            flags = 0x100000d; // QF_ACTIVE | QF_BACKFACE_CULL | QF_GOURAUD | QF_ITEM_SHADOW
                                        }
                                    }
                                    if (specialLevelType == 1) {
                                        flags &= ~8; // QF_GOURAUD
                                        if (type == 0 || type > 19) {
                                            textureRotation = Rand(4);
                                            AddQuadToAnimatedTextureChain(bonusBlockTextureChain, (Quad**)(cubeCounter * 64 + csptr), -1,
                                                                          CoordHash(xFine, yFine, zFine, dir, 0x40, NUM_TEXTURE_ANIM_FRAMES));
                                            flags &= 0xffffff;
                                            if (type > 19) {
                                                flags |= 0x10000; // QF_BONUS_ITEM_SHADOW
                                            }
                                        } else {
                                            AddQuadToAnimatedTextureChain(crumblingSpecialBlockTextureChain,
                                                                          (Quad**)(cubeCounter * 64 + csptr), -1,
                                                                          CoordHash(xFine, yFine, zFine, dir, 0x40, NUM_TEXTURE_ANIM_FRAMES));
                                        }
                                    }
                                    if (relType == OBJ_CRUMBLING_BLOCK) {
                                        flags &= ~1; // QF_ACTIVE
                                    }
                                    if (specialLevelType == 2) {
                                        flags |= 2; // QF_SEMITRANSPARENT
                                    }
                                    if (style == 2 && blockType >= 5) {
                                        itemType = entityData[((blockType - 5) * 128 + 1) + dir * 16];
                                        if (itemType >= 20 && itemType < 50 && itemType != 26 && itemType != 28 && itemType != 30) {
                                            flags |= 0x1000000; // QF_ITEM_SHADOW
                                        }
                                    }

                                    if (type == OBJ_EXIT) {
                                        levelExitQuadPPtr = (void**)(cubeCounter * 64 + csptr);
                                    }
                                    SetFaceData(numberOfCubeFaces++, (void**)(cubeCounter * 64 + csptr), textureIdx, flags, dir, xFine, yFine,
                                                zFine, textureRotation, 0x808080);
                                    isNonempty = 1;
                                    newCube = 1;
                                    ((char*)&cubeStates[cubeCounter * 16 + 12])[dir] = 0;
                                    if (type == OBJ_FIRE_PATCH) {
                                        AddQuadToAnimatedTextureChain(fireBlockTextureChain, (Quad**)(cubeCounter * 64 + csptr), -1, 0);
                                    }
                                }
                        }
                    }
                    if (isNonempty == 1) {
                        int t = cubeCounter;
                        CUBE_INDEX_AT(x, y, z) = t;
                        cubeStates[t * 16 + 14] = 0;
                        cubeStates[t * 16 + 15] = style;
                        cubeCounter = t + 1;
                    }
                    if (newCube == 1) {
                        numCubesInLevelTmp++;
                    }
                }
            }
        }
    }
}

int CoordHash(int x, int y, int z, int dir, int div, int mod) {
    switch (dir) {
        case 0:
            z -= 0x100;
            break;
        case 1:
            x += 0x100;
            break;
        case 2:
            y += 0x100;
            break;
        case 3:
            y -= 0x100;
            break;
        case 4:
            x -= 0x100;
            break;
        case 5:
            z += 0x100;
    }
    return ((x + y + z) / (div * 3)) % mod;
}

int GetRandomTextureRotation(void) {
    int ret;

    switch (tgi->randomTileRotation) {
        case 0:
            return 0;
        case 1:
            ret = Rand(2);
            if (ret == 1) {
                ret = 2;
            }
            return ret;
        default:
            return Rand(4);
    }
}

void ParseKeysAndSpecialLevelFromItemData(void) {
    int i;
    int dir;

    numKeysInLevel = 0;
    wasSpecialLevel = specialLevelType;
    for (i = 0; i < numEntities; i++) {
        if (entityData[i * 128] >= 0 && entityData[i * 128] < 5) {
            for (dir = 0; dir < 6; dir++) {
                if (entityData[128 * i + 16 * dir + 1] == OBJ_KEY) {
                    numKeysInLevel++;
                }
            }
        }
    }
    if (numKeysInLevel == 0) {
        specialLevelType = 1;
    } else {
        specialLevelType = 0;
    }
    invisBlockVisibility[0] = 0x2000;
    invisBlockVisibility[1] = 0x2000;
    invisBlockVisibility[2] = 0x2000;
    invisBlockVisibility[3] = 0x15e;
    invisBlockVisibility[4] = 0x200;
    invisBlockVisibility[5] = 0;
    for (i = 0; i < numEntities; i++) {
        if (entityData[128 * i] == 9) {
            if (entityData[128 * i + 1] == 1) {
                specialLevelType = 2;
            }
            if (entityData[128 * i + 2] == 1) {
                invisBlockVisibility[3] = 0x500;
                invisBlockVisibility[4] = 0x700;
                invisBlockVisibility[5] = 1;
            }
        }
    }
    if (gameMode == 1) {
        specialLevelType = 0;
    }
}

void ScanLevelDataForMovingBlocks1(void) {
    int i;
    int x_inc;
    int y_inc;
    int z_inc;
    int length;
    int flags;
    short* entity;
    int j;
    short* pStuff;
    int texDataIdx;
    int dir;
    int val;

    switch (specialLevelType) {
        default:
            flags = 0xd; // QF_ACTIVE | QF_BACKFACE_CULL | QF_GOURAUD
            break;
        case 1:
            flags = 0x20005; // QF_ACTIVE | QF_BACKFACE_CULL | QF_VISITED_TYPE(1)
            break;
        case 2:
            flags = 0xf; // QF_ACTIVE | QF_SEMITRANSPARENT | QF_BACKFACE_CULL | QF_GOURAUD
            break;
    }

    numMovingPlatforms = 0;
    for (i = 0; i < numEntities; i++) {
        if (entityData[i * 128] != 5) {
            continue;
        }
        entity = &entityData[i * 128];
        movingBlockEntityIndexes[numMovingPlatforms++] = (short)i;
        length = (int)entity[17];

        switch (entity[2]) {
            case 1:
                texDataIdx = 0;
                x_inc = 512;
                y_inc = 0;
                z_inc = 0;
                break;
            case 2:
                texDataIdx = 1;
                x_inc = 0;
                y_inc = 512;
                z_inc = 0;
                break;
            default:
                x_inc = 0;
                y_inc = 0;
                z_inc = 512;
                texDataIdx = 2;
                break;
        }

        for (j = 0; j < length; j++) {
            if (length == 1)
                pStuff = MOVING_BLOCK_TEXTURE_DATA[texDataIdx][0];
            else if (j == 0)
                pStuff = MOVING_BLOCK_TEXTURE_DATA[texDataIdx][1];
            else if (j < length - 1)
                pStuff = MOVING_BLOCK_TEXTURE_DATA[texDataIdx][2];
            else
                pStuff = MOVING_BLOCK_TEXTURE_DATA[texDataIdx][3];

            for (dir = 0; dir < 6; dir++) {
                val = pStuff[dir * 2];
                if (val == -1) {
                    ((Quad**)(entity + 71 + j * 12))[dir] = (Quad*)0xffffffff;
                } else {
                    if (val == -2) {
                        SetFaceData(numberOfCubeFaces, &((Quad**)(entity + 71 + j * 12))[dir],
                                    quadSomethingStartIndex + tgi->numFixedTextures + Rand(numPlainTileTextureVariations), flags, dir, 0, 0,
                                    0, pStuff[dir * 2 + 1], 0x808080);
                    } else {
                        SetFaceData(numberOfCubeFaces, &((Quad**)(entity + 71 + j * 12))[dir], quadSomethingStartIndex + val, flags, dir, 0,
                                    0, 0, pStuff[dir * 2 + 1], 0x808080);
                    }

                    if (specialLevelType == 1) {
                        AddQuadToAnimatedTextureChain(
                            crumblingSpecialBlockTextureChain, &((Quad**)(entity + 71 + j * 12))[dir], -1,
                            CoordHash(j * x_inc + 512, j * y_inc + 512, j * z_inc + 512, dir, 64, NUM_TEXTURE_ANIM_FRAMES));
                    }
                    numberOfCubeFaces++;
                }
            }
        }
    }
}

void InitLasers2(void) {
    int i;
    int j;
    int entI;
    int val;
    int* dst;
    short* pLaser;
    int x;
    int y;
    int z;
    int cubeIdx;
    int xLen;
    int dir;
    int sx, sy, sz;
    int sx_start, sx_end;
    int sy_start, sy_end;
    int sz_start, sz_end;
    short* sub;
    int* p;
    uint* ptr;
    byte* intensityData;

    entI = LASER_INTENSITY_DATA_END - LASER_INTENSITY_DATA;
    for (i = 0, intensityData = LASER_INTENSITY_DATA; i < 4; i++) {
        ptr = (uint*)laserData0 + i * entI;
        for (j = 0; j < entI; j++) {
            switch (i) {
                case 0:
                    val = intensityData[j] << 3;
                    ptr[j] = (intensityData[j] << 11) | val;
                    break;
                case 1:
                    ptr[j] = intensityData[j] << 19;
                    break;
                case 2:
                    ptr[j] = intensityData[j] << 11;
                    break;
                case 3:
                    ptr[j] = intensityData[j] << 3;
                    break;
            }
        }
    }

    dst = laserDrawBuffer;
    for (entI = 0; entI < numEntities; entI++) {
        if (entityData[entI * 128] != 8) {
            continue;
        }
        pLaser = &entityData[entI * 128];
        x = pLaser[4];
        y = pLaser[5];
        z = pLaser[6];

        cubeIdx = CUBE_INDEX_AT(x, y, z);
        dir = pLaser[2];
        if (cubeIdx != -1) {
            Quad* quad = (Quad*)cubeStates[cubeIdx * 16 + dir];
            if (quad != -1) {
                SetFaceData(quad, -1, quadSomethingStartIndex + 5,
                            (pLaser[22] << 24) | 0xd,
                            dir, x << 9, y << 9, z << 9, 0, 0x808080);
            }
        }

        xLen = abs(x - pLaser[7]);
        i = abs(y - pLaser[8]);
        if (i > xLen)
            xLen = i;
        i = abs(z - pLaser[9]);
        if (i > xLen)
            xLen = i;

        xLen--;

        dst[0] = xLen;
        dst[1] = (int)&pLaser[3];

        p = dst + 3;
        switch (pLaser[2]) {
            case 1: {
                short* lp = levelData;
                *p = 0x908;
                lp += (x + 1) * 1156 + y * 34 + z;
                dst[2] = lp;
                break;
            }
            case 2: {
                short* lp = levelData;
                *p = 0x44;
                lp += x * 1156 + (y + 1) * 34 + z;
                dst[2] = lp;
                break;
            }
            case 5: {
                short* lp = levelData;
                *p = 2;
                lp += x * 1156 + y * 34 + (z + 1);
                dst[2] = lp;
                break;
            }
        }

        switch (pLaser[22]) {
            case 0:
                dst[4] = laserData0;
                break;
            case 1:
                dst[4] = laserData1;
                break;
            case 2:
                dst[4] = laserData2;
                break;
            case 3:
                dst[4] = laserData3;
                break;
            default:
                break;
        }

        i = LASER_INTENSITY_DATA_END - LASER_INTENSITY_DATA;
        dst[5] = i;
        ((char*)dst)[24] = Rand(i);
        ((char*)dst)[25] = Rand(i);
        ((char*)dst)[26] = Rand(i);
        ((char*)dst)[27] = Rand(i);

        dst += 7;
        for (i = 0; i < xLen; i++) {
            short* pos = (short*)dst;
            sub = pos;

            switch (pLaser[2]) {
                case 1: {
                    x++;
                    sub = pos + 4;

                    pos[0] = x << 9;
                    pos[1] = y << 9;
                    pos[2] = z << 9;
                    pos[3] = 0;

                    for (j = 0; j < 4; j++) {
                        sx = x << 9;
                        sy = y << 9;
                        sz = z << 9;
                        sx_end = sx + 256;
                        sx_start = sx - 256;

                        switch (j) {
                            case 0:
                                sy += 90;
                                sz += 90;
                                break;
                            case 1:
                                sy += 90;
                                sz -= 90;
                                break;
                            case 2:
                                sy -= 90;
                                sz += 90;
                                break;
                            case 3:
                                sy -= 90;
                                sz -= 90;
                                break;
                        }

                        sub[0] = sx_end;
                        sub[1] = sy;
                        sub[8] = sz + 7;

                        sub[2] = sx_end;
                        sub[3] = sy;
                        sub[9] = sz - 7;

                        sub[4] = sx_start;
                        sub[5] = sy;
                        sub[10] = sz + 7;

                        sub[6] = sx_start;
                        sub[7] = sy;
                        sub[11] = sz - 7;

                        sub[12] = sx_end;
                        sub[13] = sy + 7;
                        sub[20] = sz;

                        sub[14] = sx_end;
                        sub[15] = sy - 7;
                        sub[21] = sz;

                        sub[16] = sx_start;
                        sub[17] = sy + 7;
                        sub[22] = sz;

                        sub[18] = sx_start;
                        sub[19] = sy - 7;
                        sub[23] = sz;

                        sub[24] = sx_start;
                        sub[25] = sy;
                        sub[28] = sz;

                        sub[26] = sx_end;
                        sub[27] = sy;
                        sub[29] = sz;
                        sub += 30;
                    }
                    break;
                }
                case 2: {
                    y++;
                    sub = pos + 4;

                    pos[0] = x << 9;
                    pos[1] = y << 9;
                    pos[2] = z << 9;
                    pos[3] = 0;

                    for (j = 0; j < 4; j++) {
                        sx = x << 9;
                        sz = z << 9;
                        sy = y << 9;
                        sy_end = sy + 256;
                        sy_start = sy - 256;

                        switch (j) {
                            case 0:
                                sx += 90;
                                sz += 90;
                                break;
                            case 1:
                                sx += 90;
                                sz -= 90;
                                break;
                            case 2:
                                sx -= 90;
                                sz += 90;
                                break;
                            case 3:
                                sx -= 90;
                                sz -= 90;
                                break;
                        }

                        sub[0] = sx + 7;
                        sub[1] = sy_end;
                        sub[8] = sz;

                        sub[3] = sy_end;
                        sub[2] = sx - 7;
                        sub[9] = sz;

                        sub[5] = sy_start;
                        sub[4] = sx + 7;
                        sub[10] = sz;

                        sub[6] = sx - 7;
                        sub[7] = sy_start;
                        sub[11] = sz;

                        sub[12] = sx;
                        sub[13] = sy_end;
                        sub[20] = sz + 7;

                        sub[14] = sx;
                        sub[15] = sy_end;
                        sub[21] = sz - 7;

                        sub[16] = sx;
                        sub[17] = sy_start;
                        sub[22] = sz + 7;

                        sub[18] = sx;
                        sub[19] = sy_start;
                        sub[23] = sz - 7;

                        sub[24] = sx;
                        sub[25] = sy_start;
                        sub[28] = sz;

                        sub[26] = sx;
                        sub[27] = sy_end;
                        sub[29] = sz;
                        sub += 30;
                    }
                    break;
                }
                case 5: {
                    z++;
                    sub = pos + 4;

                    pos[0] = x << 9;
                    pos[1] = y << 9;
                    pos[2] = z << 9;
                    pos[3] = 0;

                    for (j = 0; j < 4; j++) {
                        sx = x << 9;
                        sy = y << 9;
                        sz = z << 9;
                        sz_end = sz + 256;
                        sz_start = sz - 256;

                        switch (j) {
                            case 0:
                                sx += 90;
                                sy += 90;
                                break;
                            case 1:
                                sx += 90;
                                sy -= 90;
                                break;
                            case 2:
                                sx -= 90;
                                sy += 90;
                                break;
                            case 3:
                                sx -= 90;
                                sy -= 90;
                                break;
                        }

                        sub[0] = sx + 7;
                        sub[1] = sy;
                        sub[8] = sz_end;

                        sub[3] = sy;
                        sub[2] = sx - 7;
                        sub[9] = sz_end;

                        sub[5] = sy;
                        sub[4] = sx + 7;
                        sub[10] = sz_start;

                        sub[6] = sx - 7;
                        sub[7] = sy;
                        sub[11] = sz_start;

                        sub[12] = sx;
                        sub[13] = sy + 7;
                        sub[20] = sz_end;

                        sub[14] = sx;
                        sub[15] = sy - 7;
                        sub[21] = sz_end;

                        sub[16] = sx;
                        sub[17] = sy + 7;
                        sub[22] = sz_start;

                        sub[18] = sx;
                        sub[19] = sy - 7;
                        sub[23] = sz_start;

                        sub[24] = sx;
                        sub[25] = sy;
                        sub[28] = sz_start;

                        sub[26] = sx;
                        sub[27] = sy;
                        sub[29] = sz_end;
                        sub += 30;
                    }
                    break;
                }
            }
            dst = (int*)sub;
        }

        switch (pLaser[2]) {
            case 1:
                x++;
                break;
            case 2:
                y++;
                break;
            case 5:
                z++;
                break;
        }
        dir = FlipDir(pLaser[2]);

        cubeIdx = CUBE_INDEX_AT(x, y, z);
        if (cubeIdx != -1) {
            Quad* quad = (Quad*)cubeStates[cubeIdx * 16 + dir];
            if (quad != -1) {
                SetFaceData(quad, -1, quadSomethingStartIndex + 5,
                            (pLaser[22] << 24) | 0xd, // 5 == laser emitter
                            dir, x << 9, y << 9, z << 9, 0, 0x808080);
            }
        }
    }
    *(int*)dst = -1;
}

void ScanLevelDataForCrumblingBlocks(void) {
    int i;

    numCrumblingBlocks = 0;
    for (i = 0; i < numEntities; i++) {
        if (((CrumblingBlockEntity*)entityData)[i].entityType == OBJ_CRUMBLING_BLOCK) {
            crumblingBlockEntityIndexes[numCrumblingBlocks++] = i;
            ((CrumblingBlockEntity*)entityData)[i].counter = 0x200;
        }
    }
}

void ScanLevelDataForFlashingBlocks(void) {
    Quad* quad;
    int dir;
    FlashingEntity* eb;
    int x;
    int y;
    int z;
    int i;
    int ci;
    int initState;

    numFlashingBlocks = 0;
    for (i = 0; i < numEntities; i++) {
        eb = (FlashingEntity*)(entityData + i * 128);
        if (eb->entityType == OBJ_FLASHING_BLOCK) {
            flashingBlockEntityIndexes[numFlashingBlocks] = i;
            x = eb->x;
            y = eb->y;
            z = eb->z;
            eb->cubeType = levelData[x * 1156 + y * 34 + z];
            numFlashingBlocks++;
            ci = CUBE_INDEX_AT(x, y, z);
            initState = eb->initState;
            if (initState < 2) {
                for (dir = 0; dir < 6; dir++) {
                    *(u16*)cubeStates[16 * ci + dir] = 0x10e;
                }
                eb->state = 0;
                eb->counter = FRAMES(76) - initState * FRAMES(47);
                levelData[x * 1156 + y * 34 + z] = -1;
            } else {
                for (dir = 0; dir < 6; dir++) {
                    quad = cubeStates[16 * ci + dir];
                    *(u8*)&quad->flags = 0xf; // QF_ACTIVE | QF_SEMITRANSPARENT | QF_BACKFACE_CULL | QF_GOURAUD
                    quad->color = 0x808080;
                }
                eb->state = 3;
                eb->counter = FRAMES(164) - initState * FRAMES(47);
            }
        }
    }
}

void SetFaceData(int index, void** pointerInsideCubeState, int texIdx, int flags, int dir,
                 int xFine, int yFine, int zFine, int textureRotation, int color) {
    FaceData* fd = &faceDataPtr[index];
    unsigned char* ptr;

    if (pointerInsideCubeState != (void**)0xffffffff) {
        fd->pointerInsideCubeState = pointerInsideCubeState;
        *pointerInsideCubeState = (void*)index;
    }

    fd = (FaceData*)((char*)fd + 4);
    *(short*)fd = texIdx;
    fd = (FaceData*)((char*)fd + 2);
    *(short*)fd = dir;
    fd = (FaceData*)((char*)fd + 2);
    *(short*)fd = xFine;
    fd = (FaceData*)((char*)fd + 2);
    *(short*)fd = yFine;
    fd = (FaceData*)((char*)fd + 2);
    *(short*)fd = zFine;
    fd = (FaceData*)((char*)fd + 2);
    *(short*)fd = textureRotation;
    fd = (FaceData*)((char*)fd + 2);
    *(int*)fd = flags;
    ((int*)fd)[1] = color;
}

void CopyQuadData(void) {
    FaceData* fd;
    Quad* q;
    int i;
    int j = 0;
    int dir;
    int idx;

    for (i = quadSomethingStartIndex; i < quadSomethingStartIndex + quadSomethingCount; i++) {
        for (fd = faceDataPtr; fd < faceDataPtr + numberOfCubeFaces; fd++) {
            if (fd->texIdx != i) {
                continue;
            }
            // Sets cubeState->quadPtr[dir] to final Quad
            *fd->pointerInsideCubeState = q = &QUAD_BASE[j];
            q->flags.i32 = fd->flags;
            *(int*)&q->otagIndex = 0xffff;
            dir = fd->dir;
            idx = (tgiPart5[i * 2] * 3 + tgiPart5[i * 2 + 1] * tgi->sideBrightness[dir]) * 32;
            q->metadata = (char*)cubeTextureMetadata + idx;
            q->color = fd->color;
            j++;
            QUAD_FUNC_PTRS[dir](q, 512, fd->x, fd->y, fd->z, fd->textureRotation);
        }
    }
    // permuter hack
    idx = j;
    numTotalQuads = idx;
}


void InitAnimatedTextureChain(
        AnimatedTextureChain* dst,
        int maxQuads,
        int numTexAnimFrames,
        int numColorAnimFrames,
        char* texAnimData0,
        char* texAnimData1,
        uint* colorAnimData0,
        uint* colorAnimData1,
        uint* colorAnimData2,
        uint* colorAnimData3) {
    int* d = (int*)dst;
    d[0] = (int)((u8*)dst + 40 + maxQuads * 8);
    d[1] = (int)(dst + 1);
    d[2] = numTexAnimFrames;
    d[3] = numColorAnimFrames;
    d[4] = (int)texAnimData0;
    d[5] = (int)texAnimData1;
    d[6] = (int)colorAnimData0;
    d[7] = (int)colorAnimData1;
    d[8] = (int)colorAnimData2;
    d[9] = (int)colorAnimData3;
}

void AddQuadToAnimatedTextureChain(AnimatedTextureChain* chain, Quad** quadPtr, int initAnimFrame1,
                                   int initAnimFrame2) {
    void** tce = (void**)chain->entries;
    *tce++ = (void*)quadPtr;
    if (initAnimFrame1 == -1) {
        initAnimFrame1 = Rand(chain->numFlagAnimFrames);
    }
    if (initAnimFrame2 == -1) {
        initAnimFrame2 = Rand(chain->numColorAnimFrames);
    }
    *tce++ = (void*)(initAnimFrame2 << 16 | initAnimFrame1);
    chain->entries = (TextureChainEntry*)tce;
}

void ResolveAnimatedTextureChain(AnimatedTextureChain* tc) {
    u32* ptr = tc + 1;
    u32* end = tc->entries;
    while (ptr < end) {
        *ptr = *(u32*)*ptr;
        ptr += 2;
    }
}

void InitShadowSprites(void) {
    int i;
    int j;
    int k;
    int l;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 1; j++) {
            for (k = 0; k < 2; k++) {
                for (l = 0; l < 16; l++) {
                    shadowPrimPtrs[i][j][k][l].p0 = 0xffffffff;
                    setPolyFT4(&shadowPrims[i][j][k][l]);
                    setUV4(&shadowPrims[i][j][k][l],
                            textures[firstGuiTexture + 14 + k].u, textures[firstGuiTexture + 14 + k].v,
                            textures[firstGuiTexture + 14 + k].u + textures[firstGuiTexture + 14 + k].w - 1, textures[firstGuiTexture + 14 + k].v,
                            textures[firstGuiTexture + 14 + k].u, textures[firstGuiTexture + 14 + k].v + textures[firstGuiTexture + 14 + k].h - 1,
                            textures[firstGuiTexture + 14 + k].u + textures[firstGuiTexture + 14 + k].w - 1, textures[firstGuiTexture + 14 + k].v + textures[firstGuiTexture + 14 + k].h - 1);
                    SetSemiTrans(&shadowPrims[i][j][k][l], textures[firstGuiTexture + 14 + k].semitrans);
                    SetShadeTex(&shadowPrims[i][j][k][l], 0);
                    shadowPrims[i][j][k][l].tpage = textures[firstGuiTexture + 14 + k].tpage;
                    shadowPrims[i][j][k][l].clut = textures[firstGuiTexture + 14 + k].clut;
                }
            }
        }
    }
}

void InitPlayerSpecularSprite(void) {
    int i;
    int j;
    int k;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 1; j++) {
            for (k = 0; k < NUM_SPECULAR_PRIMS; k++) {
                specularPrimPtrs[i][j][k].p0 = 0xffffffff;
                setPolyFT4(&specularPrims[i][j][k]);
                setUV4(&specularPrims[i][j][k],
                        textures[firstGuiTexture + 18].u, textures[firstGuiTexture + 18].v,
                        textures[firstGuiTexture + 18].u + textures[firstGuiTexture + 18].w - 1, textures[firstGuiTexture + 18].v,
                        textures[firstGuiTexture + 18].u, textures[firstGuiTexture + 18].v + textures[firstGuiTexture + 18].h - 1,
                        textures[firstGuiTexture + 18].u + textures[firstGuiTexture + 18].w - 1, textures[firstGuiTexture + 18].v + textures[firstGuiTexture + 18].h - 1);
                SetSemiTrans(&specularPrims[i][j][k], textures[firstGuiTexture + 18].semitrans);
                SetShadeTex(&specularPrims[i][j][k], 0);
                setRGB0(&specularPrims[i][j][k], 0x80, 0x80, 0x80);
                specularPrims[i][j][k].tpage = textures[firstGuiTexture + 18].tpage;
                specularPrims[i][j][k].clut = textures[firstGuiTexture + 18].clut;
            }
        }
    }
}

void ScanLevelDataForBlinkingEntities(void) {
    int j, i;
    short* ent;

    for (i = 0; i < numEntities; i++) {
        if (entityData[i * 128] == 0) {
            for (j = 0; j < 6; j++) {
                ent = (short*)((i * 128 + j * 16) * 2 + (int)entityData);
                switch (ent[1]) {
                    case OBJ_EXIT:
                        ent[15] = FRAMES(16);
                        ent[16] = 255;
                        break;
                    case OBJ_TRANSPORTER:
                        ent[15] = FRAMES(16);
                        ent[16] = 255;
                        break;
                    case OBJ_BUTTON:
                        ent[15] = FRAMES(16);
                        ent[16] = 255;
                        break;
                }
            }
        }
    }
}

void DirFunc0(Quad* quad, int width, int x, int y, int z, int textureRotation) {
    x -= width / 2;
    y -= width / 2;
    z -= width / 2;

    if (textureRotation == -1) {
        textureRotation = quad->textureRotation;
    } else {
        quad->textureRotation = textureRotation;
    }

    switch (textureRotation) {
        case 0:
            quad->xy0 = y << 16 | x;
            quad->xy1 = y << 16 | x + width;
            quad->xy2 = y + width << 16 | x;
            quad->xy3 = y + width << 16 | x + width;
            quad->z01 = z << 16 | z;
            quad->z23 = z << 16 | z;
            break;
        case 1:
            quad->xy0 = y << 16 | x + width;
            quad->xy1 = y + width << 16 | x + width;
            quad->xy2 = y << 16 | x;
            quad->xy3 = y + width << 16 | x;
            quad->z01 = z << 16 | z;
            quad->z23 = z << 16 | z;
            break;
        case 2:
            quad->xy0 = y + width << 16 | x + width;
            quad->xy1 = y + width << 16 | x;
            quad->xy2 = y << 16 | x + width;
            quad->xy3 = y << 16 | x;
            quad->z01 = z << 16 | z;
            quad->z23 = z << 16 | z;
            break;
        case 3:
            quad->xy0 = y + width << 16 | x;
            quad->xy1 = y << 16 | x;
            quad->xy2 = y + width << 16 | x + width;
            quad->xy3 = y << 16 | x + width;
            quad->z01 = z << 16 | z;
            quad->z23 = z << 16 | z;
            break;
    }
}

void DirFunc1(Quad* quad, int width, int x, int y, int z, int textureRotation) {
    x -= width / 2;
    y -= width / 2;
    z -= width / 2;

    if (textureRotation == -1) {
        textureRotation = quad->textureRotation;
    } else {
        quad->textureRotation = textureRotation;
    }

    switch (textureRotation) {
        case 0:
            quad->xy0 = y << 16 | x + width;
            quad->xy1 = y << 16 | x + width;
            quad->xy2 = (y + width) << 16 | x + width;
            quad->xy3 = (y + width) << 16 | x + width;
            quad->z01 = (z + width) << 16 | z;
            quad->z23 = (z + width) << 16 | z;
            break;
        case 1:
            quad->xy0 = y << 16 | x + width;
            quad->xy1 = (y + width) << 16 | x + width;
            quad->xy2 = y << 16 | x + width;
            quad->xy3 = (y + width) << 16 | x + width;
            quad->z01 = (z + width) << 16 | z + width;
            quad->z23 = z << 16 | z;
            break;
        case 2:
            quad->xy0 = (y + width) << 16 | x + width;
            quad->xy1 = (y + width) << 16 | x + width;
            quad->xy2 = y << 16 | x + width;
            quad->xy3 = y << 16 | x + width;
            quad->z01 = z << 16 | z + width;
            quad->z23 = z << 16 | z + width;
            break;
        case 3:
            quad->xy0 = (y + width) << 16 | x + width;
            quad->xy1 = y << 16 | x + width;
            quad->xy2 = (y + width) << 16 | x + width;
            quad->xy3 = y << 16 | x + width;
            quad->z01 = z << 16 | z;
            quad->z23 = (z + width) << 16 | z + width;
            break;
    }
}

void DirFunc2(Quad* quad, int width, int x, int y, int z, int textureRotation) {
    x -= width / 2;
    y -= width / 2;
    z -= width / 2;

    if (textureRotation == -1) {
        textureRotation = quad->textureRotation;
    } else {
        quad->textureRotation = textureRotation;
    }

    switch (textureRotation) {
        case 0:
            quad->xy0 = (y + width) << 16 | x;
            quad->xy1 = (y + width) << 16 | x + width;
            quad->xy2 = (y + width) << 16 | x;
            quad->xy3 = (y + width) << 16 | x + width;
            quad->z01 = z << 16 | z;
            quad->z23 = (z + width) << 16 | z + width;
            break;
        case 1:
            quad->xy0 = (y + width) << 16 | x + width;
            quad->xy1 = (y + width) << 16 | x + width;
            quad->xy2 = (y + width) << 16 | x;
            quad->xy3 = (y + width) << 16 | x;
            quad->z01 = (z + width) << 16 | z;
            quad->z23 = (z + width) << 16 | z;
            break;
        case 2:
            quad->xy0 = (y + width) << 16 | x + width;
            quad->xy1 = (y + width) << 16 | x;
            quad->xy2 = (y + width) << 16 | x + width;
            quad->xy3 = (y + width) << 16 | x;
            quad->z01 = (z + width) << 16 | z + width;
            quad->z23 = z << 16 | z;
            break;
        case 3:
            quad->xy0 = (y + width) << 16 | x;
            quad->xy1 = (y + width) << 16 | x;
            quad->xy2 = (y + width) << 16 | x + width;
            quad->xy3 = (y + width) << 16 | x + width;
            quad->z01 = z << 16 | z + width;
            quad->z23 = z << 16 | z + width;
            break;
    }
}

void DirFunc3(Quad* quad, int width, int x, int y, int z, int textureRotation) {
    x -= width / 2;
    y -= width / 2;
    z -= width / 2;

    if (textureRotation == -1) {
        textureRotation = quad->textureRotation;
    } else {
        quad->textureRotation = textureRotation;
    }

    switch (textureRotation) {
        case 0:
            quad->xy0 = y << 16 | x;
            quad->xy1 = y << 16 | x + width;
            quad->xy2 = y << 16 | x;
            quad->xy3 = y << 16 | x + width;
            quad->z01 = (z + width) << 16 | z + width;
            quad->z23 = z << 16 | z;
            break;
        case 1:
            quad->xy0 = y << 16 | x + width;
            quad->xy1 = y << 16 | x + width;
            quad->xy2 = y << 16 | x;
            quad->xy3 = y << 16 | x;
            quad->z01 = z << 16 | z + width;
            quad->z23 = z << 16 | z + width;
            break;
        case 2:
            quad->xy0 = y << 16 | x + width;
            quad->xy1 = y << 16 | x;
            quad->xy2 = y << 16 | x + width;
            quad->xy3 = y << 16 | x;
            quad->z01 = z << 16 | z;
            quad->z23 = (z + width) << 16 | z + width;
            break;
        case 3:
            quad->xy0 = y << 16 | x;
            quad->xy1 = y << 16 | x;
            quad->xy2 = y << 16 | x + width;
            quad->xy3 = y << 16 | x + width;
            quad->z01 = (z + width) << 16 | z;
            quad->z23 = (z + width) << 16 | z;
            break;
    }
}

void DirFunc4(Quad* quad, int width, int x, int y, int z, int textureRotation) {
    x -= width / 2;
    y -= width / 2;
    z -= width / 2;

    if (textureRotation == -1) {
        textureRotation = quad->textureRotation;
    } else {
        quad->textureRotation = textureRotation;
    }

    switch (textureRotation) {
        case 0:
            quad->xy0 = y << 16 | x;
            quad->xy1 = y << 16 | x;
            quad->xy2 = (y + width) << 16 | x;
            quad->xy3 = (y + width) << 16 | x;
            quad->z01 = z << 16 | z + width;
            quad->z23 = z << 16 | z + width;
            break;
        case 1:
            quad->xy0 = y << 16 | x;
            quad->xy1 = (y + width) << 16 | x;
            quad->xy2 = y << 16 | x;
            quad->xy3 = (y + width) << 16 | x;
            quad->z01 = z << 16 | z;
            quad->z23 = (z + width) << 16 | z + width;
            break;
        case 2:
            quad->xy0 = (y + width) << 16 | x;
            quad->xy1 = (y + width) << 16 | x;
            quad->xy2 = y << 16 | x;
            quad->xy3 = y << 16 | x;
            quad->z01 = (z + width) << 16 | z;
            quad->z23 = (z + width) << 16 | z;
            break;
        case 3:
            quad->xy0 = (y + width) << 16 | x;
            quad->xy1 = y << 16 | x;
            quad->xy2 = (y + width) << 16 | x;
            quad->xy3 = y << 16 | x;
            quad->z01 = (z + width) << 16 | z + width;
            quad->z23 = z << 16 | z;
            break;
    }
}

void DirFunc5(Quad* quad, int width, int x, int y, int z, int textureRotation) {
    x -= width / 2;
    y -= width / 2;
    z -= width / 2;

    if (textureRotation == -1) {
        textureRotation = quad->textureRotation;
    } else {
        quad->textureRotation = textureRotation;
    }

    switch (textureRotation) {
        case 0:
            quad->xy0 = y << 16 | x + width;
            quad->xy1 = y << 16 | x;
            quad->xy2 = (y + width) << 16 | x + width;
            quad->xy3 = (y + width) << 16 | x;
            quad->z01 = z + width << 16 | z + width;
            quad->z23 = z + width << 16 | z + width;
            break;
        case 1:
            quad->xy0 = y << 16 | x;
            quad->xy1 = (y + width) << 16 | x;
            quad->xy2 = y << 16 | x + width;
            quad->xy3 = (y + width) << 16 | x + width;
            quad->z01 = z + width << 16 | z + width;
            quad->z23 = z + width << 16 | z + width;
            break;
        case 2:
            quad->xy0 = (y + width) << 16 | x;
            quad->xy1 = (y + width) << 16 | x + width;
            quad->xy2 = y << 16 | x;
            quad->xy3 = y << 16 | x + width;
            quad->z01 = z + width << 16 | z + width;
            quad->z23 = z + width << 16 | z + width;
            break;
        case 3:
            quad->xy0 = (y + width) << 16 | x + width;
            quad->xy1 = y << 16 | x + width;
            quad->xy2 = (y + width) << 16 | x;
            quad->xy3 = y << 16 | x;
            quad->z01 = z + width << 16 | z + width;
            quad->z23 = z + width << 16 | z + width;
            break;
    }
}

void ScanLevelDataForRetractableSpikes(void) {
    int i;
    int j;
    int variant;

    numRetractableSpikes = 0;
    for (i = 0; i < numEntities; i++) {
        if (entityData[i * 128] != 0) {
            continue;
        }
        for (j = 0; j < 6; j++) {
            int offset = j * 16;
            offset += i * 128;
            if (entityData[offset + 1] != 11) {
                continue;
            }
            retractingSpikeData[2 * numRetractableSpikes + 0] = (int)(&entityData[i * 128] + j * 16);
            retractingSpikeData[2 * numRetractableSpikes + 1] = (int)(&entityData[i * 128] + 125);
            numRetractableSpikes++;
            variant = entityData[offset + IE_VARIANT];
            entityData[offset + IE_VARIANT] = 0;
            if (variant * (VID_NTSC ? 35 : 29) < (VID_NTSC ? 63 : 52)) {
                entityData[offset + IE_STATE] = 2;
                entityData[offset + IE_UNK8] = 0;
                entityData[offset + IE_UNK16] = 0;
                entityData[offset + IE_COUNTER] = (VID_NTSC ? 63 : 52) - (VID_NTSC ? 35 : 29) * variant;
            } else if (variant * (VID_NTSC ? 35 : 29) < (VID_NTSC ? 96 : 80)) {
                entityData[offset + IE_STATE] = 1;
                entityData[offset + IE_UNK8] = 31;
                entityData[offset + IE_UNK16] = 3;
                entityData[offset + IE_COUNTER] = (VID_NTSC ? 96 : 80) - variant * (VID_NTSC ? 35 : 29);
            } else if (variant * (VID_NTSC ? 35 : 29) < (VID_NTSC ? 140 : 116)) {
                entityData[offset + IE_STATE] = 1;
                entityData[offset + IE_UNK8] = 31;
                entityData[offset + IE_UNK16] = 4;
                entityData[offset + IE_COUNTER] = (VID_NTSC ? 140 : 116) - variant * (VID_NTSC ? 35 : 29);
            }
        }
    }
}
