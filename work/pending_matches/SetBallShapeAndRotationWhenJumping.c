#include "common.h"

// Define these as static to encourage gp-relative access in this file if they were defined here,
// but the original has them as externs in other files (or just global).
// Actually, let's just use extern and see.
short landingSquishFrameCounter;
short landingSquishMagnitude;
short landingSquishMagnitudeIncrement;
short DAT_000a4374;

void SetBallShapeAndRotationWhenJumping(Player *player) {
    int rot;
    
    rot = player->rotX * -6;
    RotMatrixX(rot, &player->matrix_254);
    
    if (player->field_2bc > -750 && player->jumpingOrViewportRotationTimer > 12) {
        player->field_2bc += DAT_000a4374;
    }
    
    if (player->jumpingOrViewportRotationTimer < -4) {
        landingSquishFrameCounter--;
        player->field_2bc += landingSquishMagnitudeIncrement;
        if (player->field_2bc > 0) {
            player->field_2bc = 0;
            landingSquishMagnitude += landingSquishMagnitudeIncrement;
            player->ballMorphShape = landingSquishMagnitude;
        }
    }
}
