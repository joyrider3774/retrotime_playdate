#include <pd_api.h>
#include <stdbool.h>
#include "CGameRamIt.h"
#include "../CGame.h"
#include "../Common.h"
#include "../SDL_HelperTypes.h"
#include "../pd_helperfuncs.h"

LCDPattern kColor1 = {
	// Bitmap
	0b10101010,
	0b01010101,
	0b10101010,
	0b01010101,
	0b10101010,
	0b01010101,
	0b10101010,
	0b01010101,

	// Mask
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
};

LCDPattern kColor2 = {
	// Bitmap
	0b10011001,
	0b01100110,
	0b10011001,
	0b01100110,
	0b10011001,
	0b01100110,
	0b10011001,
	0b01100110,

	// Mask
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
};

LCDPattern kColor3 = {
	// Bitmap
	0b11101110,
	0b01110111,
	0b10111011,
	0b11011100,
	0b11101110,
	0b01110111,
	0b10111011,
	0b11011100,

	// Mask
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
};

LCDPattern kColor4 = {
	// Bitmap
	0b11001100,
	0b01100110,
	0b00110011,
	0b10011000,
	0b11001100,
	0b01100110,
	0b00110011,
	0b10011000,

	// Mask
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
};

LCDPattern kColor5 = {
	// Bitmap
	0b10001000,
	0b01000100,
	0b00100010,
	0b00010001,
	0b10001000,
	0b01000100,
	0b00100010,
	0b00010001,

	// Mask
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
};

LCDPattern kColor6 = {
	// Bitmap
	0b10001000,
	0b00010001,
	0b00100010,
	0b01000100,
	0b10001000,
	0b00010001,
	0b00100010,
	0b01000100,

	// Mask
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
};

LCDPattern kColor7 = {
	// Bitmap
	0b11000011,
	0b11101101,
	0b10111001,
	0b10010001,
	0b10010001,
	0b10111001,
	0b11101101,
	0b11000011,

	// Mask
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
};

LCDPattern kColor8 = {
	// Bitmap
	0b11000110,
	0b11000110,
	0b10110001,
	0b10110001,
	0b01101100,
	0b01101100,
	0b00011011,
	0b00011011,

	// Mask
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
};


LCDPattern kColor9 = {
	// Bitmap
	0b11100111,
	0b11011011,
	0b11100111,
	0b11011011,
	0b11100111,
	0b11011011,
	0b11100111,
	0b11011011,

	// Mask
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
};

LCDPattern kColor10 = {
	// Bitmap
	0b10010010,
	0b11011011,
	0b01101101,
	0b11011011,
	0b10010010,
	0b11011011,
	0b01101101,
	0b11011011,

	// Mask
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
};


