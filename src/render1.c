#include "common.h"

#define CUBE_TYPE_AT(x, y, z) levelData[(x) * 34 * 34 + (y) * 34 + (z)]

typedef struct {
    char data[0x15c];
} GemRandomSparkleEffect;

typedef struct MovingPlatformEntity2 {
    short tag;
    short movementDirection;
    short dir;
    short unk6;
    short startX;
    short startY;
    short startZ;
    short endX;
    short endY;
    short endZ;
    char pad1[12];
    short flags;
    short length;
    short velocity;
    short counter;
    short cubeStyle;
    short velX;
    short velY;
    short velZ;
    char pad2[92];
    Quad* quads[4][6];
    char pad3[2];
    short posX;
    short posY;
    short posZ;
    char pad4[12];
} MovingPlatformEntity2;

typedef struct FlashingEntity {
    short tag;
    short cubeType;
    short initState;
    short state;
    short counter;
    short pad[120];
    short x;
    short y;
    short z;
} FlashingEntity;

typedef struct CrumblingBlockEntity {
    short entityType;
    short state;
    short counter;
    byte pad[244];
    short x;
    short y;
    short z;
} CrumblingBlockEntity;

typedef void (*QuadFunc)(Quad* quad, int width, int x, int y, int z, int textureRotation);

extern int FlipDir(int dir);
extern int GetCubeIndexRelativeToDir(int x, int y, int z, int idx);
extern int GetFaceTypeAtRelativeToDir(int x, int y, int z, int dirIndex);
extern int UpdateSunglassModeDisabling(void);
extern void AnimateTextures(AnimatedTextureChain* param_1);
extern void CreditsRenderModels(void);
extern void DisableLightEffect(int param_1);
extern void DrawLasers(void* otag);
extern void DrawPlayerSpecularSprites(void);
extern void DrawScreenFade(void);
extern void DrawShadowSprites(void);
extern void EnableScreenShake(int param_1, int param_2, int param_3);
extern void MakeOtagDoublyLinked(void* otag);
extern void ProcessCrumblingBlocks(void);
extern void ProcessDisabledLightEffects(void);
extern void ProcessEnabledLightEffects(void);
extern void ProcessEnemiesRenderItemsAndCheckFellOff(void);
extern void ProcessFlashingBlocks(void);
extern void ProcessInvisibleBlockVisibility(InvisBlockVisibility* param_1);
extern void ProcessLightEffects(void);
extern void ProcessMovingPlatforms2(void);
extern void ProcessRecentlyVisitedCubes(void);
extern void ProcessRetractableSpikes(void);
extern void ProcessTurningMotionBlur(void);
extern void RenderBonusBackgroundPart1(void* ot);
extern void RenderBonusBackgroundPart2(void);
extern void RenderDispList(void* p);
extern void RenderItems_(void);
extern void RenderLevelGeometryQuads(void* otag);
extern void RenderNonSpecialBackground(void* ot);
extern void RenderPlayerAndItems(void);
extern void RenderStarfield(void* ot);
extern void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume);
extern void SubdivideLevelGeometryPolys(void);
extern void TurnLevelExitQuadIntoGreen(void);
extern void UnusedAsmNoop(void);
extern void UpdateGemRandomSparkleEffect(GemRandomSparkleEffect* eff);
extern void UpdateStarfield(void);
extern void Vibrate100(int constant, int amplitude, int angleIncrement, int max);
extern void Vibrate101(int param_1);

extern DR_AREA drawAreas[2][1];
extern DR_OFFSET drawOffsets[2][1];
extern DR_TPAGE drTpages1[2][1];
extern DR_TPAGE drTpages2[2][1];
extern GemRandomSparkleEffect gemRandomSparkleEffects[3][3];
extern int bonusBlockTextureChain[];
extern int cameraIndex;
extern int crumblingSpecialBlockTextureChain[];
extern int fireBlockTextureChain[];
extern int invisibleBlockTextureChain[];
extern int isInCredits;
extern int isPaused;
extern int itemsDispList[2049];
extern int itemsDispListIdx;
extern int maxPrimBufUsage;
extern int numCrumblingBlocks;
extern int numFlashingBlocks;
extern int numKeysRemaining;
extern int numMovingPlatforms;
extern int numRetractableSpikes;
extern int playerEnemyDispListIdx;
extern int primBufUsage;
extern int retractingSpikeData[64 * 2];
extern int specialLevelType;
extern int toBeDisabledLightEffects[64];
extern int turningMotionBlurEnabled;
extern int turningMotionBlurPhase;
extern int turningMotionBlurTable[70];
extern int whichDrawDispEnv;
extern InvisBlockVisibility invisBlockVisibility;
extern MATRIX perspMatrixes[2];
extern OT_TYPE otag[2][1][1026];
extern Quad* cubeStates[256];
extern QuadFunc QUAD_FUNC_PTRS[6];
extern Quad** levelExitQuadPPtr;
extern RGB farColor;
extern RGB farColor2;
extern short crumblingBlockEntityIndexes[64];
extern short* entityData;
extern short flashingBlockEntityIndexes[64];
extern short* ggiPart2DepthCueingLookup;
extern short* levelData;
extern short movingBlockEntityIndexes[16];
extern TgiFile* tgi;
extern void* playerEnemyDispList[513];
extern void* renderedPrimsBuf;

