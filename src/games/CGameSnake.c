#include <pd_api.h>
#include <stdbool.h>
#include "CGameSnake.h"
#include "../CGame.h"
#include "../SDL_HelperTypes.h"
#include "../pd_helperfuncs.h"
#include "../CInput.h"

CGameSnake* Create_CGameSnake()
{
	CGameSnake* GameSnake = (CGameSnake*) malloc(sizeof(CGameSnake));
	GameSnake->GameBase = Create_CGameBase(GSSnake, true);
	GameSnake->MusMusic = -1;
	GameSnake->SfxFood = -1;
	GameSnake->SfxDie = -1;
	GameSnake->GameBase->playfieldwidth = (CGameSnake_cols) * CGameSnake_snakesize;
	GameSnake->GameBase->playfieldheight = (CGameSnake_rows) * CGameSnake_snakesize;
	GameSnake->GameBase->screenleft = (ScreenWidth - GameSnake->GameBase->playfieldwidth) / 2;
	GameSnake->GameBase->screenright = GameSnake->GameBase->screenleft + GameSnake->GameBase->playfieldwidth;
	GameSnake->GameBase->screentop = (ScreenHeight - GameSnake->GameBase->playfieldheight) / 2;
	GameSnake->GameBase->screenbottom = GameSnake->GameBase->screentop + GameSnake->GameBase->playfieldheight;
	GameSnake->snakeheadcolor = (LCDColor) kColorGrey;

	GameSnake->snakebodycolor = kColorWhite;

	GameSnake->snakefoodcolor = kColorWhite;
	
	GameSnake->createfood = CGameSnake_createfood;
	GameSnake->drawfood = CGameSnake_drawfood;
	GameSnake->updatefood = CGameSnake_updatefood;

	GameSnake->createsnake = CGameSnake_createsnake;
	GameSnake->drawsnake = CGameSnake_drawsnake;
	GameSnake->updatesnake = CGameSnake_updatesnake;

	GameSnake->init = CGameSnake_init;
	GameSnake->deinit = CGameSnake_deinit;
	GameSnake->LoadSound = CGameSnake_LoadSound;
	GameSnake->UnLoadSound = CGameSnake_UnLoadSound;
	GameSnake->UpdateObjects = CGameSnake_UpdateObjects;
	GameSnake->DrawObjects = CGameSnake_DrawObjects;
	GameSnake->DrawBackground = CGameSnake_DrawBackground;
	GameSnake->Draw = CGameSnake_Draw;
	GameSnake->UpdateLogic = CGameSnake_UpdateLogic;

	return GameSnake;
}

void Destroy_CGameSnake(CGameSnake* GameSnake)
{
	Destroy_CGameBase(GameSnake->GameBase);
	free(GameSnake);
}

//GameSnake->food ----------------------------------------------------------------------------------------------------------------

void CGameSnake_createfood(CGameSnake* GameSnake)
{
	bool bok = false;
	while (!bok)
	{
		GameSnake->food.x = GameSnake->GameBase->screenleft + (int)((rand() % CGameSnake_cols) *CGameSnake_snakesize);
		GameSnake->food.y = GameSnake->GameBase->screentop + (int)((rand() % CGameSnake_rows) * CGameSnake_snakesize);
		bok = ((GameSnake->food.x != GameSnake->head.x) && (GameSnake->food.y != GameSnake->head.y));
		for (int i = 0; i < GameSnake->snakelength; i++)
			bok = bok && ((GameSnake->food.x != GameSnake->body[i].x) && (GameSnake->food.y != GameSnake->body[i].y));
	}
}

void CGameSnake_drawfood(CGameSnake* GameSnake)
{
	SDL_Rect r = {GameSnake->food.x, GameSnake->food.y, CGameSnake_snakesize, CGameSnake_snakesize};
	pd->graphics->fillRect(r.x, r.y, r.w, r.h, GameSnake->snakefoodcolor);
}

void CGameSnake_updatefood(CGameSnake* GameSnake)
{
	if ((GameSnake->head.x == GameSnake->food.x) && (GameSnake->head.y == GameSnake->food.y))
	{
		CAudio_PlaySound(GameSnake->SfxFood, 0);
		GameSnake->snakelength += 1;
		GameSnake->createfood(GameSnake);
		CGame_AddToScore(GameSnake->snakelength*2);
	}
}

