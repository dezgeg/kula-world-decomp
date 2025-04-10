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
int recalcSkyboxes;
TgiFile* tgi;
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

void* ParseTGI(TgiFile *tgiBuf) {
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
            SetDrawTPage(&drTpages1[i][j],0,1,GetTPage(0,0,0,0));
            SetDrawTPage(&drTpages2[i][j],0,1,GetTPage(0,0,0,0));
        }
        SetDrawTPage(&hudDrTpages[i],0,0,GetTPage(0,0,0,0));
    }
    return tgiPart9 + 0x3f00 / 2;
}

void InitCubeTextureMetadata(void) {
    short *pPart6;
    int i;
    int j;
    int k;
    short *p;
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
                for (k = 0; k < 3; pPart6 += 5,k++) {
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

void GetHighscoreCubeStyle(ushort *pTpage,ushort *pClut,byte *pU,byte *pV,int *pSemitrans) {
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

void LoadImagesFromTgiPart9(short *p) {
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
        LoadImage(&rect,p);
        DrawSync(0);
        p += w * h;
    }
    rect.x = *p++;
    rect.y = *p++;
    rect.w = *p++;
    rect.h = *p++;
    LoadImage(&rect,p);
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

INCLUDE_ASM("asm/nonmatchings/tgi", ParseLevelDataFromTgi);
