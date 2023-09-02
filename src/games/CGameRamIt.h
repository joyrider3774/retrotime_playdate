#ifndef CGAMERAMIT_H
#define CGAMERAMIT_H

#include <pd_api.h>
#include <stdbool.h>
#include "CGameBase.h"
#include "../Common.h"
#include "../SDL_HelperTypes.h"


#define CGameRamIt_bulletsteps 6
#define CGameRamIt_playerrailwidth (10*xscale)
#define CGameRamIt_playerspeed (10*xscale*FPSScale)
#define CGameRamIt_bulletspeed (5*xscale*FPSScale)
#define CGameRamIt_numblocks 16
#define CGameRamIt_blocksegments 21
#define CGameRamIt_sides 2
#define CGameRamIt_blockspacing (int)(5*yscale)
#define CGameRamIt_ticksidle 35
#define CGameRamIt_tickslevelmul 5

struct ramitblock
{
	int maxsegments;
	int segments;
	LCDColor color;
};
typedef struct ramitblock ramitblock;

typedef struct CGameRamIt CGameRamIt;

struct CGameRamIt {

	CGameBase *GameBase;

	int SfxShoot, SfxHit, SfxDie, SfxSucces, MusMusic;
	int speed, speedcount, playerdx, playerdeath, riblocksize, riblocksizespacing, segmentwidth;
	unsigned int playerdeathtime;
	SDL_Point playerpos;
	SDL_Point bulletpos;
	SDL_Point bulletvel;
	bool bulletalive;
	ramitblock playfield[CGameRamIt_sides][CGameRamIt_numblocks];

	bool (*createbullet)(CGameRamIt* GameRamIt);
	void (*updatebullet)(CGameRamIt* GameRamIt);
	void (*drawbullet)(CGameRamIt* GameRamIt);

	void (*createplayer)(CGameRamIt* GameRamIt);
	void (*drawplayer)(CGameRamIt* GameRamIt);
	void (*updateplayer)(CGameRamIt* GameRamIt);

	void (*createplayfield)(CGameRamIt* GameRamIt);
	void (*updateplayfield)(CGameRamIt* GameRamIt, bool force);
	void (*drawplayfield)(CGameRamIt* GameRamIt);

	void (*init)(CGameRamIt* GameRamIt);
	void (*deinit)(CGameRamIt* GameRamIt);
	void (*LoadSound)(CGameRamIt* GameRamIt);
	void (*UnLoadSound)(CGameRamIt* GameRamIt);
	void (*UpdateObjects)(CGameRamIt* GameRamIt,bool IsGameState);
	bool (*DrawObjects)(CGameRamIt* GameRamIt);
	void (*DrawBackground)(CGameRamIt* GameRamIt);
	void (*Draw)(CGameRamIt* GameRamIt);
	void (*UpdateLogic)(CGameRamIt* GameRamIt);
};


void CGameRamIt_Draw(CGameRamIt* GameRamIt);
bool CGameRamIt_DrawObjects(CGameRamIt* GameRamIt);
void CGameRamIt_UpdateLogic(CGameRamIt* GameRamIt);
void CGameRamIt_UpdateObjects(CGameRamIt* GameRamIt, bool IsGameState);
void CGameRamIt_UnLoadSound(CGameRamIt* GameRamIt);
void CGameRamIt_LoadSound(CGameRamIt* GameRamIt);
void CGameRamIt_deinit(CGameRamIt* GameRamIt);
void CGameRamIt_init(CGameRamIt* GameRamIt);
void CGameRamIt_DrawBackground(CGameRamIt* GameRamIt);
void CGameRamIt_drawplayfield(CGameRamIt* GameRamIt);
void CGameRamIt_updateplayfield(CGameRamIt* GameRamIt, bool force);
void CGameRamIt_createplayfield(CGameRamIt* GameRamIt);
void CGameRamIt_updateplayer(CGameRamIt* GameRamIt);
void CGameRamIt_drawplayer(CGameRamIt* GameRamIt);
void CGameRamIt_createplayer(CGameRamIt* GameRamIt);
void CGameRamIt_drawbullet(CGameRamIt* GameRamIt);
void CGameRamIt_updatebullet(CGameRamIt* GameRamIt);
bool CGameRamIt_createbullet(CGameRamIt* GameRamIt);
void Destroy_CGameRamIt(CGameRamIt* GameRamIt);
CGameRamIt* Create_CGameRamIt();

#endif