//snake ----------------------------------------------------------------------------------------------------------------

void CGameSnake_createsnake(CGameSnake* GameSnake)
{
	GameSnake->playerdeath = false;
	GameSnake->snakelength = 0;
	GameSnake->head.x = GameSnake->GameBase->screenleft + ((int)(floor(CGameSnake_cols / 2))) * CGameSnake_snakesize;
	GameSnake->head.y = GameSnake->GameBase->screentop + ((int)(floor(CGameSnake_rows / 2))) * CGameSnake_snakesize;
	GameSnake->dir.x = 1;
	GameSnake->dir.y = 0;
	GameSnake->ticks = 0;
}

void CGameSnake_drawsnake(CGameSnake* GameSnake)
{
	SDL_Rect r;
	for (int i = 0; i < GameSnake->snakelength; i++)
	{
		r.x = GameSnake->body[i].x;
		r.y = GameSnake->body[i].y;
		r.w = CGameSnake_snakesize;
		r.h = CGameSnake_snakesize;
		pd->graphics->fillRect(r.x, r.y, r.w, r.h, GameSnake->snakebodycolor);
	}
	r.x = GameSnake->head.x;
	r.y = GameSnake->head.y;
	r.w = CGameSnake_snakesize;
	r.h = CGameSnake_snakesize;
	pd->graphics->fillRect(r.x, r.y, r.w, r.h, GameSnake->snakeheadcolor);
}

