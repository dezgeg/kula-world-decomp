#include "common.h"

extern short* ggiPart2DepthCueingLookup;
extern int cameraIndex;
extern TgiFile* tgi;
extern int whichDrawDispEnv;
extern void* otag[2][1][1026];

MATRIX MATRIX_000a5184;
extern MATRIX perspMatrixes[];
extern int specialLevelType;

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

INCLUDE_ASM("asm/nonmatchings/render1", RenderEverythingElseAndProcessSomeStuff);

INCLUDE_ASM("asm/nonmatchings/render1", CalculateBlockLighting);

int GetShortFromGgiPart2(int param_1) {
    if (param_1 >= 4096) {
        return 4096;
    }
    return ggiPart2DepthCueingLookup[param_1];
}

INCLUDE_ASM("asm/nonmatchings/render1", RenderPlayerAndItems);

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

INCLUDE_ASM("asm/nonmatchings/render1", UpdateStarfield);
