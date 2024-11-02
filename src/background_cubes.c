#include "common.h"

typedef struct PmdFace {
    POLY_FT4 polys[2];
    SVECTOR vertexes[4];
} PmdFace;

typedef struct PmdCube {
    long count;
    PmdFace faces[6];
} PmdCube;

int hsCube1stSinPhaseIncrementX;
int hsCube1stSinPhaseIncrementY;
int hsCube1stSinPhaseIncrementZ;
int hsCube1stSinPhaseX;
int hsCube1stSinPhaseY;
int hsCube1stSinPhaseZ;
int hsCube2ndIndexMulX;
int hsCube2ndIndexMulY;
int hsCube2ndIndexMulZ;
int hsCube2ndSinOffsetX;
int hsCube2ndSinOffsetY;
int hsCube2ndSinOffsetZ;
int hsCube2ndSinPhaseIncrementX;
int hsCube2ndSinPhaseIncrementY;
int hsCube2ndSinPhaseIncrementZ;
int hsCube2ndSinPhaseX;
int hsCube2ndSinPhaseY;
int hsCube2ndSinPhaseZ;
int hsCube2ndSinValMulX;
int hsCube2ndSinValMulY;
int hsCube2ndSinValMulZ;
int hsCube3rdIndexMulX;
int hsCube3rdIndexMulY;
int hsCube3rdIndexMulZ;
int hsCube3rdSinPhaseIncrementX;
int hsCube3rdSinPhaseIncrementY;
int hsCube3rdSinPhaseIncrementZ;
int hsCube3rdSinPhaseX;
int hsCube3rdSinPhaseY;
int hsCube3rdSinPhaseZ;
int hsCube3rdSinValMulX;
int hsCube3rdSinValMulY;
int hsCube3rdSinValMulZ;
SVECTOR hsCubeRotationVec;
PmdCube* pmdCubesPtr;

extern int displayHeight;
extern int displayWidth;
extern int whichDrawDispEnv;
extern MATRIX highScoreCubesMatrix;
extern SVECTOR hsCubeUnusedVec;
extern void* otag[2][1][1026];