int drawGeometryAndObjects;
int sunglassCounter1[64];
int sunglassCounter2[64];
int sunglassDisablingState[1];
int sunglassSeeEverything[1];
MATRIX backgroundRenderingMatrix;
MATRIX levelGeometryRenderingMatrix;
MATRIX starfieldMatrix;
STATIC_FOR_GP_ACCESS int starfieldSinPhase1;
STATIC_FOR_GP_ACCESS int starfieldSinPhase2;
STATIC_FOR_GP_ACCESS int starfieldSinPhase3;
STATIC_FOR_GP_ACCESS int starfieldTransPhaseX1;
STATIC_FOR_GP_ACCESS int starfieldTransPhaseX2;
STATIC_FOR_GP_ACCESS int starfieldTransPhaseY1;
STATIC_FOR_GP_ACCESS int starfieldTransPhaseY2;
STATIC_FOR_GP_ACCESS int starfieldTransPhaseZ1;
STATIC_FOR_GP_ACCESS int starfieldTransPhaseZ2;
SVECTOR starfieldSinVec;
VECTOR starfieldTrans;

void RenderBackground(void) {
    backgroundRenderingMatrix = perspMatrixes[cameraIndex];
    backgroundRenderingMatrix.t[0] = (backgroundRenderingMatrix.t[0] - 0x2000) >> 4;
    backgroundRenderingMatrix.t[1] = (backgroundRenderingMatrix.t[1] - 0x2000) >> 4;
    backgroundRenderingMatrix.t[2] = (backgroundRenderingMatrix.t[2] - 0x2000) >> 4;

    if (cameraIndex == 0 && tgi->skyboxFlag == 0x401) {
        switch (specialLevelType) {
            case 1:
                RenderBonusBackgroundPart1(&otag[whichDrawDispEnv][cameraIndex][1025]);
                RenderBonusBackgroundPart2();
                break;
            case 2:
                UpdateStarfield();
                RenderStarfield(&otag[whichDrawDispEnv][cameraIndex][1025]);
                break;
            default:
                RenderNonSpecialBackground(&otag[whichDrawDispEnv][cameraIndex][1025]);
                break;
        }
    }
}

void RenderEverythingElseAndProcessSomeStuff(void) {
    int i;

    if (drawGeometryAndObjects == 1) {
        TurnLevelExitQuadIntoGreen();
        UpdateSunglassModeDisabling();

        if (cameraIndex == 0) {
            ProcessMovingPlatforms2();
            ProcessCrumblingBlocks();
            AnimateTextures(&fireBlockTextureChain);
            AnimateTextures(&invisibleBlockTextureChain);

            if (specialLevelType == 1) {
                AnimateTextures(&crumblingSpecialBlockTextureChain);
                AnimateTextures(&bonusBlockTextureChain);
                ProcessRecentlyVisitedCubes();
            }

            if (!isPaused) {
                ProcessFlashingBlocks();
                ProcessRetractableSpikes();
            }
        }

        ProcessEnabledLightEffects();
        ProcessInvisibleBlockVisibility(&invisBlockVisibility);
        ProcessLightEffects();

        levelGeometryRenderingMatrix = perspMatrixes[cameraIndex];

        CalculateBlockLighting();
        SetFarColor(farColor[0] + 4, farColor[1] + 4, farColor[2] + 4);

        RenderLevelGeometryQuads(otag[whichDrawDispEnv][cameraIndex]);
        ProcessTurningMotionBlur();
        MakeOtagDoublyLinked(otag[whichDrawDispEnv][cameraIndex]);
        UnusedAsmNoop();

        if (VER_JP && isInCredits == 1) {
            ProcessEnemiesRenderItemsAndCheckFellOff();
            CreditsRenderModels();
        } else if (isPaused == 0) {
            ProcessEnemiesRenderItemsAndCheckFellOff();
        } else {
            RenderItems_();
        }

        SubdivideLevelGeometryPolys();
        ProcessDisabledLightEffects();

        for (i = 0; i < 64; i++) {
            if (toBeDisabledLightEffects[i] != -1) {
                DisableLightEffect(toBeDisabledLightEffects[i]);
                toBeDisabledLightEffects[i] = -1;
            }
        }

        DrawLasers(otag[whichDrawDispEnv][cameraIndex]);
        DrawScreenFade();
        RenderPlayerAndItems();
    } else {
        itemsDispListIdx = 0;
        playerEnemyDispListIdx = 0;
    }
}

