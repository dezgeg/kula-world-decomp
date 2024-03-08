#include "common.h"

INCLUDE_ASM("asm/nonmatchings/main", VariousInit);

INCLUDE_ASM("asm/nonmatchings/main", InitPolys);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0001271c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00012918);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000139c8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00013be0);

INCLUDE_ASM("asm/nonmatchings/main", SetFontEntry);

INCLUDE_ASM("asm/nonmatchings/main", InitFont);

INCLUDE_ASM("asm/nonmatchings/main", SetA3CD0ToMinus1);

INCLUDE_ASM("asm/nonmatchings/main", SetTextParams);

INCLUDE_ASM("asm/nonmatchings/main", DrawText);

INCLUDE_ASM("asm/nonmatchings/main", InitWidgetSprites);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000154f0);

INCLUDE_ASM("asm/nonmatchings/main", VariousInit1);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00015b54);

INCLUDE_ASM("asm/nonmatchings/main", VariousInit3);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000164f4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00016d7c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00016da0);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00016db0);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00016e04);

INCLUDE_ASM("asm/nonmatchings/main", LoadMenuGfx);

INCLUDE_ASM("asm/nonmatchings/main", DrawWidgets);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00018088);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00018234);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00019408);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00019f10);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0001a614);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0001a628);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0001a640);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0001b690);

INCLUDE_ASM("asm/nonmatchings/main", InitHighscoreCubes);

INCLUDE_ASM("asm/nonmatchings/main", DrawHighScoreCubes);

INCLUDE_ASM("asm/nonmatchings/main", InitGuiBackgroundSprites);

INCLUDE_ASM("asm/nonmatchings/main", LogoBackgroundStuff);

INCLUDE_ASM("asm/nonmatchings/main", GuiBackgroundFade);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0001dcdc);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0001df04);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0001e1fc);

INCLUDE_ASM("asm/nonmatchings/main", DrawMainGameUI);

INCLUDE_ASM("asm/nonmatchings/main", DrawScore);

INCLUDE_ASM("asm/nonmatchings/main", DrawTimeAttackWidgets);

INCLUDE_ASM("asm/nonmatchings/main", DrawHourglassAndTimer);

INCLUDE_ASM("asm/nonmatchings/main", DrawInt);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0001f928);

INCLUDE_ASM("asm/nonmatchings/main", DrawStaticHourglass);

INCLUDE_ASM("asm/nonmatchings/main", DrawKeyWidgets);

INCLUDE_ASM("asm/nonmatchings/main", DrawFruitWidgets);

INCLUDE_ASM("asm/nonmatchings/main", SetLethargy);

#if 1
void ResetLethargyEffect() {
    extern unsigned lethargyMode;
    extern unsigned gteXScale;
    extern unsigned gteYScale;

    lethargyMode = 0;
    gteXScale = 0x1000;
    gteYScale = 0x1000;
}
#else
INCLUDE_ASM("asm/nonmatchings/main", ResetLethargyEffect);
#endif

INCLUDE_ASM("asm/nonmatchings/main", DrawLethargyEffects);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00021038);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002108c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002122c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00021378);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002224c);

INCLUDE_ASM("asm/nonmatchings/main", DrawBonusWidget);

INCLUDE_ASM("asm/nonmatchings/main", DisableScreenShake);

INCLUDE_ASM("asm/nonmatchings/main", EnableScreenShake);

INCLUDE_ASM("asm/nonmatchings/main", ProcessScreenShake);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00023264);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00023464);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00023608);

INCLUDE_ASM("asm/nonmatchings/main", ParseGGI);

INCLUDE_ASM("asm/nonmatchings/main", ParseTextures);

INCLUDE_ASM("asm/nonmatchings/main", ParseGgiInner);

INCLUDE_ASM("asm/nonmatchings/main", InitManySprites);

INCLUDE_ASM("asm/nonmatchings/main", InitHourglassSprites);

INCLUDE_ASM("asm/nonmatchings/main", Init10DigitSprites);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002429c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002448c);

INCLUDE_ASM("asm/nonmatchings/main", InitLethargyEffectSprites);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00024cdc);

INCLUDE_ASM("asm/nonmatchings/main", InitPauseMenuSprite2);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00024fb8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00025160);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002530c);

INCLUDE_ASM("asm/nonmatchings/main", InitDigitSprites);

INCLUDE_ASM("asm/nonmatchings/main", TexturedSpritePrim);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00025c64);

INCLUDE_ASM("asm/nonmatchings/main", ParseTGI);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00025f24);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000261ac);

INCLUDE_ASM("asm/nonmatchings/main", LoadImagesFromTgiPart9);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000263f8);

