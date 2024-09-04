#include "common.h"

extern short* entityData;
extern short* levelData;

#define CUBE_INDEX_AT(x, y, z) (*(short*)(0x1af000 + (x) * 34 * 34 * 2 + (y) * 34 * 2 + (z) * 2))
#define CUBE_TYPE_AT(x, y, z) levelData[(x) * 34 * 34  + (y) * 34  + (z)]

int GetFaceTypeOfBlockType(int blockType) {
    if (blockType >= 5) {
        return entityData[(blockType - 5) * 0x80];
    }
    return blockType;
}

int GetFaceTypeAtRelativeToDir(int x,int y,int z,int dir) {
    int type;
    switch(dir) {
    case 0:  return GetFaceTypeOfBlockType(CUBE_TYPE_AT(x,     y, z - 1     )); // Z-
    case 1:  return GetFaceTypeOfBlockType(CUBE_TYPE_AT(x + 1, y, z         )); // X+
    case 2:  return GetFaceTypeOfBlockType(CUBE_TYPE_AT(x,     y + 1, z     )); // Y+
    case 3:  return GetFaceTypeOfBlockType(CUBE_TYPE_AT(x,     y - 1, z     )); // Y-
    case 4:  return GetFaceTypeOfBlockType(CUBE_TYPE_AT(x - 1, y,     z     )); // X-
    default: return GetFaceTypeOfBlockType(CUBE_TYPE_AT(x,     y,      z + 1)); // Z+
    }
}

int GetCubeIndexRelativeToDir(int x,int y,int z,int dir) {
    switch(dir) {
    case 0: return CUBE_INDEX_AT(x,     y, z - 1     ); // Z-
    case 1: return CUBE_INDEX_AT(x + 1, y, z         ); // X+
    case 2: return CUBE_INDEX_AT(x,     y + 1, z     ); // Y+
    case 3: return CUBE_INDEX_AT(x,     y - 1, z     ); // Y-
    case 4: return CUBE_INDEX_AT(x - 1, y,     z     ); // X-
    case 5: return CUBE_INDEX_AT(x,     y,      z + 1); // Z+
    default: return -1;
    }
}

int FlipDir(int dir) {
    switch(dir) {
    case 0: return 5; // Z- -> Z+
    case 1: return 4; // X+ -> X-
    case 2: return 3; // Y+ -> Y-
    case 3: return 2; // Y- -> Y+
    case 4: return 1; // X- -> X+
    case 5: return 0; // Z+ -> Z-
    default: return -1;
    }
}
