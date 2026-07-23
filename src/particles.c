#include "common.h"

typedef struct Frame {
    short templateIdx;
    short angle;
    short scale;
    short color;
} Frame;

typedef struct ParticleType {
    short numPrims;
    short numFrames;
    short data[0];
} ParticleType;

typedef struct Particle {
    POLY_FT4 prims[2][2];
    SVECTOR pos;
    ParticleType* animData;
    short rotation[2];
    int frame;
    int lightEffectId;
} Particle;

extern void DisableLightEffect(int param_1);
extern uint Rand(int param_1);
extern void SetLightEffectColor(int param_1,uint param_2);

extern int cameraIndex;
extern int firstParticleTexture;
extern int numCameras;
extern int numParticleTextures;
extern int screenOffsetX;
extern int screenOffsetY;
extern int whichDrawDispEnv;
extern long projectionDistance;
extern MATRIX perspMatrixes[1];
extern Particle particles[2][16];
extern POLY_FT4 particlePolyTemplates[7];
extern PrimList primLists[2];
extern Texture textures[150];
extern void* otag[2][1][1026];

static byte PARTICLE_COLOR_DATA[7][3];
static short PARTICLE_TYPE_0[450];
static short PARTICLE_TYPE_1[258];
static short PARTICLE_TYPE_2_6_7[338];
static short PARTICLE_TYPE_3[394];
static short PARTICLE_TYPE_4_5_10[362];
static short PARTICLE_TYPE_8[354];
static short PARTICLE_TYPE_9[330];
static short* PARTICLE_TYPE_PTRS[12];
static int numParticles[2];

void InitParticles(void) {
    int i;

    for (i = 0; i < 12; i++) {
        PARTICLE_TYPE_PTRS[i] = NULL;
    }
    PARTICLE_TYPE_PTRS[0] = PARTICLE_TYPE_0;
    PARTICLE_TYPE_PTRS[1] = PARTICLE_TYPE_1;
    PARTICLE_TYPE_PTRS[2] = PARTICLE_TYPE_2_6_7;
    PARTICLE_TYPE_PTRS[3] = PARTICLE_TYPE_3;
    PARTICLE_TYPE_PTRS[4] = PARTICLE_TYPE_4_5_10;
    PARTICLE_TYPE_PTRS[5] = PARTICLE_TYPE_4_5_10;
    PARTICLE_TYPE_PTRS[6] = PARTICLE_TYPE_2_6_7;
    PARTICLE_TYPE_PTRS[7] = PARTICLE_TYPE_2_6_7;
    PARTICLE_TYPE_PTRS[8] = PARTICLE_TYPE_8;
    PARTICLE_TYPE_PTRS[9] = PARTICLE_TYPE_9;
    PARTICLE_TYPE_PTRS[10] = PARTICLE_TYPE_4_5_10;
    PARTICLE_COLOR_DATA[0][0] = 100;
    PARTICLE_COLOR_DATA[0][1] = 50;
    PARTICLE_COLOR_DATA[0][2] = 100;
    PARTICLE_COLOR_DATA[1][0] = 80;
    PARTICLE_COLOR_DATA[1][1] = 80;
    PARTICLE_COLOR_DATA[1][2] = 80;
    PARTICLE_COLOR_DATA[2][0] = 127;
    PARTICLE_COLOR_DATA[2][1] = 60;
    PARTICLE_COLOR_DATA[2][2] = 60;
    PARTICLE_COLOR_DATA[3][0] = 20;
    PARTICLE_COLOR_DATA[3][1] = 20;
    PARTICLE_COLOR_DATA[3][2] = 127;
    PARTICLE_COLOR_DATA[4][0] = 127;
    PARTICLE_COLOR_DATA[4][1] = 127;
    PARTICLE_COLOR_DATA[4][2] = 0;
    PARTICLE_COLOR_DATA[5][0] = 20;
    PARTICLE_COLOR_DATA[5][1] = 20;
    PARTICLE_COLOR_DATA[5][2] = 80;
    PARTICLE_COLOR_DATA[6][0] = 127;
    PARTICLE_COLOR_DATA[6][1] = 127;
    PARTICLE_COLOR_DATA[6][2] = 127;
    for (i = 0; i < numParticleTextures; i++) {
        if (i > 6) {
            break;
        }
        SetPolyFT4(&particlePolyTemplates[i]);
        setRGB0(&particlePolyTemplates[i], 0x80, 0x80, 0x80);
        SetSemiTrans(&particlePolyTemplates[i], textures[firstParticleTexture + i].semitrans);
        setUV4(&particlePolyTemplates[i], textures[firstParticleTexture + i].u,
               textures[firstParticleTexture + i].v,
               textures[firstParticleTexture + i].u + textures[firstParticleTexture + i].w + 0xff,
               textures[firstParticleTexture + i].v, textures[firstParticleTexture + i].u,
               textures[firstParticleTexture + i].v + textures[firstParticleTexture + i].h + 0xff,
               textures[firstParticleTexture + i].u + textures[firstParticleTexture + i].w + 0xff,
               textures[firstParticleTexture + i].v + textures[firstParticleTexture + i].h + 0xff);
        particlePolyTemplates[i].clut = textures[firstParticleTexture + i].clut;
        particlePolyTemplates[i].tpage = textures[firstParticleTexture + i].tpage;
    }
    numParticles[1] = 0;
    numParticles[0] = 0;
}

