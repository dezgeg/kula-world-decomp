#include "common.h"

#define CUBE_TYPE_AT(x, y, z) levelData[(x) * 34 * 34 + (y) * 34 + (z)]

typedef struct LightEffect {
    int color;
    void* prims[7];
} LightEffect;

extern void SetCubeFaceVisited(int x, int y, int z, int whichSide, int visited);
extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);

int AddLightEffectToCubeFace(int x, int y, int z, int dir, int param_5, int param_6, int param_7, int param_8);

extern int cubeStates[16 * 256];
extern int levelScore;
extern int numCubesRemainingInLevel[5];
extern short* entityData;
extern short* levelData;

int lightEffects[32 * 8];
RecentlyVisitedCube recentlyVisitedCubeFaces[32];

static int* curLightEffectDataPtr;

#define CS_SIZE 16
#define CS_SIDES_VISITED 12
#define CS_VISITED 14
#define CS_STYLE 15

#ifndef SKIP_UNUSED_CODE
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
#endif

void SetCubeVisited(int x, int y, int z, int visitType) {
    int cubeIndex;
    int prevVisitType;
    int i;
    SVECTOR sndPos = {};

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

void SetCubeFaceVisited(int x, int y, int z, int whichSide, int visitType) {
    int cubeIndex;
    int i;
    int** pSideFlag;
    Quad* quad;
    RecentlyVisitedCube* p;
    CubeState* cs;

    cubeIndex = CUBE_INDEX_AT(x, y, z);
    if (cubeIndex != -1) {
        cs = (CubeState*)&cubeStates[cubeIndex * 16];
        if (*(cs->sideVisited + whichSide) != visitType) {
            *(cs->sideVisited + whichSide) = visitType;
            quad = *(cs->quadPtrs + whichSide);
            if (quad != (Quad*)0xffffffff) {
                quad->flags.b[2] = quad->flags.b[2] & 0xf9 | (visitType * 2) & 6;
                pSideFlag = &cs->visitList[whichSide];
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
    RecentlyVisitedCube* p;
    int i;
    void** link;
    int counter;
    Quad* quad;
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

int GetFaceTypeOfBlockType(int blockType) {
    if (blockType >= 5) {
        return entityData[(blockType - 5) * 128];
    }
    return blockType;
}

int GetFaceTypeAtRelativeToDir(int x, int y, int z, int dir) {
    int type;
    switch (dir) {
        case 0:
            return GetFaceTypeOfBlockType(CUBE_TYPE_AT(x, y, z - 1)); // Z-
        case 1:
            return GetFaceTypeOfBlockType(CUBE_TYPE_AT(x + 1, y, z)); // X+
        case 2:
            return GetFaceTypeOfBlockType(CUBE_TYPE_AT(x, y + 1, z)); // Y+
        case 3:
            return GetFaceTypeOfBlockType(CUBE_TYPE_AT(x, y - 1, z)); // Y-
        case 4:
            return GetFaceTypeOfBlockType(CUBE_TYPE_AT(x - 1, y, z)); // X-
        default:
            return GetFaceTypeOfBlockType(CUBE_TYPE_AT(x, y, z + 1)); // Z+
    }
}

int GetCubeIndexRelativeToDir(int x, int y, int z, int dir) {
    switch (dir) {
        case 0:
            return CUBE_INDEX_AT(x, y, z - 1); // Z-
        case 1:
            return CUBE_INDEX_AT(x + 1, y, z); // X+
        case 2:
            return CUBE_INDEX_AT(x, y + 1, z); // Y+
        case 3:
            return CUBE_INDEX_AT(x, y - 1, z); // Y-
        case 4:
            return CUBE_INDEX_AT(x - 1, y, z); // X-
        case 5:
            return CUBE_INDEX_AT(x, y, z + 1); // Z+
        default:
            return -1;
    }
}

int FlipDir(int dir) {
    switch (dir) {
        case 0:
            return 5; // Z- -> Z+
        case 1:
            return 4; // X+ -> X-
        case 2:
            return 3; // Y+ -> Y-
        case 3:
            return 2; // Y- -> Y+
        case 4:
            return 1; // X- -> X+
        case 5:
            return 0; // Z+ -> Z-
        default:
            return -1;
    }
}

int AddLightEffect(int x, int y, int z, int whichSide) {
    int i;

    for (i = 0; i < 32; i++) {
        if (lightEffects[i * 8] == -1) {
            break;
        }
    }
    curLightEffectDataPtr = &lightEffects[i * 8 + 1];

    switch (whichSide) {
        case 0:
            AddLightEffectToCubeFace(x,y,z,0,2,2,2,2);

            AddLightEffectToCubeFace(x - 1,y,z - 1,1,0,2,0,2) &&
                AddLightEffectToCubeFace(x - 1,y,z,0,0,2,0,2) &&
                AddLightEffectToCubeFace(x,y,z,4,0,1,0,1);

            AddLightEffectToCubeFace(x,y - 1,z - 1,2,0,0,2,2) &&
                AddLightEffectToCubeFace(x,y - 1,z,0,0,0,2,2) &&
                AddLightEffectToCubeFace(x,y,z,3,0,0,1,1);

            AddLightEffectToCubeFace(x + 1,y,z - 1,4,2,0,2,0) &&
                AddLightEffectToCubeFace(x + 1,y,z,0,2,0,2,0) &&
                AddLightEffectToCubeFace(x,y,z,1,1,0,1,0);

            AddLightEffectToCubeFace(x,y + 1,z - 1,3,2,2,0,0) &&
                AddLightEffectToCubeFace(x,y + 1,z,0,2,2,0,0) &&
                AddLightEffectToCubeFace(x,y,z,2,1,1,0,0);
            break;
        case 1:
            AddLightEffectToCubeFace(x,y,z,1,2,2,2,2);

            AddLightEffectToCubeFace(x + 1,y,z - 1,5,0,2,0,2) &&
                AddLightEffectToCubeFace(x,y,z - 1,1,0,2,0,2) &&
                AddLightEffectToCubeFace(x,y,z,0,0,1,0,1);

            AddLightEffectToCubeFace(x + 1,y - 1,z,2,2,0,2,0) &&
                AddLightEffectToCubeFace(x,y - 1,z,1,0,0,2,2) &&
                AddLightEffectToCubeFace(x,y,z,3,0,1,0,1);

            AddLightEffectToCubeFace(x + 1,y,z + 1,0,2,0,2,0) &&
                AddLightEffectToCubeFace(x,y,z + 1,1,2,0,2,0) &&
                AddLightEffectToCubeFace(x,y,z,5,1,0,1,0);

            AddLightEffectToCubeFace(x + 1,y + 1,z,3,2,0,2,0) &&
                AddLightEffectToCubeFace(x,y + 1,z,1,2,2,0,0) &&
                AddLightEffectToCubeFace(x,y,z,2,0,1,0,1);
            break;
        case 2:
            AddLightEffectToCubeFace(x,y,z,2,2,2,2,2);

            AddLightEffectToCubeFace(x - 1,y + 1,z,1,2,2,0,0) &&
                AddLightEffectToCubeFace(x - 1,y,z,2,0,2,0,2) &&
                AddLightEffectToCubeFace(x,y,z,4,0,0,1,1);

            AddLightEffectToCubeFace(x,y + 1,z - 1,5,2,2,0,0) &&
                AddLightEffectToCubeFace(x,y,z - 1,2,0,0,2,2) &&
                AddLightEffectToCubeFace(x,y,z,0,0,0,1,1);

            AddLightEffectToCubeFace(x + 1,y + 1,z,4,2,2,0,0) &&
                AddLightEffectToCubeFace(x + 1,y,z,2,2,0,2,0) &&
                AddLightEffectToCubeFace(x,y,z,1,0,0,1,1);

            AddLightEffectToCubeFace(x,y + 1,z + 1,0,2,2,0,0) &&
                AddLightEffectToCubeFace(x,y,z + 1,2,2,2,0,0) &&
                AddLightEffectToCubeFace(x,y,z,5,0,0,1,1);
            break;
        case 3:
            AddLightEffectToCubeFace(x,y,z,3,2,2,2,2);

            AddLightEffectToCubeFace(x - 1,y - 1,z,1,0,0,2,2) &&
                AddLightEffectToCubeFace(x - 1,y,z,3,0,2,0,2) &&
                AddLightEffectToCubeFace(x,y,z,4,1,1,0,0);

            AddLightEffectToCubeFace(x,y - 1,z + 1,0,0,0,2,2) &&
                AddLightEffectToCubeFace(x,y,z + 1,3,0,0,2,2) &&
                AddLightEffectToCubeFace(x,y,z,5,1,1,0,0);

            AddLightEffectToCubeFace(x + 1,y - 1,z,4,0,0,2,2) &&
                AddLightEffectToCubeFace(x + 1,y,z,3,2,0,2,0) &&
                AddLightEffectToCubeFace(x,y,z,1,1,1,0,0);

            AddLightEffectToCubeFace(x,y - 1,z - 1,5,0,0,2,2) &&
                AddLightEffectToCubeFace(x,y,z - 1,3,2,2,0,0) &&
                AddLightEffectToCubeFace(x,y,z,0,1,1,0,0);
            break;
        case 4:
            AddLightEffectToCubeFace(x,y,z,4,2,2,2,2);

            AddLightEffectToCubeFace(x - 1,y,z + 1,0,0,2,0,2) &&
                AddLightEffectToCubeFace(x,y,z + 1,4,0,2,0,2) &&
                AddLightEffectToCubeFace(x,y,z,5,0,1,0,1);

            AddLightEffectToCubeFace(x - 1,y - 1,z,2,0,2,0,2) &&
                AddLightEffectToCubeFace(x,y - 1,z,4,0,0,2,2) &&
                AddLightEffectToCubeFace(x,y,z,3,1,0,1,0);

            AddLightEffectToCubeFace(x - 1,y,z - 1,5,2,0,2,0) &&
                AddLightEffectToCubeFace(x,y,z - 1,4,2,0,2,0) &&
                AddLightEffectToCubeFace(x,y,z,0,1,0,1,0);

            AddLightEffectToCubeFace(x - 1,y + 1,z,3,0,2,0,2) &&
                AddLightEffectToCubeFace(x,y + 1,z,4,2,2,0,0) &&
                AddLightEffectToCubeFace(x,y,z,2,1,0,1,0);
            break;
        case 5:
            AddLightEffectToCubeFace(x,y,z,5,2,2,2,2);

            AddLightEffectToCubeFace(x + 1,y,z + 1,4,0,2,0,2) &&
                AddLightEffectToCubeFace(x + 1,y,z,5,0,2,0,2) &&
                AddLightEffectToCubeFace(x,y,z,1,0,1,0,1);

            AddLightEffectToCubeFace(x,y - 1,z + 1,2,2,2,0,0) &&
                AddLightEffectToCubeFace(x,y - 1,z,5,0,0,2,2) &&
                AddLightEffectToCubeFace(x,y,z,3,1,1,0,0);

            AddLightEffectToCubeFace(x - 1,y,z + 1,1,2,0,2,0) &&
                AddLightEffectToCubeFace(x - 1,y,z,5,2,0,2,0) &&
                AddLightEffectToCubeFace(x,y,z,4,1,0,1,0);

            AddLightEffectToCubeFace(x,y + 1,z + 1,3,0,0,2,2) &&
                AddLightEffectToCubeFace(x,y + 1,z,5,2,2,0,0) &&
                AddLightEffectToCubeFace(x,y,z,2,0,0,1,1);
            break;
    }
    *curLightEffectDataPtr++ = 0;

    return i;
}

int AddLightEffectToCubeFace(int x, int y, int z, int dir, int param_5, int param_6, int param_7, int param_8) {
    int msb;
    Quad* quad;
    int ci;

    ci = CUBE_INDEX_AT(x, y, z);

    if (ci != -1) {
        quad = cubeStates[ci * 16 + dir];
        if (quad == (Quad*)-1) {
            return 1;
        }
        switch (quad->textureRotation) {
            case 0:
                msb = param_8 << 6 | param_7 << 4 | param_6 << 2 | param_5;
                break;
            case 1:
                msb = param_7 << 6 | param_5 << 4 | param_8 << 2 | param_6;
                break;
            case 2:
                msb = param_5 << 6 | param_6 << 4 | param_7 << 2 | param_8;
                break;
            default:
                msb = param_6 << 6 | param_8 << 4 | param_5 << 2 | param_7;
                break;
        }
        *curLightEffectDataPtr++ = msb << 24 | (uint)quad & 0xffffff;
        return 0;
    }
    return 1;
}

void SetLightEffectColor(int id, int color) {
    lightEffects[id * 8] = color;
}

void DisableLightEffect(int id) {
    lightEffects[id * 8] = -1;
}
