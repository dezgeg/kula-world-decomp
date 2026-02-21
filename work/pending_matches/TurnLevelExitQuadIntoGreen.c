#include "common.h"

extern Quad** levelExitQuadPPtr;
extern int numKeysRemaining;

void TurnLevelExitQuadIntoGreen(void) {
    if (numKeysRemaining == 0) {
        ((byte*)&(*levelExitQuadPPtr)->flags)[3] = 1;
    }
}
