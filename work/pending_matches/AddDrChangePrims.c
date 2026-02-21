#include "common.h"

typedef struct {
    u_long otag[1026];
} Otag;

extern DR_TPAGE drTpages1[2][1];
extern DR_TPAGE drTpages2[2][1];
extern int cameraIndex;
extern DR_AREA drawAreas[2][1];
extern DR_OFFSET drawOffsets[2][1];
extern Otag otag[2][1];
extern TgiFile * tgi;
extern int whichDrawDispEnv;

void AddDrChangePrims(void) {
    if (tgi->skyboxFlag == 1025) {
        addPrim(&otag[whichDrawDispEnv][cameraIndex].otag[1025], &drTpages1[whichDrawDispEnv][cameraIndex]);
        addPrim(&otag[whichDrawDispEnv][cameraIndex].otag[1024], &drTpages2[whichDrawDispEnv][cameraIndex]);
        addPrim(&otag[whichDrawDispEnv][cameraIndex].otag[1025], &drawAreas[whichDrawDispEnv][cameraIndex]);
        addPrim(&otag[whichDrawDispEnv][cameraIndex].otag[1025], &drawOffsets[whichDrawDispEnv][cameraIndex]);
    }
    else {
        addPrim(&otag[whichDrawDispEnv][cameraIndex].otag[tgi->skyboxFlag], &drTpages2[whichDrawDispEnv][cameraIndex]);
        addPrim(&otag[whichDrawDispEnv][cameraIndex].otag[tgi->skyboxFlag], &drawAreas[whichDrawDispEnv][cameraIndex]);
        addPrim(&otag[whichDrawDispEnv][cameraIndex].otag[tgi->skyboxFlag], &drawOffsets[whichDrawDispEnv][cameraIndex]);
    }
}
