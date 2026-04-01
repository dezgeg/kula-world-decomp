#include "common.h"

extern void SndPlaySfx(int sfx, int tag, SVECTOR * dir, int volume);

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
extern int numRetractableSpikes;
extern MATRIX perspMatrixes[];
extern int retractingSpikeData[64 * 2];

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

void ProcessRetractableSpikes(void) {
    int i;
    SVECTOR sndPos;
    short *eb;
    short *pos;
    int unkE;
    int unk1E;
    int counter;

    for (i = 0; i < numRetractableSpikes; i++) {
        eb = retractingSpikeData[i * 2];
        pos = retractingSpikeData[i * 2 + 1];

        unkE = eb[8];
        unk1E = eb[16];
        counter = eb[15];

        switch (unk1E) {
            case 0:
                if (--counter <= 0) {
                    counter = 4;
                    unk1E = 1;
                    eb[4] = 1;

                    sndPos.vx = pos[0] * 512;
                    sndPos.vy = pos[1] * 512;
                    sndPos.vz = pos[2] * 512;
                    ApplyMatrixSV(&perspMatrixes[cameraIndex], &sndPos, &sndPos);
                    sndPos.vx += perspMatrixes[cameraIndex].t[0];
                    sndPos.vy += perspMatrixes[cameraIndex].t[1];
                    sndPos.vz += perspMatrixes[cameraIndex].t[2];
                    SndPlaySfx(0x69, 0, &sndPos, 7000);
                }
                break;

            case 1:
                if (--counter >= 0) {
                    unkE = (3 - counter) * 8;
                } else {
                    unkE = 0x1f;
                    counter = 12;
                    unk1E = 2;
                }
                break;

            case 2:
                if (--counter >= 0) {
                    if ((counter & 1) == 0) {
                        unkE = 0x1f;
                    } else {
                        unkE = 0x1f - (counter * 32) / 48;
                    }
                } else {
                    unkE = 0x1f;
                    counter = 12;
                    unk1E = 3;
                }
                break;

            case 3:
                if (--counter <= 0) {
                    counter = 36;
                    unk1E = 4;

                    sndPos.vx = pos[0] * 512;
                    sndPos.vy = pos[1] * 512;
                    sndPos.vz = pos[2] * 512;
                    ApplyMatrixSV(&perspMatrixes[cameraIndex], &sndPos, &sndPos);
                    sndPos.vx += perspMatrixes[cameraIndex].t[0];
                    sndPos.vy += perspMatrixes[cameraIndex].t[1];
                    sndPos.vz += perspMatrixes[cameraIndex].t[2];
                    SndPlaySfx(0x6a, 0, &sndPos, 7000);
                }
                break;

            case 4:
                if (--counter >= 0) {
                    unkE = (counter * 32) / 36;
                    if (counter == 9) {
                        eb[4] = 2;
                    }
                } else {
                    unkE = 0;
                    counter = 52;
                    unk1E = 0;
                }
                break;
        }

        eb[8] = unkE;
        eb[15] = counter;
        eb[16] = unk1E;
    }
}

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
