#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <libgte.h>
#include <libgpu.h>

typedef char undefined;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef unsigned char byte;
typedef signed char sbyte;

enum PadButton {
    PAD_SELECT = 0x0001,
    PAD_L3 = 0x0002,
    PAD_R3 = 0x0004,
    PAD_START = 0x0008,
    PAD_U = 0x0010,
    PAD_R = 0x0020,
    PAD_D = 0x0040,
    PAD_L = 0x0080,
    PAD_L2 = 0x0100,
    PAD_R2 = 0x0200,
    PAD_L1 = 0x0400,
    PAD_R1 = 0x0800,
    PAD_TRIANGLE = 0x1000,
    PAD_CIRCLE = 0x2000,
    PAD_CROSS = 0x4000,
    PAD_SQUARE = 0x8000,
};

typedef enum HowMoving {
    JUMPING_INPLACE = 0,
    JUMPING_FORWARD = 1,
    ROLLING = 2,
    FALLING = 6,
    ROTATING = 7,
    NOT_MOVING = -1,
} HowMoving;

typedef struct PrimList {
    void* main;
    void* gui1;
    void* gui2;
    void* gui3;
} PrimList;

typedef struct TSprite {
    DR_TPAGE tpage;
    SPRT sprt;
} TSprite;

typedef struct TPolyF4 {
    DR_TPAGE tpage;
    POLY_F4 poly;
} TPolyF4;

typedef struct Texture {
    u_char bitsPerPixel;
    u_char semitrans;
    short clut;
    short tpage;
    u_char u;
    u_char v;
    short w;
    short h;
} Texture;

typedef struct DrawDisp {
    DRAWENV draw;
    DISPENV disp;
} DrawDisp;

typedef struct Highscore {
    int score;
    short level;
    short fruits;
    int time;
    char name[8];
} Highscore;

typedef struct PsxSaveHeader {
    char magic[2]; /* SC */
    byte iconFlags;
    byte blockNumber;
    ushort title[32]; /* SJIS */
    byte unused[28];
    short iconClut[16];
    byte iconBitmap[128];
} PsxSaveHeader;

typedef struct SaveSlot {
    int levelScores[150];
    int score;
    int playtime;
    short fruits;
    short curLevel;
    short curWorld;
    short fruitBitmask;
    short valid;
    short gameMode;
    short timeTrialDifficulty;
    short isFinal;
} SaveSlot;

typedef struct MemcardData {
    PsxSaveHeader header;
    short arcadeModeDone;
    short highestLevelReached;
    short musicVolume;
    short sfxVolume;
    short screenX;
    short screenY;
    Highscore highscore0;
    int highscore0LevelScores[150];
    SaveSlot saveslots[4];
    int turnDelayEnabled;
    int vibrationEnabled;
    byte pad[4804];
} MemcardData;

typedef struct Music {
    char* filename;
    ushort chan;
    short sectors;
} Music;

typedef struct Quad {
    uint flags;
    short otagIndex;
    byte textureRotation;
    byte pad;
    void* metadata;
    uint color;
    int unk;
    int rgb0;
    int rgb1;
    int rgb2;
    int xy0;
    int xy1;
    int xy2;
    int xy3;
    int z01;
    int z23;
    byte reserved[104];
} Quad;

typedef struct TextureChainEntry {
    Quad** quadPtr;
    void* anim;
} TextureChainEntry;

typedef struct AnimatedTextureChain {
    void* endPtr;
    TextureChainEntry* entries;
    int numFrames1;
    int numFrames2;
    void* ptr1;
    void* ptr2;
    void* ptr3;
    void* ptr4;
    int initToAlwaysZero9;
    int initToAlwaysZero10;
} AnimatedTextureChain;

typedef struct Enemy {
    SVECTOR dir;
    SVECTOR field1_0x8;
    SVECTOR normalVec;
    SVECTOR pos;
    SVECTOR initPos;
    int enemyType;
    byte pad_2c[12];
    SVECTOR rotationVec;
    byte pad_40[44];
    MATRIX matrix;
    MATRIX matrix2;
    short state;
    short timer;
    short field_b0;
    byte pad_b2[2];
} Enemy;

