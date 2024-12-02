#include "common.h"

extern void SetCubeFaceVisited(int x, int y, int z, int whichSide, int visited);
extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);

typedef struct Quad {
    uint flags;
    short otagIndex;
    byte textureRotation;
    byte pad;
    void* metadata;
    uint color;
    int unk;
    int rgb0;
    int rgb1;
    int rgb2;
    int xy0;
    int xy1;
    int xy2;
    int xy3;
    int z01;
    int z23;
} Quad;

typedef struct CubeState {
    Quad* quadPtrs[6];
    int field1_0x18[6];
    char sideVisited[6];
    char field3_0x36;
    char field4_0x37;
    int visited;
    int field6_0x3c;
} CubeState;

#define CS_SIZE 16
#define CS_SIDES_VISITED 12
#define CS_VISITED 14
#define CS_STYLE 15

extern int cubeStates[16 * 256];
extern SVECTOR SVECTOR_000a2dd0;
extern int levelScore;
extern int numCubesRemainingInLevel[5];

#define CUBE_INDEX_AT(x, y, z) (*(short*)(0x1af000 + (x) * 34 * 34 * 2 + (y) * 34 * 2 + (z) * 2))

int IsCubeVisited(int x,int y,int z) {
    if (CUBE_INDEX_AT(x, y, z) == -1) {
        return -1;
    }

    return cubeStates[CUBE_INDEX_AT(x, y, z) * CS_SIZE + CS_VISITED];
}

int IsCubeSideVisited(int x, int y, int z, int dir) {
    if (CUBE_INDEX_AT(x, y, z) == -1) {
        return -1;
    }

    return ((char*)&cubeStates[CUBE_INDEX_AT(x, y, z) * CS_SIZE + CS_SIDES_VISITED])[dir];
}

void SetCubeVisited(int x,int y,int z,int visitType) {
    int cubeIndex;
    int prevVisitType;
    int i;
    SVECTOR sndPos;

    sndPos = SVECTOR_000a2dd0;
    cubeIndex = CUBE_INDEX_AT(x, y, z);
    if (cubeIndex != -1) {
        prevVisitType = cubeStates[cubeIndex * CS_SIZE + CS_VISITED];
        if (prevVisitType != visitType && cubeStates[cubeIndex * CS_SIZE + CS_STYLE] != 3) {
            cubeStates[CS_SIZE * cubeIndex + CS_VISITED] = visitType;
            numCubesRemainingInLevel[prevVisitType]--;
            numCubesRemainingInLevel[visitType]++;
            if (visitType != 0) {
                levelScore += 50;
                SndPlaySfx(116,0,&sndPos,5000);
            }
            for (i = 0; i < 6; i++) {
                SetCubeFaceVisited(x,y,z,i,visitType);
            }
        }
    }
}

// https://decomp.me/scratch/FLqog
INCLUDE_ASM("asm/nonmatchings/visited_cubes", SetCubeFaceVisited);

// https://decomp.me/scratch/X4b4u
INCLUDE_ASM("asm/nonmatchings/visited_cubes", ProcessRecentlyVisitedCubes);