void CGameSnake_updatesnake(CGameSnake* GameSnake)
{
	if ((CInput_Buttons.ButLeft) ||
		(CInput_Buttons.ButLeft2) ||
		(CInput_Buttons.ButDpadLeft))
	{
		if(GameSnake->movedone && GameSnake->dir.x == 0)
		{
			GameSnake->movedone = false;
			GameSnake->dir.x = -1;
			GameSnake->dir.y = 0;
		}
	}
	else
	{
		if ((CInput_Buttons.ButRight) ||
			(CInput_Buttons.ButRight2) ||
			(CInput_Buttons.ButDpadRight))
		{
			if(GameSnake->movedone && GameSnake->dir.x == 0)
			{
				GameSnake->movedone = false;
				GameSnake->dir.x = 1;
				GameSnake->dir.y = 0;
			}
		}
		else
		{
			if ((CInput_Buttons.ButUp) ||
				(CInput_Buttons.ButUp2) ||
				(CInput_Buttons.ButDpadUp))
			{
				if(GameSnake->movedone && GameSnake->dir.y == 0)
				{
					GameSnake->movedone = false;
					GameSnake->dir.x = 0;
					GameSnake->dir.y = -1;
				}
			}
			else
			{
				if ((CInput_Buttons.ButDown) ||
					(CInput_Buttons.ButDown2) ||
					(CInput_Buttons.ButDpadDown))
				{
					if(GameSnake->movedone && GameSnake->dir.y == 0)
					{
						GameSnake->movedone = false;
						GameSnake->dir.x = 0;
						GameSnake->dir.y = 1;
					}
				}
			}
		}
	}

	GameSnake->ticks += 1;

	if(GameSnake->ticks >= CGameSnake_updateticks)
	{
		GameSnake->movedone = true;
		GameSnake->ticks = 0;
		int i = GameSnake->snakelength;
		while (i >= 1)
		{
			GameSnake->body[i] = GameSnake->body[i-1];
			i -= 1;
		}
		GameSnake->body[0] = GameSnake->head;
		GameSnake->head.x += GameSnake->dir.x * CGameSnake_snakesize;
		GameSnake->head.y += GameSnake->dir.y * CGameSnake_snakesize;
		if((GameSnake->head.x < GameSnake->GameBase->screenleft) || (GameSnake->head.x >= GameSnake->GameBase->screenright) ||
			(GameSnake->head.y < GameSnake->GameBase->screentop) || (GameSnake->head.y >= GameSnake->GameBase->screenbottom))
			GameSnake->playerdeath = true;

		for (int i = 0; i < GameSnake->snakelength; i++)
			if ((GameSnake->head.x == GameSnake->body[i].x) && (GameSnake->head.y == GameSnake->body[i].y))
				GameSnake->playerdeath = true;
	}
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameSnake_DrawBackground(CGameSnake* GameSnake)
{
	//SDL_SetRenderDrawColor(Renderer, 0x65, 0x65, 0xFF, 0xFF);
	//SDL_RenderClear(Renderer);
	//SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	//SDL_Rect r = {GameSnake->GameBase->screenleft, GameSnake->GameBase->screentop, GameSnake->GameBase->playfieldwidth, GameSnake->GameBase->playfieldheight};
	//SDL_RenderFillRect(Renderer, &r);


//	SDL_SetRenderDrawColor(Renderer, 0x65, 0x65, 0xFF, 0xFF);
	pd->graphics->clear(kColorWhite);
	SDL_Rect r = { GameSnake->GameBase->screenleft, GameSnake->GameBase->screentop, GameSnake->GameBase->playfieldwidth, GameSnake->GameBase->playfieldheight };
	//SDL_RenderFillRect(Renderer, &r);
	pd->graphics->fillRect(r.x, r.y, r.w, r.h, kColorBlack);
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameSnake_init(CGameSnake* GameSnake)
{

	GameSnake->createsnake(GameSnake);
	GameSnake->createfood(GameSnake);

	GameSnake->movedone = true;
	GameSnake->GameBase->HealthPoints = 2;
	GameSnake->LoadSound(GameSnake);
	CurrentGameMusicID = GameSnake->MusMusic;
	CAudio_PlayMusic(GameSnake->MusMusic, -1);
}

void CGameSnake_deinit(CGameSnake* GameSnake)
{
	GameSnake->UnLoadSound(GameSnake);
	SubStateCounter = 0;
	SubGameState = SGNone;
	CurrentGameMusicID = -1;
}

void CGameSnake_LoadSound(CGameSnake* GameSnake)
{
	GameSnake->SfxFood = CAudio_LoadSound("snakey/food");
	GameSnake->SfxDie = CAudio_LoadSound("common/die");
	GameSnake->MusMusic = CAudio_LoadMusic("snakey/music");
}

void CGameSnake_UnLoadSound(CGameSnake* GameSnake)
{
	CAudio_StopMusic();
	CAudio_StopSound();
	CAudio_UnLoadMusic(GameSnake->MusMusic);
	CAudio_UnLoadSound(GameSnake->SfxFood);
	CAudio_UnLoadSound(GameSnake->SfxDie);
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameSnake_UpdateObjects(CGameSnake* GameSnake, bool IsGameState)
{
	if (IsGameState)
	{
		GameSnake->updatesnake(GameSnake);
		GameSnake->updatefood(GameSnake);
	}

	if (IsGameState && GameSnake->playerdeath)
	{
		CAudio_PlaySound(GameSnake->SfxDie, 0);
		CGame_AddToScore(-50);

		if (GameSnake->GameBase->HealthPoints > 1)
		{
			GameSnake->createsnake(GameSnake);
			GameSnake->createfood(GameSnake);
			if (GameMode == GMGame)
				GameSnake->GameBase->HealthPoints -= 1;
			SubGameState = SGReadyGo;
			SubStateTime = pd->system->getCurrentTimeMilliseconds() + 500;
		}
		else
			if(GameMode == GMGame)
				GameSnake->GameBase->HealthPoints -= 1;
	}
}

void CGameSnake_UpdateLogic(CGameSnake* GameSnake)
{
	GameSnake->GameBase->UpdateLogic(GameSnake->GameBase);
	GameSnake->UpdateObjects(GameSnake, SubGameState == SGGame);
	if(SubGameState == SGGame)
		CSprites_UpdateSprites();
}


bool CGameSnake_DrawObjects(CGameSnake* GameSnake)
{
	GameSnake->drawfood(GameSnake);
	GameSnake->drawsnake(GameSnake);
	//don't call drawsprites in base object
	return false;
}

void CGameSnake_Draw(CGameSnake* GameSnake)
{
	GameSnake->DrawBackground(GameSnake);
	if (GameSnake->DrawObjects(GameSnake))
		CSprites_DrawSprites();
	GameSnake->GameBase->DrawScoreBar(GameSnake->GameBase);
	GameSnake->GameBase->DrawSubStateText(GameSnake->GameBase);
}