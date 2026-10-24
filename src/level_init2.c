#include "common.h"

int retractingSpikeData[64 * 2];
short* entityData;
short numEntities;
int numRetractableSpikes;

#define IE_ID 1
#define IE_VARIANT 3
#define IE_STATE 4
#define IE_UNK8 8
#define IE_COUNTER 15
#define IE_UNK16 16

void DirFunc0(Quad* quad, int width, int x, int y, int z, int textureRotation) {
    x -= width / 2;
    y -= width / 2;
    z -= width / 2;

    if (textureRotation == -1) {
        textureRotation = quad->textureRotation;
    } else {
        quad->textureRotation = textureRotation;
    }

    switch (textureRotation) {
        case 0:
            quad->xy0 = y << 16 | x;
            quad->xy1 = y << 16 | x + width;
            quad->xy2 = y + width << 16 | x;
            quad->xy3 = y + width << 16 | x + width;
            quad->z01 = z << 16 | z;
            quad->z23 = z << 16 | z;
            break;
        case 1:
            quad->xy0 = y << 16 | x + width;
            quad->xy1 = y + width << 16 | x + width;
            quad->xy2 = y << 16 | x;
            quad->xy3 = y + width << 16 | x;
            quad->z01 = z << 16 | z;
            quad->z23 = z << 16 | z;
            break;
        case 2:
            quad->xy0 = y + width << 16 | x + width;
            quad->xy1 = y + width << 16 | x;
            quad->xy2 = y << 16 | x + width;
            quad->xy3 = y << 16 | x;
            quad->z01 = z << 16 | z;
            quad->z23 = z << 16 | z;
            break;
        case 3:
            quad->xy0 = y + width << 16 | x;
            quad->xy1 = y << 16 | x;
            quad->xy2 = y + width << 16 | x + width;
            quad->xy3 = y << 16 | x + width;
            quad->z01 = z << 16 | z;
            quad->z23 = z << 16 | z;
            break;
    }
}

void DirFunc1(Quad* quad, int width, int x, int y, int z, int textureRotation) {
    x -= width / 2;
    y -= width / 2;
    z -= width / 2;

    if (textureRotation == -1) {
        textureRotation = quad->textureRotation;
    } else {
        quad->textureRotation = textureRotation;
    }

    switch (textureRotation) {
        case 0:
            quad->xy0 = y << 16 | x + width;
            quad->xy1 = y << 16 | x + width;
            quad->xy2 = (y + width) << 16 | x + width;
            quad->xy3 = (y + width) << 16 | x + width;
            quad->z01 = (z + width) << 16 | z;
            quad->z23 = (z + width) << 16 | z;
            break;
        case 1:
            quad->xy0 = y << 16 | x + width;
            quad->xy1 = (y + width) << 16 | x + width;
            quad->xy2 = y << 16 | x + width;
            quad->xy3 = (y + width) << 16 | x + width;
            quad->z01 = (z + width) << 16 | z + width;
            quad->z23 = z << 16 | z;
            break;
        case 2:
            quad->xy0 = (y + width) << 16 | x + width;
            quad->xy1 = (y + width) << 16 | x + width;
            quad->xy2 = y << 16 | x + width;
            quad->xy3 = y << 16 | x + width;
            quad->z01 = z << 16 | z + width;
            quad->z23 = z << 16 | z + width;
            break;
        case 3:
            quad->xy0 = (y + width) << 16 | x + width;
            quad->xy1 = y << 16 | x + width;
            quad->xy2 = (y + width) << 16 | x + width;
            quad->xy3 = y << 16 | x + width;
            quad->z01 = z << 16 | z;
            quad->z23 = (z + width) << 16 | z + width;
            break;
    }
}

