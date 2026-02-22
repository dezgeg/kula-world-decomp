#include "common.h"

extern short *ggiPart2DepthCueingLookup;

INCLUDE_ASM("asm/nonmatchings/render1", RenderBackground);

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
extern int cameraIndex;
extern DR_AREA drawAreas[2][1];
extern DR_OFFSET drawOffsets[2][1];
extern void* otag[2][1][1026];
extern TgiFile * tgi;
extern int whichDrawDispEnv;

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
