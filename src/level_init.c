#include "common.h"

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

typedef void (*QuadFunc)(Quad* quad, int width, int x, int y, int z, int textureRotation);

typedef struct {
    short entityType;
    short pad2;
    short counter;
    char pad[250];
} CrumblingBlockEntity;

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

extern void DirFunc0(Quad* quad, int width, int x, int y, int z, int textureRotation);
extern void DirFunc1(Quad* quad, int width, int x, int y, int z, int textureRotation);
extern void DirFunc2(Quad* quad, int width, int x, int y, int z, int textureRotation);
extern void DirFunc3(Quad* quad, int width, int x, int y, int z, int textureRotation);
extern void DirFunc4(Quad* quad, int width, int x, int y, int z, int textureRotation);
extern void DirFunc5(Quad* quad, int width, int x, int y, int z, int textureRotation);
extern int GetFaceTypeAtRelativeToDir(int x, int y, int z, int dirIndex);
extern int GetFaceTypeOfBlockType(int blockType);
extern uint Rand(int param_1);
extern void ProcessMovingPlatforms2(void);
extern void ScanLevelDataForRetractableSpikes(void);

int CoordHash(int x, int y, int z, int dir, int div, int mod);
int GetRandomTextureRotation(void);
void AddQuadToAnimatedTextureChain(AnimatedTextureChain* chain, Quad** quadPtr, int initAnimFrame1, int initAnimFrame2);
void FUN_000298e0(AnimatedTextureChain* textureChain);
void InitLasers2(void);
void ProcessCubesIntoFaces(void);
void ScanLevelDataForMovingBlocks1(void);
void SetFaceData(int index, void** pointerInsideCubeState, int texIdx, int flags, int dir, int xFine, int yFine, int zFine, int textureRotation, int color);

extern POLY_FT4 shadowPrims[2][1][2][16];
extern POLY_FT4 specularPrims[2][1][16];
extern P shadowPrimPtrs[2][1][2][16];
extern P specularPrimPtrs[2][1][16];
extern Texture textures[150];
extern uint firstGuiTexture;
extern int cubeStates[16 * 256];
extern short flashingBlockEntityIndexes[64];
extern byte FIRE_BLOCK_ANIM_FLAG_DATA_END[];
extern byte INVIS_BLOCK_ANIM_FLAG_DATA_END[];
extern uint INVIS_BLOCK_ANIM_COLOR_DATA_END[];
extern byte BONUS_BLOCK_ANIM_FLAG_DATA0_END[];
extern byte FIRE_BLOCK_ANIM_FLAG_DATA[52];
extern byte INVIS_BLOCK_ANIM_FLAG_DATA[124];
extern byte BONUS_BLOCK_ANIM_FLAG_DATA0[60];
extern uint NUM_TEXTURE_ANIM_FRAMES;
extern uint INVIS_BLOCK_ANIM_COLOR_DATA[89];
extern uint BONUS_BLOCK_ANIM_FLAG_DATA1[16];
extern AnimatedTextureChain bonusBlockTextureChain;
extern AnimatedTextureChain crumblingSpecialBlockTextureChain;
extern AnimatedTextureChain fireBlockTextureChain;
extern AnimatedTextureChain invisibleBlockTextureChain;
extern uint TEXTURE_ANIM_DATA[];
extern int invisBlockVisibility[6];
extern int curWorld;
extern int gameMode;
extern int numKeysInLevel;
extern char cubeTextureMetadata[];

