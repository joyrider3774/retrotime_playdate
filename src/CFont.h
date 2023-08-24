#ifndef CFONT_H
#define CFONT_H

#include <pd_api.h>
#include <stdbool.h>
#include <string.h>
#include "SDL_HelperTypes.h"


void CFont_Init(bool DebugInfo);
void CFont_DeInit();
int CFont_TextWidth(char* Font, int FontSize, char* Tekst, size_t NrOfChars);
SDL_Point CFont_TextSize(char* Font, int FontSize, char* Tekst, size_t NrOfChars, int YSpacing);
void CFont_WriteText(char* Font, int FontSize, char* Tekst, size_t NrOfChars, int X, int Y, int YSpacing, LCDColor ColorIn);
void CFont_WriteTextBitmap(bool IgnoreRenderer, LCDBitmap* Renderer, char* Font, int FontSize, char* Tekst, size_t NrOfChars, int X, int Y, int YSpacing, LCDColor ColorIn);

#endif