void InitHighscoreCubes(void) {
    u_char rgb;
    int i;
    int j;
    ushort tpage;
    ushort clut;
    byte u0;
    byte v0;
    int semitrans;

    SVECTOR zero = {};
    SVECTOR v1 = { -100, -100, 100 };
    SVECTOR v2 = { 100, -100, 100 };
    SVECTOR v3 = { -100, -100, -100 };
    SVECTOR v4 = { 100, -100, -100 };
    SVECTOR v5 = { -100, 100, 100 };
    SVECTOR v6 = { 100, 100, 100 };
    SVECTOR v7 = { -100, 100, -100 };
    SVECTOR v8 = { 100, 100, -100 };

    hsCubeRotationVec = zero;
    hsCubeUnusedVec = zero;
    hsCubeUnusedVec.vz = 1000;

    hsCube2ndSinValMulX = 900;
    hsCube2ndSinPhaseX = 0;
    hsCube2ndSinPhaseIncrementX = 30;
    hsCube2ndSinOffsetX = 700;
    hsCube3rdSinValMulX = 500;
    hsCube3rdSinPhaseX = 0;
    hsCube3rdSinPhaseIncrementX = 13;
    hsCube3rdIndexMulX = 860;
    hsCube2ndIndexMulX = 400;
    hsCube1stSinPhaseX = 0;
    hsCube1stSinPhaseIncrementX = 7;

    hsCube2ndSinValMulY = 800;
    hsCube2ndSinPhaseY = 0;
    hsCube2ndSinPhaseIncrementY = 19;
    hsCube2ndSinOffsetY = 600;
    hsCube3rdSinValMulY = 400;
    hsCube3rdSinPhaseY = 0;
    hsCube3rdSinPhaseIncrementY = 20;
    hsCube3rdIndexMulY = 730;
    hsCube2ndIndexMulY = 150;
    hsCube1stSinPhaseY = 0;
    hsCube1stSinPhaseIncrementY = 19;

    hsCube2ndSinValMulZ = 400;
    hsCube2ndSinPhaseZ = 0;
    hsCube2ndSinPhaseIncrementZ = 7;
    hsCube2ndSinOffsetZ = 37;
    hsCube3rdSinValMulZ = 200;
    hsCube3rdSinPhaseZ = 0;
    hsCube3rdSinPhaseIncrementZ = 20;
    hsCube3rdIndexMulZ = 70;
    hsCube2ndIndexMulZ = 0;
    hsCube1stSinPhaseZ = 0;
    hsCube1stSinPhaseIncrementZ = 0;

    pmdCubesPtr = 0x1fb8f0;

    for (i = 0; i < 10; i++) {
        pmdCubesPtr[i].count = 6;
        GetHighscoreCubeStyle(&tpage, &clut, &u0, &v0, &semitrans);
        for (j = 0; j < 6; j++) {
            SetPolyFT4(&pmdCubesPtr[i].faces[j].polys[0]);
            rgb = j * 16 + ((i & 1) * 8 + 64);
            setRGB0(&pmdCubesPtr[i].faces[j].polys[0], rgb, rgb, rgb);
            SetSemiTrans(&pmdCubesPtr[i].faces[j].polys[0], semitrans);
            setUV4(&pmdCubesPtr[i].faces[j].polys[0],
                u0, v0,
                u0 + 31, v0,
                u0, v0 + 31,
                u0 + 31, v0 + 31);
            pmdCubesPtr[i].faces[j].polys[0].clut = clut;
            pmdCubesPtr[i].faces[j].polys[0].tpage = tpage;
            pmdCubesPtr[i].faces[j].polys[1] = pmdCubesPtr[i].faces[j].polys[0];
        }
        pmdCubesPtr[i].faces[0].vertexes[0] = v1;
        pmdCubesPtr[i].faces[0].vertexes[1] = v2;
        pmdCubesPtr[i].faces[0].vertexes[2] = v3;
        pmdCubesPtr[i].faces[0].vertexes[3] = v4;
        pmdCubesPtr[i].faces[1].vertexes[0] = v7;
        pmdCubesPtr[i].faces[1].vertexes[1] = v8;
        pmdCubesPtr[i].faces[1].vertexes[2] = v5;
        pmdCubesPtr[i].faces[1].vertexes[3] = v6;
        pmdCubesPtr[i].faces[2].vertexes[0] = v1;
        pmdCubesPtr[i].faces[2].vertexes[1] = v3;
        pmdCubesPtr[i].faces[2].vertexes[2] = v5;
        pmdCubesPtr[i].faces[2].vertexes[3] = v7;
        pmdCubesPtr[i].faces[3].vertexes[0] = v4;
        pmdCubesPtr[i].faces[3].vertexes[1] = v2;
        pmdCubesPtr[i].faces[3].vertexes[2] = v8;
        pmdCubesPtr[i].faces[3].vertexes[3] = v6;
        pmdCubesPtr[i].faces[4].vertexes[0] = v3;
        pmdCubesPtr[i].faces[4].vertexes[1] = v4;
        pmdCubesPtr[i].faces[4].vertexes[2] = v7;
        pmdCubesPtr[i].faces[4].vertexes[3] = v8;
        pmdCubesPtr[i].faces[5].vertexes[0] = v2;
        pmdCubesPtr[i].faces[5].vertexes[1] = v1;
        pmdCubesPtr[i].faces[5].vertexes[2] = v6;
        pmdCubesPtr[i].faces[5].vertexes[3] = v5;
    }
}