typedef struct MovingPlatformEntity {
    short tag;
    short movementDirection;
    short dir;
    short unk6;
    short startX, startY, startZ;
    short endX, endY, endZ;
    byte pad_14[12];
    ushort flags;
    short length;
    short velocity;
    short counter;
    short cubeStyle;
    short velX, velY, velZ;
    byte pad_30[190];
    short posX, posY, posZ;
    byte pad_f4[12];
} MovingPlatformEntity;

typedef struct Player {
    int howMoving0;
    SVECTOR facingDirBeforeStartingTurning;
    SVECTOR facingDir;
    SVECTOR rightVec;
    SVECTOR gravityDir;
    SVECTOR perspVec3;
    SVECTOR perspVec1;
    SVECTOR perspVec2;
    SVECTOR finePos;
    SVECTOR jumpStartPos;
    SVECTOR jumpVec;
    SVECTOR svec54;
    int turnDirection;
    int rollingForward;
    int jumping;
    int surroundingBlocks[3][3][3];
    MATRIX matrix_d4;
    undefined field17_0xf4;
    undefined field18_0xf5;
    undefined field19_0xf6;
    undefined field20_0xf7;
    undefined field21_0xf8;
    undefined field22_0xf9;
    undefined field23_0xfa;
    undefined field24_0xfb;
    undefined field25_0xfc;
    undefined field26_0xfd;
    undefined field27_0xfe;
    undefined field28_0xff;
    undefined field29_0x100;
    undefined field30_0x101;
    undefined field31_0x102;
    undefined field32_0x103;
    undefined field33_0x104;
    undefined field34_0x105;
    undefined field35_0x106;
    undefined field36_0x107;
    undefined field37_0x108;
    undefined field38_0x109;
    undefined field39_0x10a;
    undefined field40_0x10b;
    undefined field41_0x10c;
    undefined field42_0x10d;
    undefined field43_0x10e;
    undefined field44_0x10f;
    undefined field45_0x110;
    undefined field46_0x111;
    undefined field47_0x112;
    undefined field48_0x113;
    MATRIX perspMatrix;
    short field_134;
    short gravityVelocity;
    short movementVelocity;
    undefined field53_0x13a;
    undefined field54_0x13b;
    undefined field55_0x13c;
    undefined field56_0x13d;
    undefined field57_0x13e;
    undefined field58_0x13f;
    short rotX;
    undefined field60_0x142;
    undefined field61_0x143;
    SVECTOR svec_144;
    SVECTOR subpixelPositionOnCube;
    SVECTOR svec_154;
    int specialBlockSideOffsetPlayerIsStandingOn;
    int specialBlockIndexPlayerIsStandingOn;
    int faceTypePlayerStandingOn;
    short startTurningLeftNextFrame;
    short startTurningRightNextFrame;
    short turnDelayTimer;
    short turningRightRelated;
    short turningLeftRelated;
    short movementInhibitTimer;
    short forcedRollForwardTimer;
    short debugCamY;
    short debugCamX;
    short debugCameraParam;
    undefined field78_0x17c;
    undefined field79_0x17d;
    undefined field80_0x17e;
    undefined field81_0x17f;
    undefined field82_0x180;
    undefined field83_0x181;
    short debugCameraMode;
    SVECTOR svec_184;
    undefined field86_0x18c;
    undefined field87_0x18d;
    undefined field88_0x18e;
    undefined field89_0x18f;
    undefined field90_0x190;
    undefined field91_0x191;
    undefined field92_0x192;
    undefined field93_0x193;
    int jumpingOrViewportRotationTimer;
    int howMoving198;
    int longJump;
    short* jumpdataPtr;
    int startTurningTo;
    int unusedTurningWhere;
    int field100_0x1ac;
    undefined field101_0x1b0;
    undefined field102_0x1b1;
    short turningTimer;
    short viewpointRotationTimer;
    undefined field105_0x1b6;
    undefined field106_0x1b7;
    undefined field107_0x1b8;
    undefined field108_0x1b9;
    short turningDelta;
    short viewpointRotationAngleIncrement;
    undefined field111_0x1be;
    undefined field112_0x1bf;
    undefined field113_0x1c0;
    undefined field114_0x1c1;
    short turningPhase;
    short viewportRotationAngle;
    undefined field117_0x1c6;
    undefined field118_0x1c7;
    int field119_0x1c8;
    short playerHasControl;
    undefined field121_0x1ce;
    undefined field122_0x1cf;
    undefined field123_0x1d0;
    undefined field124_0x1d1;
    short field_1d2;
    undefined field126_0x1d4;
    undefined field127_0x1d5;
    undefined field128_0x1d6;
    undefined field129_0x1d7;
    undefined field130_0x1d8;
    undefined field131_0x1d9;
    undefined field132_0x1da;
    undefined field133_0x1db;
    undefined field134_0x1dc;
    undefined field135_0x1dd;
    undefined field136_0x1de;
    undefined field137_0x1df;
    undefined field138_0x1e0;
    undefined field139_0x1e1;
    short field_1e2;
    undefined field141_0x1e4;
    undefined field142_0x1e5;
    short onMovingPlatform;
    short jumpingOnMovingPlatform;
    short copycatMoveIndex;
    short alreadyProcessedEntityAction;
    short onGround;
    SVECTOR cameraR1R2TurnDirection;
    SVECTOR cameraR1TurnAmount;
    int ballBlinking;
    int fireTimer;
    int iceColorChangeTimer;
    int acidTimer;
    int movingPlatformEntityIdStandingOn;
    int jumpingInplaceOnTopOfMovingPlatform;
    int turningWhere;
    int delayedLevelEndReason;
    int sunglassTimer;
    int lethargyTimer;
    int bounceTimer;
    int invulnerabilityTimer;
    int startedIceSfx;
    MATRIX matrix_234;
    MATRIX matrix_254;
    MATRIX matrix_274;
    MATRIX matrix_294;
    int turningWhereNextFrame;
    undefined field168_0x2b8;
    undefined field169_0x2b9;
    undefined field170_0x2ba;
    undefined field171_0x2bb;
    short field_2bc;
    short ballMorphShape;
    short flatteningTimer;
    short dying;
    short isRotatingViewport;
    short pad_2c6;
} Player;

