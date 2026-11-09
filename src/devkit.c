#include "common.h"
#include <ctype.h>

typedef union Swapper {
    char byte[2];
    unsigned short word;
} Swapper;

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

static char* replayPlaybackPtr;
static char replayPlaybackRleCounter;
static char replayRawButtons;
static int replayEnded;
static int screenshotNumBytes;
static RECT screenshotRect;
static short replayPlaybackCurrentButtons;

ushort SJIS_RANGES[3][2] = {
    {0x824f, 0x30},
    {0x8260, 0x41},
    {0x8281, 0x61},
};
ushort SJIS_SPECIAL_CHARS[] = {
    0x8140, 0x8149, 0x8168, 0x8194, 0x8190, 0x8193, 0x8195, 0x8166, 0x8169, 0x816a, 0x8196, 0x817b,
    0x8143, 0x817c, 0x8144, 0x815e, 0x8146, 0x8147, 0x8171, 0x8181, 0x8172, 0x8148, 0x8197, 0x816d,
    0x818f, 0x816e, 0x814f, 0x8151, 0x8165, 0x816f, 0x8162, 0x8170, 0x815d, 0x8150,
};
char S_SPECIAL_CHARS[] = {
    ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ':',
    ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '-', '~',

};

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

// Based on ASC2SJIS.C and SJIS2ASC.C in PSX/SAMPLE/SCEE/ETC/CARDCONF/
int Ascii2Sjis(unsigned char ascii) {
    int sjis_code = 0;
    char temp;
    unsigned char stmp = 0;
    unsigned char stmp2 = 0;
    Swapper swapper;

    if (ascii >= 0x20 && ascii <= 0x2f)
        stmp2 = 1;
    else if (ascii >= 0x30 && ascii <= 0x39)
        stmp = 0;
    else if (ascii >= 0x3a && ascii <= 0x40)
        stmp2 = 11;
    else if (ascii >= 0x41 && ascii <= 0x5a)
        stmp = 1;
    else if (ascii >= 0x5b && ascii <= 0x60)
        stmp2 = 37;
    else if (ascii >= 0x61 && ascii <= 0x7a)
        stmp = 2;
    else if (ascii >= 0x7b && ascii <= 0x7e)
        stmp2 = 63;
    else
        return 0;

    if (stmp2)
        sjis_code = SJIS_SPECIAL_CHARS[ascii - 0x20 - (stmp2 - 1)];
    else
        sjis_code = SJIS_RANGES[stmp][0] + ascii - SJIS_RANGES[stmp][1];

    swapper.word = sjis_code;

    temp = swapper.byte[0];
    swapper.byte[0] = swapper.byte[1];
    swapper.byte[1] = temp;

    return swapper.word;
}

int Sjis2Ascii(unsigned short sjis_code) {
    char temp;
    unsigned char ascii_code = -1;
    int i;
    Swapper swapper;

    swapper = (Swapper)sjis_code;
    temp = swapper.byte[1];
    swapper.byte[1] = swapper.byte[0];
    swapper.byte[0] = temp;

    if (swapper.byte[1] == 0 || swapper.byte[0] == 0) {
        return 0;
    }

    sjis_code = swapper.word;

    if ((sjis_code >= 0x8260) && (sjis_code <= 0x8279)) {
        ascii_code = 65 + (sjis_code - 0x8260);
    } else if ((sjis_code >= 0x8281) && (sjis_code <= 0x829A)) {
        ascii_code = 97 + (sjis_code - 0x8281);
    } else if ((sjis_code >= 0x824F) && (sjis_code <= 0x8258)) {
        ascii_code = 48 + (sjis_code - 0x824F);
    } else {
        for (i = 0; i < 33; i++) {
            if (SJIS_SPECIAL_CHARS[i] == sjis_code)
                ascii_code = S_SPECIAL_CHARS[i];
        }
    }

    return ascii_code;
}

int LooksLikeAscii(char* str) {
    char* p;
    int ret = 1;
    int asciiScore = 0;
    int length = strlen(str);
    if (length > 64) {
        length = 64;
    }

    for (p = str; p < str + length; p++) {
        if (!isprint(*p)) {
            return 0;
        }
        if ((*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') || (*p >= '0' && *p <= '9')) {
            asciiScore++;
        }
        if (*p == 0x82 || *p == 0x60) {
            asciiScore--;
        }
    }
    return asciiScore >= 4;
}

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
#if VER_JP
    saveReplayIsFirstSequence = 0;
#endif
    saveReplayWritePtr = saveReplayBuf;
    saveReplayRleButtonCount = 0;
    saveReplayCurrentButtonsShuffled = 0;
    saveReplayCurrentButtons = 0;
    saveReplayLength = 0;
#if !VER_JP
    saveReplayIsFirstSequence = 1;
#endif
}