void DrawHighScoreCubes(void) {
    int sinVal2ndX;
    int sinVal3rdX;
    int sinVal2ndY;
    int sinVal3rdY;
    int sinVal2ndZ;
    int sinVal3rdZ;
    int i;
    SVECTOR rotVec;

    hsCubeRotationVec.vx = (hsCubeRotationVec.vx + 25) & 0xfff;
    hsCubeRotationVec.vy = (hsCubeRotationVec.vy + 11) & 0xfff;
    hsCubeRotationVec.vz = (hsCubeRotationVec.vz + 41) & 0xfff;

    rotVec = hsCubeRotationVec;
    hsCube2ndSinPhaseX = hsCube2ndSinPhaseX + hsCube2ndSinPhaseIncrementX & 0xfff;
    hsCube3rdSinPhaseX = hsCube3rdSinPhaseX + hsCube3rdSinPhaseIncrementX & 0xfff;
    hsCube1stSinPhaseX = hsCube1stSinPhaseX + hsCube1stSinPhaseIncrementX & 0xfff;
    hsCube2ndSinPhaseY = hsCube2ndSinPhaseY + hsCube2ndSinPhaseIncrementY & 0xfff;
    hsCube3rdSinPhaseY = hsCube3rdSinPhaseY + hsCube3rdSinPhaseIncrementY & 0xfff;
    hsCube1stSinPhaseY = hsCube1stSinPhaseY + hsCube1stSinPhaseIncrementY & 0xfff;
    hsCube2ndSinPhaseZ = hsCube2ndSinPhaseY + hsCube2ndSinPhaseIncrementZ & 0xfff;
    hsCube3rdSinPhaseZ = hsCube3rdSinPhaseY + hsCube3rdSinPhaseIncrementZ & 0xfff;
    hsCube1stSinPhaseZ = hsCube1stSinPhaseY + hsCube1stSinPhaseIncrementZ & 0xfff;
    for (i = 0; i < 10; i++) {
        rotVec.vx = (rotVec.vx + 210) & 0xfff;
        rotVec.vy = (rotVec.vy + 133) & 0xfff;
        rotVec.vz = (rotVec.vz + 93) & 0xfff;
        RotMatrix(&rotVec, &highScoreCubesMatrix);

        sinVal2ndX = rsin(hsCube2ndSinPhaseX + (hsCube2ndSinOffsetX + hsCube2ndIndexMulX * rsin(hsCube1stSinPhaseX) * i) >> 12 );
        sinVal3rdX = rsin(hsCube3rdSinPhaseX + hsCube3rdIndexMulX * i);
        highScoreCubesMatrix.t[0] = hsCube2ndSinValMulX * sinVal2ndX + hsCube3rdSinValMulX * sinVal3rdX >> 12;

        sinVal2ndY = rsin(hsCube2ndSinPhaseY + (hsCube2ndSinOffsetY + hsCube2ndIndexMulY * rsin(hsCube1stSinPhaseY) * i) >> 12 );
        sinVal3rdY = rsin(hsCube3rdSinPhaseY + hsCube3rdIndexMulY * i);
        highScoreCubesMatrix.t[1] = hsCube2ndSinValMulY * sinVal2ndY + hsCube3rdSinValMulY * sinVal3rdY >> 12;

        sinVal2ndZ = rsin(hsCube2ndSinPhaseZ + (hsCube2ndSinOffsetZ + hsCube2ndIndexMulZ * rsin(hsCube1stSinPhaseZ) * i) >> 12 );
        sinVal3rdZ = rsin(hsCube3rdSinPhaseZ + hsCube3rdIndexMulZ * i);
        highScoreCubesMatrix.t[2] = (hsCube2ndSinValMulZ * sinVal2ndZ + hsCube3rdSinValMulZ * sinVal3rdZ >> 12) + 1000;

        SetRotMatrix(&highScoreCubesMatrix);
        SetTransMatrix(&highScoreCubesMatrix);
        RotSMD_FT4((long*)&pmdCubesPtr[i], &otag[whichDrawDispEnv][0][0], 1024, whichDrawDispEnv, 1,
                   displayWidth, displayHeight, 1);
    }
}
