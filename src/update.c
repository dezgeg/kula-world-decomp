#include "common.h"

extern TgiFile* tgi;
extern int turningMotionBlurEnabled;
extern int turningMotionBlurPhase;
extern int turningMotionBlurTable[70];
extern int numKeysRemaining;
extern Quad** levelExitQuadPPtr;
extern int cameraIndex;
extern int sunglassCounter1[];
extern int sunglassCounter2[];
extern int sunglassDisablingState[];
extern int sunglassSeeEverything[];

INCLUDE_ASM("asm/nonmatchings/update", ProcessMovingPlatforms2);

INCLUDE_ASM("asm/nonmatchings/update", ProcessCrumblingBlocks);

INCLUDE_ASM("asm/nonmatchings/update", ProcessFlashingBlocks);

INCLUDE_ASM("asm/nonmatchings/update", ProcessRetractableSpikes);

void SetSunglassMode(int on) {
    if (!on) {
        sunglassDisablingState[cameraIndex] = 1;
        sunglassCounter2[cameraIndex] = 0x15;
        sunglassCounter1[cameraIndex] = 8;
        sunglassSeeEverything[cameraIndex] = 0;
    } else {
        sunglassDisablingState[cameraIndex] = 0;
        sunglassSeeEverything[cameraIndex] = 1;
    }
}


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