short MOVING_BLOCK_TEXTURE_DATA[3][4][12] = {
    {
        { 3, 1, 0, 0, 3, 1, 3, 1, 0, 0, 3, 1 },
        { 1, 3, -1, 666, 1, 3, 1, 3, 0, 0, 1, 1 },
        { -2, 1, -1, 666, -2, 1, -2, 1, -1, 666, -2, 1 },
        { 1, 1, 0, 0, 1, 1, 1, 1, -1, 666, 1, 3 },
    },
    {
        { 3, 0, 3, 0, 0, 0, 0, 0, 3, 0, 3, 0 },
        { 1, 0, 1, 0, -1, 666, 0, 0, 1, 0, 1, 0 },
        { -2, 0, -2, 0, -1, 666, -1, 666, -2, 0, -2, 0 },
        { 1, 2, 1, 2, 0, 0, -1, 666, 1, 2, 1, 2 },
    },
    {
        { 0, 0, 3, 1, 3, 0, 3, 0, 3, 1, 0, 0 },
        { 0, 0, 1, 3, 1, 0, 1, 2, 1, 1, -1, 666 },
        { -1, 666, -2, 1, -2, 0, -2, 0, -2, 1, -1, 666 },
        { -1, 666, 1, 1, 1, 2, 1, 0, 1, 3, 0, 0 },
    },
};
int D_000A2CAC = 0x666;
void* PTR_DAT_000a2cb0 = &D_000A2CAC;
FaceData* faceDataPtr = (FaceData*)FACE_DATA_BUF;
int primBufUsage = 0;
int maxPrimBufUsage = 0;
QuadFunc QUAD_FUNC_PTRS[6] = {
    DirFunc0,
    DirFunc1,
    DirFunc2,
    DirFunc3,
    DirFunc4,
    DirFunc5,
};

TgiFile* tgi;
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
short* levelData;
int quadSomethingCount;
int quadSomethingStartIndex;
short* tgiPart5;
int D_000A54F4;
void** levelExitQuadPPtr;
int numPlainTileTextureVariations;
int numMovingPlatforms;
short movingBlockEntityIndexes[16];

#define QUAD_BASE ((Quad*)QUAD_BUF)

