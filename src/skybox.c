#include "common.h"

extern short skyboxVertices[4][3];
extern int firstUnk2Texture;
extern Texture textures[150];
extern int starfieldSinPhase1;
extern int starfieldSinPhase2;
extern int starfieldSinPhase3;
extern SVECTOR starfieldSinVec;
extern int D_000A501C;
extern int D_000A5024;
extern int D_000A5028;
extern int D_000A5030;
extern int D_000A5034;
extern int D_000A5050;
extern int D_000A51BC;
extern int D_000A51C0;
extern int D_000A51C4;

int D_000A415C;
int D_000A4164;
int countOfSomethingI;
int countOfSomethingJ;
static short recalcSkyboxes[3]; // XXX: these accesses overlap some other variables
int skyboxAngleMul1;
int skyboxAngleMul2;
int skyboxAnglesProduct;
int skyboxCountsProduct;
int skyboxMaxAngle1;
int skyboxMaxAngle2;
int skyboxMaxAngle2Plus1;
int skyboxMaxAnglesProduct;
int skyboxParam5;
int skyboxParam5Times2;
int skyboxSizeMidBlk;
int skyboxSizePolyMap;
int skyboxSizePolys;
int skyboxSizeVertices;
short* tgiPart9;

// Prototypes
extern uint Rand(int param_1);
extern void SkyboxSinCos(int angleval1, int angleval2, short * out1, short * out2, short * out3);
extern int Square(int val);
extern void SetShadeTex(void * p, int disable);
extern void SetSemiTrans(void * p, int enable);
extern void *memcpy(void *dest, const void *src, size_t n);