void DirFunc2(Quad* quad, int width, int x, int y, int z, int textureRotation) {
    x -= width / 2;
    y -= width / 2;
    z -= width / 2;

    if (textureRotation == -1) {
        textureRotation = quad->textureRotation;
    } else {
        quad->textureRotation = textureRotation;
    }

    switch (textureRotation) {
        case 0:
            quad->xy0 = (y + width) << 16 | x;
            quad->xy1 = (y + width) << 16 | x + width;
            quad->xy2 = (y + width) << 16 | x;
            quad->xy3 = (y + width) << 16 | x + width;
            quad->z01 = z << 16 | z;
            quad->z23 = (z + width) << 16 | z + width;
            break;
        case 1:
            quad->xy0 = (y + width) << 16 | x + width;
            quad->xy1 = (y + width) << 16 | x + width;
            quad->xy2 = (y + width) << 16 | x;
            quad->xy3 = (y + width) << 16 | x;
            quad->z01 = (z + width) << 16 | z;
            quad->z23 = (z + width) << 16 | z;
            break;
        case 2:
            quad->xy0 = (y + width) << 16 | x + width;
            quad->xy1 = (y + width) << 16 | x;
            quad->xy2 = (y + width) << 16 | x + width;
            quad->xy3 = (y + width) << 16 | x;
            quad->z01 = (z + width) << 16 | z + width;
            quad->z23 = z << 16 | z;
            break;
        case 3:
            quad->xy0 = (y + width) << 16 | x;
            quad->xy1 = (y + width) << 16 | x;
            quad->xy2 = (y + width) << 16 | x + width;
            quad->xy3 = (y + width) << 16 | x + width;
            quad->z01 = z << 16 | z + width;
            quad->z23 = z << 16 | z + width;
            break;
    }
}

void DirFunc3(Quad* quad, int width, int x, int y, int z, int textureRotation) {
    x -= width / 2;
    y -= width / 2;
    z -= width / 2;

    if (textureRotation == -1) {
        textureRotation = quad->textureRotation;
    } else {
        quad->textureRotation = textureRotation;
    }

    switch (textureRotation) {
        case 0:
            quad->xy0 = y << 16 | x;
            quad->xy1 = y << 16 | x + width;
            quad->xy2 = y << 16 | x;
            quad->xy3 = y << 16 | x + width;
            quad->z01 = (z + width) << 16 | z + width;
            quad->z23 = z << 16 | z;
            break;
        case 1:
            quad->xy0 = y << 16 | x + width;
            quad->xy1 = y << 16 | x + width;
            quad->xy2 = y << 16 | x;
            quad->xy3 = y << 16 | x;
            quad->z01 = z << 16 | z + width;
            quad->z23 = z << 16 | z + width;
            break;
        case 2:
            quad->xy0 = y << 16 | x + width;
            quad->xy1 = y << 16 | x;
            quad->xy2 = y << 16 | x + width;
            quad->xy3 = y << 16 | x;
            quad->z01 = z << 16 | z;
            quad->z23 = (z + width) << 16 | z + width;
            break;
        case 3:
            quad->xy0 = y << 16 | x;
            quad->xy1 = y << 16 | x;
            quad->xy2 = y << 16 | x + width;
            quad->xy3 = y << 16 | x + width;
            quad->z01 = (z + width) << 16 | z;
            quad->z23 = (z + width) << 16 | z;
            break;
    }
}

void DirFunc4(Quad* quad, int width, int x, int y, int z, int textureRotation) {
    x -= width / 2;
    y -= width / 2;
    z -= width / 2;

    if (textureRotation == -1) {
        textureRotation = quad->textureRotation;
    } else {
        quad->textureRotation = textureRotation;
    }

    switch (textureRotation) {
        case 0:
            quad->xy0 = y << 16 | x;
            quad->xy1 = y << 16 | x;
            quad->xy2 = (y + width) << 16 | x;
            quad->xy3 = (y + width) << 16 | x;
            quad->z01 = z << 16 | z + width;
            quad->z23 = z << 16 | z + width;
            break;
        case 1:
            quad->xy0 = y << 16 | x;
            quad->xy1 = (y + width) << 16 | x;
            quad->xy2 = y << 16 | x;
            quad->xy3 = (y + width) << 16 | x;
            quad->z01 = z << 16 | z;
            quad->z23 = (z + width) << 16 | z + width;
            break;
        case 2:
            quad->xy0 = (y + width) << 16 | x;
            quad->xy1 = (y + width) << 16 | x;
            quad->xy2 = y << 16 | x;
            quad->xy3 = y << 16 | x;
            quad->z01 = (z + width) << 16 | z;
            quad->z23 = (z + width) << 16 | z;
            break;
        case 3:
            quad->xy0 = (y + width) << 16 | x;
            quad->xy1 = y << 16 | x;
            quad->xy2 = (y + width) << 16 | x;
            quad->xy3 = y << 16 | x;
            quad->z01 = (z + width) << 16 | z + width;
            quad->z23 = z << 16 | z;
            break;
    }
}