INCLUDE_ASM("asm/nonmatchings/main", ParseLevelDataFromTgi);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00026894);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000271bc);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000276a8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000276b8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000277fc);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00027aa8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00028308);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00028390);

INCLUDE_ASM("asm/nonmatchings/main", ParseKeysAndSpecialLevelFromItemData);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000285a4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000289a0);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00029368);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000293e4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000295bc);

INCLUDE_ASM("asm/nonmatchings/main", CopyQuadData);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000297f0);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00029844);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000298e0);

INCLUDE_ASM("asm/nonmatchings/main", InitPlayerShadowSprites);

INCLUDE_ASM("asm/nonmatchings/main", InitPlayerSpecularSprite);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002a00c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002a0c4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002a234);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002a3b0);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002a518);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002a670);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002a7d8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002a940);

INCLUDE_ASM("asm/nonmatchings/main", RenderBackground);

INCLUDE_ASM("asm/nonmatchings/main", RenderEverythingElseAndProcessSomeStuff);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002afbc);

INCLUDE_ASM("asm/nonmatchings/main", GetShortFromGgiPart2);

INCLUDE_ASM("asm/nonmatchings/main", RenderPlayerAndItems);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002b3dc);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002b6fc);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002bb34);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002bcb4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002bfb4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002c45c);

INCLUDE_ASM("asm/nonmatchings/main", SetSunglassMode);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002c7a8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002c8ac);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002c8d8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002c9a8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002c9dc);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002caec);

INCLUDE_ASM("asm/nonmatchings/main", RenderEnemy);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002d2b4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002db04);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002e1ec);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002e230);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002e2fc);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002e3ac);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002e410);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002e478);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002e5f4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002e6f4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002e7cc);

INCLUDE_ASM("asm/nonmatchings/main", GetBlockTypeRelativeToDir);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002ea10);

INCLUDE_ASM("asm/nonmatchings/main", FlipDir);

INCLUDE_ASM("asm/nonmatchings/main", MaybeDeletePickup);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002f9b4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002fafc);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002fb14);

INCLUDE_ASM("asm/nonmatchings/main", ResetPlayerVars);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002fc08);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002fc3c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002fe28);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002fed0);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0002ff34);

INCLUDE_ASM("asm/nonmatchings/main", TurnRight);

INCLUDE_ASM("asm/nonmatchings/main", TurnLeft);

INCLUDE_ASM("asm/nonmatchings/main", PlayerDifferentMovings);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00030a60);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00030e44);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00031000);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00031288);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000313e8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00031670);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003169c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000316e4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003170c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000317e8);

INCLUDE_ASM("asm/nonmatchings/main", CalcPlayerMatrixes);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00032d70);

INCLUDE_ASM("asm/nonmatchings/main", RotateBallWhenJumpingForward);

INCLUDE_ASM("asm/nonmatchings/main", RotateBallWhenRolling);

INCLUDE_ASM("asm/nonmatchings/main", ResetPlayerMatrix5);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00033018);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00033348);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00033720);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003382c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000338d4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00033d4c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00033eb0);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003406c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003418c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000344b0);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00034518);

INCLUDE_ASM("asm/nonmatchings/main", CalcLevelBounds);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00034b78);

INCLUDE_ASM("asm/nonmatchings/main", HandleDebugCamera);

INCLUDE_ASM("asm/nonmatchings/main", HandlePauseModeRotationEffect);

INCLUDE_ASM("asm/nonmatchings/main", HandlePlayerMovementStuff);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00035dc8);

INCLUDE_ASM("asm/nonmatchings/main", HandleLevelParsingOrItemCreation);

INCLUDE_ASM("asm/nonmatchings/main", InitLasers);

INCLUDE_ASM("asm/nonmatchings/main", ResetCopycatMode);

INCLUDE_ASM("asm/nonmatchings/main", LevelInit);

INCLUDE_ASM("asm/nonmatchings/main", ProcessPlayer);

INCLUDE_ASM("asm/nonmatchings/main", ProcessEnemiesAndCheckFellOff);

INCLUDE_ASM("asm/nonmatchings/main", SetPausedOrWaitingForRestart);

INCLUDE_ASM("asm/nonmatchings/main", CalcEntityPositions_);

INCLUDE_ASM("asm/nonmatchings/main", HandlePlayerButtons);

INCLUDE_ASM("asm/nonmatchings/main", CalcWhatPlayerIsStandingOn);

INCLUDE_ASM("asm/nonmatchings/main", GetBlockAt);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003831c);

INCLUDE_ASM("asm/nonmatchings/main", MovePlayerForward);

INCLUDE_ASM("asm/nonmatchings/main", MovePlayerForward2);

INCLUDE_ASM("asm/nonmatchings/main", GetRotationIndexFromVector);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003888c);

INCLUDE_ASM("asm/nonmatchings/main", GetRotation);

