#include "common.h"

extern void Noop(void);
extern void Noop2(void);
extern void SetDebugScreenshotFilenameSuffix(int param_1);

int screenshotNumBytes;
static RECT screenshotRect;

extern byte saveReplayBuf[4076];
extern char debugFilenameBuf[];
extern int buttonSaveReplayMode;
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


static char* DEBUG_SCREENSHOT_WORLD_NAMES[13] = {
    "hir",
    "hil",
    "inc",
    "arc",
    "cow",
    "fie",
    "atl",
    "haz",
    "mar",
    "hel",
    "xxx",
    "bon",
    "hid",
};
static int screenshotIndex[13] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static uint screenshotTimHeader[5] = {
    0x10, 0x2, 0x0, 0x0, 0x0
};

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
        sprintf(debugFilenameBuf, "%s", prefix);
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
        sprintf(debugFilenameBuf, "%s%s", prefix, DEBUG_SCREENSHOT_WORLD_NAMES[worldNum]);
        num = screenshotIndex[worldNum];
        screenshotIndex[worldNum]++;
        SetDebugScreenshotFilenameSuffix(num);
    }
    DrawSync(0);
    StoreImage(&screenshotRect, 0x600000); // TODO: use symbol for this
    DrawSync(0);
    Noop2();

    file = debugFilenameBuf;
    fd = PCcreat(file, 0);
    if (fd == -1) {
        FntPrint("can not create file:\n%s\n", file);
    } else {
        PCwrite(fd, screenshotTimHeader, 20);
        PCwrite(fd, 0x600000, screenshotNumBytes); // TODO: use symbol for this
        if (PCclose(fd) < 0) {
            FntPrint("error closing file:\n%s\n", file);
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

void ResetDevkitFileNumber(void) {
    devkitFileNumber = 0;
}

void InitReplaySaving(void) {
    saveReplayWritePtr = (byte*)0x131014; // saveReplayBuf
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

    *(s32*)0x131010 += 1; // saveReplayLength++
    origArg0 = arg0;
    arg0 <<= 16;
    if (arg0 >> 16 == currentButtons) {
        newCount = buttonCount + 1;
    } else {
        if (*(s32*)0x131004 == 1) { // saveReplayIsFirstSequence == 1
            *(s32*)0x131004 = 0;    // saveReplayIsFirstSequence = 0
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

void WriteToDevkit(int param_1) {
    int fd;
    int len;
    byte* nextP;
    byte* p;
    int buts;
    int count;

    if (param_1 == 0) {
        sprintf(debugFilenameBuf, "\\psx\\cube\\pad\\%d.pad", devkitFileNumber);
    } else {
        sprintf(debugFilenameBuf, "\\psx\\cube\\pad\\rescue.pad");
    }

    *(int*)0x131000 += 1; // devkitFileNumber++;
    p = saveReplayWritePtr;
    count = saveReplayRleButtonCount;
    buts = saveReplayCurrentButtonsShuffled;
    *p++ = count;
    *p++ = buts;
    *p++ = 0;
    *p++ = 0xff;
    if (((uint)p & 3) != 0) {
        p = (byte*)(((uint)p + 4) & 0xfffffffc);
    }
    Noop2();

    fd = PCcreat(debugFilenameBuf, 0);
    if (fd == -1) {
        FntPrint("can not create file:\n%s\n", debugFilenameBuf);
    } else {
        len = (int)p - 0x131000;
        if (len < 0) {
            len = 0x4000;
        }
        if (len > 0x6cf000) {
            len = 0x6cf000;
        }
        PCwrite(fd, 0x131000, len);
        if (PCclose(fd) < 0) {
            FntPrint("error closing file:\n%s\n", debugFilenameBuf);
        }
    }

    Noop();
    saveReplayWritePtr = p;
    buttonSaveReplayMode = 0;
}
