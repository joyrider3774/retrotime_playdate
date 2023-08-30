#include <pd_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "CGameBase.h"
#include "../Common.h"
#include "../CGame.h"
#include "../Vec2F.h"
#include "../pd_helperfuncs.h"
#include "../SDL_HelperTypes.h"

int CGameBase_prevsubgamestate = -1;
float CGameBase_prevsubstatecounter = 0;
int CGameBase_selected = 0;
int CGameBase_selectedmenu = 0;
int CGameBase_maxmenus = 5;
int CGameBase_menutextsize = (int)(60.0f * yscale);
int CGameBase_menuspacing = (int)(85.0f * yscale);
bool CGameBase_readygoplayed = false;

CGameBase* Create_CGameBase(int aGameStateID, bool aUsesLevels)
{
	CGameBase* result = (CGameBase*) malloc(sizeof(CGameBase));
	
	result->UsesLevels = aUsesLevels;
	result->GameStateID = aGameStateID;
	result->level = 0;
	result->HealthPoints = 0;
	result->SubStateText[0] = '\0';
	result->playfieldheight = 0;
	result->playfieldwidth = 0;
	result->screenbottom = 0;
	result->screentop = 0;
	result->screenleft = 0;
	result->screenright = 0;
	result->DrawScoreBar = CGameBase_DrawScoreBar;
	result->PauseMenu = CGameBase_PauseMenu;
	result->DrawSubStateText = CGameBase_DrawSubstateText;
	result->UpdateLogic = CGameBase_UpdateLogic;
	return result;
}

void Destroy_CGameBase(CGameBase* GameBase)
{
	free(GameBase);
}


