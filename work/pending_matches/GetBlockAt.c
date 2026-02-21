#include "common.h"

int getBlockX;
int getBlockY;
int getBlockZ;
int getBlockResult;

extern short * levelData;

#define CUBE_TYPE_AT(x, y, z) levelData[(x) * 1156 + (y) * 34 + (z)]

int GetBlockAt(SVECTOR *coord) {
    int x, y, z;
    int res;
    
    x = (coord->vx + 0x100) >> 9;
    y = (coord->vy + 0x100) >> 9;
    z = (coord->vz + 0x100) >> 9;
    
    getBlockX = x;
    getBlockY = y;
    getBlockZ = z;
    
    if (x < 1 || y < 1 || z < 1 || x > 32 || y > 32 || z > 32) {
        return -1;
    } else {
        res = CUBE_TYPE_AT(x, y, z);
        getBlockResult = res;
        return res;
    }
}
