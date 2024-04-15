#include "common.h"

extern void Noop(void);
extern void Noop2(void);
extern void SetDebugScreenshotFilenameSuffix(int param_1);

int screenshotIndex[13];
int screenshotNumBytes;
static RECT screenshotRect;
static uint screenshotTimHeader[5];  // TODO add initializer

extern char* DEBUG_SCREENSHOT_WORLD_NAMES[13];
extern int curWorld;
extern char debugFilenameBuf[];
extern int displayHeight;
extern int displayWidth;
extern int specialLevelType;
extern int whichDrawDispEnv;

extern char S_FMTs_2[];
extern char S_FMTsFMTs[];
extern char S_can_not_create_file_FMTs[];
extern char S_error_closing_file_FMTs[];

void DebugSaveScreenshotToPc(char* prefix, int useWorldPrefix) {
    int num;
    int worldNum;
    int fd;
    char* file;

    screenshotRect.x = 0;
    screenshotRect.y = 0;
    screenshotRect.w = displayWidth;
    screenshotRect.h = displayHeight;
    screenshotNumBytes = displayWidth * displayHeight * 2;
    screenshotTimHeader[2] = screenshotNumBytes + 12;
    screenshotTimHeader[4] = displayHeight << 16 | displayWidth;

    if (whichDrawDispEnv == 0) {
        screenshotRect.x = screenshotRect.w;
    }

    if (useWorldPrefix == 0) {
        sprintf(debugFilenameBuf, S_FMTs_2, prefix);
        num = screenshotIndex[0];
        screenshotIndex[0]++;
        SetDebugScreenshotFilenameSuffix(num);
    } else {
        worldNum = 10;
        if (curWorld < 10) {
            worldNum = curWorld;
        }
        if (specialLevelType == 1) {
            worldNum = 11;
        }
        if (specialLevelType == 2) {
            worldNum = 12;
        }
        sprintf(debugFilenameBuf, S_FMTsFMTs, prefix, DEBUG_SCREENSHOT_WORLD_NAMES[worldNum]);
        num = screenshotIndex[worldNum];
        screenshotIndex[worldNum]++;
        SetDebugScreenshotFilenameSuffix(num);
    }
    DrawSync(0);
    StoreImage(&screenshotRect, 0x600000);  // TODO: use symbol for this
    DrawSync(0);
    Noop2();

    file = debugFilenameBuf;
    fd = PCcreat(file, 0);
    if (fd == -1) {
        FntPrint(S_can_not_create_file_FMTs, file);
    } else {
        PCwrite(fd, screenshotTimHeader, 20);
        PCwrite(fd, 0x600000, screenshotNumBytes);  // TODO: use symbol for this
        if (PCclose(fd) < 0) {
            FntPrint(S_error_closing_file_FMTs, file);
        }
    }
    Noop();
}

void SetDebugScreenshotFilenameSuffix(int num) {
    char* p = debugFilenameBuf;
    int y = num; // XXX: permuter mess
    while (*p) {
        p++;
    }

    *(p++) = '0' + y / 1000;
    num = y = y % 1000;
    *(p++) = ((num / 100) + '0');
    *(p++) = ((num %= 100) / 10) + '0';
    *(p++) = '0' + (y = num % 10);
    *(p++) = '.';
    *(p++) = 't';
    *(p++) = 'i';
    *(p++) = 'm';
    *(p++) = '\0';
}
