#include "common.h"

extern TgiFile * tgi;
extern int turningMotionBlurEnabled;
extern int turningMotionBlurPhase;
extern int turningMotionBlurTable[70];
extern int numKeysRemaining;
extern Quad** levelExitQuadPPtr;

INCLUDE_ASM("asm/nonmatchings/update", ProcessMovingPlatforms2);

INCLUDE_ASM("asm/nonmatchings/update", ProcessCrumblingBlocks);

INCLUDE_ASM("asm/nonmatchings/update", ProcessFlashingBlocks);

INCLUDE_ASM("asm/nonmatchings/update", ProcessRetractableSpikes);

INCLUDE_ASM("asm/nonmatchings/update", SetSunglassMode);

INCLUDE_ASM("asm/nonmatchings/update", UpdateSunglassModeDisabling);

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
        ((byte*)&(*levelExitQuadPPtr)->flags)[3] = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/update", DisableItemShadow);