CGameRamIt* Create_CGameRamIt()
{
	CGameRamIt* GameRamIt = (CGameRamIt*) malloc(sizeof(CGameRamIt));
	GameRamIt->GameBase = Create_CGameBase(GSRamIt, true);
	GameRamIt->bulletalive = false;
	GameRamIt->speed = 0;
	GameRamIt->speedcount = 0;
	GameRamIt->SfxShoot = -1;
	GameRamIt->SfxHit = -1;
	GameRamIt->SfxDie = -1;
	GameRamIt->SfxSucces = -1;
	GameRamIt->MusMusic = -1;
	GameRamIt->riblocksize = (int)(((ScreenHeight - ScoreBarHeight) / (CGameRamIt_numblocks+1)) - 2 * CGameRamIt_blockspacing);
	GameRamIt->riblocksizespacing =(int)(GameRamIt->riblocksize + 2 * CGameRamIt_blockspacing);
	GameRamIt->GameBase->playfieldwidth = (ScreenWidth - 1 * GameRamIt->riblocksize);
	GameRamIt->segmentwidth = (int)(((GameRamIt->GameBase->playfieldwidth / 2) / CGameRamIt_blocksegments));
	GameRamIt->GameBase->playfieldheight = CGameRamIt_numblocks * GameRamIt->riblocksizespacing;
	GameRamIt->GameBase->screenleft = (ScreenWidth - GameRamIt->GameBase->playfieldwidth) / 2;
	GameRamIt->GameBase->screenright = GameRamIt->GameBase->screenleft + GameRamIt->GameBase->playfieldwidth;
	GameRamIt->GameBase->screentop =  (ScreenHeight - GameRamIt->GameBase->playfieldheight + ScoreBarHeight) / 2;
	GameRamIt->GameBase->screenbottom = GameRamIt->GameBase->screentop + GameRamIt->GameBase->playfieldheight;


	GameRamIt->createbullet = CGameRamIt_createbullet;
	GameRamIt->updatebullet = CGameRamIt_updatebullet;
	GameRamIt->drawbullet = CGameRamIt_drawbullet;

	GameRamIt->createplayer = CGameRamIt_createplayer;
	GameRamIt->drawplayer = CGameRamIt_drawplayer;
	GameRamIt->updateplayer = CGameRamIt_updateplayer;

	GameRamIt->createplayfield = CGameRamIt_createplayfield;
	GameRamIt->updateplayfield = CGameRamIt_updateplayfield;
	GameRamIt->drawplayfield = CGameRamIt_drawplayfield;

	GameRamIt->init = CGameRamIt_init;
	GameRamIt->deinit = CGameRamIt_deinit;
	GameRamIt->LoadSound = CGameRamIt_LoadSound;
	GameRamIt->UnLoadSound = CGameRamIt_UnLoadSound;
	GameRamIt->UpdateObjects = CGameRamIt_UpdateObjects;
	GameRamIt->DrawObjects = CGameRamIt_DrawObjects;
	GameRamIt->DrawBackground = CGameRamIt_DrawBackground;
	GameRamIt->Draw = CGameRamIt_Draw;
	GameRamIt->UpdateLogic = CGameRamIt_UpdateLogic;
	return GameRamIt;
}

void Destroy_CGameRamIt(CGameRamIt* GameRamIt) 
{
	Destroy_CGameBase(GameRamIt->GameBase);
	free(GameRamIt);
	GameRamIt = NULL;
}

bool CGameRamIt_createbullet(CGameRamIt* GameRamIt)
{
	bool result = false;
	if (!GameRamIt->bulletalive)
	{
		GameRamIt->bulletalive = true;
		GameRamIt->bulletpos = GameRamIt->playerpos;
		GameRamIt->bulletvel.x = (int)(GameRamIt->playerdx * CGameRamIt_bulletspeed);
		GameRamIt->bulletvel.y = 0;
		result = true;
	}
	return result;
}

//BULLET ----------------------------------------------------------------------------------------------------------------

void CGameRamIt_updatebullet(CGameRamIt* GameRamIt)
{
	if (GameRamIt->bulletalive)
	{
		bool destroybullet = false;
		bool playaudiobullet = false;
		for(int steps = 0; steps < CGameRamIt_bulletsteps; steps++)
		{
			GameRamIt->bulletpos.x += GameRamIt->bulletvel.x;
			GameRamIt->bulletpos.y += GameRamIt->bulletvel.y;
			if (GameRamIt->bulletpos.x + GameRamIt->riblocksize / 2 < GameRamIt->GameBase->screenleft)
				destroybullet = true;
			if (GameRamIt->bulletpos.x - GameRamIt->riblocksize / 2 > GameRamIt->GameBase->screenright)
				destroybullet = true;

			for (int side = 0; side < CGameRamIt_sides; side++)
			{
				for (int block = 0; block < CGameRamIt_numblocks; block++)
				{
					if (GameRamIt->playfield[side][block].segments > 0)
					{
						if ((GameRamIt->bulletpos.y + GameRamIt->riblocksize / 6 >= GameRamIt->GameBase->screentop +
							block * GameRamIt->riblocksizespacing + CGameRamIt_blockspacing) &&
							(GameRamIt->bulletpos.y - GameRamIt->riblocksize / 6 <= GameRamIt->GameBase->screentop +
							block * GameRamIt->riblocksizespacing + CGameRamIt_blockspacing + GameRamIt->riblocksize))
						{
							if (side == 0)
							{
								if ((GameRamIt->bulletpos.x - GameRamIt->riblocksize / 2) <= GameRamIt->GameBase->screenleft +
									GameRamIt->playfield[side][block].segments * GameRamIt->segmentwidth)
								{
									GameRamIt->playfield[side][block].segments -= 1;
									if (GameRamIt->playfield[side][block].segments == 0)
										CGame_AddToScore((GameRamIt->GameBase->level + 1) * 2*GameRamIt->playfield[side][block].maxsegments);

									playaudiobullet = true;
									destroybullet = true;
								}
							}
							else
							{
								if ((GameRamIt->bulletpos.x - GameRamIt->riblocksize / 2) >= GameRamIt->GameBase->screenright -
									GameRamIt->playfield[side][block].segments * GameRamIt->segmentwidth)
								{
									GameRamIt->playfield[side][block].segments -= 1;
									if (GameRamIt->playfield[side][block].segments == 0)
										CGame_AddToScore((GameRamIt->GameBase->level + 2) * 2 * GameRamIt->playfield[side][block].maxsegments);

									playaudiobullet = true;
									destroybullet = true;
								}
							}
						}
					}
				}
			}

			if(destroybullet)
			{
				if (playaudiobullet)
					CAudio_PlaySound(GameRamIt->SfxHit, 0);
				GameRamIt->bulletalive = false;
				break;
			}
		}
	}
}

