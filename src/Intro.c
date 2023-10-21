#include <pd_api.h>
#include <string.h>
#include "CGame.h"
#include "Common.h"
#include "Intro.h"
#include "CTween.h"
#include "Vec2F.h"
#include "SDL_HelperTypes.h"
#include "pd_helperfuncs.h"

int maxticks = 75;
int scene = 0;
int sceneticks = 0;
int tsize = (int)(70.0f*yscale);


void Intro()
{
	if(GameState == GSIntroInit)
	{
		GameState -= initDiff;
	}

	char *s;
	int w,h;

	pd->graphics->clear(kColorBlack);
	if (scene == 0)
	{
		pd->system->formatString(&s,"Willems Davy aka Joyrider3774");
		w = CFont_TextWidth("Roboto-Regular", tsize, s, strlen(s));
		h = tsize*2;
		CFont_WriteText("Roboto-Regular", tsize, s, strlen(s), (ScreenWidth - w) / 2, (ScreenHeight - h) /2, tsize, kColorWhite);
		pd->system->realloc(s, 0);

		pd->system->formatString(&s,"presents");
		w = CFont_TextWidth("Roboto-Regular", tsize, s, strlen(s));
		h = tsize;
		CFont_WriteText("Roboto-Regular", tsize, s, strlen(s), (ScreenWidth - w) / 2, (ScreenHeight) /2, tsize, kColorWhite);
		pd->system->realloc(s, 0);
	}

	if (scene == 1)
	{
		pd->system->formatString(&s,"Retro Time");
		w = CFont_TextWidth("Roboto-Regular", tsize, s, strlen(s));
		CFont_WriteText("Roboto-Regular", tsize, s, strlen(s), (ScreenWidth - w) / 2, (int)(40.0f*yscale), tsize*2, kColorWhite);
		pd->system->realloc(s, 0);

		pd->system->formatString(&s,"A game containing");
		w = CFont_TextWidth("Roboto-Regular", tsize, s, strlen(s));
		CFont_WriteText("Roboto-Regular", tsize, s, strlen(s),(ScreenWidth - w) / 2, (int)((ScreenHeight - 3.0f * (tsize + 10.0f*yscale)) /2.0f), tsize, kColorWhite);
		pd->system->realloc(s, 0);

		pd->system->formatString(&s,"%d retro based games", Games);
		w = CFont_TextWidth("Roboto-Regular", tsize, s, strlen(s));
		CFont_WriteText("Roboto-Regular", tsize, s, strlen(s), (ScreenWidth - w) / 2,(int) ((ScreenHeight - 1 * (tsize + 10.f*yscale)) / 2.0f), tsize, kColorWhite);
		pd->system->realloc(s, 0);

		pd->system->formatString(&s, "playable in %d game modes", Modes);
		w = CFont_TextWidth("Roboto-Regular", tsize, s, strlen(s));
		CFont_WriteText("Roboto-Regular", tsize, s, strlen(s), (ScreenWidth - w) / 2, (int)((ScreenHeight + 1 * (tsize + 10*yscale)) /2.0f), tsize, kColorWhite);
		pd->system->realloc(s, 0);
	}

	if (scene >= 2)
	{
		GameState = GSTitleScreenInit;
	}

	sceneticks += 1;

	if (sceneticks >= maxticks)
	{
		scene += 1;
		sceneticks = 0;
	}

	if (CInput_Buttons.ButA || CInput_Buttons.ButB || CInput_Buttons.ButBack || CInput_Buttons.ButStart || (scene -2 >= Games))
		GameState = GSTitleScreenInit;
}