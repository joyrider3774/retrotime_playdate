#include <pd_api.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>
#include <time.h>
#include <inttypes.h>
#include "games/CGameBase.h"
#include "games/CGameFrog.h"
#include "games/CGameFastEddy.h"
#include "games/CGameRamIt.h"
#include "games/CGameSnake.h"
#include "games/CGameBlockStacker.h"
#include "games/CGameBreakOut.h"
#include "games/CGamePang.h"
#include "games/CGameInvaders.h"

#include "CFont.h"
#include "CGame.h"
#include "Common.h"
#include "TitleScreen.h"
#include "SubScoreScreen.h"
#include "Intro.h"

#include "pd_helperfuncs.h"

#undef LoadImage
#undef PlaySound

uint8_t GameAlpha = 0;
char DataPath[500];

uint32_t AlphaTimer;
uint32_t TimerTicks;
int Score;
int NextSubStateCounter, NextSubState, NextSubStateTimeAdd;

int ActiveGameGameStateId;
CGameSnake *GameSnake;
CGameBlockStacker *GameBlockStacker;
CGameFastEddy *GameFastEddy;
CGameBreakOut *GameBreakOut;
CGameInvaders *GameInvaders;
CGameRamIt *GameRamIt;
CGamePang * GamePang;
CGameFrog *GameFrog;
LCDBitmap *TexOffScreen, *TexScreen, *TexTmp;
int RetroCarouselHighScore, RetroCarouselScore;
int HighScores[Games][Modes];
int Scores[Games][Modes];
int GameState, SubGameState, GameMode, Game;
float SubStateCounter;
int MusMenu;
int SfxConfirm, SfxBack, SfxSelect, SfxScore, SfxOne, SfxTwo, SfxThree, SfxTimeOver, SfxReadyGo, SfxOneMinute;
float Timer;
uint32_t SubStateTime;
int GFXFrameID, GFXMedal;
int CurrentGameMusicID;

void CGame_DeInit()
{

	switch(ActiveGameGameStateId)
	{
		case GSSnake:
			GameSnake->deinit(GameSnake);
			Destroy_CGameSnake(GameSnake);
			break;
		case GSRamIt:
			GameRamIt->deinit(GameRamIt);
			Destroy_CGameRamIt(GameRamIt);
			break;
		case GSPang:
			GamePang->deinit(GamePang);
			Destroy_CGamePang(GamePang);
			break;
		case GSSpaceInvaders:
			GameInvaders->deinit(GameInvaders);
			Destroy_CGameInvaders(GameInvaders);
			break;
		case GSFrog:
			GameFrog->deinit(GameFrog);
			Destroy_CGameFrog(GameFrog);
			break;
		case GSEddy:
			GameFastEddy->deinit(GameFastEddy);
			Destroy_CGameFastEddy(GameFastEddy);
			break;
		case GSBreakout:
			GameBreakOut->deinit(GameBreakOut);
			Destroy_CGameBreakOut(GameBreakOut);
			break;
		case GSTetris:
			GameBlockStacker->deinit(GameBlockStacker);
			Destroy_CGameBlockStacker(GameBlockStacker);
			break;
		default:
			break;
	}

	pd->graphics->freeBitmap(TexScreen);
	pd->graphics->freeBitmap(TexTmp);
	pd->graphics->freeBitmap(TexOffScreen);

	CGame_UnLoadMusic();
	CGame_UnLoadGraphics();
	CGame_UnLoadSound();
	CGame_SaveSettings();
	CGame_SaveHighScores();

	CAudio_DeInit();
	CFont_DeInit();
	CImage_DeInit();
	CInput_DeInit();
	CSprites_DeInit();
}

void CGame_Init()
{
	srand(pd->system->getCurrentTimeMilliseconds());

	CAudio_Init(debufInfoAudio);
	CFont_Init(debufInfoFonts);
	CImage_Init(debufInfoImages);
	CInput_Init();
	CSprites_Init();
	CSprites_SetForceShowCollisionShape(debugShowCollisionShapes);

	// Main game loop that loops untile the gamestate = GSQuit
	// and calls the procedure according to the gamestate.
	CGame_LoadSettings();
	CGame_LoadHighScores();
	CGame_LoadGraphics();
	CGame_LoadMusic();
	CGame_LoadSound();

	pd->graphics->setBackgroundColor(kColorBlack);

	TexTmp = pd->graphics->newBitmap(ScreenWidth, ScreenHeight, kColorClear);
	TexOffScreen = pd->graphics->newBitmap(ScreenWidth, ScreenHeight, kColorClear);

	//Main State Variables and such
	CurrentGameMusicID = -1;
	GameState = GSIntroInit;
	Game = 0;
	GameMode = GMGame;
	GameAlpha = MaxAlpha;
	SubStateTime = 0;
	Timer = 0.0f;
	SubStateCounter = 0;
	TimerTicks = 0;
	SubGameState = SGNone;
	NextSubState = 0;
	NextSubStateCounter = 0;
	NextSubStateTimeAdd = 0;
	ActiveGameGameStateId = -1;

	//Clear score values
	CGame_ResetScores();
}

