#include "common.h"

extern int displayHeight;
extern int displayWidth;

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
    rect.w = displayWidth << 1;
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
