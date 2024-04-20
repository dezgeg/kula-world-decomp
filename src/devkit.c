#include "common.h"

extern void Noop(void);
extern void Noop2(void);
extern void SetDebugScreenshotFilenameSuffix(int param_1);

int screenshotIndex[13];
int screenshotNumBytes;
static RECT screenshotRect;
static uint screenshotTimHeader[5];  // TODO add initializer

extern byte saveReplayBuf[4076];
extern char debugFilenameBuf[];
extern char* DEBUG_SCREENSHOT_WORLD_NAMES[13];
extern int curWorld;
extern int devkitFileNumber;
extern int displayHeight;
extern int displayWidth;
extern int saveReplayIsFirstSequence;
extern int specialLevelType;
extern int whichDrawDispEnv;
extern byte* saveReplayWritePtr;

extern volatile short saveReplayCurrentButtons;
extern volatile byte saveReplayCurrentButtonsShuffled;
extern volatile int saveReplayLength;
extern volatile byte saveReplayRleButtonCount;

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
    int y = num;  // XXX: permuter mess
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

void ResetDevkitFileNumber(void) {
    devkitFileNumber = 0;
}

void InitReplaySaving(void) {
    saveReplayWritePtr = (byte*)0x131014;  // saveReplayBuf
    saveReplayRleButtonCount = 0;
    saveReplayCurrentButtonsShuffled = 0;
    saveReplayCurrentButtons = 0;
    saveReplayLength = 0;
    saveReplayIsFirstSequence = 1;
}

// XXX: this is a mess
void RecordButtonsToDevkit(s32 arg0) {
    s32 origArg0;
    s16 currentButtons;
    u8 currentButtonsShuffled;
    u8 newCount;
    u8* writePtr;
    u8 buttonCount;

    writePtr = saveReplayWritePtr;
    buttonCount = saveReplayRleButtonCount;
    currentButtonsShuffled = saveReplayCurrentButtonsShuffled;
    currentButtons = saveReplayCurrentButtons;

    *(s32*)0x131010 += 1;  // saveReplayLength++
    origArg0 = arg0;
    arg0 <<= 16;
    if (arg0 >> 16 == currentButtons) {
        newCount = buttonCount + 1;
    } else {
        if (*(s32*)0x131004 == 1) {  // saveReplayIsFirstSequence == 1
            *(s32*)0x131004 = 0;     // saveReplayIsFirstSequence = 0
        } else {
            *writePtr++ = buttonCount;
            *writePtr++ = currentButtonsShuffled;
        }
        newCount = 1;
        currentButtons = origArg0;
        currentButtonsShuffled = ((arg0 >> 20) & 0xF) | ((arg0 >> 21) & 0x10) |
                                 ((arg0 >> 22) & 0x20) | ((arg0 >> 24) & 0x40);
    }
    if ((u32)writePtr > 0x131FFFU) {
        writePtr = (u8*)0x131FFE;
    }
    saveReplayWritePtr = writePtr;
    saveReplayRleButtonCount = newCount;
    saveReplayCurrentButtonsShuffled = currentButtonsShuffled;
    saveReplayCurrentButtons = currentButtons;
}