void CGame_ResetHighScores()
{
	for(int x = 0; x < Games; x++)
		for(int y = 0; y < Modes; y++)
			HighScores[x][y] = 0;
	RetroCarouselHighScore = 0;
}

void CGame_ResetScores()
{
	for(int x = 0; x < Games; x++)
		for(int y = 0; y < Modes; y++)
			Scores[x][y] = 0;
	RetroCarouselScore = 0;
}
void CGame_LoadMusic()
{
	MusMenu = CAudio_LoadMusic("main/music");
}

void CGame_UnLoadMusic()
{
	CAudio_UnloadMusics();
}

void CGame_UnLoadGraphics()
{
	CImage_UnloadImages();
}

void CGame_DrawTitleBackground()
{
	pd->graphics->clear(kColorBlack);
}


void CGame_AddToScore(int Value)
{
	int AScore = Value;
	if (AScore < 0)
		if (GameMode == GMGame)
			AScore = 0;

	Scores[Game][GameMode] += AScore;
	if (Scores[Game][GameMode] < 0)
		Scores[Game][GameMode] = 0;
}

void CGame_LoadSound()
{
	SfxTimeOver = CAudio_LoadSound("common/timeover");
	SfxReadyGo = CAudio_LoadSound("common/readygo");
	SfxOne = CAudio_LoadSound("common/one");
	SfxTwo = CAudio_LoadSound("common/two");
	SfxThree = CAudio_LoadSound("common/three");
	SfxOneMinute = CAudio_LoadSound("common/oneminute");
	SfxConfirm = CAudio_LoadSound("main/confirm");
	SfxBack = CAudio_LoadSound("main/back");
	SfxSelect = CAudio_LoadSound("main/select");
	SfxScore = CAudio_LoadSound("main/score");
}

void CGame_UnLoadSound()
{
	CAudio_UnloadSounds();
}

void CGame_LoadGraphics()
{
	GFXFrameID = CImage_LoadImage("main/frame.png");
	GFXMedal = CImage_LoadImage("main/medal.png");

	//SDL_SaveBMPTextureScaled("./retrotimefs/graphics/main/frame.bmp", CImage_GetImage(GFXFrameID), 1,1, true, 0, 40);
	//SDL_SaveBMPTextureScaled("./retrotimefs/graphics/main/medal.bmp", CImage_GetImage(GFXMedal), 1,1, true, 0, 160);
	CImage_UnLoadImage(GFXFrameID);
	CImage_UnLoadImage(GFXMedal);

	GFXFrameID = CImage_LoadImage("main/frame.bmp");
	GFXMedal = CImage_LoadImage("main/medal.bmp");
}


void CGame_LoadHighScores()
{
	SDFile *ScoreFile;

	ScoreFile = pd->file->open("retrotimesscores", kFileReadData);
	if (ScoreFile)
	{
		pd->file->read(ScoreFile, &RetroCarouselHighScore, sizeof(int));
		for (int i = 0; i < Games; i++)
			for (int j = 0; j < Modes; j++)
			{
				pd->file->read(ScoreFile, &HighScores[i][j], sizeof(int));
			}
		pd->file->close(ScoreFile);
	}
	else
	{
		CGame_ResetHighScores();
	}
}

void CGame_SaveHighScores()
{
	SDFile *ScoreFile;

	ScoreFile = pd->file->open("retrotimesscores", kFileWrite);
	if (ScoreFile)
	{
		pd->file->write(ScoreFile, &RetroCarouselHighScore, sizeof(int));
		for (int i = 0; i < Games; i++)
			for (int j = 0; j < Modes; j++)
				pd->file->write(ScoreFile, &HighScores[i][j], sizeof(int));
		pd->file->close(ScoreFile);
	}
}

