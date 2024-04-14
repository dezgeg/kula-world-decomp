#include "common.h"

#include <LIBGTE.H>
#include <LIBSPU.H>

__asm__(".set keyStatus_, keyStatus");
typedef struct SpuVoiceState {
    int tag;
    int volume;
    int sfxIndex;
} SpuVoiceState;

short playingBonusMusic;
short bonusMusicIndex;

SpuCommonAttr spuCommonAttr;
int sfxVolume;
int musicVolume;

extern SpuVoiceState spuVoiceState[];
extern SpuVoiceAttr perSfxVoiceAttrs[];
extern char keyStatus[]; /* pointless global */

extern char keyStatus_[];  // hack to make gcc not create induction variable of sprt

extern short SFX_REMAP_TABLE[];
int sndSwapPanDir;
int sfxPanning;

extern int lethargyMode;

static int curPlayingSfx;
static VECTOR panFactor;
static VECTOR panVectorSq;

static VECTOR* panFactorPtr = &panFactor;
static VECTOR* panVectorSqPtr = &panVectorSq;

int voiceIter;

void SndPlaySfx(int sfx, int tag, SVECTOR* dir, int volume) {
    int pan;
    curPlayingSfx = sfx;

    do { } while (0);

    if (curPlayingSfx < 120) {
        curPlayingSfx = SFX_REMAP_TABLE[sfx];
    }
    if (curPlayingSfx >= 0) {
        for (voiceIter = 0; voiceIter < 23; voiceIter++) {
            if (spuVoiceState[voiceIter].sfxIndex == -1) {
                break;
            }
        }

        if (spuVoiceState[voiceIter].sfxIndex == -1) {
            panFactor.vx = dir->vx;
            panFactor.vy = dir->vy;
            panFactor.vz = dir->vz;
            Square0(panFactorPtr, panVectorSqPtr);
            volume = (volume * sfxVolume) / 0xc;
            sfxPanning = 3 * SquareRoot0(panVectorSqPtr->vx + panVectorSqPtr->vy + panVectorSqPtr->vz);

            if (sfxPanning > volume) {
                sfxPanning = volume;
            }
            if (sfxPanning == 0) {
                perSfxVoiceAttrs[curPlayingSfx].volume.right = volume;
                perSfxVoiceAttrs[curPlayingSfx].volume.left = volume;
            } else {
                if (sndSwapPanDir) {
                    perSfxVoiceAttrs[curPlayingSfx].volume.left =
                        (volume - sfxPanning) + ((panFactorPtr->vx * (volume - sfxPanning)) / sfxPanning);
                    perSfxVoiceAttrs[curPlayingSfx].volume.right =
                        (volume - sfxPanning) - ((panFactorPtr->vx * (volume - sfxPanning)) / sfxPanning);
                } else {
                    perSfxVoiceAttrs[curPlayingSfx].volume.right =
                        (volume - sfxPanning) + ((panFactorPtr->vx * (volume - sfxPanning)) / sfxPanning);
                    perSfxVoiceAttrs[curPlayingSfx].volume.left =
                        (volume - sfxPanning) - ((panFactorPtr->vx * (volume - sfxPanning)) / sfxPanning);
                }
            }
            perSfxVoiceAttrs[curPlayingSfx].mask = 0;
            perSfxVoiceAttrs[curPlayingSfx].voice = 1 << voiceIter;
            if (tag > 0x8000) {
                perSfxVoiceAttrs[curPlayingSfx].note -= 0x100;
            }
            SpuSetVoiceAttr(&perSfxVoiceAttrs[curPlayingSfx]);
            if (tag > 0x8000) {
                perSfxVoiceAttrs[curPlayingSfx].note += 0x100;
            }
            SpuSetKey(1, 1 << voiceIter);
            if (!lethargyMode) {
                SpuSetReverbVoice(0, 1 << voiceIter);
            } else {
                SpuSetReverbVoice(1, 1 << voiceIter);
            }
            spuVoiceState[voiceIter].tag = tag;
            spuVoiceState[voiceIter].volume = volume;
            spuVoiceState[voiceIter].sfxIndex = curPlayingSfx;
        }
    }
}

void SndProcessSpuVoices(void) {
    int i;
    int sfxIndex;
    SpuGetAllKeysStatus(keyStatus_);

    for (i = 0; i < 24; i++) {
        if (keyStatus[i] == SPU_OFF) {
            spuVoiceState[i].sfxIndex = -1;
            spuVoiceState[i].tag = 0;
        }
        if (keyStatus[i] == SPU_ON_ENV_OFF) {
            SpuSetKey(0, 1 << i);
        }
        if (keyStatus[i] == SPU_OFF_ENV_ON) {
            spuVoiceState[i].sfxIndex = -1;
            spuVoiceState[i].tag = 0;
            /* ?!? This will underflow the array every time... */
            sfxIndex = spuVoiceState[i].sfxIndex;
            perSfxVoiceAttrs[sfxIndex].volume.right = 0;
            perSfxVoiceAttrs[sfxIndex].volume.left = 0;
            perSfxVoiceAttrs[sfxIndex].voice = 1 << i;
            perSfxVoiceAttrs[sfxIndex].mask = 3;
            SpuSetVoiceAttr(perSfxVoiceAttrs + sfxIndex);
        }
    }
}

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