void CalculateBlockLighting(void) {
    int i;
    int j;
    int col;
    int coord;
    int ret;
    int dir;
    int* p;
    int unk[6];

    p = unk;
    col = 0;
    for (i = 0; i < 3; i++) {
        coord = perspMatrixes[cameraIndex].m[2][i];
        ret = GetShortFromGgiPart2(abs(coord));
        col += ret;
        *p++ = ret;
        if (coord < 0) {
            *p++ = 0;
        } else {
            *p++ = 3;
        }
    }

    if (col == 0) {
        col = 1;
    }

    for (i = 0; i < 3; i++) {
        unk[i * 2] = (unk[i * 2] << 12) / col;
    }

    for (i = 0; i < 3; i++) {
        col = 0;
        if (specialLevelType == 1) {
            for (j = 0; j < 3; j++) {
                dir = unk[j * 2 + 1];
                col += *(j * 6 + dir + i + tgi->specialBlockColors.arr) * unk[j * 2];
            }

            farColor[i] = col + 0x800 >> 12;
            if (farColor[i] > 0xff) {
                farColor[i] = 0xff;
            }
            farColor2[i] = (farColor[i] * *(i + tgi->specialLevelFarColor2Mul)) + 0x40 >> 7;
            if (farColor2[i] > 0xff) {
                farColor2[i] = 0xff;
            }
        } else {
            for (j = 0; j < 3; j++) {
                dir = unk[j * 2 + 1];
                col += *(j * 6 + dir + i + tgi->normalBlockColors.arr) * unk[j * 2];
            }

            farColor[i] = col + 0x800 >> 12;
            if (farColor[i] > 0xff) {
                farColor[i] = 0xff;
            }
            farColor2[i] = (farColor[i] * *(i + tgi->normalLevelFarColor2Mul)) + 0x40 >> 7;
            if (farColor2[i] > 0xff) {
                farColor2[i] = 0xff;
            }
        }
    }
}

int GetShortFromGgiPart2(int param_1) {
    if (param_1 >= 4096) {
        return 4096;
    }
    return ggiPart2DepthCueingLookup[param_1];
}

void RenderPlayerAndItems(void) {
    int i, j;
    int temp;

    itemsDispList[itemsDispListIdx++] = NULL;
    playerEnemyDispList[playerEnemyDispListIdx++] = NULL;

    SetFarColor(farColor2[0], farColor2[1], farColor2[2]);

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            UpdateGemRandomSparkleEffect(&gemRandomSparkleEffects[i][j]);
        }
    }

    RenderDispList(itemsDispList);
    DrawPlayerSpecularSprites();
    RenderDispList(playerEnemyDispList);
    DrawShadowSprites();

    temp = (int)renderedPrimsBuf - (int)RENDERED_PRIMS_BUF;
    primBufUsage = temp - (whichDrawDispEnv * 0xc000);
    if (primBufUsage > maxPrimBufUsage) {
        maxPrimBufUsage = primBufUsage;
    }

    itemsDispListIdx = 0;
    playerEnemyDispListIdx = 0;
}

