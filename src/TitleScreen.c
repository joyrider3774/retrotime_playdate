#include <pd_api.h>
#include <string.h>
#include "CGame.h"
#include "Common.h"
#include "TitleScreen.h"
#include "Vec2F.h"
#include "SDL_HelperTypes.h"
#include "pd_helperfuncs.h"

int SelectedMenu = 0;
int CurrentMainMenu = -1;
int SelMenu = 0;
int SelOptions = 0;

const int menutextsize = (int)(60.0f*yscale);
const int menuspacing = (int)(90.0f*yscale);
const int rcolor = 64;
const int rdcolor = 1;

void InitTitleScreen()
{
	CAudio_PlayMusic(MusMenu, -1);
	CGame_StartCrossFade(GameState, SGNone, 0, 0);
}

void TitleScreen()
{
	if(GameState == GSTitleScreenInit)
	{
		InitTitleScreen();
		GameState -= initDiff;
	}

	CGame_DrawTitleBackground();

	SDL_Point FramePos = {ScreenWidth / 2, ScreenHeight / 2};
	Vec2F FrameScale = {16.0f / 4 * xscale, 12.8f *yscale};
	CImage_DrawImageFuze(GFXFrameID, true, &FramePos, 0, &FrameScale, 255, 255, 255, 255);

	LCDColor color = kColorWhite;
	char *Text;
	switch (CurrentMainMenu)
	{
		case MMOptions:
		{
			int selectedmenu = 0;

			selectedmenu = OMOptionMenus[SelOptions].menu;
			pd->system->formatString(&Text, "Options");
			CFont_WriteText("Roboto-Regular", (int)(80.0f*yscale), Text, strlen(Text),(int)(525.0f*xscale), (int)(50.0f*yscale), 0, color);
			pd->system->realloc(Text, 0);
			int menu;

			for(int i = 0; i < OptionMenus; i++)
			{
				menu = OMOptionMenus[i].menu;
				if (menu == selectedmenu)
					color = kColorWhite;
				else
					color = (LCDColor)kColorGrey;

				switch(menu)
				{
					case OMSoundVol:
						pd->system->formatString(&Text, "%s%d%%", OMOptionMenus[menu].name,(int)(CAudio_GetVolumeSound()*100/128));
						CFont_WriteText("Roboto-Regular", menutextsize, Text, strlen(Text), (int)(300.0f*xscale), (int)(185.0f*yscale + i * menuspacing), 0, color);
						pd->system->realloc(Text, 0);
						break;
					case OMMusicVol:
						pd->system->formatString(&Text, "%s%d%%", OMOptionMenus[menu].name,(int)(CAudio_GetVolumeMusic()*100/128));
						CFont_WriteText("Roboto-Regular", menutextsize, Text, strlen(Text), (int)(300.0f*xscale),(int)(185.0f*yscale + i * menuspacing), 0, color);
						pd->system->realloc(Text, 0);
						break;
					default:
						CFont_WriteText("Roboto-Regular", menutextsize, OMOptionMenus[menu].name, strlen(OMOptionMenus[menu].name),(int) (300.0f*xscale),
							(int)(185.0f*yscale + i * menuspacing), 0, color);
						break;
				}
			}
			color = kColorWhite;
			pd->system->formatString(&Text, "Use dpad to switch between options. (A) to select and (B) for back");
			CFont_WriteText("Roboto-Regular", (int)(34.0f*yscale), Text, strlen(Text), (int)(90.0f*xscale), (int)(630.0f*yscale), 0, color);
			pd->system->realloc(Text, 0);

			if ((!CInput_PrevButtons.ButDown && CInput_Buttons.ButDown) ||
				(!CInput_PrevButtons.ButDown2 && CInput_Buttons.ButDown2) ||
				(!CInput_PrevButtons.ButDpadDown && CInput_Buttons.ButDpadDown))
			{
				CAudio_PlaySound(SfxSelect, 0);
				SelOptions += 1;
				if (SelOptions == OptionMenus)
					SelOptions = 0;
			}

			if ((!CInput_PrevButtons.ButUp && CInput_Buttons.ButUp) ||
				(!CInput_PrevButtons.ButUp2 && CInput_Buttons.ButUp2) ||
				(!CInput_PrevButtons.ButDpadUp && CInput_Buttons.ButDpadUp))
			{
				CAudio_PlaySound(SfxSelect, 0);

				SelOptions -= 1;
				if( SelOptions == -1)
					SelOptions = OptionMenus - 1;
			}

			if ((!CInput_PrevButtons.ButLeft && CInput_Buttons.ButLeft) ||
				(!CInput_PrevButtons.ButLeft2 && CInput_Buttons.ButLeft2) ||
				(!CInput_PrevButtons.ButDpadLeft && CInput_Buttons.ButDpadLeft))
			{
				CAudio_PlaySound(SfxSelect, 0);
				bool wasplaying;
				switch (selectedmenu)
				{
					case OMSoundVol:
						CAudio_DecVolumeSound();
						break;

					case OMMusicVol:
						wasplaying = CAudio_IsMusicPlaying();
						CAudio_DecVolumeMusic();
						if (!wasplaying)
							CAudio_PlayMusic(MusMenu, -1);
						break;

				}
			}

			if ((!CInput_PrevButtons.ButRight && CInput_Buttons.ButRight) ||
				(!CInput_PrevButtons.ButRight2 && CInput_Buttons.ButRight2) ||
				(!CInput_PrevButtons.ButDpadRight && CInput_Buttons.ButDpadRight))
			{
				CAudio_PlaySound(SfxSelect, 0);
				bool wasplaying;
				switch (selectedmenu)
				{
					case OMSoundVol:
						CAudio_IncVolumeSound();
						break;

					case OMMusicVol:
						wasplaying = CAudio_IsMusicPlaying();
						CAudio_IncVolumeMusic();
						if (!wasplaying)
							CAudio_PlayMusic(MusMenu, -1);
						break;
				}
			}


			if ((!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack) ||
				(!CInput_PrevButtons.ButB && CInput_Buttons.ButB))
			{
				CAudio_PlaySound(SfxBack, 0);
				if (titleAlphaEnabled)
					CGame_StartFade();
				CurrentMainMenu = -1;
				CGame_SaveSettings();
			}

			if ((!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart) ||
				(!CInput_PrevButtons.ButA && CInput_Buttons.ButA))
			{
				CAudio_PlaySound(SfxSelect, 0);
				bool wasplaying;
				switch(selectedmenu)
				{
					case OMBack:
						CurrentMainMenu = -1;
						if (titleAlphaEnabled)
							CGame_StartFade();
						CGame_SaveSettings();
						break;
					case OMResetHighScores:
						CGame_ResetHighScores();
						CGame_SaveHighScores();
						break;
					case OMSoundVol:
						CAudio_IncVolumeSound();
						break;
					case OMMusicVol:
						wasplaying = CAudio_IsMusicPlaying();
						CAudio_IncVolumeMusic();
						if(!wasplaying)
							CAudio_PlayMusic(MusMenu, -1);
						break;
				}
			}
			break;
		}


		case MMHelp:
		{
			pd->system->formatString(&Text,"Help");
			CFont_WriteText("Roboto-Regular", (int)(80.0f*yscale), Text, strlen(Text), (int)(560.0f*xscale), (int)(50.0f*yscale), 0, color);
			pd->system->realloc(Text, 0);

			pd->system->formatString(&Text, "Retro time is a collection of retro games, each playable in different\n"
				"game modes.\n"
				"\n"
				"You can control players in games with the dpad. Extra actions are\n"
				"available, using (A), depending on the game.\n"
				"\n"
				"During gameplay you can access the pause menu, by pressing\n"
				"(B) button. Pressing it again will resume gameplay.\n"
				"\n"
				"There are 3 game modes: Retro Carousel, Time Attack and\n"
				"Lives mode.");
			int tw = CFont_TextWidth("Roboto-Regular", (int)(38.0f*yscale), Text, strlen(Text));
			CFont_WriteText("Roboto-Regular", (int)(38.0f*yscale), Text, strlen(Text), (int)((ScreenWidth-tw)/2.0f),(int)(140.0f*yscale), 0, color);
			pd->system->realloc(Text, 0);

			if ((!CInput_PrevButtons.ButA && CInput_Buttons.ButA) ||
				(!CInput_PrevButtons.ButB && CInput_Buttons.ButB) ||
				(!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack) ||
				(!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart))
			{
				CAudio_PlaySound(SfxBack, 0);
				if (titleAlphaEnabled)
					CGame_StartFade();
				CurrentMainMenu = -1;
			}
			break;
		}
		case MMCredits:
		{
			pd->system->formatString(&Text, "Credits");
			CFont_WriteText("Roboto-Regular", (int)(80.0f*yscale), Text, strlen(Text), (int)(500.0f*xscale),(int)(50.0f*yscale), 0, color);

			pd->system->formatString(&Text,
			"Block Stacker code is heavily inspired on this video from one lone\n"
			"coder (javidx9) with my own adaptations: https://youtu.be/8OK8_tHeCIA\n"
	//		"\n"
			"Snakey code is based on an edureka article (by Wajiha Urooj) about making\n"
			"python snake: https://www.edureka.co/blog/snake-game-with-pygame\n"
	//		"\n"
			"Brick Breaker ball / block collision uses a detection seen in wireframe\n"
			"magazine nr 11: https://wireframe.raspberrypi.org\n"
	//		"\n"
			"Toady water and grass background tiles have been created by me,\n"
			"Willems Davy aka joyrider3774 feel free to use them. Music is\n"
			"created, payed and owned by me using Strofe: https://www.strofe.com\n"
	//		"\n"
			"All other assets (including sound) come from either payed or free\n"
			"asset packs. For a complete list check the github or itch.io page:\n"
			"https://github.com/joyrider3774/RetroTime or\n"
			"https://joyrider3774.itch.io/retrotime\n");

			int tw = CFont_TextWidth("Roboto-Regular", (int)(35.0f*yscale), Text, strlen(Text));
			CFont_WriteText("Roboto-Regular", (int)(35.0f*yscale), Text, strlen(Text), (int)((ScreenWidth - tw) / 2.0f), (int)(140.0f*yscale), 0, color);
			pd->system->realloc(Text, 0);

			if ((!CInput_PrevButtons.ButA && CInput_Buttons.ButA) ||
				(!CInput_PrevButtons.ButB && CInput_Buttons.ButB) ||
				(!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack) ||
				(!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart))
			{
				CAudio_PlaySound(SfxBack, 0);
				if (titleAlphaEnabled)
					CGame_StartFade();
				CurrentMainMenu = -1;
			}
			break;
		}
		case MMHighScores:
		{
			pd->system->formatString(&Text,"High Scores");
			CFont_WriteText("Roboto-Regular", (int)(80.0f*yscale), Text, strlen(Text), (int)(400.0f*xscale), (int)(50.0f*yscale), 0, color);
			pd->system->realloc(Text, 0);

			pd->system->formatString(&Text, "Retro Carousel Total highscore: %d", RetroCarouselHighScore);
			CFont_WriteText("Roboto-Regular", (int)(50.0f*yscale), Text, strlen(Text),(int)(150.0f*xscale),(int)(195.0f*yscale), 0, color);
			pd->system->realloc(Text, 0);

			CFont_WriteText("Roboto-Regular", (int)(50.0f*yscale), GSGames[Game].name, strlen(GSGames[Game].name),(int)(150.0f*xscale),(int)(290.0f*yscale), 0, color);

			for(int mode = 0; mode < Modes; mode++)
			{
				pd->system->formatString(&Text, "%s: %d", GMModes[mode].name, HighScores[Game][mode]);
				CFont_WriteText("Roboto-Regular", (int)(50.0f*yscale), Text, strlen(Text), (int)(150.0f*xscale), (int)((350 + mode * 46)*yscale), 0, color);
				pd->system->realloc(Text, 0);
			}

			pd->system->formatString(&Text, "Use dpad or (A) to switch between games. (B) for back");
			CFont_WriteText("Roboto-Regular", (int)(34.0f*yscale), Text, strlen(Text), (int)(200.0f*xscale),(int)(630.0f*yscale), 0, color);
			pd->system->realloc(Text, 0);

			 if ((!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack) ||
				(!CInput_PrevButtons.ButB && CInput_Buttons.ButB))
			 {
				CAudio_PlaySound(SfxBack, 0);
				if (titleAlphaEnabled)
					CGame_StartFade();
				CurrentMainMenu = -1;
			 }

			if ((!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart) ||
				(!CInput_PrevButtons.ButRight && CInput_Buttons.ButRight) ||
				(!CInput_PrevButtons.ButRight2 && CInput_Buttons.ButRight2) ||
				(!CInput_PrevButtons.ButDpadRight && CInput_Buttons.ButDpadRight) ||
				(!CInput_PrevButtons.ButA && CInput_Buttons.ButA))
			{
				CAudio_PlaySound(SfxSelect, 0);

				Game += 1;
				if (Game == Games)
					Game = 0;
			}

			 if ((!CInput_PrevButtons.ButLeft && CInput_Buttons.ButLeft) ||
				 (!CInput_PrevButtons.ButLeft2 && CInput_Buttons.ButLeft2) ||
				 (!CInput_PrevButtons.ButDpadLeft && CInput_Buttons.ButDpadLeft))
			 {
				CAudio_PlaySound(SfxSelect, 0);

				Game -= 1;
				if (Game == -1)
					Game = Games - 1;
			 }
			 break;
		}
		case MMSelectGame:
		{
			int gamestate = GSGames[Game].state;
			pd->system->formatString(&Text, "Select Game");
			CFont_WriteText("Roboto-Regular", (int)(80.0f*yscale), Text, strlen(Text), (int)(465.0f*xscale), (int)(50.0f*yscale), 0, color);
			pd->system->realloc(Text, 0);

			pd->system->formatString(&Text, GSGames[Game].name);
			CFont_WriteText("Roboto-Regular", (int)(50.0f*yscale), Text, strlen(Text), (int)(50.0f*xscale), (int)(120.0f*yscale), 0, color);
			pd->system->realloc(Text, 0);

			pd->system->formatString(&Text, "%s High Score: %d", GMModes[GameMode].name, HighScores[Game][GameMode]);
			CFont_WriteText("Roboto-Regular", (int)(40.0f*yscale), Text, strlen(Text), (int)(50.0f*xscale), (int)(170.0f*yscale), 0, color);
			pd->system->realloc(Text, 0);

			CFont_WriteText("Roboto-Regular", (int)(38.0f*yscale), GSGames[Game].description, strlen(GSGames[Game].description), (int)(50.0f*xscale), (int)(230.0f*yscale), 2, color);

			pd->system->formatString(&Text, "Use dpad to switch between games. (A) to select and (B) for back");
			CFont_WriteText("Roboto-Regular", (int)(34.0f*yscale), Text, strlen(Text), (int)(90.0f*xscale), (int)(630.0f*yscale), 0, color);
			pd->system->realloc(Text, 0);

			if ((!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack) ||
				(!CInput_PrevButtons.ButB && CInput_Buttons.ButB))
			{
				CAudio_PlaySound(SfxBack, 0);
				CurrentMainMenu = MMSelectGameMode;
				if (titleAlphaEnabled)
					CGame_StartFade();
			}

			if ((!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart) ||
				(!CInput_PrevButtons.ButA && CInput_Buttons.ButA))
			{
				CAudio_PlaySound(SfxConfirm, 0);
				if (titleAlphaEnabled)
					CGame_StartFade();
				GameState = gamestate;
				CGame_ResetScores();
				CurrentMainMenu = -1;
				CInput_ResetButtons();
			}

			if ((!CInput_PrevButtons.ButLeft && CInput_Buttons.ButLeft) ||
				(!CInput_PrevButtons.ButLeft2 && CInput_Buttons.ButLeft2) ||
				(!CInput_PrevButtons.ButDpadLeft && CInput_Buttons.ButDpadLeft))
			{
				CAudio_PlaySound(SfxSelect, 0);

				Game -= 1;
				if (Game == -1)
					Game = Games - 1;
			}
			if ((!CInput_PrevButtons.ButRight && CInput_Buttons.ButRight) ||
				(!CInput_PrevButtons.ButRight2 && CInput_Buttons.ButRight2) ||
				(!CInput_PrevButtons.ButDpadRight && CInput_Buttons.ButDpadRight))
			{
				CAudio_PlaySound(SfxSelect, 0);

				Game += 1;
				if (Game == Games)
					Game = 0;
			}
			break;
		}
		case MMSelectGameMode:
		{
			pd->system->formatString(&Text, "Select Game Mode");
			CFont_WriteText("Roboto-Regular", (int)(80.0f*yscale), Text, strlen(Text), (int)(285.0f*xscale), (int)(50.0f*yscale), 0, color);
			pd->system->realloc(Text, 0);

			int ModeIterator;
			for(int i = 0; i < Modes; i++)
			{
				ModeIterator = GMModes[i].mode;
				if (ModeIterator == GameMode)
					color = kColorWhite;
				else
					color = (LCDColor)kColorGrey;

				CFont_WriteText("Roboto-Regular", menutextsize, GMModes[i].name, strlen(GMModes[i].name), (int)(470.0f*xscale), (int)(135.0f*yscale + i * menuspacing), 0, color);
			}
			color = kColorWhite;

			CFont_WriteText("Roboto-Regular", (int)(40.0f*yscale), GMModes[GameMode].description, strlen(GMModes[GameMode].description),
				(int)(90.0f*xscale),(int)(400.0f*yscale), 0, color);

			pd->system->formatString(&Text, "Use dpad to switch between game modes. (A) to select and (B) for back");
			CFont_WriteText("Roboto-Regular", (int)(34.0f*yscale), Text, strlen(Text), (int)(90.0f*xscale), (int)(630.0f*yscale), 0, color);
			pd->system->realloc(Text, 0);

			if ((!CInput_PrevButtons.ButDown && CInput_Buttons.ButDown) ||
				(!CInput_PrevButtons.ButDown2 && CInput_Buttons.ButDown2) ||
				(!CInput_PrevButtons.ButDpadDown && CInput_Buttons.ButDpadDown))
			{
				CAudio_PlaySound(SfxSelect, 0);
				GameMode += 1;
				if (GameMode == Modes)
					GameMode = 0;
			}

			if ((!CInput_PrevButtons.ButUp && CInput_Buttons.ButUp) ||
				(!CInput_PrevButtons.ButUp2 && CInput_Buttons.ButUp2) ||
				(!CInput_PrevButtons.ButDpadUp && CInput_Buttons.ButDpadUp))
			{
				CAudio_PlaySound(SfxSelect, 0);

				GameMode -= 1;
				if (GameMode == -1)
					GameMode = Modes - 1;
			}

			if ((!CInput_PrevButtons.ButB && CInput_Buttons.ButB) ||
				(!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack))
			{
				CAudio_PlaySound(SfxBack, 0);
				CurrentMainMenu = -1;
				if (titleAlphaEnabled)
					CGame_StartFade();
			}

			if ((!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart) ||
				(!CInput_PrevButtons.ButA && CInput_Buttons.ButA))
			{
				CAudio_PlaySound(SfxConfirm, 0);

				if (GameMode == GMRetroCarousel)
				{
					Game = 0;
					CGame_ResetScores();
					CurrentMainMenu = -1;
					GameState = GSGames[Game].state;
				}
				else
					CurrentMainMenu = MMSelectGame;
				if (titleAlphaEnabled)
					CGame_StartFade();
			}
			break;
		}
		default:
		{
			SelMenu = MMMainMenus[SelectedMenu].menu;

			pd->system->formatString(&Text, "Retro Time");
			CFont_WriteText("Roboto-Regular", (int)(80.0f*yscale), Text, strlen(Text), (int)(465.0f*xscale), (int)(50.0f*yscale), 0, color);
			pd->system->realloc(Text, 0);

			int MenuIterator;
			for (int i = 0; i < MainMenus; i++)
			{
				MenuIterator = MMMainMenus[i].menu;
				if (MenuIterator == SelMenu)
					color = kColorWhite;
				else
					color = (LCDColor)kColorGrey;
				CFont_WriteText("Roboto-Regular", menutextsize, MMMainMenus[i].name, strlen(MMMainMenus[i].name), (int)(500.0f*xscale), (int)(180.0f*yscale + i * menuspacing), 0, color);
			}
			color = kColorWhite;

			if ((!CInput_PrevButtons.ButDown && CInput_Buttons.ButDown) ||
				(!CInput_PrevButtons.ButDown2 && CInput_Buttons.ButDown2) ||
				(!CInput_PrevButtons.ButDpadDown && CInput_Buttons.ButDpadDown))
			{
				CAudio_PlaySound(SfxSelect, 0);
				SelectedMenu += 1;
				if (SelectedMenu == MainMenus)
					SelectedMenu = 0;
			}

			if ((!CInput_PrevButtons.ButUp && CInput_Buttons.ButUp) ||
				(!CInput_PrevButtons.ButUp2 && CInput_Buttons.ButUp2) ||
				(!CInput_PrevButtons.ButDpadUp && CInput_Buttons.ButDpadUp))

			{
				CAudio_PlaySound(SfxSelect, 0);

				SelectedMenu -= 1;
				if(SelectedMenu == -1)
					SelectedMenu = MainMenus - 1;
			}

			if ((!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart) ||
				(!CInput_PrevButtons.ButA && CInput_Buttons.ButA))
			{
				if (titleAlphaEnabled)
					CGame_StartFade();
				CurrentMainMenu = SelMenu;
				CAudio_PlaySound(SfxConfirm, 0);

				if (SelMenu == MMStart)
					CurrentMainMenu = MMSelectGameMode;
			}
		}
	}
}