void RecalcSkyboxes01(int angleOfSomethingI, int angleOfSomethingJ, int countI, int countJ, int param_5) {
    short *ptr;
    int i;
    int j;
    int maxDist;
    int l;
    int s;
    int u;
    int v;
    int w;
    int k;
    short *texcoords;

    skyboxAngleMul1 = angleOfSomethingI;
    skyboxAngleMul2 = angleOfSomethingJ;
    countOfSomethingJ = countI;
    countOfSomethingI = countJ;
    skyboxParam5 = param_5;
    skyboxAnglesProduct = angleOfSomethingI * angleOfSomethingJ;
    skyboxParam5Times2 = param_5 * 2;
    skyboxMaxAngle1 = countI * angleOfSomethingI;
    D_000A415C = skyboxMaxAngle1;
    skyboxMaxAngle2 = countJ * angleOfSomethingJ;
    skyboxMaxAngle2Plus1 = skyboxMaxAngle2 + 1;
    skyboxCountsProduct = countI * countJ;
    skyboxSizeMidBlk = (skyboxCountsProduct + 2) * 8;
    skyboxMaxAnglesProduct = skyboxMaxAngle1 * skyboxMaxAngle2;
    D_000A4164 = skyboxMaxAngle1 * skyboxMaxAngle2Plus1;
    skyboxSizePolyMap = D_000A4164;
    skyboxSizeVertices = (skyboxMaxAnglesProduct * 20) / 3;
    if ((D_000A4164 & 0x1f) > 0) {
        skyboxSizePolyMap = (D_000A4164 + 32) & -32;
    }
    skyboxSizePolys = D_000A4164 * skyboxParam5Times2;

    ptr = (short *)0x001c3000;

    for (i = 0; i < countOfSomethingI; i++) {
        for (j = 0; j < countOfSomethingJ; j++) {
            SkyboxSinCos(j * skyboxAngleMul1 - 1, i * skyboxAngleMul2 - 1, &skyboxVertices[0][0], &skyboxVertices[0][1], &skyboxVertices[0][2]);
            SkyboxSinCos((j + 1) * skyboxAngleMul1, i * skyboxAngleMul2 - 1, &skyboxVertices[1][0], &skyboxVertices[1][1], &skyboxVertices[1][2]);
            SkyboxSinCos(j * skyboxAngleMul1 - 1, (i + 1) * skyboxAngleMul2, &skyboxVertices[2][0], &skyboxVertices[2][1], &skyboxVertices[2][2]);
            SkyboxSinCos((j + 1) * skyboxAngleMul1, (i + 1) * skyboxAngleMul2, &skyboxVertices[3][0], &skyboxVertices[3][1], &skyboxVertices[3][2]);

            for (k = 0; k < 3; k++) {
                *ptr++ = recalcSkyboxes[k] = (((skyboxVertices[0][k] + skyboxVertices[1][k]) + skyboxVertices[2][k]) + skyboxVertices[3][k]) / 4;
            }

            maxDist = 0;
            for (k = 0; k < 4; k++) {
                s = SquareRoot0(Square(recalcSkyboxes[0] - skyboxVertices[k][0]) +
                               Square(recalcSkyboxes[1]  - skyboxVertices[k][1]) +
                               Square(recalcSkyboxes[2] - skyboxVertices[k][2]));
                if (maxDist < s)
                    maxDist = s;
            }
            *ptr++ = maxDist;
        }
    }

    *(int *)&ptr[0] = 0;
    *(int *)&ptr[2] = 0;
    *(int *)&ptr[4] = -1;
    *(int *)&ptr[6] = -1;
    ptr += 8;

    if (param_5 == 36) {
        for (i = 0; i < countOfSomethingI; i++) {
            for (j = 0; j < countOfSomethingJ; j++) {
                for (k = 0; k < skyboxAngleMul1; k++) {
                    for (l = 0; l < (skyboxAngleMul2 / 3); l++) {
                        SkyboxSinCos(j * skyboxAngleMul1 + k, i * skyboxAngleMul2 + l * 3, &ptr[0], &ptr[1], &ptr[6]);
                        SkyboxSinCos(j * skyboxAngleMul1 + k, i * skyboxAngleMul2 + l * 3 + 1, &ptr[2], &ptr[3], &ptr[7]);
                        SkyboxSinCos(j * skyboxAngleMul1 + k, i * skyboxAngleMul2 + l * 3 + 2, &ptr[4], &ptr[5], &ptr[8]);
                        ptr += 10;
                    }
                }
            }
        }
    } else {
        for (i = 0; i < countOfSomethingI; i++) {
            for (j = 0; j < countOfSomethingJ; j++) {
                for (k = 0; k < skyboxAngleMul2; k++) {
                    for (l = 0; l < (skyboxAngleMul1 / 3); l++) {
                        SkyboxSinCos(j * skyboxAngleMul1 + l * 3, i * skyboxAngleMul2 + k, &ptr[0], &ptr[1], &ptr[6]);
                        SkyboxSinCos(j * skyboxAngleMul1 + l * 3 + 1, i * skyboxAngleMul2 + k, &ptr[2], &ptr[3], &ptr[7]);
                        SkyboxSinCos(j * skyboxAngleMul1 + l * 3 + 2, i * skyboxAngleMul2 + k, &ptr[4], &ptr[5], &ptr[8]);
                        ptr += 10;
                    }
                }
            }
        }
    }

    ptr += skyboxSizePolyMap / 2;
    if (skyboxParam5 == 36) {
        unsigned char *pu = (unsigned char *)ptr;
        for (i = 0; i < D_000A4164 * 2; i++) {
            setPolyG4(pu);
            pu += 36;
        }
    } else {
        texcoords = tgiPart9;
        ptr += (D_000A415C * skyboxParam5Times2) / 2;
        for (i = 0; i < 24; i++) {
            for (j = 0; j < 48; j++) {
                setPolyFT4((POLY_FT4 *)ptr);
                SetShadeTex((POLY_FT4 *)ptr, 0);
                SetSemiTrans((POLY_FT4 *)ptr, 0);
                setRGB0((POLY_FT4 *)ptr, 128, 128, 128);
                ((POLY_FT4 *)(ptr))->clut = GetClut(texcoords[0], texcoords[1]);
                ((POLY_FT4 *)(ptr))->tpage = GetTPage(1, 0, texcoords[2], texcoords[3]);

                u = texcoords[4];
                v = texcoords[5];
                w = texcoords[6];
                *(u_short *)&((POLY_FT4 *)(ptr))->u0 = (v << 8) | u;
                *(u_short *)&((POLY_FT4 *)(ptr))->u1 = (v << 8) | (u + w);
                *(u_short *)&((POLY_FT4 *)(ptr))->u2 = ((v + 15) << 8) | u;
                *(u_short *)&((POLY_FT4 *)(ptr))->u3 = ((v + 15) << 8) | (u + w);
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


int RecalcSkyboxes2(void) {
    short *dst;
    short *ptr;
    short *ptr2;
    int x;
    int i;
    int y;
    int z;
    int j;
    int k;
    int l;
    int *u1, *u2;
    u32 *p_u32;
    int clut;
    int outer_i;
    int dummy[16];

    starfieldSinPhase1 = 0x200;
    starfieldSinPhase2 = 0;
    starfieldSinPhase3 = 0x400;
    starfieldSinVec.vx = 0;
    starfieldSinVec.vy = 0;
    starfieldSinVec.vz = 0;

    D_000A501C = 0x200;
    D_000A5024 = 0x400;
    D_000A5028 = 0;
    D_000A5030 = 0x400;
    D_000A5034 = 0;
    D_000A5050 = 0;
    D_000A51BC = 0;
    D_000A51C0 = 0;
    D_000A51C4 = 0;

    ptr = (short*)0x1c3000;
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

    *(int*)0x1cd8e8 = SquareRoot0(0x7adb853);

    dst = (short*)0x001cd8ec;
    for (z = -0x7FFD; z < 0x7FFD; z += 0x3332) {
        for (y = -0x7FFD; y < 0x7FFD; y += 0x3332) {
            for (x = -0x7FFD; x < 0x7FFD; x += 0x3332) {
                ptr = (short*)0x1c3000;
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
                    } while(0);
                }
            }
        }
    }

    dst[0] = 0;
    dst[1] = 0;
    dst[2] = 0;
    dst[3] = -1;
    dst += 4;

    u1 = (int*)0x1cda3c;
    p_u32 = (int*)0x1cda48;
    for (i = 0; i < 125; i++) {
        u1[-2] = p_u32[0];
        u1[-1] = u1[4];
        u1 += 87;
        p_u32 += 87;
    }

    p_u32 = (u32*)0x1c3000;
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

    return (u32)dst - 0x1C3000;
}

int Square(int param_1) {
    return param_1 * param_1;
}

void SkyboxSinCos(int angleval1, int angleval2, short *out1, short *out2, short *out3) {
    int cosParam2;
    int cosParam1;
    int sinParam2;
    int sinParam1;
    int angleval2_;
    int out1_val;

    if (angleval1 < 0) {
        angleval1 = angleval1 + skyboxMaxAngle1;
    }
    if (skyboxMaxAngle1 <= angleval1) {
        angleval1 = angleval1 - skyboxMaxAngle1;
    }
    if (angleval2 < 0) {
        angleval2 = 0;
    }
    angleval2_ = angleval2 << 11;
    if (skyboxMaxAngle2 <= angleval2) {
        angleval2 = skyboxMaxAngle2 - 1;
        angleval2_ = angleval2 << 11;
    }
    cosParam2 = rcos(angleval2_ / (skyboxMaxAngle2 - 1) + 0x400);
    cosParam1 = rcos((angleval1 << 12) / skyboxMaxAngle1);
    cosParam1 = cosParam1 * cosParam2;
    if (cosParam1 < 0) {
        cosParam1 = cosParam1 + 0xfff;
    }
    out1_val = cosParam1 >> 12;
    sinParam2 = rsin(angleval2_ / (skyboxMaxAngle2 - 1) + 0x400);
    sinParam1 = rsin((angleval1 << 12) / skyboxMaxAngle1);
    sinParam1 = sinParam1 * cosParam2;
    if (sinParam1 < 0) {
        sinParam1 = sinParam1 + 0xfff;
    }
    *out1 = (short)out1_val;
    *out2 = (short)sinParam2;
    *out3 = (short)(sinParam1 >> 12);
}
