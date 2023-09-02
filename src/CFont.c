#include <pd_api.h>
#include <string.h>
#include <stdbool.h>
#include "pd_helperfuncs.h"
#include "CFont.h"

#define FontCacheMax 50
#define MaxLines 20
#define MaxCharsPerLine 255

bool CFont_DebugInfo = false;
bool CFont_GlobalFontEnabled = true;

int fontCacheItems = 0;

struct FontCacheItem {
	LCDFont* Font;
	char Filename[100];
};

typedef struct FontCacheItem FontCacheItem;

FontCacheItem CFont_FontCache[FontCacheMax];

void CFont_Init(bool DebugInfo)
{
	CFont_DebugInfo = DebugInfo;
	CFont_GlobalFontEnabled = true;

	for (int i = 0; i < FontCacheMax; i++)
	{
		CFont_FontCache[i].Font = NULL;
		CFont_FontCache[i].Filename[0] = '\0';
	}

}

void CFont_DeInit()
{
	if(CFont_GlobalFontEnabled)
	{
		/*for (int i = 0; i < fontCacheItems; i++)
			TTF_CloseFont(CFont_FontCache[i].Font);*/
	}
}

int CFont_TextWidth(char* Font, int FontSize, char* Tekst, size_t NrOfChars)
{
	SDL_Point Tmp = CFont_TextSize(Font, FontSize, Tekst, NrOfChars, 0);
	return Tmp.x;
}

SDL_Point CFont_TextSize(char* Font, int FontSize, char* Tekst, size_t NrOfChars, int YSpacing)
{
	SDL_Point Result = {0,0};
	if (!CFont_GlobalFontEnabled || (NrOfChars == 0))
		return Result;

	LCDFont* FontIn = NULL;
	char FontNameSize[100];
	char* TmpFormat;
	pd->system->formatString(&TmpFormat, "%s_%d", Font, FontSize);
	strcpy(FontNameSize, TmpFormat);
	pd->system->realloc(TmpFormat, 0);

	bool bfound = false;
	for (int i = 0; i < fontCacheItems; i++)
		if (strcmp(CFont_FontCache[i].Filename, FontNameSize) == 0)
		{
			FontIn = CFont_FontCache[i].Font;
			bfound = true;
			break;
		}

	if (fontCacheItems < FontCacheMax)
	{
		if (!bfound)
		{
			char* Filename;
			pd->system->formatString(&Filename, "fonts/%s", FontNameSize);
			FontIn = loadFontAtPath(Filename);
			if (!FontIn)
			{
				if (CFont_DebugInfo)
					pd->system->logToConsole("Failed Loading Font %s\n", Filename);
				pd->system->realloc(Filename, 0);
				return Result;
			}
			if (CFont_DebugInfo)
				pd->system->logToConsole("Loaded Font %s\n", Filename);
			pd->system->realloc(Filename, 0);

			CFont_FontCache[fontCacheItems].Font = FontIn;
			strcpy(CFont_FontCache[fontCacheItems].Filename, FontNameSize);
			fontCacheItems++;
		}
	}

	int Lines = 1;
	size_t Chars = 0;
	char* p = Tekst;
	while ((*p != '\0') && (Chars < NrOfChars))
	{
		if (*p == '\n')
			Lines++;
		p++;
		Chars++;
	}
	Result.y = Lines * pd->graphics->getFontHeight(FontIn);
	Result.x = pd->graphics->getTextWidth(FontIn, Tekst, strlen(Tekst), kASCIIEncoding, 0);
	return Result;
}

void CFont_WriteText(char* Font, int FontSize, char* Tekst, size_t NrOfChars, int X, int Y, int YSpacing, LCDColor ColorIn)
{
	CFont_WriteTextBitmap(true, NULL, Font, FontSize, Tekst, NrOfChars, X, Y, YSpacing, ColorIn);
}

void CFont_WriteTextBitmap(bool IgnoreRenderer, LCDBitmap *Renderer, char* Font, int FontSize, char* Tekst, size_t NrOfChars, int X, int Y, int YSpacing, LCDColor ColorIn)
{
	if(!CFont_GlobalFontEnabled || (NrOfChars == 0))
		return;
	LCDFont* FontIn = NULL;
	char FontNameSize[100];
	char* TmpFormat;
	pd->system->formatString(&TmpFormat, "%s_%d", Font, FontSize);
	strcpy(FontNameSize, TmpFormat);
	pd->system->realloc(TmpFormat, 0);

	bool bfound = false;
	for (int i = 0; i < fontCacheItems; i++)
		if (strcmp(CFont_FontCache[i].Filename, FontNameSize) == 0)
		{
			FontIn = CFont_FontCache[i].Font;
			bfound = true;
			break;
		}

	if (fontCacheItems < FontCacheMax)
	{
		if (!bfound)
		{
			char* Filename;
			pd->system->formatString(&Filename, "fonts/%s", FontNameSize);
			FontIn = loadFontAtPath(Filename);
			if (!FontIn)
			{
				if(CFont_DebugInfo)
					pd->system->logToConsole("Failed Loading Font %s\n", Filename);
			}
			if (CFont_DebugInfo)
				pd->system->logToConsole("Loaded Font %s\n", Filename);
			pd->system->realloc(Filename, 0);
			CFont_FontCache[fontCacheItems].Font = FontIn;
			strcpy(CFont_FontCache[fontCacheItems].Filename, FontNameSize);
			fontCacheItems++;
		}
	}
	if (FontIn)
	{
		drawTextColor(IgnoreRenderer, Renderer, FontIn, Tekst, strlen(Tekst), kASCIIEncoding, X, Y, ColorIn, false);
	}
}