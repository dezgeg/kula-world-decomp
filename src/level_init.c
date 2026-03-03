#include "common.h"

typedef struct P {
    ulong* p0;
    ulong* p1;
} P;

typedef struct FaceData {
    void **pointerInsideCubeState;
    short texIdx;
    short dir;
    short x, y, z;
    short textureRotation;
    int flags;
    int color;
} FaceData;

typedef struct {
    short entityType;
    short pad2;
    short counter;
    char pad[250];
} CrumblingBlockEntity;

extern uint Rand(int param_1);

extern POLY_FT4 shadowPrims[2][1][2][16];
extern POLY_FT4 specularPrims[2][1][16];
extern P shadowPrimPtrs[2][1][2][16];
extern P specularPrimPtrs[2][1][16];
extern Texture textures[150];
extern Texture textures[150];
extern uint firstGuiTexture;
extern uint firstGuiTexture;

TgiFile* tgi;
FaceData *faceDataPtr;
CrumblingBlockEntity *entityData; // XXX: Fix type
int numCrumblingBlocks;
short numEntities;
short crumblingBlockEntityIndexes[64];

INCLUDE_ASM("asm/nonmatchings/level_init", ProcessLevelData);

INCLUDE_ASM("asm/nonmatchings/level_init", ProcessCubesIntoFaces);

int CoordHash(int x,int y,int z,int dir,int div,int mod) {
    switch(dir) {
    case 0:
        z = z + -0x100;
        break;
    case 1:
        x = x + 0x100;
        break;
    case 2:
        y = y + 0x100;
        break;
    case 3:
        y = y + -0x100;
        break;
    case 4:
        x = x + -0x100;
        break;
    case 5:
        z = z + 0x100;
    }
    return ((x + y + z) / (div * 3)) % mod;
}

int GetRandomTextureRotation(void) {
    int ret;

    switch (tgi->randomTileRotation) {
        case 0:
            return 0;
        case 1:
            ret = Rand(2);
            if (ret == 1) {
                ret = 2;
            }
            return ret;
        default:
            return Rand(4);
    }
}

INCLUDE_ASM("asm/nonmatchings/level_init", ParseKeysAndSpecialLevelFromItemData);

INCLUDE_ASM("asm/nonmatchings/level_init", ScanLevelDataForMovingBlocks1);

INCLUDE_ASM("asm/nonmatchings/level_init", InitLasers2);

