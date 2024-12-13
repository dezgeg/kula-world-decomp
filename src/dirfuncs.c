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

void DirFunc0(Quad *quad,int width,int x,int y,int z,int textureRotation) {
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

void DirFunc1(Quad *quad,int width,int x,int y,int z,int textureRotation) {
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

void DirFunc2(Quad *quad,int width,int x,int y,int z,int textureRotation) {
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
            quad->z01 = z * 0x10000 | z;
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
            quad->xy0 = (y + width) * 0x10000 | x;
            quad->xy1 = (y + width) * 0x10000 | x;
            quad->xy2 = (y + width) << 16 | x + width;
            quad->xy3 = (y + width) << 16 | x + width;
            quad->z01 = z << 16 | z + width;
            quad->z23 = z << 16 | z + width;
            break;
    }
}

void DirFunc3(Quad *quad,int width,int x,int y,int z,int textureRotation) {
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

void DirFunc4(Quad *quad,int width,int x,int y,int z,int textureRotation) {
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
            quad->xy0 = y * 0x10000 | x;
            quad->xy1 = y * 0x10000 | x;
            quad->xy2 = (y + width) * 0x10000 | x;
            quad->xy3 = (y + width) * 0x10000 | x;
            quad->z01 = z * 0x10000 | z + width;
            quad->z23 = z * 0x10000 | z + width;
            break;
        case 1:
            quad->xy0 = y * 0x10000 | x;
            quad->xy1 = (y + width) * 0x10000 | x;
            quad->xy2 = y * 0x10000 | x;
            quad->xy3 = (y + width) * 0x10000 | x;
            quad->z01 = z * 0x10000 | z;
            quad->z23 = (z + width) * 0x10000 | z + width;
            break;
        case 2:
            quad->xy0 = (y + width) * 0x10000 | x;
            quad->xy1 = (y + width) * 0x10000 | x;
            quad->xy2 = y * 0x10000 | x;
            quad->xy3 = y * 0x10000 | x;
            quad->z01 = (z + width) * 0x10000 | z;
            quad->z23 = (z + width) * 0x10000 | z;
            break;
        case 3:
            quad->xy0 = (y + width) * 0x10000 | x;
            quad->xy1 = y * 0x10000 | x;
            quad->xy2 = (y + width) * 0x10000 | x;
            quad->xy3 = y * 0x10000 | x;
            quad->z01 = (z + width) * 0x10000 | z + width;
            quad->z23 = z * 0x10000 | z;
            break;
    }
}

void DirFunc5(Quad *quad,int width,int x,int y,int z,int textureRotation) {
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