INCLUDE_ASM("asm/nonmatchings/main", TweakPlayerMatrix6);

INCLUDE_ASM("asm/nonmatchings/main", HandleItemTouching);

INCLUDE_ASM("asm/nonmatchings/main", CalcEntityPositions);

INCLUDE_ASM("asm/nonmatchings/main", SetEntityRotation);

INCLUDE_ASM("asm/nonmatchings/main", MatrixFromDirectionIndex);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003ac0c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003ae10);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003b1ac);

INCLUDE_ASM("asm/nonmatchings/main", SubtractLevelTimer);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003bd90);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003bdc8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003bdec);

INCLUDE_ASM("asm/nonmatchings/main", InitEnemies);

INCLUDE_ASM("asm/nonmatchings/main", UpdateEnemies);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003d758);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003d8b8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003da18);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003da64);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003db64);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003dbe0);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003dce0);

INCLUDE_ASM("asm/nonmatchings/main", IsCollidingWithEnemy);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0003de64);

INCLUDE_ASM("asm/nonmatchings/main", ProcessEnemies);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00040178);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000403ec);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00040490);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00040544);

INCLUDE_ASM("asm/nonmatchings/main", MainGameVSyncCallback);

INCLUDE_ASM("asm/nonmatchings/main", LoadingScreenVSyncCallback);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000406e8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00040768);

INCLUDE_ASM("asm/nonmatchings/main", main);

INCLUDE_ASM("asm/nonmatchings/main", OnVsync2);

INCLUDE_ASM("asm/nonmatchings/main", MainGameLoop);

INCLUDE_ASM("asm/nonmatchings/main", LevelCompletedOrDied);

INCLUDE_ASM("asm/nonmatchings/main", DecideNextLevel);

INCLUDE_ASM("asm/nonmatchings/main", ReceiveBufFromSio);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000431a4);

INCLUDE_ASM("asm/nonmatchings/main", UnusedDisplayModeChange);

INCLUDE_ASM("asm/nonmatchings/main", ClearMemcardData);

INCLUDE_ASM("asm/nonmatchings/main", GetMaxSaveSlotScore);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00043348);

INCLUDE_ASM("asm/nonmatchings/main", MemCardUi);

INCLUDE_ASM("asm/nonmatchings/main", MemCardUiPart);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00045e54);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00046160);

INCLUDE_ASM("asm/nonmatchings/main", LoadHighscoresFromMemcardData);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00046678);

INCLUDE_ASM("asm/nonmatchings/main", LoadSaveSlot);

INCLUDE_ASM("asm/nonmatchings/main", SaveMemCard);

INCLUDE_ASM("asm/nonmatchings/main", LoadSaveFromMemoryCard);

INCLUDE_ASM("asm/nonmatchings/main", Ascii2SJIS);

INCLUDE_ASM("asm/nonmatchings/main", Sjis2Ascii);

INCLUDE_ASM("asm/nonmatchings/main", LooksLikeAscii);

INCLUDE_ASM("asm/nonmatchings/main", DebugSaveScreenshotToPc);

INCLUDE_ASM("asm/nonmatchings/main", SetDebugScreenshotFilename);

INCLUDE_ASM("asm/nonmatchings/main", ResetDevkitFileNumber);

INCLUDE_ASM("asm/nonmatchings/main", InitReplaySaving);

INCLUDE_ASM("asm/nonmatchings/main", RecordButtonsToDevkit);

INCLUDE_ASM("asm/nonmatchings/main", WriteToDevkit);

INCLUDE_ASM("asm/nonmatchings/main", InitReplayPlayback);

INCLUDE_ASM("asm/nonmatchings/main", GetButtonsFromReplay);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00047a68);

INCLUDE_ASM("asm/nonmatchings/main", Srand);

INCLUDE_ASM("asm/nonmatchings/main", Rand);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00047bb4);

INCLUDE_ASM("asm/nonmatchings/main", HandleCheats);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00047fa0);

INCLUDE_ASM("asm/nonmatchings/main", PauseMenu);

INCLUDE_ASM("asm/nonmatchings/main", OptionsMenu);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000489bc);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00048a1c);

INCLUDE_ASM("asm/nonmatchings/main", ScreenAdjustMenu);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00048e70);

INCLUDE_ASM("asm/nonmatchings/main", MainMenu);

INCLUDE_ASM("asm/nonmatchings/main", SinglePlayerMenu);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000496ac);

INCLUDE_ASM("asm/nonmatchings/main", TwoPlayerMenu);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00049ed8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004a228);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004a65c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004a72c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004a7e8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004a800);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004ac84);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004aef8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004b0c0);

INCLUDE_ASM("asm/nonmatchings/main", InitHighscores);