typedef int RGB[3];
typedef union BlockColors {
    struct {
        RGB left;
        RGB right;
        RGB forward;
        RGB backward;
        RGB up;
        RGB down;
    } colors;
    int arr[3 * 6];
} BlockColors;

typedef struct TgiFile {
    RGB normalLevelBgColor;
    RGB normalLevelFarColor2Mul;
    RGB normalLevelModelDarkColor;
    RGB normalLevelModelMediumColor;
    RGB normalLevelModelLightColor;
    BlockColors normalBlockColors;
    RGB specialLevelBgColor;
    RGB specialLevelFarColor2Mul;
    RGB specialLevelModelDarkColor;
    RGB specialLevelModelMediumColor;
    RGB specialLevelModelLightColor;
    BlockColors specialBlockColors;
    int unk108;
    int skyboxFlag;
    int lodDistance[7];
    int unk12c[6];
    int randomTileRotation;
    int unk148;
    int unk14c;
    int unk150;
    int numPlainTileTextureVariationsNormalLevel;
    int unk158;
    int numPlainTileTextureVariationsSpecialLevel;
    int unk160;
    int part0Words;
    int part1Words;
    int part2Words;
    int part3Words;
    int part4Words;
    int part5Words;
    int part6Words;
    int part7Words;
    int part8Words;
    int part9Words;
    int part10Words;
} TgiFile;

#define INCLUDE_ASM(FOLDER, NAME) \
    void __maspsx_include_asm_hack_##NAME() { \
        __asm__( \
            ".text # maspsx-keep \n" \
            "\t.align\t2 # maspsx-keep\n" \
            "\t.set noreorder # maspsx-keep\n" \
            "\t.set noat # maspsx-keep\n" \
            ".include \""FOLDER"/"#NAME".s\" # maspsx-keep\n" \
            "\t.set reorder # maspsx-keep\n" \
            "\t.set at # maspsx-keep\n" \
        ); \
    }
#define INCLUDE_RODATA(FOLDER, NAME) \
__asm__( \
    ".section .rodata\n" \
    "\t.include \""FOLDER"/"#NAME".s\"\n" \
    ".section .text" \
)

#endif
