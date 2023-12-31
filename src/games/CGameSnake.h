#ifndef CGAMESNAKE_H
#define CGAMESNAKE_H

#include <pd_api.h>
#include <stdbool.h>
#include "CGameBase.h"
#include "../Common.h"
#include "../SDL_HelperTypes.h"

//floor(35 * yscale)
#define CGameSnake_snakesize 10
//(int)(ScreenHeight / snakesize) -1;
#define CGameSnake_rows 20
//(int)(ScreenWidth / snakesize) -1;
#define CGameSnake_cols 38
#define CGameSnake_updateticks 8 / FPSScale
//rows*cols
#define CGameSnake_totalcells 760

typedef struct CGameSnake CGameSnake;

struct CGameSnake {

	CGameBase *GameBase;
	LCDColor snakeheadcolor;
	LCDColor snakebodycolor;
	LCDColor snakefoodcolor;

	bool movedone;
	int snakelength;
	int ticks;
	SDL_Point head;
	SDL_Point body[CGameSnake_totalcells];
	SDL_Point dir;
	SDL_Point food;
	unsigned int playerdeathtime;
	int playerdeath;
	int MusMusic, SfxFood, SfxDie;

	void (*createfood)(CGameSnake* GameSnake);
	void (*drawfood)(CGameSnake* GameSnake);
	void (*updatefood)(CGameSnake* GameSnake);

	void (*createsnake)(CGameSnake* GameSnake);
	void (*drawsnake)(CGameSnake* GameSnake);
	void (*updatesnake)(CGameSnake* GameSnake);

	void (*init)(CGameSnake* GameSnake);
	void (*deinit)(CGameSnake* GameSnake);
	void (*LoadSound)(CGameSnake* GameSnake);
	void (*UnLoadSound)(CGameSnake* GameSnake);
	void (*UpdateObjects)(CGameSnake* GameSnake, bool IsGameState);
	bool (*DrawObjects)(CGameSnake* GameSnake);
	void (*DrawBackground)(CGameSnake* GameSnake);
	void (*Draw)(CGameSnake* GameSnake);
	void (*UpdateLogic)(CGameSnake* GameSnake);
};

void CGameSnake_Draw(CGameSnake* GameSnake);
bool CGameSnake_DrawObjects(CGameSnake* GameSnake);
void CGameSnake_UpdateLogic(CGameSnake* GameSnake);
void CGameSnake_UpdateObjects(CGameSnake* GameSnake, bool IsGameState);
void CGameSnake_UnLoadSound(CGameSnake* GameSnake);
void CGameSnake_LoadSound(CGameSnake* GameSnake);
void CGameSnake_deinit(CGameSnake* GameSnake);
void CGameSnake_init(CGameSnake* GameSnake);
void CGameSnake_DrawBackground(CGameSnake* GameSnake);
void CGameSnake_updatesnake(CGameSnake* GameSnake);
void CGameSnake_drawsnake(CGameSnake* GameSnake);
void CGameSnake_createsnake(CGameSnake* GameSnake);
void CGameSnake_updatefood(CGameSnake* GameSnake);
void CGameSnake_drawfood(CGameSnake* GameSnake);
void CGameSnake_createfood(CGameSnake* GameSnake);
void Destroy_CGameSnake(CGameSnake* GameSnake);
CGameSnake* Create_CGameSnake();

#endif