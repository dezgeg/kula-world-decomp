#include "common.h"

INCLUDE_ASM("asm/nonmatchings/skybox", RecalcSkyboxes01);

INCLUDE_ASM("asm/nonmatchings/skybox", RecalcSkyboxes2);

int Square(int param_1) {
    return param_1 * param_1;
}

int skyboxMaxAngle1;
int skyboxMaxAngle2;

void SkyboxSinCos(int angleval1, int angleval2, short *out1, short *out2, short *out3) {
    int cosParam2;
    int cosParam1;
    int sinParam2;
    int sinParam1;
    int angleval2_;
    int out1_val;

    if (angleval1 < 0) {
        angleval1 = angleval1 + skyboxMaxAngle1;
    }
    if (skyboxMaxAngle1 <= angleval1) {
        angleval1 = angleval1 - skyboxMaxAngle1;
    }
    if (angleval2 < 0) {
        angleval2 = 0;
    }
    angleval2_ = angleval2 << 11;
    if (skyboxMaxAngle2 <= angleval2) {
        angleval2 = skyboxMaxAngle2 - 1;
        angleval2_ = angleval2 << 11;
    }
    cosParam2 = rcos(angleval2_ / (skyboxMaxAngle2 - 1) + 0x400);
    cosParam1 = rcos((angleval1 << 12) / skyboxMaxAngle1);
    cosParam1 = cosParam1 * cosParam2;
    if (cosParam1 < 0) {
        cosParam1 = cosParam1 + 0xfff;
    }
    out1_val = cosParam1 >> 12;
    sinParam2 = rsin(angleval2_ / (skyboxMaxAngle2 - 1) + 0x400);
    sinParam1 = rsin((angleval1 << 12) / skyboxMaxAngle1);
    sinParam1 = sinParam1 * cosParam2;
    if (sinParam1 < 0) {
        sinParam1 = sinParam1 + 0xfff;
    }
    *out1 = (short)out1_val;
    *out2 = (short)sinParam2;
    *out3 = (short)(sinParam1 >> 12);
}
