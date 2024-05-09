#include "common.h"

typedef struct DigitSprites {
    TSprite sprites[2][10];
} DigitSprites;

extern void DrawInt(DigitSprites* sprites, int style, int digits, int max, int value);
extern void UpdateStaticHourglassClut(void);

uint firstGuiTexture;
short* ggiPart1HourglassAnim;
int hourglassIsRotating;
int hourglassRotationTimer;
int levelTimeLeft;

extern POLY_FT4 hourglassSprites[2][3];
extern DigitSprites levelTimeLeftDigitSprites;
extern PrimList primLists[2];
extern Texture textures[150];
extern int whichDrawDispEnv;

void DrawHourglassAndTimer(void) {
    int i;
    int j;
    int color;
    int w;
    int dx;
    int secs;
    short* p;

    if (hourglassIsRotating) {
        hourglassRotationTimer += 8;
        if (hourglassRotationTimer >= 320) {
            hourglassRotationTimer = 0;
            hourglassIsRotating = 0;
            p = &ggiPart1HourglassAnim[hourglassRotationTimer];
            for (j = 0; j < 3; j++) {
                setXY4(&hourglassSprites[!whichDrawDispEnv][j],
                        p[j * 320 + 0] + 160, p[j * 320 + 1] + 26,
                        p[j * 320 + 2] + 160, p[j * 320 + 3] + 26,
                        p[j * 320 + 4] + 160, p[j * 320 + 5] + 26,
                        p[j * 320 + 6] + 160, p[j * 320 + 7] + 26);
            }
        }

        for (j = 0; j < 3; j++) {
            setXY4(&hourglassSprites[whichDrawDispEnv][j],
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 0] + 160,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 1] + 26,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 2] + 160,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 3] + 26,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 4] + 160,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 5] + 26,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 6] + 160,
                   ggiPart1HourglassAnim[hourglassRotationTimer + j * 320 + 7] + 26);
        }

        switch (hourglassRotationTimer) {
            case 0:
                for (i = 0; i < 2; i++) {
                    for (j = 1; j < 3; j++) {
                        setUV4(&hourglassSprites[i][j], textures[firstGuiTexture + j].u,
                               textures[firstGuiTexture + j].v,
                               textures[firstGuiTexture + j].u + textures[firstGuiTexture + j].w + 0xff,
                               textures[firstGuiTexture + j].v, textures[firstGuiTexture + j].u,
                               textures[firstGuiTexture + j].v + textures[firstGuiTexture + j].h + 0xff,
                               textures[firstGuiTexture + j].u + textures[firstGuiTexture + j].w + 0xff,
                               textures[firstGuiTexture + j].v + textures[firstGuiTexture + j].h + 0xff);
                    }
                }
                break;
            case 160:
                UpdateStaticHourglassClut();
                setUV4(&hourglassSprites[whichDrawDispEnv][1],
                       textures[firstGuiTexture + 1].u + textures[firstGuiTexture + 1].w + 0xff,
                       textures[firstGuiTexture + 1].v + textures[firstGuiTexture + 1].h + 0xff,
                       textures[firstGuiTexture + 1].u,
                       textures[firstGuiTexture + 1].v + textures[firstGuiTexture + 1].h + 0xff,
                       textures[firstGuiTexture + 1].u + textures[firstGuiTexture + 1].w + 0xff,
                       textures[firstGuiTexture + 1].v, textures[firstGuiTexture + 1].u,
                       textures[firstGuiTexture + 1].v);
                for (i = 0; i < 2; i++) {
                    setUV4(&hourglassSprites[i][2],
                           textures[firstGuiTexture + 2].u + textures[firstGuiTexture + 2].w + 0xff,
                           textures[firstGuiTexture + 2].v + textures[firstGuiTexture + 2].h + 0xff,
                           textures[firstGuiTexture + 2].u,
                           textures[firstGuiTexture + 2].v + textures[firstGuiTexture + 2].h + 0xff,
                           textures[firstGuiTexture + 2].u + textures[firstGuiTexture + 2].w + 0xff,
                           textures[firstGuiTexture + 2].v, textures[firstGuiTexture + 2].u,
                           textures[firstGuiTexture + 2].v);
                }
                break;
            case 168:
                setUV4(&hourglassSprites[whichDrawDispEnv][1],
                       textures[firstGuiTexture + 1].u + textures[firstGuiTexture + 1].w + 0xff,
                       textures[firstGuiTexture + 1].v + textures[firstGuiTexture + 1].h + 0xff,
                       textures[firstGuiTexture + 1].u,
                       textures[firstGuiTexture + 1].v + textures[firstGuiTexture + 1].h + 0xff,
                       textures[firstGuiTexture + 1].u + textures[firstGuiTexture + 1].w + 0xff,
                       textures[firstGuiTexture + 1].v, textures[firstGuiTexture + 1].u,
                       textures[firstGuiTexture + 1].v);
        }
    } else {
        UpdateStaticHourglassClut();
    }

    secs = 1 + levelTimeLeft / 50;
    if (secs <= 50) {
        w = textures[firstGuiTexture + 3].w / 13;
        if (secs >= 10) {
            dx = 0;

        } else {
            dx = -w / 2;
        }
        color = (50 - secs) * 128 / 50;

        for (j = 0; j < 10; j++) {
            setXY0(&levelTimeLeftDigitSprites.sprites[whichDrawDispEnv][j].sprt, dx + 152 + w * j, 50);
            setRGB0(&levelTimeLeftDigitSprites.sprites[whichDrawDispEnv][j].sprt, color, color, color);
        }

        DrawInt(&levelTimeLeftDigitSprites, 0, 2, 100, secs);
    }

    addPrim(&primLists[whichDrawDispEnv].gui1, &hourglassSprites[whichDrawDispEnv][2]);
    addPrim(&primLists[whichDrawDispEnv].gui1, &hourglassSprites[whichDrawDispEnv][1]);
    addPrim(&primLists[whichDrawDispEnv].gui1, &hourglassSprites[whichDrawDispEnv][0]);
}