void ProcessLevelData(void) {
    int i;
    AnimatedTextureChain* fbtc = &fireBlockTextureChain;
    AnimatedTextureChain* ibtc = &invisibleBlockTextureChain;

    numberOfCubeFaces = 0;
    numCubesInLevelTmp = 0;
    cubeCounter = 0;

    InitAnimatedTextureChain(fbtc, 0x80,
            FIRE_BLOCK_ANIM_FLAG_DATA_END - FIRE_BLOCK_ANIM_FLAG_DATA, 0,
            FIRE_BLOCK_ANIM_FLAG_DATA, NULL,
            NULL, NULL, NULL, NULL);
    InitAnimatedTextureChain(ibtc, 0x200,
            INVIS_BLOCK_ANIM_FLAG_DATA_END - INVIS_BLOCK_ANIM_FLAG_DATA, INVIS_BLOCK_ANIM_COLOR_DATA_END - INVIS_BLOCK_ANIM_COLOR_DATA,
            INVIS_BLOCK_ANIM_FLAG_DATA, NULL,
            INVIS_BLOCK_ANIM_COLOR_DATA, NULL, NULL, NULL);

    if (specialLevelType == 1) {
        uint* animData = TEXTURE_ANIM_DATA;
        InitAnimatedTextureChain(&crumblingSpecialBlockTextureChain, 0x300,
                0, NUM_TEXTURE_ANIM_FRAMES,
                NULL, NULL,
                animData + (curWorld % 2) * (NUM_TEXTURE_ANIM_FRAMES << 1), animData + ((curWorld % 2) * 2 + 1) * NUM_TEXTURE_ANIM_FRAMES, NULL, NULL);
        InitAnimatedTextureChain(&bonusBlockTextureChain, 0x300,
                BONUS_BLOCK_ANIM_FLAG_DATA0_END - BONUS_BLOCK_ANIM_FLAG_DATA0, NUM_TEXTURE_ANIM_FRAMES,
                BONUS_BLOCK_ANIM_FLAG_DATA0, BONUS_BLOCK_ANIM_FLAG_DATA1,
                animData + (curWorld % 2) * (NUM_TEXTURE_ANIM_FRAMES << 1), animData + ((curWorld % 2) * 2 + 1) * NUM_TEXTURE_ANIM_FRAMES, NULL, NULL);
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

void ProcessCubesIntoFaces(void) {
    // FIXME: eliminate as many local variables as possible
    int x;
    int y;
    int z;
    int xFine;
    int yFine;
    int zFine;
    int dir;

    int textureIdx;
    int itemType;
    int style;
    int newCube;
    int blockType;
    int type;
    int relType;
    int isNonempty;
    int flags;
    int textureRotation;

    int csptr;

    levelExitQuadPPtr = &PTR_DAT_000a2cb0;
    for (z = 1; z < 33; z++) {
        for (y = 1; y < 33; y++) {
            for (x = 1; x < 33; x++) {
                blockType = levelData[x * 34 * 34 + y * 34 + z];
                style = GetFaceTypeOfBlockType(blockType);
                if (!((style >= 0 && style <= 1) || (style >= 2 && style <= 3) || style == 4 || (style >= 6 && style <= 7))) {
                    continue;
                }

                isNonempty = 0;
                newCube = 0;
                dir = 0;
                csptr = (int)cubeStates;
                xFine = x << 9;
                yFine = y << 9;
                zFine = z << 9;
                for (; dir < 6; dir++, csptr += 4) {
                    if (style == 0 && blockType >= 5) {
                        type = entityData[((blockType - 5) * 128 + 1) + dir * 16];
                        if (type == OBJ_PLAYER_SPAWN_PAUSED) {
                            type = OBJ_TIMER_PAUSE;
                        }
                        if (type >= OBJ_SLOW_STAR) {
                            // enemy
                            type = 0;
                        }
                    } else {
                        type = style;
                        if (style >= 5) {
                            type = -style;
                        }
                    }
                    relType = GetFaceTypeAtRelativeToDir(x, y, z, dir);
                    switch (type) {
                        case OBJ_INVISIBLE_PATCH:
                            if (!((relType >= 0 && relType <= 1) || (relType >= 2 && relType <= 3) || relType == 4)) {
                            // 6 == invisible
                                SetFaceData(numberOfCubeFaces++, (void**)(cubeCounter * 64 + csptr), quadSomethingStartIndex + 6, 0x100,
                                            dir, xFine, yFine, zFine, 0, 0x808080);
                                AddQuadToAnimatedTextureChain(&invisibleBlockTextureChain, (Quad**)(cubeCounter * 64 + csptr), -1,
                                                              CoordHash(xFine, yFine, zFine, dir, 0x40,
                                                                        INVIS_BLOCK_ANIM_COLOR_DATA_END - INVIS_BLOCK_ANIM_COLOR_DATA));
                                isNonempty = 1;
                                ((char*)&cubeStates[cubeCounter * 16 + 12])[dir] = 0;
                            }
                            break;
                        case -OBJ_CRUMBLING_BLOCK:
                            flags = 0xc; // QF_BACKFACE_CULL | QF_GOURAUD
                            if (!((relType >= 0 && relType <= 1) || relType == 2 || relType == 4 || relType == 6)) {
                                flags = 0xd; // QF_ACTIVE | QF_BACKFACE_CULL | QF_GOURAUD
                            }
                            if (specialLevelType == 1) {
                                AddQuadToAnimatedTextureChain(&crumblingSpecialBlockTextureChain, (Quad**)(cubeCounter * 64 + csptr), -1,
                                                              CoordHash(xFine, yFine, zFine, dir, 0x40, NUM_TEXTURE_ANIM_FRAMES));
                                flags &= ~8; // QF_BACKFACE_CULL
                            }
                            if (specialLevelType == 2) {
                                flags |= 2; // QF_SEMITRANSPARENT
                            }
                        // 4 == crumbling
                            SetFaceData(numberOfCubeFaces++, (void**)(cubeCounter * 64 + csptr), quadSomethingStartIndex + 4, flags, dir,
                                        xFine, yFine, zFine, 0, 0x808080);
                            isNonempty = 1;
                            newCube = 1;
                            ((char*)&cubeStates[cubeCounter * 16 + 12])[dir] = 0;
                            break;

                        case -OBJ_EXIT:
                        // FIXME: flashing or exit - which one is it?
                            flags = 0xd; // QF_ACTIVE | QF_BACKFACE_CULL | QF_GOURAUD
                            textureIdx = quadSomethingStartIndex + tgi->numFixedTextures;
                            textureRotation = GetRandomTextureRotation();
                            if (specialLevelType == 1) {
                                AddQuadToAnimatedTextureChain(&bonusBlockTextureChain, (Quad**)(cubeCounter * 64 + csptr), -1,
                                                              CoordHash(xFine, yFine, zFine, dir, 0x40, NUM_TEXTURE_ANIM_FRAMES));
                                flags = 5; // QF_ACTIVE | QF_BACKFACE_CULL
                            }
                            SetFaceData(numberOfCubeFaces++, (void**)(cubeCounter * 64 + csptr), textureIdx, flags, dir, xFine, yFine,
                                        zFine, textureRotation, 0xa0a0a0);
                            isNonempty = 1;
                            newCube = 1;
                            ((char*)&cubeStates[cubeCounter * 16 + 12])[dir] = 0;
                            break;

                        default:
                            if (!((relType >= 0 && relType < 2) || relType == 2 || relType == 4)) {
                                flags = 0xd; // QF_ACTIVE | QF_BACKFACE_CULL | QF_GOURAUD
                                textureRotation = 0;
                                textureIdx = quadSomethingStartIndex + tgi->numFixedTextures;
                                if (type == 0) {
                                    textureIdx += Rand(numPlainTileTextureVariations);
                                    textureRotation = GetRandomTextureRotation();
                                } else {
                                    textureIdx += numPlainTileTextureVariations + type;
                                    if (type > 19) {
                                    // item or enemy
                                        flags = 0x100000d; // QF_ACTIVE | QF_BACKFACE_CULL | QF_GOURAUD | QF_ITEM_SHADOW
                                    }
                                }
                                if (specialLevelType == 1) {
                                    flags &= ~8; // QF_GOURAUD
                                    if (type == 0 || type > 19) {
                                        textureRotation = Rand(4);
                                        AddQuadToAnimatedTextureChain(&bonusBlockTextureChain, (Quad**)(cubeCounter * 64 + csptr), -1,
                                                                      CoordHash(xFine, yFine, zFine, dir, 0x40, NUM_TEXTURE_ANIM_FRAMES));
                                        flags &= 0xffffff;
                                        if (type > 19) {
                                            flags |= 0x10000; // QF_BONUS_ITEM_SHADOW
                                        }
                                    } else {
                                        AddQuadToAnimatedTextureChain(&crumblingSpecialBlockTextureChain,
                                                                      (Quad**)(cubeCounter * 64 + csptr), -1,
                                                                      CoordHash(xFine, yFine, zFine, dir, 0x40, NUM_TEXTURE_ANIM_FRAMES));
                                    }
                                }
                                if (relType == OBJ_CRUMBLING_BLOCK) {
                                    flags &= ~1; // QF_ACTIVE
                                }
                                if (specialLevelType == 2) {
                                    flags |= 2; // QF_SEMITRANSPARENT
                                }
                                if (style == 2 && blockType >= 5) {
                                    itemType = entityData[((blockType - 5) * 128 + 1) + dir * 16];
                                    if (itemType >= 20 && itemType < 50 && itemType != 26 && itemType != 28 && itemType != 30) {
                                        flags |= 0x1000000; // QF_ITEM_SHADOW
                                    }
                                }

                                if (type == OBJ_EXIT) {
                                    levelExitQuadPPtr = (void**)(cubeCounter * 64 + csptr);
                                }
                                SetFaceData(numberOfCubeFaces++, (void**)(cubeCounter * 64 + csptr), textureIdx, flags, dir, xFine, yFine,
                                            zFine, textureRotation, 0x808080);
                                isNonempty = 1;
                                newCube = 1;
                                ((char*)&cubeStates[cubeCounter * 16 + 12])[dir] = 0;
                                if (type == OBJ_FIRE_PATCH) {
                                    AddQuadToAnimatedTextureChain(&fireBlockTextureChain, (Quad**)(cubeCounter * 64 + csptr), -1, 0);
                                }
                            }
                    }
                }
                if (isNonempty == 1) {
                    int t = cubeCounter;
                    CUBE_INDEX_AT(x, y, z) = t;
                    cubeStates[t * 16 + 14] = 0;
                    cubeStates[t * 16 + 15] = style;
                    cubeCounter = t + 1;
                }
                if (newCube == 1) {
                    numCubesInLevelTmp++;
                }
            }
        }
    }
}

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
    int dir;

    numKeysInLevel = 0;
    wasSpecialLevel = specialLevelType;
    for (i = 0; i < numEntities; i++) {
        if (entityData[i * 128] >= 0 && entityData[i * 128] < 5) {
            for (dir = 0; dir < 6; dir++) {
                if (entityData[128 * i + 16 * dir + 1] == OBJ_KEY) {
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

void ScanLevelDataForMovingBlocks1(void) {
    int i;
    int x_inc;
    int y_inc;
    int z_inc;
    int length;
    int flags;
    short* entity;
    int j;
    short* pStuff;
    int texDataIdx;
    int dir;
    int val;

    switch (specialLevelType) {
        default:
            flags = 0xd; // QF_ACTIVE | QF_BACKFACE_CULL | QF_GOURAUD
            break;
        case 1:
            flags = 0x20005; // QF_ACTIVE | QF_BACKFACE_CULL | QF_VISITED_TYPE(1)
            break;
        case 2:
            flags = 0xf; // QF_ACTIVE | QF_SEMITRANSPARENT | QF_BACKFACE_CULL | QF_GOURAUD
            break;
    }

    numMovingPlatforms = 0;
    for (i = 0; i < numEntities; i++) {
        if (entityData[i * 128] != 5) {
            continue;
        }
        entity = &entityData[i * 128];
        movingBlockEntityIndexes[numMovingPlatforms++] = (short)i;
        length = (int)entity[17];

        switch (entity[2]) {
            case 1:
                texDataIdx = 0;
                x_inc = 512;
                y_inc = 0;
                z_inc = 0;
                break;
            case 2:
                texDataIdx = 1;
                x_inc = 0;
                y_inc = 512;
                z_inc = 0;
                break;
            default:
                x_inc = 0;
                y_inc = 0;
                z_inc = 512;
                texDataIdx = 2;
                break;
        }

        for (j = 0; j < length; j++) {
            if (length == 1)
                pStuff = MOVING_BLOCK_TEXTURE_DATA[texDataIdx][0];
            else if (j == 0)
                pStuff = MOVING_BLOCK_TEXTURE_DATA[texDataIdx][1];
            else if (j < length - 1)
                pStuff = MOVING_BLOCK_TEXTURE_DATA[texDataIdx][2];
            else
                pStuff = MOVING_BLOCK_TEXTURE_DATA[texDataIdx][3];

            for (dir = 0; dir < 6; dir++) {
                val = pStuff[dir * 2];
                if (val == -1) {
                    ((Quad**)(entity + 71 + j * 12))[dir] = (Quad*)0xffffffff;
                } else {
                    if (val == -2) {
                        SetFaceData(numberOfCubeFaces, &((Quad**)(entity + 71 + j * 12))[dir],
                                    quadSomethingStartIndex + tgi->numFixedTextures + Rand(numPlainTileTextureVariations), flags, dir, 0, 0,
                                    0, pStuff[dir * 2 + 1], 0x808080);
                    } else {
                        SetFaceData(numberOfCubeFaces, &((Quad**)(entity + 71 + j * 12))[dir], quadSomethingStartIndex + val, flags, dir, 0,
                                    0, 0, pStuff[dir * 2 + 1], 0x808080);
                    }

                    if (specialLevelType == 1) {
                        AddQuadToAnimatedTextureChain(
                            &crumblingSpecialBlockTextureChain, &((Quad**)(entity + 71 + j * 12))[dir], -1,
                            CoordHash(j * x_inc + 512, j * y_inc + 512, j * z_inc + 512, dir, 64, NUM_TEXTURE_ANIM_FRAMES));
                    }
                    numberOfCubeFaces++;
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/level_init", InitLasers2);

void ScanLevelDataForCrumblingBlocks(void) {
    int i;

    numCrumblingBlocks = 0;
    for (i = 0; i < numEntities; i++) {
        if (((CrumblingBlockEntity*)entityData)[i].entityType == OBJ_CRUMBLING_BLOCK) {
            crumblingBlockEntityIndexes[numCrumblingBlocks++] = i;
            ((CrumblingBlockEntity*)entityData)[i].counter = 0x200;
        }
    }
}

void ScanLevelDataForFlashingBlocks(void) {
    Quad* quad;
    int dir;
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
        if (eb->entityType == OBJ_FLASHING_BLOCK) {
            flashingBlockEntityIndexes[numFlashingBlocks++] = i;
            x = eb->x;
            y = eb->y;
            z = eb->z;
            eb->cubeType = levelData[x * 1156 + y * 34 + z];
            ci = CUBE_INDEX_AT(x, y, z);
            initState = eb->initState;
            if (initState < 2) {
                for (dir = 0; dir < 6; dir++) {
                    *(u16*)cubeStates[16 * ci + dir] = 0x10e;
                }
                eb->counter = 76 - initState * 47;
                eb->state = 0;
                levelData[x * 1156 + y * 34 + z] = -1;
            } else {
                for (dir = 0; dir < 6; dir++) {
                    quad = cubeStates[16 * ci + dir];
                    *(u8*)&quad->flags = 0xf; // QF_ACTIVE | QF_SEMITRANSPARENT | QF_BACKFACE_CULL | QF_GOURAUD
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

void CopyQuadData(void) {
    FaceData* fd;
    Quad* q;
    int i;
    int j = 0;
    int dir;
    int idx;

    for (i = quadSomethingStartIndex; i < quadSomethingStartIndex + quadSomethingCount; i++) {
        for (fd = faceDataPtr; fd < faceDataPtr + numberOfCubeFaces; fd++) {
            if (fd->texIdx != i) {
                continue;
            }
            // Sets cubeState->quadPtr[dir] to final Quad
            *fd->pointerInsideCubeState = q = &QUAD_BASE[j];
            q->flags.i32 = fd->flags;
            *(int*)&q->otagIndex = 0xffff;
            dir = fd->dir;
            idx = (tgiPart5[i * 2] * 3 + tgiPart5[i * 2 + 1] * tgi->sideBrightness[dir]) * 32;
            q->metadata = &cubeTextureMetadata[idx];
            q->color = fd->color;
            j++;
            QUAD_FUNC_PTRS[dir](q, 512, fd->x, fd->y, fd->z, fd->textureRotation);
        }
    }
    // permuter hack
    idx = j;
    D_000A54F4 = idx;
}


void InitAnimatedTextureChain(
        AnimatedTextureChain* dst,
        int maxQuads,
        int numTexAnimFrames,
        int numColorAnimFrames,
        char* texAnimData0,
        char* texAnimData1,
        uint* colorAnimData0,
        uint* colorAnimData1,
        uint* colorAnimData2,
        uint* colorAnimData3) {
    int* d = (int*)dst;
    d[0] = (int)((u8*)dst + 40 + maxQuads * 8);
    d[1] = (int)(dst + 1);
    d[2] = numTexAnimFrames;
    d[3] = numColorAnimFrames;
    d[4] = (int)texAnimData0;
    d[5] = (int)texAnimData1;
    d[6] = (int)colorAnimData0;
    d[7] = (int)colorAnimData1;
    d[8] = (int)colorAnimData2;
    d[9] = (int)colorAnimData3;
}

void AddQuadToAnimatedTextureChain(AnimatedTextureChain* chain, Quad** quadPtr, int initAnimFrame1,
                                   int initAnimFrame2) {
    void** tce = (void**)chain->entries;
    *tce++ = (void*)quadPtr;
    if (initAnimFrame1 == -1) {
        initAnimFrame1 = Rand(chain->numFlagAnimFrames);
    }
    if (initAnimFrame2 == -1) {
        initAnimFrame2 = Rand(chain->numColorAnimFrames);
    }
    *tce++ = (void*)(initAnimFrame2 << 16 | initAnimFrame1);
    chain->entries = (TextureChainEntry*)tce;
}

void FUN_000298e0(AnimatedTextureChain* tc) {
    u32* ptr = tc + 1;
    u32* end = tc->entries;
    while (ptr < end) {
        *ptr = *(u32*)*ptr;
        ptr += 2;
    }
}

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

void ScanLevelDataForBlinkingEntities(void) {
    int j, i;
    short* ent;

    for (i = 0; i < numEntities; i++) {
        if (entityData[i * 128] == 0) {
            for (j = 0; j < 6; j++) {
                ent = (short*)((i * 128 + j * 16) * 2 + (int)entityData);
                switch (ent[1]) {
                    case OBJ_EXIT:
                        ent[15] = 16;
                        ent[16] = 255;
                        break;
                    case OBJ_TRANSPORTER: // xxx: can this trigger?
                        ent[15] = 16;
                        ent[16] = 255;
                        break;
                    case OBJ_BUTTON:
                        ent[15] = 16;
                        ent[16] = 255;
                        break;
                }
            }
        }
    }
}
