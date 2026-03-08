#include "common.h"

typedef void (*QuadFunc)(Quad *quad, int width, int x, int y, int z, int textureRotation);

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

extern QuadFunc QUAD_FUNC_PTRS[6];
extern short movingBlockEntityIndexes[16];
extern short* entityData;
extern int numMovingPlatforms;

void ProcessMovingPlatforms2(void) {
    MovingPlatformEntity2 *mpe;
    Quad* quad;
    int i;
    int j;
    int dir;
    int x;
    int y;
    int z;

    for (i = 0; i < numMovingPlatforms; i++) {
        mpe = (MovingPlatformEntity2 *) &entityData[movingBlockEntityIndexes[i] * 128];
        x = mpe->posX;
        y = mpe->posY;
        z = mpe->posZ;
        for (j = 0; j < mpe->length; j++) {
            for (dir = 0; dir < 6; dir++) {
                quad = *(Quad **)((char *)mpe + j * 24 + dir * 4 + 142);
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


void UpdateSunglassModeDisabling(void) {
    int idx = cameraIndex;
    
    switch (sunglassDisablingState[idx]) {
    case 1:
        if (--sunglassCounter1[idx] > 0) {
            return;
        }
        if (--sunglassCounter2[idx] <= 0) {
            sunglassDisablingState[idx] = 0;
            return;
        }
        sunglassCounter1[idx] = 8;
        sunglassDisablingState[idx] = 2;
        sunglassSeeEverything[idx] = 1;
        break;
    case 2:
        if (--sunglassCounter1[idx] > 0) {
            return;
        }
        if (--sunglassCounter2[idx] <= 0) {
            sunglassDisablingState[idx] = 0;
            return;
        }
        sunglassCounter1[idx] = 8;
        sunglassDisablingState[idx] = 1;
        sunglassSeeEverything[idx] = 0;
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
        ((byte*)&(*levelExitQuadPPtr)->flags)[3] = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/update", DisableItemShadow);