INCLUDE_ASM("asm/nonmatchings/main", HighScoreUi);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004d460);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004d500);

INCLUDE_ASM("asm/nonmatchings/main", ReadDataFile);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004d8bc);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004d974);

INCLUDE_ASM("asm/nonmatchings/main", LoadWarningTim);

INCLUDE_ASM("asm/nonmatchings/main", LoadingScreen);

INCLUDE_ASM("asm/nonmatchings/main", PlayMusic);

INCLUDE_ASM("asm/nonmatchings/main", PlayBonusMusic);

INCLUDE_ASM("asm/nonmatchings/main", PauseMusic);

INCLUDE_ASM("asm/nonmatchings/main", SwitchFromBonusToNormalMusic);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004e70c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004e75c);

INCLUDE_ASM("asm/nonmatchings/main", InitSoundsFromSfxFile);

INCLUDE_ASM("asm/nonmatchings/main", PlaySfx);

INCLUDE_ASM("asm/nonmatchings/main", ProcessSpuVoices);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004f068);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004f250);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004f310);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004f440);

INCLUDE_ASM("asm/nonmatchings/main", SoundMuteStuff);

INCLUDE_ASM("asm/nonmatchings/main", MuteAllSpuChannels);

INCLUDE_ASM("asm/nonmatchings/main", SetSpuMusicVolume);

INCLUDE_ASM("asm/nonmatchings/main", InitStuff);

INCLUDE_ASM("asm/nonmatchings/main", PutDrawAndDispEnv);

INCLUDE_ASM("asm/nonmatchings/main", SetupDisplay);

INCLUDE_ASM("asm/nonmatchings/main", GetControllerButtons);

INCLUDE_ASM("asm/nonmatchings/main", GetControllerStatus);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004fdf8);

INCLUDE_ASM("asm/nonmatchings/main", Vibrate1);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004fe30);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004fe84);

INCLUDE_ASM("asm/nonmatchings/main", Vibrate2);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0004ff04);

INCLUDE_ASM("asm/nonmatchings/main", Vibrate3);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00050124);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00050168);

INCLUDE_ASM("asm/nonmatchings/main", Noop);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00050308);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00050310);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00050468);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000504d8);

INCLUDE_ASM("asm/nonmatchings/main", SomethingRelatedToLevelGeometryTextures);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00050790);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00051964);

INCLUDE_ASM("asm/nonmatchings/main", RandomEffect);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00051dd0);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00051f24);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00051f70);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00052004);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000520c8);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00052504);

INCLUDE_ASM("asm/nonmatchings/main", WaitForVBlank);

INCLUDE_ASM("asm/nonmatchings/main", BiosVersionCheck);

INCLUDE_ASM("asm/nonmatchings/main", LoadScaledGteMatrix);

INCLUDE_ASM("asm/nonmatchings/main", GteTransformAndClipQuad);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00052cf4);

INCLUDE_ASM("asm/nonmatchings/main", divZeroCounter);

INCLUDE_ASM("asm/nonmatchings/main", ShowEndingFMV);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00053420);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000534c8);

INCLUDE_ASM("asm/nonmatchings/main", DctCallback);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000536fc);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000537a0);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0005392c);

INCLUDE_ASM("asm/nonmatchings/main", SeekAndStreamFmvFromCd);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00053a64);

INCLUDE_ASM("asm/nonmatchings/main", RenderPlayerOrObjects);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00055a0c);

INCLUDE_ASM("asm/nonmatchings/main", RenderNonSpecialBackground);

INCLUDE_ASM("asm/nonmatchings/main", SkyboxStuff);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00056f48);

INCLUDE_ASM("asm/nonmatchings/main", RenderLevelGeometryQuads);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00057834);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00057a18);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00057afc);

INCLUDE_ASM("asm/nonmatchings/main", inflate_blocks);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000588c0);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000589d4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00058a20);

INCLUDE_ASM("asm/nonmatchings/main", inflateEnd);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00058b18);

INCLUDE_ASM("asm/nonmatchings/main", inflateInit);

INCLUDE_ASM("asm/nonmatchings/main", inflate);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0005933c);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00059418);

INCLUDE_ASM("asm/nonmatchings/main", FUN_000595b0);

INCLUDE_ASM("asm/nonmatchings/main", inflate_trees_bits);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00059c34);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00059db4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_00059fa4);

INCLUDE_ASM("asm/nonmatchings/main", func_0005A028);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0005a054);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0005a0d0);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0005a8e4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0005a910);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0005aab4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0005ae94);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0005aea4);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0005af34);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0005af48);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0005af70);

INCLUDE_ASM("asm/nonmatchings/main", FUN_0005b05c);

INCLUDE_ASM("asm/nonmatchings/main", func_0005B148);
