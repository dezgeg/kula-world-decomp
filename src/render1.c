#include "common.h"

extern short* ggiPart2DepthCueingLookup;
extern int cameraIndex;
extern TgiFile* tgi;
extern int whichDrawDispEnv;
extern void* otag[2][1][1026];

extern void ASM_FUN_00050310(void* otag);
extern void ASM_FUN_00052504(void* otag);
extern void CubeTextureStuff(AnimatedTextureChain* param_1);
extern void DisableLightEffect(int param_1);
extern void DrawScreenFade(void);
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
extern void RenderItems_(void);
extern void RenderLevelGeometryQuads(void* otag);
extern void RenderPlayerAndItems(void);
extern void SubdivideLevelGeometryPolys(void);
extern void TurnLevelExitQuadIntoGreen(void);
extern void UnusedAsmNoop(void);
extern void UpdateSunglassModeDisabling(void);

int drawGeometryAndObjects;
MATRIX levelGeometryRenderingMatrix;

MATRIX MATRIX_000a5184;
extern MATRIX perspMatrixes[];
extern int specialLevelType;
extern RGB farColor;
extern RGB farColor2;

extern AnimatedTextureChain bonusBlockTextureChain;
extern AnimatedTextureChain crumblingSpecialBlockTextureChain;
extern AnimatedTextureChain fireBlockTextureChain;
extern InvisBlockVisibility invisBlockVisibility;
extern AnimatedTextureChain invisibleBlockTextureChain;
extern int isPaused;
extern int toBeDisabledLightEffects[64];

static int starfieldSinPhase1;
static int starfieldSinPhase2;
static int starfieldSinPhase3;

static int D_000A501C;
static int D_000A5024;
static int D_000A5028;
static int D_000A5030;
static int D_000A5034;
static int D_000A5050;

SVECTOR starfieldSinVec;

int D_000A51BC;
extern int D_000A51C0;
extern int D_000A51C4;

extern MATRIX starfieldMatrix;

typedef struct {
    char data[0x15c];
} GemRandomSparkleEffect;

extern GemRandomSparkleEffect GemRandomSparkleEffect_ARRAY_ARRAY_000dd760[3][3];
extern int itemsDispList[10];
extern int itemsDispListIdx;
extern int maxPrimBufUsage;
extern void* playerEnemyDispList[4];
extern int playerEnemyDispListIdx;
extern int primBufUsage;
extern void* renderedPrimsBuf;

extern void UpdateGemRandomSparkleEffect(GemRandomSparkleEffect * eff);
extern void RenderDispList(void* p);
extern void DrawPlayerSpecularSprites(void);
extern void DrawShadowSprites(void);

extern void RenderBonusBackground(void * ot);
extern void RenderNonSpecialBackground(void * ot);
extern void RenderStarfield(void * ot);
extern void ASM_00053a64(void);
extern void UpdateStarfield(void);

