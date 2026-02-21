#include "common.h"

int ballTextureIndex;
int curController;
short numCopycatMoves;
short unusedNumCopycatRounds;

extern short copycatMoves[1024];
extern short copycatNewOrCopyMoves;

void ResetCopycatMode(int param_1) {
    int i;
    
    for (i = 0; i < 1024; i++) {
        copycatMoves[i] = -1;
    }
    
    numCopycatMoves = 1;
    curController = param_1;
    ballTextureIndex = param_1;
    unusedNumCopycatRounds = 0;
    copycatNewOrCopyMoves = 1;
}
