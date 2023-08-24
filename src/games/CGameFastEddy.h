#ifndef CGAMEFASTEDDY_H
#define CGAMEFASTEDDY_H

#include <pd_api.h>
#include <stdbool.h>
#include "CGameBase.h"
#include "CSpriteObject.h"
#include "../Common.h"

#define GameFastEddy_playerstateunknown -1
#define GameFastEddy_playerstateidle 0
#define GameFastEddy_playerstateleft 1
#define GameFastEddy_playerstateleftjump 2
#define GameFastEddy_playerstateright 3
#define GameFastEddy_playerstaterightjump 4
#define GameFastEddy_playerstatejump 5
#define GameFastEddy_playerstateclimbup 6
#define GameFastEddy_playerstateclimbdown 7

#define GameFastEddy_enemystateidle 0
#define GameFastEddy_enemystatemoveleft 1
#define GameFastEddy_enemystatewaitmove 2
#define GameFastEddy_enemystatemoveright 3

#define GameFastEddy_collectablestatemoveleft 0
#define GameFastEddy_collectablestatemoveright 1
#define GameFastEddy_collectablestateidle 2

#define GameFastEddy_laddersfitrows 10
//tilesize / 2 oroginals are 64x64 was 32x32 but i tried to divide by 3 but actually divided by 3.2
#define GameFastEddy_rowfloorsizex (400.0f * xscale)
//tilesize / 2 oroginals are 64x64 was 32x32 but i tried to divide by 3 but actually divided by 3.2
#define GameFastEddy_rowfloorsizey (32.0f * yscale)
#define GameFastEddy_rows 5

#define GameFastEddy_playerspeed (6.4f*xscale * FPSScale)
#define GameFastEddy_playerjumpspeed 2.0f * FPSScale
#define GameFastEddy_playerclimbspeed (2.0f * yscale * FPSScale)
#define GameFastEddy_collectablespeed (2*xscale * FPSScale)

#define GameFastEddy_maxenemies 10
#define GameFastEddy_maxcollectables 2

//(rows - 1) * 2;
#define GameFastEddy_maxladders 8
//(int)(ScreenWidth / rowfloorsizex * rows)
#define GameFastEddy_maxfloors 16 

typedef struct CGameFastEddy CGameFastEddy;
struct CGameFastEddy {
	CGameBase *GameBase;
	 
	float playerjumpspeeddec;

	int numfloortilesperrow;
	int maxfloortiles;
	float rowspacingsize;
	float ladderwidth;
	float rowsize;
	float playerheight;
	float enemyheight;
	float playerjumpheight;
	float collectableheight;
	float keyheight;

	int spritesheet;
	int spritesheetladder;
	int spritesheetplayerclimb;
	int spritesheetplayerrun;
	int spritesheetplayeridle;
	int spritesheetplayerjump;
	int spritesheetenemy;
	int spritesheetenemy2;
	int spritesheetcollectable;
	int spritesheetkey;
	int background;
	SDL_Point backgroundtz;

	int MusMusic, SfxSucces, SfxCollect, SfxDie;

	int rowzeroenemyindex;
	int collecteditems;
	int collectedcreated;
	CSpriteObject player;
	CSpriteObject ladders[GameFastEddy_maxladders];
	CSpriteObject floors[GameFastEddy_maxfloors]; 
	CSpriteObject enemies[GameFastEddy_maxenemies];
	CSpriteObject collectables[GameFastEddy_maxcollectables];
	CSpriteObject key;

