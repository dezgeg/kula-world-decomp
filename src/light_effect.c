#include "common.h"

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

int AddLightEffectToCube(int x, int y, int z, int param_4, int param_5, int param_6, int param_7, int param_8);

typedef struct LightEffect {
    int color;
    void* prims[7];
} LightEffect;

int* curLightEffectDataPtr;
int lightEffects[32 * 8];

extern int cubeStates[16 * 256];

#define CUBE_INDEX_AT(x, y, z) (*(short*)(0x1af000 + (x) * 34 * 34 * 2 + (y) * 34 * 2 + (z) * 2))

int AddLightEffect(int x,int y,int z,int whichSide) {
    int i;

    for (i = 0; i < 32; i++) {
        if (lightEffects[i * 8] == -1) {
            break;
        }
    }
    curLightEffectDataPtr = &lightEffects[i * 8 + 1];

    switch(whichSide) {
        case 0:
            AddLightEffectToCube(x,y,z,0,2,2,2,2);

            AddLightEffectToCube(x - 1,y,z - 1,1,0,2,0,2) &&
                AddLightEffectToCube(x - 1,y,z,0,0,2,0,2) &&
                AddLightEffectToCube(x,y,z,4,0,1,0,1);

            AddLightEffectToCube(x,y - 1,z - 1,2,0,0,2,2) &&
                AddLightEffectToCube(x,y - 1,z,0,0,0,2,2) &&
                AddLightEffectToCube(x,y,z,3,0,0,1,1);

            AddLightEffectToCube(x + 1,y,z - 1,4,2,0,2,0) &&
                AddLightEffectToCube(x + 1,y,z,0,2,0,2,0) &&
                AddLightEffectToCube(x,y,z,1,1,0,1,0);

            AddLightEffectToCube(x,y + 1,z - 1,3,2,2,0,0) &&
                AddLightEffectToCube(x,y + 1,z,0,2,2,0,0) &&
                AddLightEffectToCube(x,y,z,2,1,1,0,0);
            break;
        case 1:
            AddLightEffectToCube(x,y,z,1,2,2,2,2);

            AddLightEffectToCube(x + 1,y,z - 1,5,0,2,0,2) &&
                AddLightEffectToCube(x,y,z - 1,1,0,2,0,2) &&
                AddLightEffectToCube(x,y,z,0,0,1,0,1);

            AddLightEffectToCube(x + 1,y - 1,z,2,2,0,2,0) &&
                AddLightEffectToCube(x,y - 1,z,1,0,0,2,2) &&
                AddLightEffectToCube(x,y,z,3,0,1,0,1);

            AddLightEffectToCube(x + 1,y,z + 1,0,2,0,2,0) &&
                AddLightEffectToCube(x,y,z + 1,1,2,0,2,0) &&
                AddLightEffectToCube(x,y,z,5,1,0,1,0);

            AddLightEffectToCube(x + 1,y + 1,z,3,2,0,2,0) &&
                AddLightEffectToCube(x,y + 1,z,1,2,2,0,0) &&
                AddLightEffectToCube(x,y,z,2,0,1,0,1);
            break;
        case 2:
            AddLightEffectToCube(x,y,z,2,2,2,2,2);

            AddLightEffectToCube(x - 1,y + 1,z,1,2,2,0,0) &&
                AddLightEffectToCube(x - 1,y,z,2,0,2,0,2) &&
                AddLightEffectToCube(x,y,z,4,0,0,1,1);

            AddLightEffectToCube(x,y + 1,z - 1,5,2,2,0,0) &&
                AddLightEffectToCube(x,y,z - 1,2,0,0,2,2) &&
                AddLightEffectToCube(x,y,z,0,0,0,1,1);

            AddLightEffectToCube(x + 1,y + 1,z,4,2,2,0,0) &&
                AddLightEffectToCube(x + 1,y,z,2,2,0,2,0) &&
                AddLightEffectToCube(x,y,z,1,0,0,1,1);

            AddLightEffectToCube(x,y + 1,z + 1,0,2,2,0,0) &&
                AddLightEffectToCube(x,y,z + 1,2,2,2,0,0) &&
                AddLightEffectToCube(x,y,z,5,0,0,1,1);
            break;
        case 3:
            AddLightEffectToCube(x,y,z,3,2,2,2,2);

            AddLightEffectToCube(x - 1,y - 1,z,1,0,0,2,2) &&
                AddLightEffectToCube(x - 1,y,z,3,0,2,0,2) &&
                AddLightEffectToCube(x,y,z,4,1,1,0,0);

            AddLightEffectToCube(x,y - 1,z + 1,0,0,0,2,2) &&
                AddLightEffectToCube(x,y,z + 1,3,0,0,2,2) &&
                AddLightEffectToCube(x,y,z,5,1,1,0,0);

            AddLightEffectToCube(x + 1,y - 1,z,4,0,0,2,2) &&
                AddLightEffectToCube(x + 1,y,z,3,2,0,2,0) &&
                AddLightEffectToCube(x,y,z,1,1,1,0,0);

            AddLightEffectToCube(x,y - 1,z - 1,5,0,0,2,2) &&
                AddLightEffectToCube(x,y,z - 1,3,2,2,0,0) &&
                AddLightEffectToCube(x,y,z,0,1,1,0,0);
            break;
        case 4:
            AddLightEffectToCube(x,y,z,4,2,2,2,2);

            AddLightEffectToCube(x - 1,y,z + 1,0,0,2,0,2) &&
                AddLightEffectToCube(x,y,z + 1,4,0,2,0,2) &&
                AddLightEffectToCube(x,y,z,5,0,1,0,1);

            AddLightEffectToCube(x - 1,y - 1,z,2,0,2,0,2) &&
                AddLightEffectToCube(x,y - 1,z,4,0,0,2,2) &&
                AddLightEffectToCube(x,y,z,3,1,0,1,0);

            AddLightEffectToCube(x - 1,y,z - 1,5,2,0,2,0) &&
                AddLightEffectToCube(x,y,z - 1,4,2,0,2,0) &&
                AddLightEffectToCube(x,y,z,0,1,0,1,0);

            AddLightEffectToCube(x - 1,y + 1,z,3,0,2,0,2) &&
                AddLightEffectToCube(x,y + 1,z,4,2,2,0,0) &&
                AddLightEffectToCube(x,y,z,2,1,0,1,0);
            break;
        case 5:
            AddLightEffectToCube(x,y,z,5,2,2,2,2);

            AddLightEffectToCube(x + 1,y,z + 1,4,0,2,0,2) &&
                AddLightEffectToCube(x + 1,y,z,5,0,2,0,2) &&
                AddLightEffectToCube(x,y,z,1,0,1,0,1);

            AddLightEffectToCube(x,y - 1,z + 1,2,2,2,0,0) &&
                AddLightEffectToCube(x,y - 1,z,5,0,0,2,2) &&
                AddLightEffectToCube(x,y,z,3,1,1,0,0);

            AddLightEffectToCube(x - 1,y,z + 1,1,2,0,2,0) &&
                AddLightEffectToCube(x - 1,y,z,5,2,0,2,0) &&
                AddLightEffectToCube(x,y,z,4,1,0,1,0);

            AddLightEffectToCube(x,y + 1,z + 1,3,0,0,2,2) &&
                AddLightEffectToCube(x,y + 1,z,5,2,2,0,0) &&
                AddLightEffectToCube(x,y,z,2,0,0,1,1);
            break;
    }
    *curLightEffectDataPtr++ = 0;

    return i;
}

int AddLightEffectToCube(int x,int y,int z,int param_4,int param_5,int param_6,int param_7,int param_8) {
    int msb;
    Quad* quad;
    int ci;

    ci = CUBE_INDEX_AT(x, y, z);

    if (ci != -1) {
        quad = cubeStates[ci * 16 + param_4];
        if (quad != -1) {
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
    }
    return 1;
}

void SetLightEffectColor(int id, int color) {
    lightEffects[id * 8] = color;
}

void DisableLightEffect(int id) {
    lightEffects[id * 8] = -1;
}