void AddDrChangePrims(void) {
    if (tgi->skyboxFlag == 1025) {
        addPrim(&otag[whichDrawDispEnv][cameraIndex][1025], &drTpages1[whichDrawDispEnv][cameraIndex]);
        addPrim(&otag[whichDrawDispEnv][cameraIndex][1024], &drTpages2[whichDrawDispEnv][cameraIndex]);
        addPrim(&otag[whichDrawDispEnv][cameraIndex][1025], &drawAreas[whichDrawDispEnv][cameraIndex]);
        addPrim(&otag[whichDrawDispEnv][cameraIndex][1025], &drawOffsets[whichDrawDispEnv][cameraIndex]);
    } else {
        addPrim(&otag[whichDrawDispEnv][cameraIndex][tgi->skyboxFlag], &drTpages2[whichDrawDispEnv][cameraIndex]);
        addPrim(&otag[whichDrawDispEnv][cameraIndex][tgi->skyboxFlag], &drawAreas[whichDrawDispEnv][cameraIndex]);
        addPrim(&otag[whichDrawDispEnv][cameraIndex][tgi->skyboxFlag], &drawOffsets[whichDrawDispEnv][cameraIndex]);
    }
}

void UpdateStarfield(void) {
    SVECTOR svec;
    VECTOR vec;
    MATRIX rot;
    int vx, vy, vz;

    starfieldSinPhase1 = (starfieldSinPhase1 + 1) % 4096;
    starfieldSinVec.vx = rsin(starfieldSinPhase1);

    starfieldSinPhase2 = (starfieldSinPhase2 + 1) % 4096;
    starfieldSinVec.vy = rsin(starfieldSinPhase2);

    starfieldSinPhase3 = (starfieldSinPhase3 + 1) % 4096;
    starfieldSinVec.vz = rsin(starfieldSinPhase3);

    RotMatrix(&starfieldSinVec, &rot);
    MulMatrix0(&levelGeometryRenderingMatrix, &rot, &starfieldMatrix);
    starfieldMatrix.t[0] = 0;
    starfieldMatrix.t[1] = 0;
    starfieldMatrix.t[2] = 0;

    starfieldTransPhaseX1 = (starfieldTransPhaseX1 + 2) % 4096;
    starfieldTransPhaseX2 = (starfieldTransPhaseX2 + 7) % 4096;
    starfieldTrans.vx = (rsin(starfieldTransPhaseX1) * 50000 / 4096) + (rsin(starfieldTransPhaseX2) * 10000 / 4096);

    starfieldTransPhaseY1 = (starfieldTransPhaseY1 + 3) % 4096;
    starfieldTransPhaseY2 = (starfieldTransPhaseY2 + 8) % 4096;
    starfieldTrans.vy = (rsin(starfieldTransPhaseY1) * 50000 / 4096) + (rsin(starfieldTransPhaseY2) * 10000 / 4096);

    starfieldTransPhaseZ1 = (starfieldTransPhaseZ1 + 2) % 4096;
    starfieldTransPhaseZ2 = (starfieldTransPhaseZ2 + 6) % 4096;
    starfieldTrans.vz = (rsin(starfieldTransPhaseZ1) * 60000 / 4096) + (rsin(starfieldTransPhaseZ2) * 10000 / 4096);

    vx = starfieldTrans.vx;
    vy = starfieldTrans.vy;
    vz = starfieldTrans.vz;
    while (vx > 6552)
        vx -= 13106;
    while (vx < -6553)
        vx += 13106;

    while (vy > 6552)
        vy -= 13106;
    while (vy < -6553)
        vy += 13106;

    while (vz > 6552)
        vz -= 13106;
    while (vz < -6553)
        vz += 13106;

    svec.vx = vx;
    svec.vy = vy;
    svec.vz = vz;

    ApplyMatrix(&starfieldMatrix, &svec, &vec);
    starfieldMatrix.t[0] = vec.vx;
    starfieldMatrix.t[1] = vec.vy;
    starfieldMatrix.t[2] = vec.vz;
}