void CGameRamIt_drawbullet(CGameRamIt* GameRamIt)
{
	if (GameRamIt->bulletalive)
	{
		SDL_Rect r = {GameRamIt->bulletpos.x - GameRamIt->riblocksize / 2, GameRamIt->bulletpos.y - GameRamIt->riblocksize / 6, GameRamIt->riblocksize, GameRamIt->riblocksize / 3};
		//SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
		//SDL_RenderFillRect(Renderer, &r);
		pd->graphics->fillRect(r.x, r.y, r.w, r.h, kColorWhite);
	}
}

//PLAYER ----------------------------------------------------------------------------------------------------------------

void CGameRamIt_createplayer(CGameRamIt* GameRamIt)
{
	GameRamIt->playerpos.x = GameRamIt->GameBase->screenleft + (GameRamIt->GameBase->screenright - GameRamIt->GameBase->screenleft) / 2;
	GameRamIt->playerpos.y = GameRamIt->GameBase->screentop + (GameRamIt->GameBase->screenbottom - GameRamIt->GameBase->screentop) / 2;
	GameRamIt->playerdx = 1;
}

void CGameRamIt_drawplayer(CGameRamIt* GameRamIt)
{
	SDL_Rect r;
	r.x = (int)(GameRamIt->GameBase->screenleft + (GameRamIt->GameBase->screenright - GameRamIt->GameBase->screenleft -CGameRamIt_playerrailwidth) / 2);
	r.y = GameRamIt->GameBase->screentop;
	r.w = (int)CGameRamIt_playerrailwidth;
	r.h = GameRamIt->GameBase->screenbottom - GameRamIt->GameBase->screentop;
	pd->graphics->fillRect(r.x, r.y, r.w, r.h, kColorWhite);

	r.x = GameRamIt->playerpos.x - GameRamIt->riblocksize / 2;
	r.y = GameRamIt->playerpos.y - GameRamIt->riblocksize / 2;
	r.w = GameRamIt->riblocksize;
	r.h = GameRamIt->riblocksize;
	pd->graphics->fillRect(r.x, r.y, r.w, r.h, kColorWhite);

	if (GameRamIt->playerdx == 1)
	{
		r.x = GameRamIt->playerpos.x + GameRamIt->riblocksize / 2;
		r.y = GameRamIt->playerpos.y - GameRamIt->riblocksize / 4;
		r.w = GameRamIt->riblocksize / 2;
		r.h = GameRamIt->riblocksize / 2;
	}
	else
	{
		r.x = GameRamIt->playerpos.x - GameRamIt->riblocksize;
		r.y = GameRamIt->playerpos.y - GameRamIt->riblocksize / 4;
		r.w = GameRamIt->riblocksize / 2;
		r.h = GameRamIt->riblocksize / 2;
	}
	pd->graphics->fillRect(r.x, r.y, r.w, r.h, kColorWhite);
}

