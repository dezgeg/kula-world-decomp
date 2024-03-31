#include "common.h"

#include <ctype.h>

// Based on AS2CSJIS.C and SJIS2ASC.C in PSX/SAMPLE/SCEE/ETC/CARDCONF/

extern ushort SJIS_RANGES[3][2];
extern ushort SJIS_SPECIAL_CHARS[34];
extern char S_SPECIAL_CHARS[];

typedef union Swapper {
    char byte[2];
    unsigned short word;
} Swapper;

int Ascii2Sjis(unsigned char ascii_code) {
    int sjis_code = 0;
    char temp;
    unsigned char stmp = 0;
    unsigned char stmp2 = 0;
    Swapper swapper;

    if ((ascii_code >= 0x20) && (ascii_code <= 0x2f))
        stmp2 = 1;
    else if ((ascii_code >= 0x30) && (ascii_code <= 0x39))
        stmp = 0;
    else if ((ascii_code >= 0x3a) && (ascii_code <= 0x40))
        stmp2 = 11;
    else if ((ascii_code >= 0x41) && (ascii_code <= 0x5a))
        stmp = 1;
    else if ((ascii_code >= 0x5b) && (ascii_code <= 0x60))
        stmp2 = 37;
    else if ((ascii_code >= 0x61) && (ascii_code <= 0x7a))
        stmp = 2;
    else if ((ascii_code >= 0x7b) && (ascii_code <= 0x7e))
        stmp2 = 63;
    else
        return 0;

    if (stmp2)
        sjis_code = SJIS_SPECIAL_CHARS[ascii_code - 0x20 - (stmp2 - 1)];
    else
        sjis_code = SJIS_RANGES[stmp][0] + ascii_code - SJIS_RANGES[stmp][1];

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

int LooksLikeAscii(char *str) {
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