void ProcessMovingPlatforms2(void) {
    MovingPlatformEntity2* mpe;
    Quad* quad;
    int i;
    int j;
    int dir;
    int x;
    int y;
    int z;

    for (i = 0; i < numMovingPlatforms; i++) {
        mpe = (MovingPlatformEntity2*)&entityData[movingBlockEntityIndexes[i] * 128];
        x = mpe->posX;
        y = mpe->posY;
        z = mpe->posZ;
        for (j = 0; j < mpe->length; j++) {
            for (dir = 0; dir < 6; dir++) {
                quad = *(Quad**)((char*)mpe + j * 24 + dir * 4 + 142);
                if (quad != (Quad*)-1) {
                    QUAD_FUNC_PTRS[dir](quad, 512, x, y, z, -1);
                }
            }
            switch (mpe->dir) {
                case 1:
                    x += 512;
                    break;
                case 2:
                    y += 512;
                    break;
                case 5:
                    z += 512;
                    break;
            }
        }
    }
}
void ProcessCrumblingBlocks(void) {
    int i;
    int x, y, z;
    int x9, y9, z9;
    int dir;
    int faceType;
    CrumblingBlockEntity* eb;
    int cubeIndex;

    for (i = 0; i < numCrumblingBlocks; i++) {
        eb = (CrumblingBlockEntity*)&entityData[crumblingBlockEntityIndexes[i] * 128];
        x = eb->x;
        y = eb->y;
        z = eb->z;
        x9 = x << 9;
        y9 = y << 9;
        z9 = z << 9;
        cubeIndex = CUBE_INDEX_AT(x, y, z);

        switch (eb->state) {
            case 2:
                for (dir = 0; dir < 6; dir++) {
                    cubeStates[cubeIndex * 16 + dir]->flags.u8 |= 1; // QF_ACTIVE
                    faceType = GetFaceTypeAtRelativeToDir(x, y, z, dir);
                    if (faceType != -1 && faceType != -2 && faceType != 3 && faceType != 5) {
                        cubeStates[GetCubeIndexRelativeToDir(x, y, z, dir) * 16 + FlipDir(dir)]->flags.u8 |= 1; // QF_ACTIVE
                    }
                }
                eb->state = 3;
                EnableScreenShake(4, FRAMES(30), 3);
                Vibrate100(200, 0, 0, 1);
                Vibrate101(FRAMES(40));
                /* fallthrough */
            case 3:
                eb->counter -= VELOCITY(32);
                if (eb->counter >= 0) {
                    for (dir = 0; dir < 6; dir++) {
                        QUAD_FUNC_PTRS[dir](cubeStates[cubeIndex * 16 + dir], eb->counter, x9, y9, z9, 0);
                    }
                } else {
                    for (dir = 0; dir < 6; dir++) {
                        cubeStates[cubeIndex * 16 + dir]->flags.i32 &= ~1; // QF_ACTIVE
                    }
                    eb->state = 0;
                    CUBE_TYPE_AT(x, y, z) = -1;
                }
                break;
        }
    }
}