void CGameRamIt_updateplayer(CGameRamIt* GameRamIt)
{
	if ((CInput_Buttons.ButLeft) ||
		(CInput_Buttons.ButLeft2) ||
		(CInput_Buttons.ButDpadLeft))
		GameRamIt->playerdx = -1;

	if ((CInput_Buttons.ButRight) ||
		(CInput_Buttons.ButRight2) ||
		(CInput_Buttons.ButDpadRight))
		GameRamIt->playerdx = 1;

	if ((CInput_Buttons.ButDown) ||
		(CInput_Buttons.ButDown2) ||
		(CInput_Buttons.ButDpadDown))
	{
		if (GameRamIt->playerpos.y + GameRamIt->riblocksize / 2 + CGameRamIt_playerspeed < GameRamIt->GameBase->screenbottom)
			GameRamIt->playerpos.y += (int)CGameRamIt_playerspeed;
		else
			GameRamIt->playerpos.y = GameRamIt->GameBase->screenbottom - GameRamIt->riblocksize / 2;
	}

	if ((CInput_Buttons.ButUp) ||
		(CInput_Buttons.ButUp2) ||
		(CInput_Buttons.ButDpadUp))
	{
		if (GameRamIt->playerpos.y - GameRamIt->riblocksize / 2 - CGameRamIt_playerspeed > GameRamIt->GameBase->screentop)
			GameRamIt->playerpos.y -= (int)CGameRamIt_playerspeed;
		else
			GameRamIt->playerpos.y = GameRamIt->GameBase->screentop + GameRamIt->riblocksize / 2;
	}

	if (CInput_Buttons.ButA && !CInput_PrevButtons.ButA)
		if (GameRamIt->createbullet(GameRamIt))
		{
			CAudio_PlaySound(GameRamIt->SfxShoot, 0);
		}
}

//playfield ----------------------------------------------------------------------------------------------------------------

void CGameRamIt_createplayfield(CGameRamIt* GameRamIt)
{
	int prevpiece = -1;
	int piece = -1;
	LCDColor color = (LCDColor) kColorWhite;
	for (int side = 0 ; side < CGameRamIt_sides; side++)
	{
		for (int block = 0; block < CGameRamIt_numblocks; block++)
		{
			while(prevpiece == piece)
				piece = rand() % 17;

			prevpiece = piece;

			if (piece == 0)
			 	color = (LCDColor) kColorWhite;

			if (piece == 1)
				color = (LCDColor) kColor1;

			if (piece == 2)
				color = (LCDColor) kColor2;

			if (piece == 3)
				color = (LCDColor) kColor3;

			if (piece == 4)
				color = (LCDColor) kColor4;

			if (piece == 5)
				color = (LCDColor) kColor5;

			if (piece == 6)
				color = (LCDColor) kColor6;

			if (piece == 7)
				color = (LCDColor) kColor7;

			if (piece == 8)
				color = (LCDColor) kColor8;

			if (piece == 9)
				color = (LCDColor) kColor9;

			if (piece == 10)
				color = (LCDColor) kColor10;

			if (piece == 11)
				color = (LCDColor) kColorWhite;

			if (piece == 12)
				color = (LCDColor) kColor1;

			if (piece == 13)
				color = (LCDColor) kColor2;

			if (piece == 14)
				color = (LCDColor) kColor3;

			if (piece == 15)
				color = (LCDColor) kColor4;

			if (piece == 16)
				color = (LCDColor) kColor5;

			GameRamIt->playfield[side][block].color = color;
			GameRamIt->playfield[side][block].segments = 2;
			GameRamIt->playfield[side][block].maxsegments = 2;
		}
	}
}

