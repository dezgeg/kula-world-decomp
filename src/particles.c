#include "common.h"

typedef struct Particle {
    POLY_FT4 prims[2][2];
    SVECTOR pos;
    void* animData;
    short rotation[2];
    int frame;
    int lightEffectId;
} Particle;

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
extern Particle particles[2][16];

extern int firstParticleTexture;
extern int numParticleTextures;
extern int cameraIndex;
extern int numCameras;
extern POLY_FT4 particlePolyTemplates[7];
extern Texture textures[150];

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
            particles[cameraIndex][numParticles[cameraIndex]].animData = PARTICLE_TYPE_PTRS[type];
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

// https://decomp.me/scratch/PaoVU
INCLUDE_ASM("asm/nonmatchings/particles", ProcessAndRenderParticles);