void ProcessFlashingBlocks(void) {
    Quad* quad;
    int i;
    int j;
    uint color;
    int counter;
    int x;
    int y;
    int z;
    int cubeIndex;
    FlashingEntity* eb;

    for (i = 0; i < numFlashingBlocks; i++) {
        eb = (FlashingEntity*)(entityData + flashingBlockEntityIndexes[i] * 128);
        counter = eb->counter;
        z = (int)eb->z;
        x = (int)eb->x;
        y = (int)eb->y;
        cubeIndex = CUBE_INDEX_AT(x, y, z);
        switch (eb->state) {
            case 0:
                counter--;
                if (counter <= 0) {
                    for (j = 0; j < 6; j++) {
                        quad = cubeStates[cubeIndex * 16 + j];
                        // FIXME: no idea why volatile makes this match
                        if (*(volatile int*)&specialLevelType == 1) {
                            quad->flags.u16 = 0x107; // QF_ACTIVE | QF_SEMITRANSPARENT | QF_BACKFACE_CULL | QF_INVISIBLE
                        } else {
                            quad->flags.u16 = 0x10f; // QF_ACTIVE | QF_SEMITRANSPARENT | QF_BACKFACE_CULL | QF_GOURAUD | QF_INVISIBLE
                        }
                        quad->color = 0;
                    }
                    counter = FRAMES(8);
                    CUBE_TYPE_AT(x, y, z) = eb->cubeType;
                    eb->state = 1;
                }
                break;
            case 1:
                counter--;
                if (counter > 0) {
                    color = ((FRAMES(8) - counter) * 0xff) / FRAMES(8);
                    color = color << 16 | color << 8 | color;
                    for (j = 0; j < 6; j++) {
                        cubeStates[cubeIndex * 16 + j]->color = color;
                    }
                } else {
                    for (j = 0; j < 6; j++) {
                        cubeStates[cubeIndex * 16 + j]->color = 0xffffff;
                    }
                    eb->state = 2;
                    counter = VID_NTSC ? 5 : 4;
                }
                break;
            case 2:
                counter--;
                if (counter > 0) {
                    color = counter * 224 / (VID_NTSC ? 5 : 4) + 32;
                    color = color << 16 | color << 8 | color;
                    for (j = 0; j < 6; j++) {
                        cubeStates[cubeIndex * 16 + j]->color = color;
                    }
                } else {
                    for (j = 0; j < 6; j++) {
                        quad = cubeStates[cubeIndex * 16 + j];
                        *(char*)((int)&quad->flags + 1) = 0; // QF_INVISIBLE
                        quad->color = 0x808080;
                    }
                    eb->state = 3;
                    counter = FRAMES(76);
                }
                break;
            case 3:
                counter--;
                if (counter <= 0) {
                    eb->state = 4;
                    counter = FRAMES(8);
                    for (j = 0; j < 6; j++) {
                        quad = cubeStates[cubeIndex * 16 + j];
                        *(byte*)((int)&quad->flags + 1) = 1; // QF_INVISIBLE
                        quad->color = 0x202020;
                    }
                }
                break;
            case 4:
                counter--;
                if (counter > 0) {
                    color = 256 - counter * 224 / FRAMES(8);
                    color = color << 16 | color << 8 | color;
                    for (j = 0; j < 6; j++) {
                        cubeStates[cubeIndex * 16 + j]->color = color;
                    }
                } else {
                    for (j = 0; j < 6; j++) {
                        cubeStates[cubeIndex * 16 + j]->color = 0xffffff;
                    }
                    eb->state = 5;
                    counter = FRAMES(16);
                }
                break;
            case 5:
                counter--;
                if (counter > 0) {
                    color = counter * 255 / FRAMES(16);
                    color = color << 16 | color << 8 | color;
                    for (j = 0; j < 6; j++) {
                        cubeStates[cubeIndex * 16 + j]->color = color;
                    }
                } else {
                    for (j = 0; j < 6; j++) {
                        cubeStates[cubeIndex * 16 + j]->flags.u8 = 6; // QF_SEMITRANSPARENT | QF_BACKFACE_CULL
                    }
                    counter = FRAMES(76);
                    CUBE_TYPE_AT(x, y, z) = -1;
                    eb->state = 0;
                }
        }
        eb->counter = counter;
    }
}

#define EXTEND_FRAMES (VID_NTSC ? 5 : 4)
#define RETRACT_FRAMES (VID_NTSC ? 44 : 36)

void ProcessRetractableSpikes(void) {
    int i;
    SVECTOR sndPos;
    short* eb;
    short* pos;
    int unkE;
    int unk1E;
    int counter;

    for (i = 0; i < numRetractableSpikes; i++) {
        eb = retractingSpikeData[i * 2];
        pos = retractingSpikeData[i * 2 + 1];

        unkE = eb[8];
        unk1E = eb[16];
        counter = eb[15];

        switch (unk1E) {
            case 0:
                if (--counter <= 0) {
                    counter = EXTEND_FRAMES;
                    unk1E = 1;
                    eb[4] = 1;

                    sndPos.vx = pos[0] * 512;
                    sndPos.vy = pos[1] * 512;
                    sndPos.vz = pos[2] * 512;
                    ApplyMatrixSV(&perspMatrixes[cameraIndex], &sndPos, &sndPos);
                    sndPos.vx += perspMatrixes[cameraIndex].t[0];
                    sndPos.vy += perspMatrixes[cameraIndex].t[1];
                    sndPos.vz += perspMatrixes[cameraIndex].t[2];
                    SndPlaySfx(SFX_MOVING_SPIKE_EXTEND, 0, &sndPos, 7000);
                }
                break;

            case 1:
                if (--counter >= 0) {
                    unkE = ((EXTEND_FRAMES - 1 - counter) * 32) / EXTEND_FRAMES;
                } else {
                    unkE = 0x1f;
                    counter = FRAMES(12);
                    unk1E = 2;
                }
                break;

            case 2:
                if (--counter >= 0) {
                    if ((counter & 1) == 0) {
                        unkE = 0x1f;
                    } else {
                        unkE = 0x1f - (counter * 32) / (FRAMES(12) * 4);
                    }
                } else {
                    unkE = 0x1f;
                    counter = FRAMES(12);
                    unk1E = 3;
                }
                break;

            case 3:
                if (--counter <= 0) {
                    counter = RETRACT_FRAMES;
                    unk1E = 4;

                    sndPos.vx = pos[0] * 512;
                    sndPos.vy = pos[1] * 512;
                    sndPos.vz = pos[2] * 512;
                    ApplyMatrixSV(&perspMatrixes[cameraIndex], &sndPos, &sndPos);
                    sndPos.vx += perspMatrixes[cameraIndex].t[0];
                    sndPos.vy += perspMatrixes[cameraIndex].t[1];
                    sndPos.vz += perspMatrixes[cameraIndex].t[2];
                    SndPlaySfx(SFX_MOVING_SPIKE_RETRACT, 0, &sndPos, 7000);
                }
                break;

            case 4:
                if (--counter >= 0) {
                    unkE = (counter * 32) / RETRACT_FRAMES;
                    if (counter == RETRACT_FRAMES / 4) {
                        eb[4] = 2;
                    }
                } else {
                    unkE = 0;
                    counter = VID_NTSC ? 63 : 52;
                    unk1E = 0;
                }
                break;
        }

        eb[8] = unkE;
        eb[15] = counter;
        eb[16] = unk1E;
    }
}