void CGame_LoadSettings()
{
	SDFile *SettingsFile;

	SettingsFile = pd->file->open("retrotimesettings", kFileReadData);
	if (SettingsFile)
	{
		int VolumeMusic, VolumeSound;
		if (pd->file->read(SettingsFile, &VolumeMusic, sizeof(int)) > 0)
		{
			CAudio_SetVolumeMusic(VolumeMusic);
		}
		else
			CAudio_SetVolumeMusic(128);
		
		if (pd->file->read(SettingsFile, &VolumeSound, sizeof(int)) > 0)
		{
			CAudio_SetVolumeSound(VolumeSound);
		}
		else
			CAudio_SetVolumeSound(128);
		pd->file->close(SettingsFile);
	}
	else
	{
		CAudio_SetVolumeMusic(128);
		CAudio_SetVolumeSound(128);
	}
}

void CGame_SaveSettings()
{
	SDFile *SettingsFile;

	SettingsFile = pd->file->open("retrotimesettings", kFileWrite);
	if (SettingsFile)
	{
		int VolumeMusic = CAudio_GetVolumeMusic();
		int VolumeSound = CAudio_GetVolumeSound();
		pd->file->write(SettingsFile, &VolumeMusic, sizeof(int));
		pd->file->write(SettingsFile, &VolumeSound, sizeof(int));
		pd->file->close(SettingsFile);
	}
}

void CGame_StartCrossFade(int SetGameState, int SetNextSubState, int SetNextSubStateCounter, uint32_t SetNextSubStateTimeAdd)
{
	AlphaTimer = pd->system->getCurrentTimeMilliseconds();
	GameAlpha = 0;
	SubGameState = SGFadeIn;
	GameState = SetGameState;
	NextSubState = SetNextSubState;
	NextSubStateTimeAdd = SetNextSubStateTimeAdd;
	NextSubStateCounter = SetNextSubStateCounter;
}

void CGame_ResetTimer()
{
	Timer = 120;
	if (GameMode == GMRetroCarousel)
	{
		Timer = 2 * 60;
	}
	if (GameMode == GMTimeAttack)
	{
		Timer = 5 * 60;
	}
	TimerTicks = pd->system->getCurrentTimeMilliseconds();
}

void CGame_UpdateTimer()
{
	if (TimerTicks + 250 < pd->system->getCurrentTimeMilliseconds())
	{
		TimerTicks = pd->system->getCurrentTimeMilliseconds();
		if (SubGameState == SGGame)
		{
			if (GameMode == GMGame)
				Timer += 0.25f;
			else
			{
				Timer -= 0.25f;

				if (Timer == 60)
					CAudio_PlaySound(SfxOneMinute, 0);

				if (Timer == 3)
					CAudio_PlaySound(SfxThree, 0);

				if (Timer == 2)
					CAudio_PlaySound(SfxTwo, 0);

				if (Timer == 1)
					CAudio_PlaySound(SfxOne, 0);


				if (Timer <= 0)
				{
					CAudio_PlaySound(SfxTimeOver, 0);
					SubGameState = SGTimeUp;
					SubStateTime = pd->system->getCurrentTimeMilliseconds() + 750;
					SubStateCounter = 0;
				}
			}
		}
	}
}

