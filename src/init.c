#include "common.h"

// Prototypes
extern void ResetVibration(void);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering, u_char use24Bit);

// gprel-used variables (defined in this file)
int latestControllerSlotPolled;

// non-gprel-used variables (extern)
extern int displayHeight;
extern int displayWidth;
extern char padData[2][34];
extern u_char padVibrationModeEntered[2];
extern long projectionDistance;

void InitStuff(void)

{
    int j;
    int i;
    RECT rect;

    ResetCallback();
    ResetGraph(0);
    SetGraphDebug(0);
    MemCardInit(0);
    MemCardStart();
    PadInitDirect(&padData[0][0], &padData[1][0]);
    PadStartCom();

    for (i = 0; i < 2; i++) {
        padVibrationModeEntered[i] = 0;
        for (j = 0; j < 34; j++) {
            padData[i][j] = 0;
        }
    }

    ResetVibration();
    latestControllerSlotPolled = 0;
    InitGeom();
    SetGeomScreen(projectionDistance);
    SetVideoMode(1);
    SetDispMask(1);
    rect.w = 640;
    rect.x = 0;
    rect.y = 0;
    rect.h = 256;
    ClearImage(&rect, 0, 0, 0);
    SetupDisplay(0, 0, 0, 0, 0, 0);
    FntLoad(576, 256);
    SetDumpFnt(FntOpen(16, 8, displayWidth + -0x10, displayHeight + -8, 0,
                        (displayWidth / 8 - 2) * (displayHeight / 8 - 1)));
    return;
}