void CGameRamIt_updateplayfield(CGameRamIt* GameRamIt, bool force)
{
	GameRamIt->speedcount += 1;

	if (force || (GameRamIt->speedcount >= GameRamIt->speed - ((GameRamIt->GameBase->level-1) * CGameRamIt_tickslevelmul)))
	{
		GameRamIt->speedcount = 0;
		int stageclear = true;
		int numalive = 0;
		int side;
		int block;
		for (side = 0; side < CGameRamIt_sides; side++)
			for(block = 0; block < CGameRamIt_numblocks; block++)
				if (GameRamIt->playfield[side][block].segments != 0)
				{
					stageclear = false;
					numalive += 1;
				}

		if (stageclear)
		{
			CAudio_PlaySound(GameRamIt->SfxSucces, 0);
			GameRamIt->createplayfield(GameRamIt);
			GameRamIt->GameBase->level += 1;
			CGame_AddToScore((GameRamIt->GameBase->level-1) * 100);
		}

		if (!stageclear)
		{
			int r = 1;
			if (numalive <= 5)
				r = rand() % 2;

			if (r == 1)
			{
				side = rand() % CGameRamIt_sides;
				block = rand() % CGameRamIt_numblocks;
				while (GameRamIt->playfield[side][block].segments == 0)
				{
					side = rand() % CGameRamIt_sides;
					block = rand() % CGameRamIt_numblocks;
				}
				GameRamIt->playfield[side][block].segments += 2;
				if (GameRamIt->playfield[side][block].segments > CGameRamIt_blocksegments)
					GameRamIt->playfield[side][block].segments = CGameRamIt_blocksegments;

				if (GameRamIt->playfield[side][block].segments > GameRamIt->playfield[side][block].maxsegments)
					GameRamIt->playfield[side][block].maxsegments = GameRamIt->playfield[side][block].segments;

				//death
				for (side = 0; side < CGameRamIt_sides; side++)
					for (block = 0; block < CGameRamIt_numblocks; block++)
						if (GameRamIt->playfield[side][block].segments == CGameRamIt_blocksegments)
						{
							GameRamIt->playerdeathtime = pd->system->getCurrentTimeMilliseconds() + 500;
							GameRamIt->playerdeath = true;
							CAudio_PlaySound(GameRamIt->SfxDie, 0);
							break;
						}
			}
		}
	}
}

void CGameRamIt_drawplayfield(CGameRamIt* GameRamIt)
{
	for (int side = 0; side < CGameRamIt_sides; side++)
		for (int block = 0; block < CGameRamIt_numblocks; block++)
		{
			SDL_Rect r;
			if (side == 0)
			{
				r.x = GameRamIt->GameBase->screenleft;
				r.y = (int)(GameRamIt->GameBase->screentop + block * GameRamIt->riblocksizespacing + CGameRamIt_blockspacing);
				r.w = GameRamIt->playfield[side][block].segments * GameRamIt->segmentwidth;
				r.h =GameRamIt->riblocksize;
			}
			else
			{
				r.x = GameRamIt->GameBase->screenright - GameRamIt->playfield[side][block].segments * GameRamIt->segmentwidth;
				r.y = (int)(GameRamIt->GameBase->screentop + block * GameRamIt->riblocksizespacing + CGameRamIt_blockspacing);
				r.w = GameRamIt->playfield[side][block].segments * GameRamIt->segmentwidth;
				r.h = GameRamIt->riblocksize;
			}
			pd->graphics->fillRect(r.x, r.y, r.w, r.h, GameRamIt->playfield[side][block].color);
		}
}

//background ----------------------------------------------------------------------------------------------------------------

void CGameRamIt_DrawBackground(CGameRamIt* GameRamIt)
{
	pd->graphics->clear(kColorWhite);
	SDL_Rect r = {GameRamIt->GameBase->screenleft, GameRamIt->GameBase->screentop, GameRamIt->GameBase->playfieldwidth, GameRamIt->GameBase->playfieldheight};
	pd->graphics->fillRect(r.x, r.y, r.w, r.h, kColorBlack);
	
}

//init - deinit ----------------------------------------------------------------------------------------------------------------

void CGameRamIt_init(CGameRamIt* GameRamIt)
{
	GameRamIt->bulletalive = false;
	GameRamIt->speed = CGameRamIt_ticksidle;
	GameRamIt->GameBase->level = 1;
	GameRamIt->playerdeath = false;
	GameRamIt->playerdeathtime = 0;
	GameRamIt->createplayer(GameRamIt);
	GameRamIt->createplayfield(GameRamIt);
	GameRamIt->GameBase->HealthPoints = 3;
	GameRamIt->LoadSound(GameRamIt);
	CurrentGameMusicID = GameRamIt->MusMusic;
	CAudio_PlayMusic(GameRamIt->MusMusic, -1);
}

