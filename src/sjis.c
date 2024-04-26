#include "common.h"

#include <ctype.h>

// Based on ASC2SJIS.C and SJIS2ASC.C in PSX/SAMPLE/SCEE/ETC/CARDCONF/

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

typedef union Swapper {
    char byte[2];
    unsigned short word;
} Swapper;

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

    swapper.word = sjis_code;
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
            if (SJIS_SPECIAL_CHARS[i] == sjis_code) ascii_code = S_SPECIAL_CHARS[i];
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