void CGameBase_PauseMenu(CGameBase* GameBase)
{
	if ((CGameBase_prevsubgamestate > -1) && ((SubGameState == SGFrame) || (SubGameState == SGPauseMenu) || (SubGameState == SGGameHelp)))
	{
		CGameBase_selectedmenu = GPGamePauseMenus[Game].menus[CGameBase_selected];

		SDL_Point FramePos = { ScreenWidth / 2, ScreenHeight / 2 };
		Vec2F FrameScale = { 16.0f / 4 * xscale, 12.8f * yscale };
		CImage_DrawImageFuze(GFXFrameID, true, &FramePos, 0, &FrameScale, 255, 255, 255, 255);
		LCDColor color = kColorWhite;
		char* Text;
		if (SubGameState == SGPauseMenu)
		{

			pd->system->formatString(&Text, "Paused");
			CFont_WriteText("Roboto-Regular", (int)(80.0f * yscale), Text, strlen(Text), (int)(510.0f * xscale), (int)(50.0f * yscale), 0, color);
			pd->system->realloc(Text, 0);
			int menu;

			for (int i = 0; i < CGameBase_maxmenus; i++)
			{
				menu = GPGamePauseMenus[Game].menus[i];
				if (menu == CGameBase_selectedmenu)
					color = kColorWhite;
				else
					color = (LCDColor) kColorGrey;

				switch (menu)
				{
					case PMSoundVol:
						pd->system->formatString(&Text, "%s %d %%", PMPauseMenus[menu].name, (int)(CAudio_GetVolumeSound() * 100.0f / 128.0f));
						CFont_WriteText("Roboto-Regular", CGameBase_menutextsize, Text, strlen(Text), (int)(300.0f * xscale), (int)(185.0f * yscale + i * CGameBase_menuspacing), 0, color);
						pd->system->realloc(Text, 0);
						break;
					case PMMusicVol:
						pd->system->formatString(&Text, "%s %d %%", PMPauseMenus[menu].name, (int)(CAudio_GetVolumeMusic() * 100.0f / 128.0f));
						CFont_WriteText("Roboto-Regular", CGameBase_menutextsize, Text, strlen(Text), (int)(300.0f * xscale),(int)(185.0f * yscale + i * CGameBase_menuspacing), 0, color);
						pd->system->realloc(Text, 0);
						break;
					default:
						CFont_WriteText("Roboto-Regular", CGameBase_menutextsize, PMPauseMenus[menu].name, strlen(PMPauseMenus[menu].name),(int)(300.0f * xscale),
							(int)(185.0f * yscale + i * CGameBase_menuspacing), 0, color);
						break;
				}
			}

			color = kColorWhite;
			pd->system->formatString(&Text, "Use dpad to switch between options. (A) to select and (B) for back");
			CFont_WriteText("Roboto-Regular", (int)(34.0f * yscale), Text, strlen(Text), (int)(90.0f * xscale), (int)(630.0f * yscale), 0, color);
			pd->system->realloc(Text, 0);
		}

		if (SubGameState == SGGameHelp)
		{
			pd->system->formatString(&Text, "Game Help");
			CFont_WriteText("Roboto-Regular", (int)(80.0f * yscale), Text, strlen(Text), (int)(485.0f * xscale), (int)(50.0f * yscale), 0, color);
			pd->system->realloc(Text, 0);

			pd->system->formatString(&Text, "%s", GSGames[Game].name);
			CFont_WriteText("Roboto-Regular", (int)(50.0f * yscale), Text, strlen(Text),(int)(75.0f * xscale), (int)(150.0f * yscale), 0, color);
			pd->system->realloc(Text, 0);

			pd->system->formatString(&Text, "%s %s High Score: %d", GSGames[Game].name, GMModes[GameMode].name, HighScores[Game][GameMode]);
			CFont_WriteText("Roboto-Regular", (int)(38.0f * yscale), Text, strlen(Text), (int)(75.0f * xscale), (int)(210.0f * yscale), 0, color);
			pd->system->realloc(Text, 0);

			pd->system->formatString(&Text, "%s", GSGames[Game].description);
			CFont_WriteText("Roboto-Regular", (int)(38.0f * yscale), Text, strlen(Text), (int)(75.0f * xscale), (int)(255.0f * yscale), 0, color);
			pd->system->realloc(Text, 0);

			pd->system->formatString(&Text, "Press (A) or (B) for back");
			CFont_WriteText("Roboto-Regular", (int)(34.0f * yscale), Text, strlen(Text), (int)(485.0f * xscale),(int)(630.0f * yscale), 0, color);
			pd->system->realloc(Text, 0);
		}

		if (SubGameState == SGGameHelp)
		{
			if ((CInput_Buttons.ButB && !CInput_PrevButtons.ButB) ||
				(CInput_Buttons.ButA && !CInput_PrevButtons.ButA) ||
				(!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack) ||
				(!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart))
			{
				CAudio_PlaySound(SfxBack, 0);
				SubGameState = SGPauseMenu;
				CGame_StartFade();
				CInput_ResetButtons();
			}
		}

		if (SubGameState == SGPauseMenu)
		{
			if ((!CInput_PrevButtons.ButLeft && CInput_Buttons.ButLeft) ||
				(!CInput_PrevButtons.ButLeft2 && CInput_Buttons.ButLeft2) ||
				(!CInput_PrevButtons.ButDpadLeft && CInput_Buttons.ButDpadLeft))
			{
				CAudio_PlaySound(SfxSelect, 0);

				switch (CGameBase_selectedmenu)
				{
					case PMSoundVol:
					{
						CAudio_DecVolumeSound();
						break;
					}

					case PMMusicVol:
					{
						bool wasplaying = CAudio_IsMusicPlaying();
						CAudio_DecVolumeMusic();
						if (!wasplaying)
							CAudio_PlayMusic(CurrentGameMusicID, -1);
						break;
					}

				}
			}

			if ((!CInput_PrevButtons.ButRight && CInput_Buttons.ButRight) ||
				(!CInput_PrevButtons.ButRight2 && CInput_Buttons.ButRight2) ||
				(!CInput_PrevButtons.ButDpadRight && CInput_Buttons.ButDpadRight))
			{
				CAudio_PlaySound(SfxSelect, 0);
				switch (CGameBase_selectedmenu)
				{
					case PMSoundVol:
					{
						CAudio_IncVolumeSound();
						break;
					}

					case PMMusicVol:
					{
						bool wasplaying = CAudio_IsMusicPlaying();
						CAudio_IncVolumeMusic();
						if (!wasplaying)
							CAudio_PlayMusic(CurrentGameMusicID, -1);
						break;
					}
				}
			}

			if ((!CInput_PrevButtons.ButDown && CInput_Buttons.ButDown) ||
				(!CInput_PrevButtons.ButDown2 && CInput_Buttons.ButDown2) ||
				(!CInput_PrevButtons.ButDpadDown && CInput_Buttons.ButDpadDown))
			{
				CAudio_PlaySound(SfxSelect, 0);

				CGameBase_selected += 1;
				if (CGameBase_selected == CGameBase_maxmenus)
					CGameBase_selected = 0;
			}

			if ((!CInput_PrevButtons.ButUp && CInput_Buttons.ButUp) ||
				(!CInput_PrevButtons.ButUp2 && CInput_Buttons.ButUp2) ||
				(!CInput_PrevButtons.ButDpadUp && CInput_Buttons.ButDpadUp))
			{
				CAudio_PlaySound(SfxSelect, 0);

				CGameBase_selected -= 1;
				if (CGameBase_selected == -1)
					CGameBase_selected = CGameBase_maxmenus - 1;
			}

			if (!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack)
			{
				CAudio_PlaySound(SfxConfirm, 0);
				CInput_ResetButtons();
				SubGameState = CGameBase_prevsubgamestate;
				SubStateCounter = CGameBase_prevsubstatecounter;
				CGameBase_prevsubgamestate = -1;
				CGame_StartFade();
			}

			if ((!CInput_PrevButtons.ButA && CInput_Buttons.ButA) ||
				(!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart))
			{
				CAudio_PlaySound(SfxConfirm, 0);

				switch (CGameBase_selectedmenu)
				{
				case PMContinue:
				{
					SubGameState = CGameBase_prevsubgamestate;
					SubStateCounter = CGameBase_prevsubstatecounter;
					CGameBase_prevsubgamestate = -1;
					CInput_ResetButtons();
					CGame_StartFade();
					break;
				}

				case PMQuit:
				{
					CGameBase_prevsubgamestate = -1;
					CGame_StartCrossFade(GSTitleScreenInit, SGNone, 3, 500);
					break;
				}

				case PMSoundVol:
				{
					CAudio_IncVolumeSound();
					break;
				}

				case PMMusicVol:
				{
					bool wasplaying = CAudio_IsMusicPlaying();
					CAudio_IncVolumeMusic();
					if (!wasplaying)
						CAudio_PlayMusic(CurrentGameMusicID, -1);
					break;
				}

				case PMGameHelp:
				{
					SubGameState = SGGameHelp;
					CInput_ResetButtons();
					CGame_StartFade();
					break;
				}
				}

			}
		}
	}
}