void SetSunglassMode(int on) {
    if (!on) {
        sunglassDisablingState[cameraIndex] = 1;
        sunglassCounter2[cameraIndex] = 0x15;
        sunglassCounter1[cameraIndex] = FRAMES(8);
        sunglassSeeEverything[cameraIndex] = 0;
    } else {
        sunglassDisablingState[cameraIndex] = 0;
        sunglassSeeEverything[cameraIndex] = 1;
    }
}

int UpdateSunglassModeDisabling(void) {
    switch (sunglassDisablingState[cameraIndex]) {
        case 1:
            if (--sunglassCounter1[cameraIndex] > 0) {
                return;
            }
            if (--sunglassCounter2[cameraIndex] <= 0) {
                sunglassDisablingState[cameraIndex] = 0;
                return;
            }
            sunglassCounter1[cameraIndex] = FRAMES(8);
            sunglassDisablingState[cameraIndex] = 2;
            sunglassSeeEverything[cameraIndex] = 1;
            break;
        case 2:
            if (--sunglassCounter1[cameraIndex] > 0) {
                return;
            }
            if (--sunglassCounter2[cameraIndex] <= 0) {
                sunglassDisablingState[cameraIndex] = 0;
                return;
            }
            sunglassCounter1[cameraIndex] = FRAMES(8);
            sunglassDisablingState[cameraIndex] = 1;
            sunglassSeeEverything[cameraIndex] = 0;
            break;
    }
}

void EnableTurningMotionBlur(void) {
    if (turningMotionBlurEnabled == 0) {
        turningMotionBlurEnabled = 1;
        turningMotionBlurPhase = 0;
    }
}

void ProcessTurningMotionBlur(void) {
    int i;

    if (turningMotionBlurEnabled != 0) {
        if (turningMotionBlurEnabled == 1) {
            turningMotionBlurPhase++;
            if (turningMotionBlurPhase == 9) {
                turningMotionBlurPhase -= 2;
                turningMotionBlurEnabled = 2;
            }
        } else {
            turningMotionBlurPhase--;
            if (turningMotionBlurPhase == 0) {
                turningMotionBlurEnabled = 0;
            }
        }

        for (i = 0; i < 7; i++) {
            tgi->lodDistance[i] = turningMotionBlurTable[turningMotionBlurPhase * 7 + i];
        }
    }
}

void TurnLevelExitQuadIntoGreen(void) {
    if (numKeysRemaining == 0) {
        // Set texture variant to 1
        ((byte*)&(*levelExitQuadPPtr)->flags)[3] = 1;
    }
}

int DisableItemShadow(int entityIndex, int side, int const0) {
    Quad* quad;
    int cubeIndex;
    short* ed = entityData + entityIndex * 128 + 125;

    cubeIndex = CUBE_INDEX_AT(ed[0], ed[1], ed[2]);

    if ((int)cubeStates[cubeIndex * 16 + 15] == 0) {
        quad = (Quad*)cubeStates[cubeIndex * 16 + side];
        if (specialLevelType == 1) {
            quad->flags.i32 = (quad->flags.i32 & 0xfffeffff) | (const0 << 16);
        } else {
            quad->flags.b[3] = const0;
        }
    }

    if ((int)cubeStates[cubeIndex * 16 + 15] == 2) {
        quad = (Quad*)cubeStates[cubeIndex * 16 + side];
        quad->flags.b[3] = const0;
    }
}
