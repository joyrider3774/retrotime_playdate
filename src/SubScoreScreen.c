#include <pd_api.h>
#include <string.h>
#include <stdbool.h>
#include "CGame.h"
#include "games/CGameBase.h"
#include "Common.h"
#include "SubScoreScreen.h"
#include "Vec2F.h"
#include "games/CGameBlockStacker.h"
#include "games/CGameFastEddy.h"
#include "games/CGameFrog.h"
#include "games/CGameBreakOut.h"
#include "games/CGameInvaders.h"
#include "games/CGamePang.h"
#include "games/CGameRamIt.h"
#include "games/CGameSnake.h"
#include "pd_helperfuncs.h"
#include "SDL_HelperTypes.h"
#include "scoresubmit/scoresubmit.h"
#include "codekey.h"

bool wasnewhighscorecarousel;
bool wasnewhighscoregame;
bool submithighscore = false;
int prevretrocarouselscore;
char* verifyCode;
LCDBitmap* QRCode = NULL;
int view = 0;

void DeInitSubScoreScreen()
{
	if(QRCode)
	{
		pd->graphics->freeBitmap(QRCode);
		QRCode = NULL;
	}
}

void InitSubScoreScreen()
{
	SubGameState = SGFrame;
	SubStateCounter = 0;
	view = 0;
	if (Game == 0)
		submithighscore = false;
	wasnewhighscorecarousel = false;
	wasnewhighscoregame = false;
	prevretrocarouselscore = RetroCarouselScore;
	if (GameMode == GMRetroCarousel)
		RetroCarouselScore += Scores[Game][GameMode];

	if (RetroCarouselScore > RetroCarouselHighScore)
	{
		RetroCarouselHighScore = RetroCarouselScore;
		wasnewhighscorecarousel = true;
		submithighscore = true;
	}

	if (Scores[Game][GameMode] > HighScores[Game][GameMode])
	{
		HighScores[Game][GameMode] = Scores[Game][GameMode];
		wasnewhighscoregame = true;
	}

	//savehighscoresoptions()
	CAudio_PlaySound(SfxScore, 0);
	if ((GameMode == GMRetroCarousel) && (Game == Games-1) && submithighscore)
	{
		if (QRCode)
			pd->graphics->freeBitmap(QRCode);
		if (verifyCode)
			pd->system->realloc(verifyCode, 0);
		volatile char codekey[64] = "SECRET01SECRET01SECRET01SECRET01SECRET01SECRET01SECRET01SECRET01";
#ifdef RETROTIME_PLAYDATE_CODEKEY
		RETROTIME_PLAYDATE_CODEKEY
#endif 
			QRCode = CreateQrCodeSubmit(pd, (char*)codekey, 42, RetroCarouselHighScore, 210, &verifyCode);
		pd->system->logToConsole("score: %u verifier:%s", RetroCarouselHighScore, verifyCode);
	}
}