void CGameBase_DrawScoreBar(CGameBase* GameBase)
{
	char Text[1000];
	char *TmpText;
	Text[0] = '\0';
	pd->graphics->fillRect(0, 0, ScreenWidth, ScoreBarHeight, kColorBlack);

	if(GameBase->UsesLevels)
	{
		pd->system->formatString(&TmpText,"Lev:%d ",GameBase->level);
		strncat(Text, TmpText, 999);
		pd->system->realloc(TmpText, 0);
	}

	if (GameMode == GMGame)
	{

		pd->system->formatString(&TmpText,"Lives:%d Score:%d High:%d ",  GameBase->HealthPoints,
			Scores[Game][GameMode], HighScores[Game][GameMode]);
		strncat(Text, TmpText, 999);
		pd->system->realloc(TmpText, 0);
	}
	else
	{
		if(GameMode == GMRetroCarousel)
		{
			pd->system->formatString(&TmpText,"Tmr:%.0f Tot Score:%d Score:%d Prev Tot high:%d Prev High:%d",
				Timer, RetroCarouselScore, Scores[Game][GameMode], RetroCarouselHighScore,
				HighScores[Game][GameMode]);
			strncat(Text, TmpText, 999);
			pd->system->realloc(TmpText, 0);
		}
		else
		{
			pd->system->formatString(&TmpText, "Tmr:%.0f Score:%d Prev High:%d ",
				Timer,Scores[Game][GameMode], HighScores[Game][GameMode]);
			strncat(Text, TmpText, 999);
			pd->system->realloc(TmpText, 0);
		}
			
	}
	CFont_WriteText("Roboto-Regular", 11, Text, strlen(Text), 0, 0, 0, kColorWhite);
}

