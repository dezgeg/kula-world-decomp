#include "common.h"

extern DrawDisp drawdisp[2];
extern int whichDrawDispEnv;

void PutDrawAndDispEnvs(void) {
    PutDrawEnv(&drawdisp[whichDrawDispEnv].draw);
    PutDispEnv(&drawdisp[whichDrawDispEnv].disp);
}

