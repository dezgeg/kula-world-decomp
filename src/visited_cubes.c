#include "common.h"

typedef struct CubeState {
    Quad* quadPtrs[6];
    int field1_0x18[6];
    char sideVisited[6];
    char field3_0x36;
    char field4_0x37;
    int visited;
    int field6_0x3c;
} CubeState;

typedef struct RecentlyVisitedCubeData {
    Quad* quad;
    int counter;
} RecentlyVisitedCubeData;

typedef struct RecentlyVisitedCube {
    void** link;
    RecentlyVisitedCubeData data;
    int pad;
} RecentlyVisitedCube;

extern void SetCubeFaceVisited(int x, int y, int z, int whichSide, int visited);
extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);

extern int cubeStates[16 * 256];
extern RecentlyVisitedCube recentlyVisitedCubeFaces[32];
extern SVECTOR SVECTOR_000a2dd0;
extern int levelScore;
extern int numCubesRemainingInLevel[5];

#define CS_SIZE 16
#define CS_SIDES_VISITED 12
#define CS_VISITED 14
#define CS_STYLE 15
#define CUBE_INDEX_AT(x, y, z) (*(short*)(0x1af000 + (x) * 34 * 34 * 2 + (y) * 34 * 2 + (z) * 2))

int IsCubeVisited(int x, int y, int z) {
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

void SetCubeVisited(int x, int y, int z, int visitType) {
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
                SndPlaySfx(SFX_BONUS_TILE, 0, &sndPos, 5000);
            }
            for (i = 0; i < 6; i++) {
                SetCubeFaceVisited(x, y, z, i, visitType);
            }
        }
    }
}

void SetCubeFaceVisited(int x,int y,int z,int whichSide,int visitType) {
    int cubeIndex;
    int i;
    int **pSideFlag;
    Quad *quad;
    RecentlyVisitedCube *p;
    CubeState* cs;

    cubeIndex = CUBE_INDEX_AT(x, y, z);
    if (cubeIndex != -1) {
        cs = (CubeState*)&cubeStates[cubeIndex * 16];
        if (*(cs->sideVisited + whichSide) != visitType) {
            *(cs->sideVisited + whichSide) = visitType;
            quad = cs->quadPtrs[whichSide];
            if (quad != (Quad *)0xffffffff) {
                quad->flags.b[2] = quad->flags.b[2] & 0xf9 | (visitType * 2) & 6;
                pSideFlag = &cs->field1_0x18[whichSide];
                if (*pSideFlag == (int*)-1) {
                    p = recentlyVisitedCubeFaces;
                    for (i = 0; i < 32; i++) {
                        if (p[i].link == (void**)-1) {
                            break;
                        }
                    }
                    *pSideFlag = (int*)(i * sizeof(RecentlyVisitedCube) + (int)p);
                    p[i].link = (void**)pSideFlag;
                    p[i].data.quad = quad;
                    p[i].data.counter = 0;
                }
            }
        }
    }
}

void ProcessRecentlyVisitedCubes(void) {
    RecentlyVisitedCube *p;
    int i;
    void **link;
    int counter;
    Quad *quad;
    int color;
    int r, g, b;

    p = recentlyVisitedCubeFaces;
    for (i = 0; i < 32; i++) {
        link = p->link;
        if (link != (void**)-1) {
            counter = p->data.counter;
            counter += 0x80;
            if (counter >= 0x1000) {
                *link = (void*)-1;
                p->link = (void**)-1;
            } else {
                p->data.counter = counter;
                quad = p->data.quad;
                color = quad->color;

                r = (color & 0xff);
                g = (color >> 8) & 0xff;
                b = (color >> 16) & 0xff;

                r = ((r - 0xa0) * counter >> 12) + 0xa0;
                g = ((g - 0xa0) * counter >> 12) + 0xa0;
                b = ((b - 0xa0) * counter >> 12) + 0xa0;

                color = (b << 16) | (g << 8) | r;
                quad->color = color;
            }
        }
        p++;
    }
}