void CGameBase_DrawSubstateText(CGameBase* GameBase)
{
	int w = CFont_TextWidth("Roboto-Regular", 23, GameBase->SubStateText, strlen(GameBase->SubStateText));
	CFont_WriteText("Roboto-Regular", 23, GameBase->SubStateText, strlen(GameBase->SubStateText), (int)(-1.0f + GameBase->screenleft + ((GameBase->screenright - GameBase->screenleft) / 2.0f) - w / 2.0f),
		(int)(-1.0f + GameBase->screentop + ((GameBase->screenbottom - GameBase->screentop) / 2.0f) - 110.0f * xscale), 0, (LCDColor)kColorBlack);
	CFont_WriteText("Roboto-Regular", 23, GameBase->SubStateText, strlen(GameBase->SubStateText), (int)(-1.0f + GameBase->screenleft + ((GameBase->screenright - GameBase->screenleft) / 2.0f) - w / 2.0f),
		(int)(1.0f + GameBase->screentop + ((GameBase->screenbottom - GameBase->screentop) / 2.0f) - 110.0f * xscale), 0, (LCDColor)kColorBlack);
	CFont_WriteText("Roboto-Regular", 23, GameBase->SubStateText, strlen(GameBase->SubStateText), (int)(1.0f + GameBase->screenleft + ((GameBase->screenright - GameBase->screenleft) / 2.0f) - w / 2.0f),
		(int)(-1.0f + GameBase->screentop + ((GameBase->screenbottom - GameBase->screentop) / 2.0f) - 110.0f * xscale), 0, (LCDColor)kColorBlack);
	CFont_WriteText("Roboto-Regular", 23, GameBase->SubStateText, strlen(GameBase->SubStateText), (int)(1.0f + GameBase->screenleft + ((GameBase->screenright - GameBase->screenleft) / 2.0f) - w / 2.0f),
		(int)(1.0f + GameBase->screentop + ((GameBase->screenbottom - GameBase->screentop) / 2.0f) - 110.0f * xscale), 0, (LCDColor)kColorBlack);
	CFont_WriteText("Roboto-Regular", 23, GameBase->SubStateText, strlen(GameBase->SubStateText), (int)(GameBase->screenleft + ((GameBase->screenright - GameBase->screenleft) / 2.0f) - w / 2.0f),
		(int)(GameBase->screentop + ((GameBase->screenbottom - GameBase->screentop) / 2.0f) - 110.0f * xscale), 0, (LCDColor)kColorWhite);
}

bool CGameBase_UpdateLogic(CGameBase* GameBase)
{
	bool result = false;
	//check for pause menu only when in game substate
	if ((CGameBase_prevsubgamestate == -1) && (SubGameState == SGGame) && CInput_Buttons.ButBack && !CInput_PrevButtons.ButBack)
	{
		CAudio_PlaySound(SfxConfirm, 0);
		//subgamestate = sgframe;
		//global.substatecounter = 10.6
		CGameBase_selected = 0;
		CGameBase_selectedmenu = 0;
		CInput_ResetButtons();
		CGameBase_prevsubgamestate = SubGameState;
		CGameBase_prevsubstatecounter = SubStateCounter;
		SubGameState = SGPauseMenu;
		CGame_StartFade();
	}

	GameBase->PauseMenu(GameBase);
	
	//we are in the pause menu
	if (CGameBase_prevsubgamestate > -1)
		return result;

	if (GameMode == GMGame)
	{
		if (SubGameState == SGGame)
		{
			if(GameBase->HealthPoints == 0)
			{
				SubGameState = SGTimeUp;
				SubStateTime = pd->system->getCurrentTimeMilliseconds() + 750;
				SubStateCounter = 0;
			}
		}
	}

	GameBase->SubStateText[0] = '\0';
	if (SubGameState != SGFadeIn)
	{
		if ((SubGameState == SGReadyGo)||
			(SubGameState == SGTimeUp))
		{
			if (SubStateTime > pd->system->getCurrentTimeMilliseconds())
			{
				if (SubStateCounter >= 0)
				{
					char *Tmp;
					pd->system->formatString(&Tmp,"%d",(int)SubStateCounter);
					strcpy(GameBase->SubStateText, Tmp);
					pd->system->realloc(Tmp, 0);
					if(SubStateCounter == 2)
					{
						if (!CGameBase_readygoplayed)
						{
							CAudio_PlaySound(SfxReadyGo, 0);
							CGameBase_readygoplayed = true;
						}
					}

					if (SubStateCounter == 0)
					{
						CGameBase_readygoplayed = false;
						if (SubGameState == SGReadyGo)
						{
							strcpy(GameBase->SubStateText, "GO");
						}
						else
						{
							if (GameMode != GMGame)
							{
								strcpy(GameBase->SubStateText, "Time Up");
							}
							else
								strcpy(GameBase->SubStateText, "Game Over");

						}
					}
				}
			}
			else
			{
				if (SubStateCounter > 0)
				{
					if (SubStateCounter > 1)
						SubStateTime = pd->system->getCurrentTimeMilliseconds() + 500;
					else
						SubStateTime = pd->system->getCurrentTimeMilliseconds() + 250;
					SubStateCounter -= 1;
				}
				else
				{
					if (SubGameState == SGReadyGo)
					{
						result = true;//OnGameStart();
						SubGameState = SGGame;
					}
					else
					{
						GameState = GSSubScoreInit;
					}
				}
			}
		}
	}
	return result;
}