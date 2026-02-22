#include "common.h"

INCLUDE_ASM("asm/nonmatchings/skybox", RecalcSkyboxes01);

INCLUDE_ASM("asm/nonmatchings/skybox", RecalcSkyboxes2);

int Square(int param_1) {
    return param_1 * param_1;
}

INCLUDE_ASM("asm/nonmatchings/skybox", SkyboxSinCos);