void CGame_CreateActiveGame()
{
	switch(ActiveGameGameStateId)
	{
		case GSSnake:
			GameSnake->deinit(GameSnake);
			Destroy_CGameSnake(GameSnake);
			ActiveGameGameStateId = -1;
			break;
		case GSRamIt:
			GameRamIt->deinit(GameRamIt);
			Destroy_CGameRamIt(GameRamIt);
			ActiveGameGameStateId = -1;
			break;
		case GSPang:
			GamePang->deinit(GamePang);
			Destroy_CGamePang(GamePang);
			ActiveGameGameStateId = -1;
			break;
		case GSSpaceInvaders:
			GameInvaders->deinit(GameInvaders);
			Destroy_CGameInvaders(GameInvaders);
			ActiveGameGameStateId = -1;
			break;
		case GSFrog:
			GameFrog->deinit(GameFrog);
			Destroy_CGameFrog(GameFrog);
			ActiveGameGameStateId = -1;
			break;
		case GSEddy:
			GameFastEddy->deinit(GameFastEddy);
			Destroy_CGameFastEddy(GameFastEddy);
			ActiveGameGameStateId = -1;
			break;
		case GSBreakout:
			GameBreakOut->deinit(GameBreakOut);
			Destroy_CGameBreakOut(GameBreakOut);
			ActiveGameGameStateId = -1;
			break;
		case GSTetris:
			GameBlockStacker->deinit(GameBlockStacker);
			Destroy_CGameBlockStacker(GameBlockStacker);
			ActiveGameGameStateId = -1;
			break;
		default:			
			ActiveGameGameStateId = -1;
			break;
	}

	switch (GameState)
	{
		case GSSnakeInit:
			GameSnake = Create_CGameSnake();
			ActiveGameGameStateId = GSSnake;
			break;
		case GSTetrisInit:
			GameBlockStacker = Create_CGameBlockStacker();
			ActiveGameGameStateId = GSTetris;
			break;
		case GSRamItInit:
			GameRamIt = Create_CGameRamIt();
			ActiveGameGameStateId = GSRamIt;
			break;
		case GSEddyInit:
			GameFastEddy = Create_CGameFastEddy();
			ActiveGameGameStateId = GSEddy;
			break;
		case GSFrogInit:
			GameFrog = Create_CGameFrog();
			ActiveGameGameStateId = GSFrog;
			break;
		case GSBreakoutInit:
			GameBreakOut = Create_CGameBreakOut();
			ActiveGameGameStateId = GSBreakout;
			break;
		case GSPangInit:
			GamePang = Create_CGamePang();
			ActiveGameGameStateId = GSPang;
			break;
		case GSSpaceInvadersInit:
			GameInvaders = Create_CGameInvaders();
			ActiveGameGameStateId = GSSpaceInvaders;
			break;
		default:
			ActiveGameGameStateId = -1;
			break;
	}
}