	void (*createkey)(CGameFastEddy* GameFastEddy);
	void (*destroykey)(CGameFastEddy* GameFastEddy);
	void (*updatekey)(CGameFastEddy* GameFastEddy);
	void (*createladders)(CGameFastEddy* GameFastEddy);
	void (*destroyladders)(CGameFastEddy* GameFastEddy);
	void (*createfloors)(CGameFastEddy* GameFastEddy);
	void (*destroyfloors)(CGameFastEddy* GameFastEddy);
	void (*updateplayer)(CGameFastEddy* GameFastEddy);
	void (*createplayer)(CGameFastEddy* GameFastEddy);
	void (*destroyplayer)(CGameFastEddy* GameFastEddy);
	void (*updateenemies)(CGameFastEddy* GameFastEddy);
	void (*createenemies)(CGameFastEddy* GameFastEddy,bool levelsucces);
	void (*createenemy)(CGameFastEddy* GameFastEddy,int row, float x, int state, int group, int multiply);
	void (*enemyenablelevelend)(CGameFastEddy* GameFastEddy);
	void (*destroyenemies)(CGameFastEddy* GameFastEddy);
	void (*updatecollectables)(CGameFastEddy* GameFastEddy);
	void (*createcollectables)(CGameFastEddy* GameFastEddy,int ignorerow);
	void (*destroycollectable)(CGameFastEddy* GameFastEddy,int index);
	void (*destroyallcollectables)(CGameFastEddy* GameFastEddy);
	void (*init)(CGameFastEddy* GameFastEddy);
	void (*deinit)(CGameFastEddy* GameFastEddy);
	void (*UnloadGraphics)(CGameFastEddy* GameFastEddy);
	void (*LoadGraphics)(CGameFastEddy* GameFastEddy);
	void (*LoadSound)(CGameFastEddy* GameFastEddy);
	void (*UnLoadSound)(CGameFastEddy* GameFastEddy);
	void (*UpdateObjects)(CGameFastEddy* GameFastEddy, bool IsGameState);
	void (*DrawBackground)(CGameFastEddy* GameFastEddy);
	void (*Draw)(CGameFastEddy* GameFastEddy);
	void (*UpdateLogic)(CGameFastEddy* GameFastEddy);
};


void CGameFastEddy_Draw(CGameFastEddy* GameFastEddy);
void CGameFastEddy_UpdateLogic(CGameFastEddy* GameFastEddy);
void CGameFastEddy_UpdateObjects(CGameFastEddy* GameFastEddy, bool IsGameState);
void CGameFastEddy_UnloadGraphics(CGameFastEddy* GameFastEddy);
void CGameFastEddy_LoadGraphics(CGameFastEddy* GameFastEddy);
void CGameFastEddy_UnLoadSound(CGameFastEddy* GameFastEddy);
void CGameFastEddy_LoadSound(CGameFastEddy* GameFastEddy);
void CGameFastEddy_deinit(CGameFastEddy* GameFastEddy);
void CGameFastEddy_init(CGameFastEddy* GameFastEddy);
void CGameFastEddy_DrawBackground(CGameFastEddy* GameFastEddy);
void CGameFastEddy_createladders(CGameFastEddy* GameFastEddy);
void CGameFastEddy_destroyladders(CGameFastEddy* GameFastEddy);
void CGameFastEddy_createfloors(CGameFastEddy* GameFastEddy);
void CGameFastEddy_destroyfloors(CGameFastEddy* GameFastEddy);
void CGameFastEddy_updateplayer(CGameFastEddy* GameFastEddy);
void CGameFastEddy_createplayer(CGameFastEddy* GameFastEddy);
void CGameFastEddy_destroyplayer(CGameFastEddy* GameFastEddy);
void CGameFastEddy_updateenemies(CGameFastEddy* GameFastEddy);
void CGameFastEddy_createenemies(CGameFastEddy* GameFastEddy, bool levelsucces);
void CGameFastEddy_createenemy(CGameFastEddy* GameFastEddy, int row, float x, int state, int group, int multiply);
void CGameFastEddy_enemyenablelevelend(CGameFastEddy* GameFastEddy);
void CGameFastEddy_destroyenemies(CGameFastEddy* GameFastEddy);
void CGameFastEddy_updatecollectables(CGameFastEddy* GameFastEddy);
void CGameFastEddy_createcollectables(CGameFastEddy* GameFastEddy, int ignorerow);
void CGameFastEddy_destroycollectable(CGameFastEddy* GameFastEddy, int index);
void CGameFastEddy_destroyallcollectables(CGameFastEddy* GameFastEddy);
void CGameFastEddy_updatekey(CGameFastEddy* GameFastEddy);
void CGameFastEddy_destroykey(CGameFastEddy* GameFastEddy);
void CGameFastEddy_createkey(CGameFastEddy* GameFastEddy);
void Destroy_CGameFastEddy(CGameFastEddy* GameFastEddy);
CGameFastEddy* Create_CGameFastEddy();

#endif