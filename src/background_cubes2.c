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
extern MATRIX highScoreCubesMatrix;
extern int whichDrawDispEnv;
extern void* otag[2][1][1026];

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
