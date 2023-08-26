#ifndef CGAME_H
#define CGAME_H

#include <pd_api.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "CAudio.h"
#include "CInput.h"
#include "CFont.h"
#include "CImage.h"
#include "CSprites.h"
#include "Common.h"
#include "games/CGameBlockStacker.h"
#include "games/CGameFastEddy.h"
#include "games/CGameFrog.h"
#include "games/CGameBreakOut.h"
#include "games/CGameInvaders.h"
#include "games/CGamePang.h"
#include "games/CGameRamIt.h"
#include "games/CGameSnake.h"


extern uint8_t GameAlpha;
extern uint32_t AlphaTimer;
extern uint32_t TimerTicks;
extern int Score;
extern int NextSubStateCounter, NextSubState, NextSubStateTimeAdd;

extern int ActiveGameGameStateId;

extern CGameSnake *GameSnake;
extern CGameBlockStacker *GameBlockStacker;
extern CGameFastEddy *GameFastEddy;
extern CGameBreakOut *GameBreakOut;
extern CGameInvaders *GameInvaders;
extern CGameRamIt *GameRamIt;
extern CGamePang * GamePang;
extern CGameFrog *GameFrog;

extern LCDBitmap *TexOffScreen, *TexTmp;
extern int RetroCarouselHighScore, RetroCarouselScore;
extern int HighScores[Games][Modes];
extern int Scores[Games][Modes];
extern int GameState, SubGameState, GameMode, Game;
extern float SubStateCounter;
extern int MusMenu;
extern int SfxConfirm, SfxBack, SfxSelect, SfxScore, SfxOne, SfxTwo, SfxThree, SfxTimeOver, SfxReadyGo, SfxOneMinute;
extern float Timer;
extern uint32_t SubStateTime;
extern int GFXFrameID, GFXMedal;
extern int CurrentGameMusicID;


void CGame_UpdateTimer();
int CGame_MainLoop(void* ud);
void CGame_Init();
void CGame_DeInit();
void CGame_CreateActiveGame();


void CGame_LoadSound();
void CGame_UnLoadSound();
void CGame_LoadMusic();
void CGame_UnLoadMusic();
void CGame_UnLoadGraphics();
void CGame_LoadGraphics();
void CGame_LoadSettings();
void CGame_SaveSettings();
void CGame_LoadHighScores();
void CGame_SaveHighScores();
void CGame_ResetTimer();
void CGame_ResetScores();
void CGame_ResetHighScores();
void CGame_AddToScore(int Value);
void CGame_DrawTitleBackground();
void CGame_StartCrossFade(int SetGameState, int SetNextSubState, int SetNextSubStateCounter, uint32_t SetNextSubStateTimeAdd);

#endif