int CGame_MainLoop(void* ud)
{
	CSprites_SpritesDrawnReset();
	CGame_UpdateTimer();

	CInput_Update();
	
	switch (GameState)
	{
		case GSIntroInit:
		case GSIntro:
			Intro();
			break;

		case GSSubScoreInit:
		case GSSubScore:
			SubScoreScreen();
			break;

		case GSTitleScreenInit:
		case GSTitleScreen:
			//to clear the game data & set NULL to ActiveGame
			CGame_CreateActiveGame();
			TitleScreen();
			break;

		case GSSnakeInit:
		case GSRamItInit:
		case GSPangInit:
		case GSSpaceInvadersInit:
		case GSFrogInit:
		case GSEddyInit:
		case GSBreakoutInit:
		case GSTetrisInit:
			CGame_CreateActiveGame();
			switch (ActiveGameGameStateId)
			{
				case GSSnake:
					GameSnake->init(GameSnake);
					break;
				case GSRamIt:
					GameRamIt->init(GameRamIt);
					break;
				case GSPang:
					GamePang->init(GamePang);
					break;
				case GSSpaceInvaders:
					GameInvaders->init(GameInvaders);
					break;
				case GSFrog:
					GameFrog->init(GameFrog);
					break;
				case GSEddy:
					GameFastEddy->init(GameFastEddy);
					break;
				case GSBreakout:
					GameBreakOut->init(GameBreakOut);
					break;
				case GSTetris:
					GameBlockStacker->init(GameBlockStacker);
					break;
			}
			CGame_ResetTimer();
			CGame_StartCrossFade(ActiveGameGameStateId, SGReadyGo, 3, 500);
			break;			
	
		case GSSnake:
		case GSRamIt:
		case GSPang:
		case GSSpaceInvaders:
		case GSFrog:
		case GSEddy:
		case GSBreakout:
		case GSTetris:
			switch (ActiveGameGameStateId)
			{
				case GSSnake:
					GameSnake->UpdateLogic(GameSnake);
					GameSnake->Draw(GameSnake);
					break;
				case GSRamIt:
					GameRamIt->UpdateLogic(GameRamIt);
					GameRamIt->Draw(GameRamIt);
					break;
				case GSPang:
					GamePang->UpdateLogic(GamePang);
					GamePang->Draw(GamePang);
					break;
				case GSSpaceInvaders:
					GameInvaders->UpdateLogic(GameInvaders);
					GameInvaders->Draw(GameInvaders);
					break;
				case GSFrog:
					GameFrog->UpdateLogic(GameFrog);
					GameFrog->Draw(GameFrog);
					break;
				case GSEddy:
					GameFastEddy->UpdateLogic(GameFastEddy);
					GameFastEddy->Draw(GameFastEddy);
					break;
				case GSBreakout:
					GameBreakOut->UpdateLogic(GameBreakOut);
					GameBreakOut->Draw(GameBreakOut);
					break;
				case GSTetris:
					GameBlockStacker->UpdateLogic(GameBlockStacker);
					GameBlockStacker->Draw(GameBlockStacker);
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	
	if (GameAlpha < MaxAlpha)
	{
		GameAlpha = MaxAlpha;
		if (GameAlpha >= MaxAlpha)
		{
			GameAlpha = MaxAlpha;
			SubGameState = NextSubState;
			SubStateTime = pd->system->getCurrentTimeMilliseconds() + NextSubStateTimeAdd;
			SubStateCounter = (float)NextSubStateCounter;
		}
		else
		{
			//SDL_SetTextureBlendMode(TexOffScreen, SDL_BLENDMODE_BLEND);
			//SDL_SetTextureAlphaMod(TexOffScreen, GameAlpha);
		}
	}

	if (debugInfoStats || ShowFPS)
	{
		Frames++;
		if (pd->system->getCurrentTimeMilliseconds() - FrameTime >= 1000)
		{
			CurrentMs = (float)(1000.0f / Frames);
			Frames = 0;
			FrameTime += 1000;
		}

		char Text[500];
		char* TmpText;
		Text[0] = '\0';
		pd->system->formatString(&TmpText, "FPS: %.0f\n", 1000.0f / CurrentMs);
		strcat(Text, TmpText);
		pd->system->realloc(TmpText, 0);
		if (ShowFPS && !debugInfoStats)
		{
			pd->system->drawFPS(0, 0);
			/*SDL_Point tz = CFont_TextSize("Roboto-Regular", 11, Text, strlen(Text), 0);
			pd->graphics->fillRect(ScreenWidth - tz.x, 0, tz.x, tz.y, kColorWhite);
			CFont_WriteText("Roboto-Regular", 11, Text, strlen(Text), ScreenWidth - tz.x, 0, 0, (LCDColor)kColorBlack);*/
		}
		else if(debugInfoStats)
		{
			pd->system->formatString(&TmpText, "FrameTime: %.5f\n", CurrentMs);
			strncat(Text, TmpText, 100);
			pd->system->realloc(TmpText, 0);
			pd->system->formatString(&TmpText, "GFX Slots: %d/%d\n", CImage_ImageSlotsUsed(), CImage_ImageSlotsMax());
			strncat(Text, TmpText, 100);
			pd->system->realloc(TmpText, 0);
			pd->system->formatString(&TmpText, "SND Slots: %d/%d\n", CAudio_SoundSlotsUsed(), CAudio_SoundSlotsMax());
			strncat(Text, TmpText, 100);
			pd->system->realloc(TmpText, 0);
			pd->system->formatString(&TmpText, "MUS Slots: %d/%d\n", CAudio_MusicSlotsUsed(), CAudio_MusicSlotsMax());
			strncat(Text, TmpText, 100);
			pd->system->realloc(TmpText, 0);
			pd->system->formatString(&TmpText, "SPR Slots: %d/%d\n", CSprites_SpriteSlotsUsed(), CSprites_SpriteSlotsMax());
			strncat(Text, TmpText, 100);
			pd->system->realloc(TmpText, 0);
			pd->system->formatString(&TmpText, "SPR Resets: %d\n", CSprites_UpdateImageResetsCount());
			strncat(Text, TmpText, 100);
			pd->system->realloc(TmpText, 0);
			pd->system->formatString(&TmpText, "SPR Draws: %d\n", CSprites_SpritesDrawnCount());
			strncat(Text, TmpText, 100);
			pd->system->realloc(TmpText, 0);
			pd->system->formatString(&TmpText, "SCL Loaded: %d/%d\n", CImage_ScaledImagesLoadedCount(), CImage_ScaledImagesLoadedMax());
			strncat(Text, TmpText, 100);
			pd->system->realloc(TmpText, 0);
			SDL_Point tz = CFont_TextSize("Roboto-Regular", 11, Text, strlen(Text), 0);
			pd->graphics->fillRect(ScreenWidth - tz.x, 0, tz.x, tz.y, kColorWhite);
			CFont_WriteText("Roboto-Regular", 11, Text, strlen(Text), ScreenWidth - tz.x, 0, 0, (LCDColor)kColorBlack);
		}
		
	}
	if (BatteryMonitoring)
	{
		pd->system->setAutoLockDisabled(true);
		prevLogTime = logPower("power.csv", 60u, prevLogTime);
	}
	return 1;
}