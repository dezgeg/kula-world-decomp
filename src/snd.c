#include "common.h"

#include <LIBSPU.H>

typedef struct SpuVoiceState {
    int tag;
    int volume;
    int sfxIndex;
} SpuVoiceState;

short playingBonusMusic = 0;
short bonusMusicIndex = 0;

SpuCommonAttr spuCommonAttr;
int musicVolume;

extern SpuVoiceState spuVoiceState[];
extern SpuVoiceAttr perSfxVoiceAttrs[];

void SndFadeVoiceVolume(int tag) {
    int i;
    int sfxIndex;
    short volumeL, volumeR;

    for (i = 0; i < 24; i++) {
        if (spuVoiceState[i].tag == tag) {
            break;
        }
    }

    /* Bug: this will read past the buffer if not found */
    if (spuVoiceState[i].tag == tag) {
        SpuGetVoiceVolume(i, &volumeL, &volumeR);

        volumeL -= 200;
        if (volumeL < 0) {
            volumeL = 0;
        }
        volumeR -= 200;
        if (volumeR < 0) {
            volumeR = 0;
        }

        sfxIndex = spuVoiceState[i].sfxIndex;
        perSfxVoiceAttrs[sfxIndex].voice = 1 << i;
        /* Bug: Yes, they really swapped left and right here! */
        perSfxVoiceAttrs[sfxIndex].volume.right = volumeL;
        perSfxVoiceAttrs[sfxIndex].volume.left = volumeR;
        perSfxVoiceAttrs[sfxIndex].mask = 3;
        SpuSetVoiceAttr(&perSfxVoiceAttrs[sfxIndex]);
    }
}

void SndMuteVoiceByTag(int tag) {
    int i;

    for (i = 0; i < 24; i++) {
        if (spuVoiceState[i].tag == tag) {
            break;
        }
    }

    /* Bug: this will read past the buffer if not found */
    if (spuVoiceState[i].tag == tag) {
        spuVoiceState[i].sfxIndex = -1;
        spuVoiceState[i].tag = 0;
        SpuSetKey(0, 1 << i);
    }
}

void SndMuteAllTaggedVoices(void) {
    char keyStatus[24];
    int i;

    SpuGetAllKeysStatus(&keyStatus[0]);
    for (i = 0; i < 24; i++) {
        if (keyStatus[i] && spuVoiceState[i].tag) {
            spuVoiceState[i].sfxIndex = -1;
            spuVoiceState[i].tag = 0;
            SpuSetKey(0, 1 << i);
        }
    }
}

void SndMuteAllVoices(void) {
    char keyStatus[24];
    int i;

    SpuGetAllKeysStatus(&keyStatus[0]);
    for (i = 0; i < 24; i++) {
        if (keyStatus[i]) {
            spuVoiceState[i].sfxIndex = -1;
            spuVoiceState[i].tag = 0;
            SpuSetKey(0, 1 << i);
        }
    }
}

void SndSetMusicVolume(void) {
    int vol;
    SpuCommonAttr* ca;

    spuCommonAttr.mask = 0x2c3;
    spuCommonAttr.mvol.left = 0x3fff;
    spuCommonAttr.mvol.right = 0x3fff;
    vol = musicVolume;
    if (playingBonusMusic == 1) {
        vol = musicVolume + musicVolume / 3;
        if (vol > 0xc) {
            vol = 0xc;
        }
    }

    ca = &spuCommonAttr;
    spuCommonAttr.cd.mix = 1;
    spuCommonAttr.cd.volume.left = (vol * 0x3fff) / 0xc;
    spuCommonAttr.cd.volume.right = spuCommonAttr.cd.volume.left;
    SpuSetCommonAttr(ca);
}