void AddParticles(int type, SVECTOR* pos, int lightEffectId) {
    if (PARTICLE_TYPE_PTRS[type] != NULL) {
        if (numParticles[cameraIndex] < 16 && (pos->vz < 0 || numCameras == 1)) {
            particles[cameraIndex][numParticles[cameraIndex]].animData = (ParticleType*)PARTICLE_TYPE_PTRS[type];
            particles[cameraIndex][numParticles[cameraIndex]].pos.vx = pos->vx;
            particles[cameraIndex][numParticles[cameraIndex]].pos.vy = pos->vy;
            particles[cameraIndex][numParticles[cameraIndex]].pos.vz = pos->vz;
            particles[cameraIndex][numParticles[cameraIndex]].frame = 0;
            particles[cameraIndex][numParticles[cameraIndex]].lightEffectId = lightEffectId;
            particles[cameraIndex][numParticles[cameraIndex]].rotation[0] = 0;
            particles[cameraIndex][numParticles[cameraIndex]].rotation[1] = 0;
            numParticles[cameraIndex]++;
        }
    }
}

void ProcessAndRenderParticles(void) {
    long sxy;
    long dummy;
    int i;
    int lightFlag;
    char colorR;
    char colorG;
    char colorB;
    Frame* fd;
    int j;

    int sx;
    int sy;

    int scaledCosVal;
    int scaledSinVal;
    int scale;
    int z;
    int clipped;
    int off;
    int t;

    for (i = 0; i < numParticles[cameraIndex]; i++) {
        colorB = 0;
        colorG = 0;
        colorR = 0;
        lightFlag = -1;
        while (particles[cameraIndex][i].frame >= particles[cameraIndex][i].animData->numFrames) {
            DisableLightEffect(particles[cameraIndex][i].lightEffectId);
            numParticles[cameraIndex]--;

            for (j = i; j < numParticles[cameraIndex]; j++) {
                particles[cameraIndex][j].pos = particles[cameraIndex][j + 1].pos;
                particles[cameraIndex][j].animData = particles[cameraIndex][j + 1].animData;
                particles[cameraIndex][j].frame = particles[cameraIndex][j + 1].frame;
                particles[cameraIndex][j].lightEffectId = particles[cameraIndex][j + 1].lightEffectId;
                particles[cameraIndex][j].rotation[0] = particles[cameraIndex][j + 1].rotation[0];
                particles[cameraIndex][j].rotation[1] = particles[cameraIndex][j + 1].rotation[1];
            }
            if (i >= numParticles[cameraIndex]) {
                return;
            }
        }
        if (i >= numParticles[cameraIndex]) {
            return;
        }

        // pArr[0] - template idx
        // pArr[1] - rotation angle (<0 -> set to random)
        // pArr[2] - scale (if negative some shear(?) effect)
        fd = (Frame*)particles[cameraIndex][i].animData;
        fd = (Frame*)((short*)fd + 2 + (((ParticleType*)fd)->numPrims * 4 * particles[cameraIndex][i].frame));
        for (j = 0; j < ((ParticleType*)particles[cameraIndex][i].animData)->numPrims; j++) {
            scale = abs(fd->scale);
            if (particles[cameraIndex][i].pos.vz  < 0) {
                particles[cameraIndex][i].prims[whichDrawDispEnv][j] = particlePolyTemplates[fd->templateIdx];
                setRGB0(&particles[cameraIndex][i].prims[whichDrawDispEnv][j], fd->color,fd->color,fd->color);
                sx = particles[cameraIndex][i].pos.vx;
                sy = particles[cameraIndex][i].pos.vy;
                if (fd->angle < 0) {
                    particles[cameraIndex][i].rotation[j] = Rand(0x1000);
                    scaledCosVal = scale * rcos(particles[cameraIndex][i].rotation[j]) >> 18;
                    scaledSinVal = scale * rsin(particles[cameraIndex][i].rotation[j]) >> 18;
                } else {
                    scaledCosVal = scale * rcos(fd->angle + particles[cameraIndex][i].rotation[j]) >> 18;
                    scaledSinVal = scale * rsin(fd->angle + particles[cameraIndex][i].rotation[j]) >> 18;

                }

                if (fd->scale >= 0) {
                    setXY4(&particles[cameraIndex][i].prims[whichDrawDispEnv][j],
                        sx + scaledCosVal, sy + scaledSinVal,
                        sx - scaledSinVal, sy + scaledCosVal,
                        sx + scaledSinVal, sy - scaledCosVal,
                        sx - scaledCosVal, sy - scaledSinVal);

                } else {
                    setXY4(&particles[cameraIndex][i].prims[whichDrawDispEnv][j],
                        sx + scaledCosVal, sy + scaledSinVal,
                        sx + scaledCosVal - scaledSinVal, sy + scaledCosVal + scaledSinVal,
                        sx, sy,
                        sx - scaledSinVal, sy + scaledCosVal);
                }
                addPrim(&primLists[whichDrawDispEnv].main, &particles[cameraIndex][i].prims[whichDrawDispEnv][j]);
            } else {
                SetRotMatrix(&perspMatrixes[cameraIndex]);
                SetTransMatrix(&perspMatrixes[cameraIndex]);
                z = RotTransPers(&particles[cameraIndex][i].pos, &sxy, &dummy, &dummy) << 2;

                if (z > 120) {
                    sx = (short)sxy;
                    sy = sxy >> 16;
                    if (fd->angle < 0) {
                        particles[cameraIndex][i].rotation[j] = Rand(0x1000);
                        t = rcos(particles[cameraIndex][i].rotation[j]) >> 16;
                        if (t == 0) {
                            t = 1;
                        }
                        scaledCosVal = (scale * projectionDistance) / t;
                        t = rsin(particles[cameraIndex][i].rotation[j]) >> 16;
                        if (t == 0) {
                            t = 1;
                        }
                        scaledSinVal = (scale * projectionDistance) / t;
                    } else {
                        scaledCosVal = ((scale * projectionDistance) / z) * rcos(fd->angle + particles[cameraIndex][i].rotation[j]) >> 16;
                        scaledSinVal = ((scale * projectionDistance) / z) * rsin(fd->angle + particles[cameraIndex][i].rotation[j]) >> 16;
                    }
                    clipped = screenOffsetX <= (sx + scaledCosVal & 0x7fff) && screenOffsetY <= (sy + scaledSinVal & 0x7fff) &&
                            screenOffsetX <= (sx - scaledSinVal & 0x7fff) && screenOffsetY <= (sy + scaledCosVal & 0x7fff) &&
                            screenOffsetX <= (sx + scaledSinVal & 0x7fff) && screenOffsetY <= (sy - scaledCosVal & 0x7fff) &&
                            screenOffsetX <= (sx - scaledCosVal & 0x7fff) && screenOffsetY <= (sy - scaledSinVal & 0x7fff);
                    if (!clipped) {
                        lightFlag = 0;
                        colorR += (fd->color * PARTICLE_COLOR_DATA[fd->templateIdx][0]) >> 9;
                        colorG += (fd->color * PARTICLE_COLOR_DATA[fd->templateIdx][1]) >> 9;
                        colorB += (fd->color * PARTICLE_COLOR_DATA[fd->templateIdx][2]) >> 9;
                        particles[cameraIndex][i].prims[whichDrawDispEnv][j] = particlePolyTemplates[fd->templateIdx];
                        setRGB0(&particles[cameraIndex][i].prims[whichDrawDispEnv][j],fd->color,fd->color,fd->color);
                        if (fd->scale >= 0) {
                            setXY4(&particles[cameraIndex][i].prims[whichDrawDispEnv][j],
                                sx + scaledCosVal, sy + scaledSinVal,
                                sx - scaledSinVal, sy + scaledCosVal,
                                sx + scaledSinVal, sy - scaledCosVal,
                                sx - scaledCosVal, sy - scaledSinVal);
                        } else {
                            setXY4(&particles[cameraIndex][i].prims[whichDrawDispEnv][j],
                                sx + scaledCosVal,  sy + scaledSinVal,
                                sx + scaledCosVal - scaledSinVal, sy + scaledCosVal + scaledSinVal,
                                sx, sy,
                                sx - scaledSinVal, sy + scaledCosVal);
                        }
                        addPrim(&otag[whichDrawDispEnv][cameraIndex][0], &particles[cameraIndex][i].prims[whichDrawDispEnv][j]);
                    }
                }
            }
            fd++;
        }
        if (lightFlag == 0) {
            SetLightEffectColor(particles[cameraIndex][i].lightEffectId, (colorR << 16) + (colorG << 8) | colorB);
        }

        particles[cameraIndex][i].frame++;
    }
}
