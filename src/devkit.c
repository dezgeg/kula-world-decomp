#include "common.h"

extern void Noop(void);
extern void Noop2(void);
extern void SetDebugScreenshotFilenameSuffix(int param_1);

extern int buttonSaveReplayMode;
extern int curWorld;
extern int displayHeight;
extern int displayWidth;
extern int specialLevelType;
extern int whichDrawDispEnv;

char debugFilenameBuf[256];

static int screenshotNumBytes;
static RECT screenshotRect;

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

static uint screenshotTimHeader[5] = {0x10, 0x2, 0x0, 0x0, 0x0};

#define devkitFileNumber (*(int*)DEVKIT_REPLAY_BUF)
#define saveReplayBuf (byte*)(DEVKIT_REPLAY_BUF + 0x14)
#define saveReplayLength (*(s32*)(DEVKIT_REPLAY_BUF + 0x10))
#define saveReplayIsFirstSequence *(s32*)(DEVKIT_REPLAY_BUF + 0x04)
#define saveReplayWritePtr *(byte**)(DEVKIT_REPLAY_BUF + 0x08)
#define saveReplayRleButtonCount *(byte*)(DEVKIT_REPLAY_BUF + 0x0c)
#define saveReplayCurrentButtonsShuffled *(byte*)(DEVKIT_REPLAY_BUF + 0x0d)
#define saveReplayCurrentButtons *(u16*)(DEVKIT_REPLAY_BUF + 0x0e)

#ifndef SKIP_UNUSED_CODE
void DebugSaveScreenshotToPc(char* prefix, int useWorldPrefix) {
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
        SetDebugScreenshotFilenameSuffix(screenshotIndex[0]++);
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
        SetDebugScreenshotFilenameSuffix(screenshotIndex[worldNum]++);
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
    char* p = debugFilenameBuf - 1;
    while (*++p) {
    }

    *p++ = '0' + num / 1000;
    num %= 1000;
    *p++ = '0' + num / 100;
    num %= 100;
    *p++ = '0' + num / 10;
    num %= 10;
    *p++ = '0' + num;
    *p++ = '.';
    *p++ = 't';
    *p++ = 'i';
    *p++ = 'm';
    *p++ = '\0';
}

void ResetDevkitFileNumber(void) {
    devkitFileNumber = 0;
}
#endif

void InitReplaySaving(void) {
    saveReplayWritePtr = saveReplayBuf;
    saveReplayRleButtonCount = 0;
    saveReplayCurrentButtonsShuffled = 0;
    saveReplayCurrentButtons = 0;
    saveReplayLength = 0;
    saveReplayIsFirstSequence = 1;
}

void RecordButtonsToDevkit(int buttons) {
    s32 origArg0;
    u8* writePtr;
    s32 count;
    u8 shuffled;
    s16 current;

    writePtr = saveReplayWritePtr;
    count = saveReplayRleButtonCount;
    shuffled = saveReplayCurrentButtonsShuffled;
    current = saveReplayCurrentButtons;

    saveReplayLength++;
    origArg0 = buttons;
    buttons <<= 16;
    if (buttons >> 16 == current) {
        count++;
    } else {
        if (saveReplayIsFirstSequence == 1) {
            saveReplayIsFirstSequence = 0;
        } else {
            *writePtr++ = count;
            *writePtr++ = shuffled;
        }
        count = 1;
        current = origArg0;
        do {
            shuffled = ((buttons >> (16 + 4)) & 0xF) |
                       ((buttons >> (16 + 5)) & 0x10) |
                       ((buttons >> (16 + 6)) & 0x20) |
                       ((buttons >> (16 + 8)) & 0x40);
        } while(0);
    }

    if ((u32)writePtr > (u32)DEVKIT_REPLAY_BUF + 0xFFFU) {
        writePtr = (u8*)((char*)DEVKIT_REPLAY_BUF + 0xFFE);
    }

    saveReplayWritePtr = writePtr;
    saveReplayRleButtonCount = count;
    saveReplayCurrentButtonsShuffled = shuffled;
    saveReplayCurrentButtons = current;
}

void WriteToDevkit(int param_1) {
    int fd;
    int len;
    byte* nextP;
    byte* p;
    int buts;
    int count;
    unsigned char new_var; // HACK

    if (param_1 == 0) {
        sprintf(debugFilenameBuf, "\\psx\\cube\\pad\\%d.pad", devkitFileNumber);
    } else {
        sprintf(debugFilenameBuf, "\\psx\\cube\\pad\\rescue.pad");
    }

    devkitFileNumber++;
    p = saveReplayWritePtr;
    count = saveReplayRleButtonCount;
    new_var = count;
    buts = saveReplayCurrentButtonsShuffled;
    *p++ = new_var;
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
        len = (int)p - (int)DEVKIT_REPLAY_BUF;
        if (len < 0) {
            len = 0x4000;
        }
#if !VER_EU
        if (len > 0x600000) {
            len = 0x600000;
        }
#else
        if (len > 0x6cf000) {
            len = 0x6cf000;
        }
#endif
        PCwrite(fd, DEVKIT_REPLAY_BUF, len);
        if (PCclose(fd) < 0) {
            FntPrint("error closing file:\n%s\n", debugFilenameBuf);
        }
    }

    Noop();
    saveReplayWritePtr = p;
    buttonSaveReplayMode = 0;
}
