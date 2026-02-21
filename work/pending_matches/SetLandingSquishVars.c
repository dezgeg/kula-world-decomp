#include "common.h"
short landingSquishDamping;
short landingSquishFrameCounter;
short landingSquishMagnitude;
short landingSquishMagnitudeIncrement;

void SetLandingSquishVars(void) {
    int pad[2];

    landingSquishFrameCounter = 4;
    landingSquishMagnitudeIncrement = 187;
    landingSquishMagnitude = 0;
    landingSquishDamping = 100;
}