void RenderBackground(void) {
    MATRIX_000a5184 = perspMatrixes[cameraIndex];
    MATRIX_000a5184.t[0] = (MATRIX_000a5184.t[0] - 0x2000) >> 4;
    MATRIX_000a5184.t[1] = (MATRIX_000a5184.t[1] - 0x2000) >> 4;
    MATRIX_000a5184.t[2] = (MATRIX_000a5184.t[2] - 0x2000) >> 4;

    if (cameraIndex == 0 && tgi->skyboxFlag == 0x401) {
        switch (specialLevelType) {
        case 1:
            RenderBonusBackground(&otag[whichDrawDispEnv][cameraIndex][1025]);
            ASM_00053a64();
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
            CubeTextureStuff(&fireBlockTextureChain);
            CubeTextureStuff(&invisibleBlockTextureChain);

            if (specialLevelType == 1) {
                CubeTextureStuff(&crumblingSpecialBlockTextureChain);
                CubeTextureStuff(&bonusBlockTextureChain);
                ProcessRecentlyVisitedCubes();
            }

            if (isPaused == 0) {
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
        ASM_FUN_00050310(otag[whichDrawDispEnv][cameraIndex]);
        UnusedAsmNoop();

        if (isPaused == 0) {
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

        ASM_FUN_00052504(otag[whichDrawDispEnv][cameraIndex]);
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
    int* p;
    int unk[3][2];

    p = &unk[0][0];
    col = 0;
    i = 0;
    for (; i < 3; i++) {
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
        unk[i][0] = (unk[i][0] << 12) / col;
    }

    for (i = 0; i < 3; i++) {
        col = 0;
        if (specialLevelType == 1) {
            for (j = 0; j < 3; j++) {
                col += tgi->specialBlockColors.arr[j * 6 + unk[j][1] + i] * unk[j][0];
            }

            farColor[i] = col + 0x800 >> 12;
            if (farColor[i] > 0xff) {
                farColor[i] = 0xff;
            }
            farColor2[i] = (farColor[i] * tgi->specialLevelFarColor2Mul[i]) + 0x40 >> 7;
            if (farColor2[i] > 0xff) {
                farColor2[i] = 0xff;
            }
        } else {
            for (j = 0; j < 3; j++) {
                col += tgi->normalBlockColors.arr[j * 6 + unk[j][1] + i] * unk[j][0];
            }

            farColor[i] = col + 0x800 >> 12;
            if (farColor[i] > 0xff) {
                farColor[i] = 0xff;
            }
            farColor2[i] = (farColor[i] * tgi->normalLevelFarColor2Mul[i]) + 0x40 >> 7;
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
    int iDl = itemsDispListIdx;
    int pDl = playerEnemyDispListIdx;
    int temp;
    
    itemsDispList[iDl] = 0;
    playerEnemyDispList[pDl] = NULL;
    itemsDispListIdx = iDl + 1;
    playerEnemyDispListIdx = pDl + 1;
    
    SetFarColor(farColor2[0], farColor2[1], farColor2[2]);
    
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            UpdateGemRandomSparkleEffect(&GemRandomSparkleEffect_ARRAY_ARRAY_000dd760[i][j]);
        }
    }
    
    RenderDispList(itemsDispList);
    DrawPlayerSpecularSprites();
    RenderDispList(playerEnemyDispList);
    DrawShadowSprites();
    
    temp = (int)renderedPrimsBuf + 0xffe87000;
    primBufUsage = temp - (whichDrawDispEnv * 0xc000);
    if (primBufUsage > maxPrimBufUsage) {
        maxPrimBufUsage = primBufUsage;
    }
    
    itemsDispListIdx = 0;
    playerEnemyDispListIdx = 0;
}

extern DR_TPAGE drTpages1[2][1];
extern DR_TPAGE drTpages2[2][1];
extern DR_AREA drawAreas[2][1];
extern DR_OFFSET drawOffsets[2][1];

void AddDrChangePrims(void) {
    if (tgi->skyboxFlag == 1025) {
        addPrim(&otag[whichDrawDispEnv][cameraIndex][1025], &drTpages1[whichDrawDispEnv][cameraIndex]);
        addPrim(&otag[whichDrawDispEnv][cameraIndex][1024], &drTpages2[whichDrawDispEnv][cameraIndex]);
        addPrim(&otag[whichDrawDispEnv][cameraIndex][1025], &drawAreas[whichDrawDispEnv][cameraIndex]);
        addPrim(&otag[whichDrawDispEnv][cameraIndex][1025], &drawOffsets[whichDrawDispEnv][cameraIndex]);
    }
    else {
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

    D_000A501C = (D_000A501C + 2) % 4096;
    D_000A5024 = (D_000A5024 + 7) % 4096;
    D_000A51BC = (rsin(D_000A501C) * 50000 / 4096) + (rsin(D_000A5024) * 10000 / 4096);

    D_000A5028 = (D_000A5028 + 3) % 4096;
    D_000A5030 = (D_000A5030 + 8) % 4096;
    D_000A51C0 = (rsin(D_000A5028) * 50000 / 4096) + (rsin(D_000A5030) * 10000 / 4096);

    D_000A5034 = (D_000A5034 + 2) % 4096;
    D_000A5050 = (D_000A5050 + 6) % 4096;
    D_000A51C4 = (rsin(D_000A5034) * 60000 / 4096) + (rsin(D_000A5050) * 10000 / 4096);

    vx = D_000A51BC;
    vy = D_000A51C0;
    vz = D_000A51C4;
    while (vx > 6552) vx -= 13106;
    while (vx < -6553) vx += 13106;

    while (vy > 6552) vy -= 13106;
    while (vy < -6553) vy += 13106;

    while (vz > 6552) vz -= 13106;
    while (vz < -6553) vz += 13106;

    svec.vx = vx;
    svec.vy = vy;
    svec.vz = vz;

    ApplyMatrix(&starfieldMatrix, &svec, &vec);
    starfieldMatrix.t[0] = vec.vx;
    starfieldMatrix.t[1] = vec.vy;
    starfieldMatrix.t[2] = vec.vz;
}