void RecordButtonsToDevkit(int buttons) {
    s32 maskedButtons;
    u8* writePtr;
    s32 count;
    u8 shuffled;
    s16 current;
    s32 rawButtons;

    writePtr = saveReplayWritePtr;
    count = saveReplayRleButtonCount;
    shuffled = saveReplayCurrentButtonsShuffled;
    current = saveReplayCurrentButtons;

    saveReplayLength++;
    rawButtons = buttons;
#if VER_JP
    if (buttons & 0xf000) {
        rawButtons = buttons | 0x4000;
    }
#endif
    maskedButtons = VER_JP ? (rawButtons & 0x4af0) : rawButtons;
    buttons = maskedButtons << 16;

    if (VER_JP ? maskedButtons == current : buttons >> 16 == current) {
        count++;
#if VER_JP
        if (count >= 0x100) {
            *writePtr++ = 0xff;
            *writePtr++ = rawButtons = shuffled;
            count = 1;
        }
#endif
    } else {
        if (saveReplayIsFirstSequence == 1) {
            saveReplayIsFirstSequence = 0;
        } else {
            *writePtr++ = count;
            *writePtr++ = shuffled;
        }
        count = 1;
        current = maskedButtons;
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
    byte* p;
    byte count;
    byte buts;

    if (param_1 == 0) {
        sprintf(debugFilenameBuf, "\\psx\\cube\\pad\\%d.pad", devkitFileNumber);
    } else {
        sprintf(debugFilenameBuf, "\\psx\\cube\\pad\\rescue.pad");
    }

    devkitFileNumber++;
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

int InitReplayPlayback(char* param_1) {
    replayEnded = 0;
    replayPlaybackPtr = param_1 + 20;
    replayPlaybackRleCounter = *replayPlaybackPtr++;
    replayRawButtons = *replayPlaybackPtr++;
    replayPlaybackCurrentButtons = ((replayRawButtons << 4) & 0xf0) | ((replayRawButtons << 5) & 0x200) |
                                   ((replayRawButtons << 6) & 0x800) | ((replayRawButtons << 8) & 0x4000);
    return *(int*)(param_1 + 16);
}

int GetButtonsFromReplay(void) {
    if (!replayEnded) {
        if (replayPlaybackRleCounter != 0) {
            replayPlaybackRleCounter--;
        } else {
            replayPlaybackRleCounter = *replayPlaybackPtr++;
            replayPlaybackRleCounter--;
            replayRawButtons = *replayPlaybackPtr++;
            if (replayRawButtons == 0xff) {
                replayEnded = 1;
                return 0;
            }
            replayPlaybackCurrentButtons = ((replayRawButtons << 4) & 0xf0) | ((replayRawButtons << 5) & 0x200) |
                                           ((replayRawButtons << 6) & 0x800) | ((replayRawButtons << 8) & 0x4000);
        }
        return replayPlaybackCurrentButtons;
    }
    return 0;
}

#ifndef SKIP_UNUSED_CODE
void UnusedLoadFullScreenPicture(void* param_1) {
    int halfPaddingY;
    int halfPaddingX;
    int height;
    uint heightAndWidth;
    int width;
    RECT rect;

    heightAndWidth = *(uint*)(param_1 + 0x10);
    width = heightAndWidth & 0xffff;
    height = heightAndWidth >> 0x10;
    halfPaddingX = (displayWidth - width) / 2;
    halfPaddingY = (displayHeight - height) / 2;
    DrawSync(0);

    rect.x = 0;
    rect.y = 0;
    rect.w = displayWidth * 2;
    rect.h = displayHeight;
    ClearImage(&rect, 0, 0, 0);

    rect.x = halfPaddingX;
    rect.y = halfPaddingY;
    rect.w = width;
    rect.h = height;
    LoadImage(&rect, param_1 + 0x14);

    rect.x = displayWidth + halfPaddingX;
    rect.y = halfPaddingY;
    rect.w = width;
    rect.h = height;
    LoadImage(&rect, param_1 + 0x14);

    DrawSync(0);
}
#endif

int randomSeed = 1;

void Srand(int seed) {
    randomSeed = seed;
}

int Rand(unsigned v) {
    int seed = randomSeed * 0x41c64e6d + 12345;
    randomSeed = seed;
    return (v * (seed & 0xFFFF)) >> 16;
}
