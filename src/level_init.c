#include "common.h"

#define CUBE_INDEX_AT(x, y, z) (*(short*)(0x1af000 + (x) * 34 * 34 * 2 + (y) * 34 * 2 + (z) * 2))

typedef struct P {
    ulong* p0;
    ulong* p1;
} P;

typedef struct FaceData {
    void** pointerInsideCubeState;
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

typedef struct CubeState {
    Quad* quadPtrs[6];
    int field1_0x18[6];
    char sideVisited[6];
    char field3_0x36;
    char field4_0x37;
    int visited;
    int field6_0x3c;
} CubeState;

typedef struct FlashingEntity {
    short entityType;
    short cubeType;
    short initState;
    short state;
    short counter;
    short pad[120];
    short x;
    short y;
    short z;
} FlashingEntity;

extern uint Rand(int param_1);

extern POLY_FT4 shadowPrims[2][1][2][16];
extern POLY_FT4 specularPrims[2][1][16];
extern P shadowPrimPtrs[2][1][2][16];
extern P specularPrimPtrs[2][1][16];
extern Texture textures[150];
extern Texture textures[150];
extern uint firstGuiTexture;
extern uint firstGuiTexture;
extern CubeState cubeStates[256];
extern short flashingBlockEntityIndexes[64];

TgiFile* tgi;
FaceData* faceDataPtr;
short* entityData;
int numCrumblingBlocks;
short numEntities;
short crumblingBlockEntityIndexes[64];
int specialLevelType;
int wasSpecialLevel;
int numFlashingBlocks;
int cubeCounter;
int numCubesInLevelTmp;
int numCubesRemainingInLevel[5];
int numberOfCubeFaces;

extern byte D_000735B5[];
extern byte D_00073634[];
extern uint D_00073798[];
extern byte D_000737D4[];

extern byte BYTE_ARRAY_00073588[52];
extern byte BYTE_ARRAY_000735bc[124];
extern byte BYTE_ARRAY_0007379c[60];
extern uint SIZE_OF_UNK_ENTRY;
extern uint UINT_ARRAY_00073638[89];
extern uint UINT_ARRAY_000737d8[16];
extern AnimatedTextureChain bonusBlockTextureChain;
extern AnimatedTextureChain crumblingSpecialBlockTextureChain;
extern int curWorld;
extern AnimatedTextureChain fireBlockTextureChain;
extern AnimatedTextureChain invisibleBlockTextureChain;
extern uint UNK_ENTRIES[];

extern void ProcessCubesIntoFaces(void);
extern void ScanLevelDataForMovingBlocks1(void);
extern void InitLasers2(void);
extern void ProcessMovingPlatforms2(void);
extern void FUN_000298e0(AnimatedTextureChain * textureChain);
extern void ScanLevelDataForRetractableSpikes(void);

extern int invisBlockVisibility[6];
extern int gameMode;
extern int numKeysInLevel;
short* levelData;

void ProcessLevelData(void) {
    int i;
    AnimatedTextureChain* fbtc = &fireBlockTextureChain;
    AnimatedTextureChain* ibtc = &invisibleBlockTextureChain;

    numberOfCubeFaces = 0;
    numCubesInLevelTmp = 0;
    cubeCounter = 0;

    InitAnimatedTextureChain(fbtc, 0x80, D_000735B5 - BYTE_ARRAY_00073588, 0, BYTE_ARRAY_00073588, NULL, NULL, NULL, 0, 0);
    InitAnimatedTextureChain(ibtc, 0x200, D_00073634 - BYTE_ARRAY_000735bc, D_00073798 - UINT_ARRAY_00073638, BYTE_ARRAY_000735bc, NULL, UINT_ARRAY_00073638, NULL, 0, 0);

    if (specialLevelType == 1) {
        uint* unkEntries = UNK_ENTRIES;
        InitAnimatedTextureChain(&crumblingSpecialBlockTextureChain, 0x300, 0, SIZE_OF_UNK_ENTRY, NULL, NULL,
                                 unkEntries + (curWorld % 2) * (SIZE_OF_UNK_ENTRY << 1),
                                 unkEntries + ((curWorld % 2) * 2 + 1) * SIZE_OF_UNK_ENTRY, 0, 0);
        InitAnimatedTextureChain(&bonusBlockTextureChain, 0x300, D_000737D4 - BYTE_ARRAY_0007379c, SIZE_OF_UNK_ENTRY, BYTE_ARRAY_0007379c,
                                 UINT_ARRAY_000737d8,
                                 unkEntries + (curWorld % 2) * (SIZE_OF_UNK_ENTRY << 1),
                                 unkEntries + ((curWorld % 2) * 2 + 1) * SIZE_OF_UNK_ENTRY, 0, 0);
    }

    ProcessCubesIntoFaces();
    ScanLevelDataForMovingBlocks1();
    InitLasers2();
    CopyQuadData();
    ScanLevelDataForCrumblingBlocks();
    ScanLevelDataForFlashingBlocks();
    ProcessMovingPlatforms2();

    FUN_000298e0(fbtc);
    FUN_000298e0(ibtc);
    if (specialLevelType == 1) {
        FUN_000298e0(&crumblingSpecialBlockTextureChain);
        FUN_000298e0(&bonusBlockTextureChain);
    }

    ScanLevelDataForRetractableSpikes();

    numCubesRemainingInLevel[0] = numCubesInLevelTmp;
    for (i = 1; i < 4; i++) {
        numCubesRemainingInLevel[i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/level_init", ProcessCubesIntoFaces);

int CoordHash(int x, int y, int z, int dir, int div, int mod) {
    switch (dir) {
        case 0:
            z -= 0x100;
            break;
        case 1:
            x += 0x100;
            break;
        case 2:
            y += 0x100;
            break;
        case 3:
            y -= 0x100;
            break;
        case 4:
            x -= 0x100;
            break;
        case 5:
            z += 0x100;
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

void ParseKeysAndSpecialLevelFromItemData(void) {
    int i;
    int j;

    numKeysInLevel = 0;
    wasSpecialLevel = specialLevelType;
    for (i = 0; i < numEntities; i++) {
        if (entityData[i * 128] >= 0 && entityData[i * 128] < 5) {
            for (j = 0; j < 6; j++) {
                if (entityData[128 * i + 16 * j + 1] == 31) {
                    numKeysInLevel++;
                }
            }
        }
    }
    if (numKeysInLevel == 0) {
        specialLevelType = 1;
    } else {
        specialLevelType = 0;
    }
    invisBlockVisibility[0] = 0x2000;
    invisBlockVisibility[1] = 0x2000;
    invisBlockVisibility[2] = 0x2000;
    invisBlockVisibility[3] = 0x15e;
    invisBlockVisibility[4] = 0x200;
    invisBlockVisibility[5] = 0;
    for (i = 0; i < numEntities; i++) {
        if (entityData[128 * i] == 9) {
            if (entityData[128 * i + 1] == 1) {
                specialLevelType = 2;
            }
            if (entityData[128 * i + 2] == 1) {
                invisBlockVisibility[3] = 0x500;
                invisBlockVisibility[4] = 0x700;
                invisBlockVisibility[5] = 1;
            }
        }
    }
    if (gameMode == 1) {
        specialLevelType = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/level_init", ScanLevelDataForMovingBlocks1);

INCLUDE_ASM("asm/nonmatchings/level_init", InitLasers2);

void ScanLevelDataForCrumblingBlocks(void) {
    int i;

    numCrumblingBlocks = 0;
    for (i = 0; i < numEntities; i++) {
        if (((CrumblingBlockEntity*)entityData)[i].entityType == 6) {
            crumblingBlockEntityIndexes[numCrumblingBlocks++] = i;
            ((CrumblingBlockEntity*)entityData)[i].counter = 0x200;
        }
    }
}

void ScanLevelDataForFlashingBlocks(void) {
    Quad* quad;
    int j;
    FlashingEntity* eb;
    int x;
    int y;
    int z;
    int i;
    int ci;
    int initState;

    numFlashingBlocks = 0;
    for (i = 0; i < numEntities; i++) {
        eb = (FlashingEntity*)(entityData + i * 128);
        if (eb->entityType == 7) {
            flashingBlockEntityIndexes[numFlashingBlocks++] = i;
            x = eb->x;
            y = eb->y;
            z = eb->z;
            eb->cubeType = levelData[x * 1156 + y * 34 + z];
            ci = CUBE_INDEX_AT(x, y, z);
            initState = eb->initState;
            if (initState < 2) {
                for (j = 0; j < 6; j++) {
                    *(u16*)&cubeStates[0].quadPtrs[16 * ci + j]->flags = 0x10e;
                }
                eb->counter = 76 - initState * 47;
                eb->state = 0;
                levelData[x * 1156 + y * 34 + z] = -1;
            } else {
                for (j = 0; j < 6; j++) {
                    quad = cubeStates[0].quadPtrs[16 * ci + j];
                    *(u8*)&quad->flags = 0xf;
                    quad->color = 0x808080;
                }
                eb->state = 3;
                eb->counter = 164 - initState * 47;
            }
        }
    }
}

void SetFaceData(int index, void** pointerInsideCubeState, int texIdx, int flags, int dir,
                 int xFine, int yFine, int zFine, int textureRotation, int color) {
    FaceData* fd = &faceDataPtr[index];
    unsigned char* ptr;

    if (pointerInsideCubeState != (void**)0xffffffff) {
        fd->pointerInsideCubeState = pointerInsideCubeState;
        *pointerInsideCubeState = (void*)index;
    }

    fd = (FaceData*)((char*)fd + 4);
    *(short*)fd = texIdx;
    fd = (FaceData*)((char*)fd + 2);
    *(short*)fd = dir;
    fd = (FaceData*)((char*)fd + 2);
    *(short*)fd = xFine;
    fd = (FaceData*)((char*)fd + 2);
    *(short*)fd = yFine;
    fd = (FaceData*)((char*)fd + 2);
    *(short*)fd = zFine;
    fd = (FaceData*)((char*)fd + 2);
    *(short*)fd = textureRotation;
    fd = (FaceData*)((char*)fd + 2);
    *(int*)fd = flags;
    ((int*)fd)[1] = color;
}

INCLUDE_ASM("asm/nonmatchings/level_init", CopyQuadData);

void InitAnimatedTextureChain(AnimatedTextureChain* dst, int numEntries, int numFrames1,
                              int numFrames2, void* ptr1, void* ptr2, void* ptr3, void* ptr4,
                              int zero9, int zero10) {
    int* d = (int*)dst;
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

void AddQuadToAnimatedTextureChain(AnimatedTextureChain* chain, Quad** quadPtr, int initAnimFrame1,
                                   int initAnimFrame2) {
    void** tce = (void**)chain->entries;
    *tce++ = (void*)quadPtr;
    if (initAnimFrame1 == -1) {
        initAnimFrame1 = Rand(chain->numFrames1);
    }
    if (initAnimFrame2 == -1) {
        initAnimFrame2 = Rand(chain->numFrames2);
    }
    *tce++ = (void*)(initAnimFrame2 << 16 | initAnimFrame1);
    chain->entries = (TextureChainEntry*)tce;
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