void CGameRamIt_deinit(CGameRamIt* GameRamIt)
{
	GameRamIt->UnLoadSound(GameRamIt);
	SubStateCounter = 0;
	SubGameState = SGNone;
	CurrentGameMusicID = -1;
}

void CGameRamIt_LoadSound(CGameRamIt* GameRamIt)
{
	GameRamIt->SfxShoot = CAudio_LoadSound("ramit/shoot");
	GameRamIt->SfxHit = CAudio_LoadSound("ramit/hit");
	GameRamIt->SfxDie = CAudio_LoadSound("common/die");
	GameRamIt->SfxSucces = CAudio_LoadSound("common/succes");
	GameRamIt->MusMusic = CAudio_LoadMusic("ramit/music");
}

void CGameRamIt_UnLoadSound(CGameRamIt* GameRamIt)
{
	CAudio_StopMusic();
	CAudio_UnLoadMusic(GameRamIt->MusMusic);
	CAudio_UnLoadSound(GameRamIt->SfxShoot);
	CAudio_UnLoadSound(GameRamIt->SfxHit);
	CAudio_UnLoadSound(GameRamIt->SfxDie);
	CAudio_UnLoadSound(GameRamIt->SfxSucces);
}

//Update ----------------------------------------------------------------------------------------------------------------

void CGameRamIt_UpdateObjects(CGameRamIt* GameRamIt, bool IsGameState)
{
	if (IsGameState)
	{
		if (!GameRamIt->playerdeath)
		{
			GameRamIt->updateplayer(GameRamIt);
			GameRamIt->updateplayfield(GameRamIt, false);
			GameRamIt->updatebullet(GameRamIt);
		}
		//don't handle death state immediatly after updating
		//otherwise it won't draw the last state hence the else
		else
		{
			if (GameRamIt->GameBase->HealthPoints > 1)
			{
				if (GameRamIt->playerdeathtime < pd->system->getCurrentTimeMilliseconds())
				{
					if (GameMode == GMGame)
						GameRamIt->GameBase->HealthPoints -= 1;

					GameRamIt->createplayfield(GameRamIt);
					GameRamIt->playerdeath = false;	
					SubGameState = SGReadyGo;
					SubStateTime = pd->system->getCurrentTimeMilliseconds() + 500;
				}
			}
			else
				if (GameMode == GMGame)
					if (GameRamIt->GameBase->HealthPoints > 0)
						GameRamIt->GameBase->HealthPoints -= 1;
		}
	}
}

void CGameRamIt_UpdateLogic(CGameRamIt* GameRamIt)
{
	GameRamIt->GameBase->UpdateLogic(GameRamIt->GameBase);

	if ((GameState == GSTitleScreenInit) || (SubGameState == SGPauseMenu) || (SubGameState == SGFrame) || (SubGameState == SGGameHelp))
		return;

	GameRamIt->UpdateObjects(GameRamIt, SubGameState == SGGame);
	if(SubGameState == SGGame)
		CSprites_UpdateSprites();
}

bool CGameRamIt_DrawObjects(CGameRamIt* GameRamIt)
{
	GameRamIt->drawbullet(GameRamIt);
	GameRamIt->drawplayer(GameRamIt);
	GameRamIt->drawplayfield(GameRamIt);
	//don't call drawsprites
	return false;
}


void CGameRamIt_Draw(CGameRamIt* GameRamIt)
{
	if ((GameState == GSTitleScreenInit) || (SubGameState == SGPauseMenu) || (SubGameState == SGFrame) || (SubGameState == SGGameHelp))
		return;

	GameRamIt->DrawBackground(GameRamIt);
	if (GameRamIt->DrawObjects(GameRamIt))
		CSprites_DrawSprites();
	GameRamIt->GameBase->DrawScoreBar(GameRamIt->GameBase);
	GameRamIt->GameBase->DrawSubStateText(GameRamIt->GameBase);
}