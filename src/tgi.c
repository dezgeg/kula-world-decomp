#include "common.h"

typedef struct Entry {
    short* clut;
    union {
        short u16;
        struct {
            byte u;
            byte v;
        } u8;
    } uv;
    short tpage;
} Entry;

typedef struct CubeTextureMetadata {
    short unk0;
    short unk2;
    short unk4;
    short unk6;
    Entry entries[3];
} CubeTextureMetadata;

extern void InitCubeTextureMetadata(void);
extern void InitTurningMotionBlur(void);
extern void LoadImagesFromTgiPart9(short* p);

// gprel-used variables (defined in this file)
int DAT_000a4128;
int itemsDispListIdx;
int numPlainTileTextureVariations;
int playerEnemyDispListIdx;
int quadSomethingCount;
int quadSomethingStartIndex;
int recalcSkyboxes;
TgiFile* tgi;
int turningMotionBlurEnabled;
short* tgiEnd;
short* tgiPart0;
short* tgiPart1;
short* tgiPart2;
short* tgiPart3;
short* tgiPart4;
short* tgiPart5;
short* tgiPart6;
short* tgiPart7;
short* tgiPart8;
short* tgiPart9;
short* tgiPart10;

extern CubeTextureMetadata cubeTextureMetadata[1300];
extern DR_TPAGE drTpages1[2][1];
extern DR_TPAGE drTpages2[2][1];
extern DR_TPAGE hudDrTpages[2];
extern int HIGHSCORE_CUBE_RANDOM_TEXTURES[16];
extern int INT_ARRAY_000a2cd8[7];
extern int LOD_THRESHOLDS[9];
extern int turningMotionBlurTable[9 * 7];

extern short DAT_00171314;
extern short DAT_00171316[];
extern short uncompLevelData[];
extern short cubeIndexes[39304];
extern void* cubeStates[256*8];
extern int isFinal;
extern int lightEffects[32 * 8];
extern int recentlyVisitedCubeFaces[4 * 32];
extern int sunglassDisablingState[1];
extern int sunglassSeeEverything[1];
extern int toBeDisabledLightEffects[64];
extern int unkOtagLengths[2][1];

short* entityData;
short* levelData;
short numEntities;
int specialLevelType;
int wasSpecialLevel;

extern void DisableScreenShake(void);
extern void InitLasers(void);
extern void InitManySprites(void);
extern void InitPlayerSpecularSprite(void);
extern void InitShadowSprites(void);
extern void ParseKeysAndSpecialLevelFromItemData(void);
extern void ProcessLevelData(void);
extern void RecalcSkyboxes01(int angleOfSomethingI, int angleOfSomethingJ, int countOfSomethingI, int countOfSomethingJ, int param_5);
extern void RecalcSkyboxes2(void);
extern void ResetCheats(void);
extern void ResetVibration(void);
extern void ScanLevelDataForBlinkingEntities(void);
extern void SetupDisplay(int isbg, int bgR, int bgG, int bgB, int useDithering, int use24Bit);
extern void TweakSkyboxColors(uint param_1);

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
    int k;
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
                *de++ = pPart6[i];
                tx = pPart6[3];
                ty = pPart6[4];
                mx = (tx % 64) * 2;
                my = ty % 256;
                *de++ = my << 8 | mx;
                *de++ = GetTPage(1, 0, tx, ty);
                *de++ = 0;
                pPart6 += 5;
                for (k = 0; k < 3; pPart6 += 5, k++) {
                    *(int*)de = &tgiPart0[pPart6[i] * tgi->unk108];
                    de += 2;
                    tx = pPart6[3];
                    ty = pPart6[4];
                    mx = (tx % 64) * 4;
                    my = ty % 256;
                    switch (k) {
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
                    *de++ = my << 8 | mx;
                    *de++ = GetTPage(0, 0, tx, ty);
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
    // texture = variation;
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
            texture = tgi->unk150 + Rand(tgi->numPlainTileTextureVariationsNormalLevel);
            break;
        case 668:
            texture = tgi->unk150 + tgi->numPlainTileTextureVariationsNormalLevel + 2;
            break;
    }
    md = &cubeTextureMetadata[tgiPart5[texture * 2] * 3 + tgiPart5[texture * 2 + 1]];
    *pTpage = md->entries[0].tpage | 0x20;
    *pClut = *md->entries[0].clut;
    *pU = md->entries[0].uv.u8.u - 0x1f;
    *pV = md->entries[0].uv.u8.v;
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

    constp = &INT_ARRAY_000a2cd8[0];
    tgip = &tgi->lodDistance[0];

    for (i = 0; i < 7; i++) {
        fromTgi = *tgip++;
        fromConst = *constp++;
        for (j = 0; j < 9; j++) {
            turningMotionBlurTable[j * 7 + i] = fromTgi + ((fromConst - fromTgi) * LOD_THRESHOLDS[j]) / 100;
        }
    }
}

void ParseLevelDataFromTgi(void) {
    int i;
    int j;
    unsigned k;
    int* ptr;
    int* ptr2;

    levelData = (void*)0x0015e000;
    entityData = (void*)0x00171316;
    numEntities = *(short*)0x00171314;

    InitLasers();
    ParseKeysAndSpecialLevelFromItemData();
    ResetVibration();
    DisableScreenShake();

    for (i = 0; i < 39304; i++) {
        ((short*)0x1af000)[i] = -1;
    }

    ptr = cubeStates;
    for (i = 0; i < 4096; i++) {
        *ptr++ = -1;
    }

    if (specialLevelType == 1) {
        numPlainTileTextureVariations = tgi->numPlainTileTextureVariationsSpecialLevel;
        DAT_000a4128 = tgi->unk160;
        quadSomethingStartIndex = tgi->unk150 + tgi->numPlainTileTextureVariationsNormalLevel + tgi->unk158;
    } else {
        quadSomethingStartIndex = 0;
        numPlainTileTextureVariations = tgi->numPlainTileTextureVariationsNormalLevel;
        DAT_000a4128 = tgi->unk158;
    }

    quadSomethingCount = tgi->unk150 + numPlainTileTextureVariations + DAT_000a4128;

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
    InitPlayerSpecularSprite();
    ScanLevelDataForBlinkingEntities();

    for (i = 0; i < 64; i++) {
        toBeDisabledLightEffects[i] = -1;
    }

    turningMotionBlurEnabled = 0;
    for (i = 0; i < 7; i++) {
        tgi->lodDistance[i] = turningMotionBlurTable[0 * 7 + i];
    }

    if (tgi->skyboxFlag == 1025) {
        if (specialLevelType != wasSpecialLevel || recalcSkyboxes == 1) {
            switch (specialLevelType) {
            case 0:
                RecalcSkyboxes01(3, 5, 16, 5, 40);
                break;
            case 1:
                RecalcSkyboxes01(4, 3, 16, 10, 36);
                break;
            case 2:
                RecalcSkyboxes2();
                break;
            }
            recalcSkyboxes = 0;
        }
    }

    if (isFinal == 1) {
        TweakSkyboxColors(0x787878);
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
