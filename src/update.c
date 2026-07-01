#include "common.h"

#define CUBE_TYPE_AT(x, y, z) levelData[(x) * 34 * 34  + (y) * 34  + (z)]
#define CUBE_INDEX_AT(x, y, z) (*(short*)(0x1af000 + (x) * 34 * 34 * 2 + (y) * 34 * 2 + (z) * 2))

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

typedef struct FlashingEntity {
    short tag;
    short cubeType;
    short initState;
    short state;
    short counter;
    short pad[120];
    short x;
    short y;
    short z;
} FlashingEntity;

typedef void (*QuadFunc)(Quad *quad, int width, int x, int y, int z, int textureRotation);

extern void SndPlaySfx(int sfx, int tag, SVECTOR * dir, int volume);

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
extern Quad* cubeStates[256];
extern short flashingBlockEntityIndexes[64];
extern short* levelData;
extern int numFlashingBlocks;
extern int specialLevelType;

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

void ProcessFlashingBlocks(void) {
    Quad *quad;
    int i;
    int j;
    uint color;
    int counter;
    int x;
    int y;
    int z;
    int cubeIndex;
    FlashingEntity *eb;

    for (i = 0; i < numFlashingBlocks; i++) {
        eb = (FlashingEntity *)(entityData + flashingBlockEntityIndexes[i] * 128);
        counter = eb->counter;
        z = (int)eb->z;
        x = (int)eb->x;
        y = (int)eb->y;
        cubeIndex = CUBE_INDEX_AT(x, y, z);
        switch(eb->state) {
        case 0:
            counter--;
            if (counter <= 0) {
                for (j = 0; j < 6; j++) {
                    quad = cubeStates[cubeIndex * 16 + j];
                    // FIXME: no idea why volatile makes this match
                    if (*(volatile int*)&specialLevelType == 1) {
                        quad->flags.u16 = 0x107;
                    } else {
                        quad->flags.u16 = 0x10f;
                    }
                    quad->color = 0;
                }
                counter = 8;
                CUBE_TYPE_AT(x, y, z) = eb->cubeType;
                eb->state = 1;
            }
            break;
        case 1:
            counter--;
            if (counter > 0) {
                color = ((8 - counter) * 0xff) / 8;
                color = color << 16 | color << 8 | color;
                for (j = 0; j < 6; j++) {
                    cubeStates[cubeIndex * 16 + j]->color = color;
                }
            } else {
                for (j = 0; j < 6; j++) {
                    cubeStates[cubeIndex * 16 + j]->color = 0xffffff;
                }
                eb->state = 2;
                counter = 4 ;
            }
            break;
        case 2:
            counter--;
            if (counter > 0) {
                color = counter * 56 + 32;
                color = color << 16 | color << 8 | color;
                for (j = 0; j < 6; j++) {
                    cubeStates[cubeIndex * 16 + j]->color = color;
                }
            } else {
                for (j = 0; j < 6; j++) {
                    quad = cubeStates[cubeIndex * 16 + j];
                    *(char *)((int)&quad->flags + 1) = 0;
                    quad->color = 0x808080;
                }
                eb->state = 3;
                counter = 76;
            }
            break;
        case 3:
            counter--;
            if (counter <= 0) {
                eb->state = 4;
                counter = 8;
                for (j = 0; j < 6; j++) {
                    quad = cubeStates[cubeIndex * 16 + j];
                    *(byte *)((int)&quad->flags + 1) = 1;
                    quad->color = 0x202020;
                }
            }
            break;
        case 4:
            counter--;
            if (counter > 0) {
                color = 256 - counter * 28;
                color = color << 16 | color << 8 | color;
                for (j = 0; j < 6; j++) {
                    cubeStates[cubeIndex * 16 + j]->color = color;
                }
            } else {
                for (j = 0; j < 6; j++) {
                    cubeStates[cubeIndex * 16 + j]->color = 0xffffff;
                }
                eb->state = 5;
                counter = 16;
            }
            break;
        case 5:
            counter--;
            if (counter > 0) {
                color = counter * 255 / 16;
                color = color << 16 | color << 8 | color;
                for (j = 0; j < 6; j++) {
                    cubeStates[cubeIndex * 16 + j]->color = color;
                }
            } else {
                for (j = 0; j < 6; j++) {
                    cubeStates[cubeIndex * 16 + j]->flags.u8 = 6;
                }
                counter = 76;
                CUBE_TYPE_AT(x, y, z) = -1;
                eb->state = 0;
            }
        }
        eb->counter = counter;
    }
}

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

int DisableItemShadow(int entityIndex, int side, int const0) {
    Quad *quad;
    int cubeIndex;
    short *ed = entityData + entityIndex * 128;

    cubeIndex = CUBE_INDEX_AT(ed[125], ed[126], ed[127]);

    if ((int)cubeStates[cubeIndex * 16 + 15] == 0) {
        quad = (Quad*)cubeStates[cubeIndex * 16 + side];
        if (specialLevelType == 1) {
            quad->flags.i32 = (quad->flags.i32 & 0xfffeffff) | (const0 << 16);
        } else {
            quad->flags.b[3] = const0;
        }
    }

    if ((int)cubeStates[cubeIndex * 16 + 15] == 2) {
        quad = (Quad*)cubeStates[cubeIndex * 16 + side];
        quad->flags.b[3] = const0;
    }
}