void DirFunc5(Quad* quad, int width, int x, int y, int z, int textureRotation) {
    x -= width / 2;
    y -= width / 2;
    z -= width / 2;

    if (textureRotation == -1) {
        textureRotation = quad->textureRotation;
    } else {
        quad->textureRotation = textureRotation;
    }

    switch (textureRotation) {
        case 0:
            quad->xy0 = y << 16 | x + width;
            quad->xy1 = y << 16 | x;
            quad->xy2 = (y + width) << 16 | x + width;
            quad->xy3 = (y + width) << 16 | x;
            quad->z01 = z + width << 16 | z + width;
            quad->z23 = z + width << 16 | z + width;
            break;
        case 1:
            quad->xy0 = y << 16 | x;
            quad->xy1 = (y + width) << 16 | x;
            quad->xy2 = y << 16 | x + width;
            quad->xy3 = (y + width) << 16 | x + width;
            quad->z01 = z + width << 16 | z + width;
            quad->z23 = z + width << 16 | z + width;
            break;
        case 2:
            quad->xy0 = (y + width) << 16 | x;
            quad->xy1 = (y + width) << 16 | x + width;
            quad->xy2 = y << 16 | x;
            quad->xy3 = y << 16 | x + width;
            quad->z01 = z + width << 16 | z + width;
            quad->z23 = z + width << 16 | z + width;
            break;
        case 3:
            quad->xy0 = (y + width) << 16 | x + width;
            quad->xy1 = y << 16 | x + width;
            quad->xy2 = (y + width) << 16 | x;
            quad->xy3 = y << 16 | x;
            quad->z01 = z + width << 16 | z + width;
            quad->z23 = z + width << 16 | z + width;
            break;
    }
}

void ScanLevelDataForRetractableSpikes(void) {
    int i;
    int j;
    int variant;

    numRetractableSpikes = 0;
    for (i = 0; i < numEntities; i++) {
        if (entityData[i * 128] != 0) {
            continue;
        }
        for (j = 0; j < 6; j++) {
            int offset = j * 16;
            offset += i * 128;
            if (entityData[offset + 1] != 11) {
                continue;
            }
            retractingSpikeData[2 * numRetractableSpikes + 0] = (int)(&entityData[i * 128] + j * 16);
            retractingSpikeData[2 * numRetractableSpikes + 1] = (int)(&entityData[i * 128] + 125);
            numRetractableSpikes++;
            variant = entityData[offset + IE_VARIANT];
            entityData[offset + IE_VARIANT] = 0;
            if (variant * (VID_NTSC ? 35 : 29) < (VID_NTSC ? 63 : 52)) {
                entityData[offset + IE_STATE] = 2;
                entityData[offset + IE_UNK8] = 0;
                entityData[offset + IE_UNK16] = 0;
                entityData[offset + IE_COUNTER] = (VID_NTSC ? 63 : 52) - (VID_NTSC ? 35 : 29) * variant;
            } else if (variant * (VID_NTSC ? 35 : 29) < (VID_NTSC ? 96 : 80)) {
                entityData[offset + IE_STATE] = 1;
                entityData[offset + IE_UNK8] = 31;
                entityData[offset + IE_UNK16] = 3;
                entityData[offset + IE_COUNTER] = (VID_NTSC ? 96 : 80) - variant * (VID_NTSC ? 35 : 29);
            } else if (variant * (VID_NTSC ? 35 : 29) < (VID_NTSC ? 140 : 116)) {
                entityData[offset + IE_STATE] = 1;
                entityData[offset + IE_UNK8] = 31;
                entityData[offset + IE_UNK16] = 4;
                entityData[offset + IE_COUNTER] = (VID_NTSC ? 140 : 116) - variant * (VID_NTSC ? 35 : 29);
            }
        }
    }
}