void SubScoreScreen()
{
	if (GameState == GSSubScoreInit)
	{
		InitSubScoreScreen();
		GameState -= initDiff;
	}
	//pd->graphics->pushContext(TexOffScreen);
	switch (ActiveGameGameStateId)
	{
	case GSSnake:
		GameSnake->Draw(GameSnake);
		break;
	case GSRamIt:
		GameRamIt->Draw(GameRamIt);
		break;
	case GSPang:
		GamePang->Draw(GamePang);
		break;
	case GSSpaceInvaders:
		GameInvaders->Draw(GameInvaders);
		break;
	case GSEddy:
		GameFastEddy->Draw(GameFastEddy);
		break;
	case GSBreakout:
		GameBreakOut->Draw(GameBreakOut);
		break;
	case GSFrog:
		GameFrog->Draw(GameFrog);
		break;
	case GSTetris:
		GameBlockStacker->Draw(GameBlockStacker);
		break;
	}

	if (SubGameState == SGFrame)
	{
		if (SubStateCounter < 16.0f)
			SubStateCounter += 0.4f;
		else
			SubGameState = SGCalcScore;
	}

	//so we can can copy the transparant part with the blue and text from this image
	SDL_Point pos = { ScreenWidth / 2, ScreenHeight / 2 };
	Vec2F Scale = { SubStateCounter / 4 * xscale, (0.0f > SubStateCounter - 3.2f ? 0.0f : SubStateCounter - 3.2f) * yscale };
	CImage_DrawImageFuze(GFXFrameID, true, &pos, 0, &Scale, 255, 255, 255, 240);
	char* TextTmp;

	SDL_Point MedalTz = CImage_ImageSize(GFXMedal);
	Vec2F MedalScale = { 50.0f / MedalTz.y * xscale, 50.0f / MedalTz.y * yscale };
	if (SubGameState == SGCalcScore)
	{
		if (view == 1)
		{
			if (QRCode)
			{
				char* msg;
				pd->system->formatString(&msg, "SUBMIT YOUR HI-SCORE ONLINE: %d", RetroCarouselHighScore);
				int tw = CFont_TextWidth("Roboto-Regular", (int)(50.0f * yscale), msg, strlen(msg));
				CFont_WriteText("Roboto-Regular", (int)(50.0f * yscale), msg, strlen(msg), (ScreenWidth - tw) >> 1, 18, 0, kColorWhite);
				pd->system->realloc(msg, 0);
				if (QRCode)
				{
					int w, h;
					pd->graphics->getBitmapData(QRCode, &w, &h, NULL, NULL, NULL);
					pd->graphics->fillRect(((ScreenWidth - w) >> 1) - 1, 42, w + 2, h + 2, kColorWhite);
					//-80 from drawoffset set
					pd->graphics->drawBitmap(QRCode, (ScreenWidth - w) >> 1, 43, kBitmapUnflipped);
				}
			}
		}
		else
		{
			if ((Game == Games - 1) && (GameMode == GMRetroCarousel))
			{
				pd->system->formatString(&TextTmp, "Final Results");
				int Texw = CFont_TextWidth("Roboto-Regular", (int)(80.0f * yscale), TextTmp, strlen(TextTmp));
				CFont_WriteText("Roboto-Regular", (int)(80.0f * yscale), TextTmp, strlen(TextTmp), (int)((ScreenWidth - Texw) / 2.0f), (int)(50.0f * yscale), 0, kColorWhite);
				pd->system->realloc(TextTmp, 0);

				pd->system->formatString(&TextTmp, "-------------------------");
				Texw = CFont_TextWidth("Roboto-Regular", (int)(80.0f * yscale), TextTmp, strlen(TextTmp));
				CFont_WriteText("Roboto-Regular", (int)(80.0f * yscale), TextTmp, strlen(TextTmp), (int)((ScreenWidth - Texw) / 2.0f), (int)(85.0f * yscale), 0, kColorWhite);
				pd->system->realloc(TextTmp, 0);
			}
			else
			{
				pd->system->formatString(&TextTmp, "Results");
				int Texw = CFont_TextWidth("Roboto-Regular", (int)(80.0f * yscale), TextTmp, strlen(TextTmp));
				CFont_WriteText("Roboto-Regular", (int)(80.0f * yscale), TextTmp, strlen(TextTmp), (int)((ScreenWidth - Texw) / 2.0f), (int)(50.0f * yscale), 0, kColorWhite);
				pd->system->realloc(TextTmp, 0);

				pd->system->formatString(&TextTmp, "----------------");
				Texw = CFont_TextWidth("Roboto-Regular", (int)(80.0f * yscale), TextTmp, strlen(TextTmp));
				CFont_WriteText("Roboto-Regular", (int)(80.0f * yscale), TextTmp, strlen(TextTmp), (int)((ScreenWidth - Texw) / 2.0f), (int)(85.0f * yscale), 0, kColorWhite);
				pd->system->realloc(TextTmp, 0);
			}


			pd->system->formatString(&TextTmp, "Game: %s", GSGames[Game].name);
			CFont_WriteText("Roboto-Regular", (int)(50.0f * yscale), TextTmp, strlen(TextTmp), (int)(150.0f * xscale), (int)(180.0f * yscale), 0, kColorWhite);
			pd->system->realloc(TextTmp, 0);
			pd->system->formatString(&TextTmp, "Game Mode: %s", GMModes[GameMode].name);
			CFont_WriteText("Roboto-Regular", (int)(50.0f * yscale), TextTmp, strlen(TextTmp), (int)(150.0f * xscale), (int)(225.0f * yscale), 0, kColorWhite);
			pd->system->realloc(TextTmp, 0);
			if (GameMode == GMRetroCarousel)
			{
				pd->system->formatString(&TextTmp, "Previous Rounds Score: %d", prevretrocarouselscore);
				CFont_WriteText("Roboto-Regular", (int)(50.0f * yscale), TextTmp, strlen(TextTmp), (int)(150.0f * xscale), (int)(285.0f * yscale), 0, kColorWhite);
				pd->system->realloc(TextTmp, 0);

				pd->system->formatString(&TextTmp, "Game Score: %d", Scores[Game][GameMode]);
				CFont_WriteText("Roboto-Regular", (int)(50.0f * yscale), TextTmp, strlen(TextTmp), (int)(150.0f * xscale), (int)(330.0f * yscale), 0, kColorWhite);
				pd->system->realloc(TextTmp, 0);

				if (Game < Games - 1)
					pd->system->formatString(&TextTmp, "New Total Rounds Score: %d", RetroCarouselScore);
				else
					pd->system->formatString(&TextTmp, "Final Total Rounds Score: %d", RetroCarouselScore);
				CFont_WriteText("Roboto-Regular", (int)(50.0f * yscale), TextTmp, strlen(TextTmp), (int)(150.0f * xscale), (int)(375.0f * yscale), 0, kColorWhite);
				pd->system->realloc(TextTmp, 0);

				if (wasnewhighscoregame)
				{
					pd->system->formatString(&TextTmp, "New Game High Score: %d", HighScores[Game][GameMode]);
					CFont_WriteText("Roboto-Regular", (int)(50.0f * yscale), TextTmp, strlen(TextTmp), (int)(150.0f * xscale), (int)(425.0f * yscale), 0, kColorWhite);
					pd->system->realloc(TextTmp, 0);
					SDL_Point pos = { (int)(120.0f * xscale),(int)(425.0f * yscale) };
					CImage_DrawImageFuze(GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
				}
				else
				{
					pd->system->formatString(&TextTmp, "Game High Score: %d", HighScores[Game][GameMode]);
					CFont_WriteText("Roboto-Regular", (int)(50.0f * yscale), TextTmp, strlen(TextTmp), (int)(150.0f * xscale), (int)(425.0f * yscale), 0, kColorWhite);
					pd->system->realloc(TextTmp, 0);
				}

				if (wasnewhighscorecarousel)
				{
					pd->system->formatString(&TextTmp, "New All Rounds High Score: %d", RetroCarouselHighScore);
					CFont_WriteText("Roboto-Regular", (int)(50.0f * yscale), TextTmp, strlen(TextTmp), (int)(150.0f * xscale), (int)(470.0f * yscale), 0, kColorWhite);
					pd->system->realloc(TextTmp, 0);
					SDL_Point pos = { (int)(120.0f * xscale),(int)(470.0f * yscale) };
					CImage_DrawImageFuze(GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);
				}
				else
				{
					pd->system->formatString(&TextTmp, "All Rounds High Score: %d", RetroCarouselHighScore);
					CFont_WriteText("Roboto-Regular", (int)(50.0f * yscale), TextTmp, strlen(TextTmp), (int)(150.0f * xscale), (int)(470.0f * yscale), 0, kColorWhite);
					pd->system->realloc(TextTmp, 0);
				}
				if (Game < Games - 1)
					pd->system->formatString(&TextTmp, "Press (A) for next game: %s", GSGames[Game + 1].name);
				else
					pd->system->formatString(&TextTmp, "Press (A) for titlescreen");
				int Texw = CFont_TextWidth("Roboto-Regular", (int)(34.0f * yscale), TextTmp, strlen(TextTmp));
				CFont_WriteText("Roboto-Regular", (int)(34.0f * yscale), TextTmp, strlen(TextTmp), (int)((ScreenWidth - Texw) / 2.0f), (int)(630.0f * yscale), 0, kColorWhite);
				pd->system->realloc(TextTmp, 0);
			}
			else
			{
				pd->system->formatString(&TextTmp, "Game Score: %d", Scores[Game][GameMode]);
				CFont_WriteText("Roboto-Regular", (int)(50.0f * yscale), TextTmp, strlen(TextTmp), (int)(150.0f * xscale), (int)(285.0f * yscale), 0, kColorWhite);
				pd->system->realloc(TextTmp, 0);

				if (wasnewhighscoregame)
				{
					pd->system->formatString(&TextTmp, "New Game High Score: %d", HighScores[Game][GameMode]);
					CFont_WriteText("Roboto-Regular", (int)(50.0f * yscale), TextTmp, strlen(TextTmp), (int)(150.0f * xscale), (int)(330.0f * yscale), 0, kColorWhite);
					pd->system->realloc(TextTmp, 0);
					SDL_Point pos = { (int)(120.0f * xscale),(int)(330.0f * yscale) };
					CImage_DrawImageFuze(GFXMedal, false, &pos, 0, &MedalScale, 255, 255, 255, 255);

				}
				else
				{
					pd->system->formatString(&TextTmp, "Game High Score: %d", HighScores[Game][GameMode]);
					CFont_WriteText("Roboto-Regular", (int)(50.0f * yscale), TextTmp, strlen(TextTmp), (int)(150.0f * xscale), (int)(330.0f * yscale), 0, kColorWhite);
					pd->system->realloc(TextTmp, 0);
				}
				pd->system->formatString(&TextTmp, "Press (A) for titlescreen");
				int Texw = CFont_TextWidth("Roboto-Regular", (int)(34.0f * yscale), TextTmp, strlen(TextTmp));
				CFont_WriteText("Roboto-Regular", (int)(34.0f * yscale), TextTmp, strlen(TextTmp), (int)((ScreenWidth - Texw) / 2.0f), (int)(630.0f * yscale), 0, kColorWhite);
				pd->system->realloc(TextTmp, 0);
			}
		}
	}


	if ((!CInput_PrevButtons.ButA && CInput_Buttons.ButA) ||
		(!CInput_PrevButtons.ButBack && CInput_Buttons.ButBack) ||
		(!CInput_PrevButtons.ButStart && CInput_Buttons.ButStart))
	{
		CAudio_PlaySound(SfxSelect, 0);

		GameState = GSTitleScreenInit;
		if(GameMode == GMRetroCarousel)
		{
			if (Game < Games - 1)
			{
				Game += 1;
				GameState = GSGames[Game].state;
			}
			else
			{
				if (view == 0)
				{
					if (submithighscore)
					{
						view++;
						GameState = GSSubScore;
					}
				}
			}
		}

		if (GameState == GSTitleScreenInit)
			CGame_SaveHighScores();
	}
}
