#include "common.h"

#include <LIBGTE.H>
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

void SndUpdateVolumeBasedOnDirVec(int tag, SVECTOR* pan) {
    int i;
    int sfxIndex;
    int curVolume;
    int newVolume;
    int delta;

    VECTOR panVec;
    VECTOR panSqrVec;
    /* XXX: Did they really declare these pointless pointers? */
    long* panVecPtr = &panVec.vx;
    long* panSqrVecPtr = &panSqrVec.vx;

    for (i = 0; i < 24; i++) {
        if (spuVoiceState[i].tag == tag) {
            break;
        }
    }

    /* Bug: this will read past the buffer if not found */
    if (spuVoiceState[i].tag == tag) {
        curVolume = spuVoiceState[i].volume;
        sfxIndex = spuVoiceState[i].sfxIndex;
        panVec.vx = pan->vx;
        panVec.vy = pan->vy;
        panVec.vz = pan->vz;
        Square0(panVecPtr, panSqrVecPtr);
        newVolume = SquareRoot0(panSqrVecPtr[0] + panSqrVecPtr[1] + panSqrVecPtr[2]) * 3;
        if (newVolume > curVolume) {
            newVolume = curVolume;
        }
        if (newVolume == 0) {
            perSfxVoiceAttrs[sfxIndex].volume.right = curVolume >> 1;
            perSfxVoiceAttrs[sfxIndex].volume.left = curVolume >> 1;
        } else {
            delta = curVolume - newVolume;
            perSfxVoiceAttrs[sfxIndex].volume.right = delta + (*panVecPtr * delta) / newVolume >> 1;
            perSfxVoiceAttrs[sfxIndex].volume.left = delta - (*panVecPtr * delta) / newVolume >> 1;
        }
        perSfxVoiceAttrs[sfxIndex].voice = 1 << i;
        perSfxVoiceAttrs[sfxIndex].mask = 3;
        SpuSetVoiceAttr(&perSfxVoiceAttrs[sfxIndex]);
    }
}

void SndUpdateVoiceNote(int tag, int note) {
    int i;
    int sfxIndex;
    short volumeL, volumeR;

    for (i = 0; i < 24; i++) {
        int lsb = tag & 0xf;
        int res = (tag & 0x7f00) + note;
        if ((spuVoiceState[i].tag & 0xf) == lsb) {
            sfxIndex = spuVoiceState[i].sfxIndex;
            if (sfxIndex > 24) {
                perSfxVoiceAttrs[sfxIndex].mask = SPU_VOICE_NOTE;
                perSfxVoiceAttrs[sfxIndex].note = res;
                SpuSetVoiceAttr(&perSfxVoiceAttrs[sfxIndex]);
            }
        }
    }
}

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
