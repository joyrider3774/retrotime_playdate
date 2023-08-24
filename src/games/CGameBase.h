#ifndef CGAMEBASE_H
#define CGAMEBASE_H

#include <pd_api.h>
#include <stdbool.h>

typedef struct CGameBase CGameBase;
struct CGameBase {
	char SubStateText[500];
	bool UsesLevels;
	int level;
	int screenleft, screenright, screentop, screenbottom, playfieldwidth, playfieldheight, HealthPoints;
	int GameStateID;

	void (*DrawScoreBar)(CGameBase* GameBase);
	void (*DrawSubStateText)(CGameBase* GameBase);
	void (*PauseMenu)(CGameBase* GameBase);
	bool (*UpdateLogic)(CGameBase* GameBase);
};


CGameBase* Create_CGameBase(int aGameStateID, bool aUsesLevels);	
void Destroy_CGameBase(CGameBase* GameBase);
void CGameBase_PauseMenu(CGameBase* GameBase);
void CGameBase_DrawScoreBar(CGameBase* GameBase);
void CGameBase_DrawSubstateText(CGameBase* GameBase);
bool CGameBase_UpdateLogic(CGameBase* GameBase);

#endif