void ScanLevelDataForCrumblingBlocks(void) {
    int i;

    numCrumblingBlocks = 0;
    for (i = 0; i < numEntities; i++) {
        if (entityData[i].entityType == 6) {
            crumblingBlockEntityIndexes[numCrumblingBlocks++] = i;
            entityData[i].counter = 0x200;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/level_init", ScanLevelDataForFlashingBlocks);

void SetFaceData(int index, void **pointerInsideCubeState, int texIdx, int flags, int dir,
                 int xFine, int yFine, int zFine, int textureRotation, int color) {
    FaceData *fd = &faceDataPtr[index];
    unsigned char *ptr;

    if (pointerInsideCubeState != (void **)0xffffffff) {
        fd->pointerInsideCubeState = pointerInsideCubeState;
        *pointerInsideCubeState = (void *)index;
    }

    fd = (FaceData *)((char *)fd + 4);
    *(short *)fd = texIdx;
    fd = (FaceData *)((char *)fd + 2);
    *(short *)fd = dir;
    fd = (FaceData *)((char *)fd + 2);
    *(short *)fd = xFine;
    fd = (FaceData *)((char *)fd + 2);
    *(short *)fd = yFine;
    fd = (FaceData *)((char *)fd + 2);
    *(short *)fd = zFine;
    fd = (FaceData *)((char *)fd + 2);
    *(short *)fd = textureRotation;
    fd = (FaceData *)((char *)fd + 2);
    *(int *)fd = flags;
    ((int *)fd)[1] = color;
}

INCLUDE_ASM("asm/nonmatchings/level_init", CopyQuadData);

void InitAnimatedTextureChain(AnimatedTextureChain *dst, int numEntries, int numFrames1, int numFrames2, void *ptr1,
                             void *ptr2, void *ptr3, void *ptr4, int zero9, int zero10) {
    int *d = (int *)dst;
    d[0] = (int)((u8*)dst + 40 + numEntries * 8);
    d[1] = (int)(dst + 1);
    d[2] = numFrames1;
    d[3] = numFrames2;
    d[4] = (int)ptr1;
    d[5] = (int)ptr2;
    d[6] = (int)ptr3;
    d[7] = (int)ptr4;
    d[8] = zero9;
    d[9] = zero10;
}

void AddQuadToAnimatedTextureChain(AnimatedTextureChain *chain, Quad **quadPtr, int initAnimFrame1, int initAnimFrame2) {
    void **tce = (void **)chain->entries;
    *tce++ = (void *)quadPtr;
    if (initAnimFrame1 == -1) {
        initAnimFrame1 = Rand(chain->numFrames1);
    }
    if (initAnimFrame2 == -1) {
        initAnimFrame2 = Rand(chain->numFrames2);
    }
    *tce++ = (void *)(initAnimFrame2 << 16 | initAnimFrame1);
    chain->entries = (TextureChainEntry *)tce;
}

INCLUDE_ASM("asm/nonmatchings/level_init", FUN_000298e0);

void InitShadowSprites(void) {
    int i;
    int j;
    int k;
    int l;
    int tex;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 1; j++) {
            for (k = 0; k < 2; k++) {
                for (l = 0; l < 16; l++) {
                    tex = k + 14;
                    shadowPrimPtrs[i][j][k][l].p0 = 0xffffffff;
                    setPolyFT4(&shadowPrims[i][j][k][l]);
                    setUV4(&shadowPrims[i][j][k][l],
                            textures[firstGuiTexture + tex].u, textures[firstGuiTexture + tex].v,
                            textures[firstGuiTexture + tex].u + textures[firstGuiTexture + tex].w - 1, textures[firstGuiTexture + tex].v,
                            textures[firstGuiTexture + tex].u, textures[firstGuiTexture + tex].v + textures[firstGuiTexture + tex].h - 1,
                            textures[firstGuiTexture + tex].u + textures[firstGuiTexture + tex].w - 1, textures[firstGuiTexture + tex].v + textures[firstGuiTexture + tex].h - 1);
                    SetSemiTrans(&shadowPrims[i][j][k][l], textures[firstGuiTexture + tex].semitrans);
                    SetShadeTex(&shadowPrims[i][j][k][l], 0);
                    shadowPrims[i][j][k][l].tpage = textures[firstGuiTexture + tex].tpage;
                    shadowPrims[i][j][k][l].clut = textures[firstGuiTexture + tex].clut;
                }
            }
        }
    }


}

void InitPlayerSpecularSprite(void) {
    int i;
    int j;
    int k;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 1; j++) {
            for (k = 0; k < 16; k++) {
                specularPrimPtrs[i][j][k].p0 = 0xffffffff;
                setPolyFT4(&specularPrims[i][j][k]);
                setUV4(&specularPrims[i][j][k],
                        textures[firstGuiTexture + 18].u, textures[firstGuiTexture + 18].v,
                        textures[firstGuiTexture + 18].u + textures[firstGuiTexture + 18].w - 1, textures[firstGuiTexture + 18].v,
                        textures[firstGuiTexture + 18].u, textures[firstGuiTexture + 18].v + textures[firstGuiTexture + 18].h - 1,
                        textures[firstGuiTexture + 18].u + textures[firstGuiTexture + 18].w - 1, textures[firstGuiTexture + 18].v + textures[firstGuiTexture + 18].h - 1);
                SetSemiTrans(&specularPrims[i][j][k], textures[firstGuiTexture + 18].semitrans);
                SetShadeTex(&specularPrims[i][j][k], 0);
                setRGB0(&specularPrims[i][j][k], 0x80, 0x80, 0x80);
                specularPrims[i][j][k].tpage = textures[firstGuiTexture + 18].tpage;
                specularPrims[i][j][k].clut = textures[firstGuiTexture + 18].clut;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/level_init", ScanLevelDataForBlinkingEntities);
