#include <pd_api.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "CImage.h"
#include "Vec2F.h"
#include "pd_helperfuncs.h"
#include "SDL_HelperTypes.h"
#include "Common.h"


#define GFX_Max 100
#define CImage_ScaledImagesLoadedMaxCount 200

struct ScaledImagesStruct {
	char basefilename[200];
	Vec2F resolution;
	LCDBitmap* texture;
};

typedef struct ScaledImagesStruct ScaledImagesStruct;

ScaledImagesStruct ScaledImages[CImage_ScaledImagesLoadedMaxCount];

CTexture *CImage_Images[GFX_Max];
bool CImage_ImgEnabled = false;
bool CImage_DebugInfo = false;
int CImage_ScaledImagesLoaded = 0;

void CImage_Init(bool ADebugInfo)
{
	CImage_ImgEnabled = true;
	CImage_DebugInfo = ADebugInfo;
	pd->system->logToConsole("Images Succesfully initialised!\n");
	
	for (int i=0; i < GFX_Max; i++)
		CImage_Images[i] = NULL;
	
	for (int i=0; i < CImage_ScaledImagesLoadedMaxCount; i++)
	{
		ScaledImages[i].basefilename[0] = '\0';
		ScaledImages[i].resolution.x = 0;
		ScaledImages[i].resolution.y = 0;
		ScaledImages[i].texture = NULL;
	}
}

void CImage_DeInit()
{
	CImage_UnloadImages();

	for(int i = 0; i < CImage_ScaledImagesLoaded; i++)
	{
		if (ScaledImages[i].texture != NULL)
			pd->graphics->freeBitmap(ScaledImages[i].texture);
	}
}

int CImage_ScaledImagesLoadedMax()
{
	return CImage_ScaledImagesLoadedMaxCount;
}

int CImage_ScaledImagesLoadedCount()
{
	return CImage_ScaledImagesLoaded;
}

LCDBitmap *CImage_LoadScaledImage(int GFXID, Vec2F Scale)
{
	if(!CImage_ImgEnabled)
		return NULL;

	if ((GFXID < 0) || (GFXID >= GFX_Max))
		return NULL;

	if (CImage_Images[GFXID] == NULL)
		return NULL;

	
	int w, h;
	pd->graphics->getBitmapData(CImage_Images[GFXID]->Img, &w, &h, NULL, NULL, NULL);
	w = (int)ceil(w * fabsf(Scale.x));
	h = (int)ceil(h * fabsf(Scale.y));
	Vec2F Resolution = {(float)w,(float)h};

	for(int i = 0; i < CImage_ScaledImagesLoaded; i++)
	{
		if ((strcmp(ScaledImages[i].basefilename, CImage_Images[GFXID]->BaseFilename) ==0) &&
			((ScaledImages[i].resolution.x  <= Resolution.x + epsilion)) && 
			((ScaledImages[i].resolution.x >= Resolution.x - epsilion)) &&
			((ScaledImages[i].resolution.y <= Resolution.y + epsilion)) &&
			(ScaledImages[i].resolution.y >= Resolution.y - epsilion))
			return ScaledImages[i].texture;
	}

	LCDBitmap *Tmp = NULL;
	//not loaded yet
	if (CImage_ScaledImagesLoaded < CImage_ScaledImagesLoadedMaxCount)
	{
		char *FullFileName;
		pd->system->formatString(&FullFileName, "%s_%dx%d", CImage_Images[GFXID]->BaseFilename, w, h);
		
		Tmp = loadImageAtPath(FullFileName);
		pd->system->realloc(FullFileName, 0);
		if(Tmp)
		{
			if (CImage_DebugInfo)
				pd->system->logToConsole("Loaded Graphic %s\n", FullFileName);
			strcpy(ScaledImages[CImage_ScaledImagesLoaded].basefilename, CImage_Images[GFXID]->BaseFilename);
			ScaledImages[CImage_ScaledImagesLoaded].resolution.x = Resolution.x;
			ScaledImages[CImage_ScaledImagesLoaded].resolution.y = Resolution.y;
			ScaledImages[CImage_ScaledImagesLoaded].texture = Tmp;
			CImage_ScaledImagesLoaded++;
		}
		else
			if (CImage_DebugInfo)
				pd->system->logToConsole("Failed Loading Graphic %s\n", FullFileName);

	}

	return Tmp;
}

int CImage_LoadImage(const char* FileName)
{
	return CImage_LoadImageEx(FileName, 0, 0, false);
}

int CImage_LoadImageEx(const char* FileName, int bayerver, int whitethreshold, bool cansave)
{
	int result = -1;

	char *BaseFileName;
	pd->system->formatString(&BaseFileName, "graphics/%s", FileName);

	for (size_t j = strlen(BaseFileName) - 1; j > 0; j--)
		if (BaseFileName[j] == '.')
		{
			BaseFileName[j] = '\0';
			break;
		}
	
	

	if(CImage_ImgEnabled)
		for (int i=0; i < GFX_Max; i++)
		{
			if(CImage_Images[i] == NULL)
			{
				LCDBitmap *Img = loadImageAtPath(BaseFileName);
				if(Img)
				{				
					CImage_Images[i] = (CTexture*) malloc(sizeof(CTexture));
					strcpy(CImage_Images[i]->BaseFilename, BaseFileName);
					CImage_Images[i]->bayerversion = bayerver;
					CImage_Images[i]->whiteThreshold = whitethreshold;
					CImage_Images[i]->cansave = cansave;
					CImage_Images[i]->Img = Img;
					if(CImage_DebugInfo)
						pd->system->logToConsole("Loaded Graphic %s\n", BaseFileName);
					result = i;
					break;
				}
			}
		}

	if(result == -1)
		if (CImage_DebugInfo)
			pd->system->logToConsole("Failed Loading Graphic %s\n", BaseFileName);
	
	return result;
}

void CImage_UnLoadImage(int GFXID)
{
	if(!CImage_ImgEnabled)
		return;

	if ((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	if (CImage_Images[GFXID] != NULL)
	{
		pd->graphics->freeBitmap(CImage_Images[GFXID]->Img);
		
		free(CImage_Images[GFXID]);
		CImage_Images[GFXID] = NULL;
	}


}

void CImage_UnloadImages()
{
	for (int i=0; i < GFX_Max; i++)
		CImage_UnLoadImage(i);
}

void CImage_DrawImage(int GFXID, SDL_Rect* Src, SDL_Rect* Dst)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	CImage_DrawImageTex(CImage_Images[GFXID]->Img, Src, Dst);
}

void CImage_DrawImageTex(LCDBitmap *Texture, SDL_Rect* Src, SDL_Rect* Dst)
{
	if(!CImage_ImgEnabled)
		return;

	if(Texture == NULL)
		return;
	
	int texw, texh;
	pd->graphics->getBitmapData(Texture, &texw, &texh, NULL, NULL, NULL);
	if ((Src == NULL) && (Dst == NULL))
	{
		if ((ScreenWidth == texw) && (ScreenHeight == texh))
		{
			DrawBitmapSrcRec(Texture, 0, 0, 0, 0, ScreenWidth, ScreenHeight, kBitmapUnflipped);
		}
		else
		{
			DrawBitmapScaledSrcRec(Texture, (float)ScreenWidth / texw, (float)ScreenHeight / texh, 0, 0, 0, 0, texw, texh);
		}
	}	
	else
	{
		if ((Src != NULL) && (Dst == NULL))
		{
			if ((ScreenWidth == Src->w) && (ScreenHeight == Src->h))
			{
				DrawBitmapSrcRec(Texture, 0, 0, Src->x, Src->y,  Src->w, Src->h, kBitmapUnflipped);
			}
			else
			{
				DrawBitmapScaledSrcRec(Texture, (float)ScreenWidth / Src->w, (float)ScreenHeight / Src->h, 0, 0, Src->x, Src->y, Src->w, Src->h);
			}
		}
		else
		{
			if ((Src == NULL) && (Dst != NULL))
			{
				if((Dst->w == texw) && (Dst->h == texh))
				{
					DrawBitmapSrcRec(Texture, Dst->x, Dst->y, 0, 0, texw, texh, kBitmapUnflipped);
				}
				else
				{
					DrawBitmapScaledSrcRec(Texture, (float)Dst->w / texw, (float)Dst->h / texh, Dst->x, Dst->y, 0, 0, texw, texh);
				}
			}
			else
			{
				if ((Dst->w == Src->w) && (Dst->h == Src->h))
				{
					DrawBitmapSrcRec(Texture, Dst->x, Dst->y, Src->x, Src->y, Src->w, Src->h, kBitmapUnflipped);
				}
				else
				{
					DrawBitmapScaledSrcRec(Texture, (float)Dst->w / Src->w, (float)Dst->h / Src->h, Dst->x, Dst->y, Src->x, Src->y, Src->w, Src->h);
				}
			}
		}
	}
}

//fuze used center points for positions and a floating point scale vector
void CImage_DrawImageFuzeSrcRectTintFloat(int GFXID, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	CImage_DrawImageFuzeTex(CImage_Images[GFXID]->Img, SrcRect, CenterImagePos, Pos, Angle, Scale, (uint8_t)floor(255.0f * TintR) , (uint8_t)floor(255.0f * TintG), (uint8_t)floor(255.0f * TintB) , (uint8_t)floor(255.0f * Alpha));
}

void CImage_DrawImageFuzeSrcRectTintFloatTex(LCDBitmap* Texture, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if(Texture == NULL)
		return;

	CImage_DrawImageFuzeTex(Texture, SrcRect, CenterImagePos, Pos, Angle, Scale, (uint8_t)floor(255.0f * TintR) , (uint8_t)floor(255.0f * TintG), (uint8_t)floor(255.0f * TintB) , (uint8_t)floor(255.0f * Alpha));
}

//fuze used center points for positions and a floating point scale vector
void CImage_DrawImageFuzeTintFloat(int GFXID, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	CImage_DrawImageFuzeTex(CImage_Images[GFXID]->Img, NULL, CenterImagePos, Pos, Angle, Scale, (uint8_t)floor(255.0f * TintR) , (uint8_t)floor(255.0f * TintG), (uint8_t)floor(255.0f * TintB) , (uint8_t)floor(255.0f * Alpha));
}

void CImage_DrawImageFuzeTintFloatTex(LCDBitmap *Texture, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, float TintR, float TintG, float TintB, float Alpha)
{
	if(Texture == NULL)
		return;

	CImage_DrawImageFuzeTex(Texture, NULL, CenterImagePos, Pos, Angle, Scale, (uint8_t)floor(255.0f * TintR) , (uint8_t)floor(255.0f * TintG), (uint8_t)floor(255.0f * TintB) , (uint8_t)floor(255.0f * Alpha));
}

void CImage_DrawImageFuze(int GFXID, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, uint8_t TintR, uint8_t TintG, uint8_t TintB, uint8_t Alpha)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return;

	CImage_DrawImageFuzeTex(CImage_Images[GFXID]->Img, NULL, CenterImagePos, Pos, Angle, Scale, TintR, TintG, TintB, Alpha);
}


//fuze used center points for positions and a floating point scale vector
void CImage_DrawImageFuzeTex(LCDBitmap *Texture, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, uint8_t TintR, uint8_t TintG, uint8_t TintB, uint8_t Alpha)
{
	if(!CImage_ImgEnabled)
		return;

	if(Texture == NULL)
		return;

	float dstW;
	float dstH;
	int srcX;
	int srcY;
	int srcW;
	int srcH;
	if(SrcRect)
	{
		srcX = SrcRect->x;
		srcY = SrcRect->y;
		srcW = SrcRect->w;
		srcH = SrcRect->h;
		dstW = (SrcRect->w * fabsf(Scale->x));
		dstH = (SrcRect->h * fabsf(Scale->y));
	}
	else
	{
		srcX = 0;
		srcY = 0;
		SDL_Point ImageTz = CImage_ImageSizeTex(Texture);
		srcW = ImageTz.x;
		srcH = ImageTz.y;
		dstW = (ImageTz.x * fabsf(Scale->x));
		dstH = (ImageTz.y * fabsf(Scale->y));
	}
	SDL_Rect Dst;
	if(CenterImagePos)
	{
		Dst.x = (int)(Pos->x - ((dstW) / 2));
		Dst.y = (int)(Pos->y - ((dstH) / 2));
		Dst.w = (int)(dstW);
		Dst.h = (int)(dstH);
	}
	else
	{
		Dst.x = Pos->x;
		Dst.y = Pos->y;
		Dst.w = (int)(dstW);
		Dst.h = (int)(dstH);
	}

	//save values
	LCDBitmapFlip flip = kBitmapUnflipped;

	if ((Scale->x < 0) && (Scale->y < 0))
		flip = kBitmapFlippedXY;
	else
	{
		if (Scale->x < 0)
			flip = kBitmapFlippedX;
		else
		{
			if (Scale->y < 0)
				flip = kBitmapFlippedY;
		}
	}

	//Draw

	if ((fabsf(Angle) >= epsilion) || (fabsf(Angle) <= -epsilion))
	{
		SDL_Point ImageTz = CImage_ImageSizeTex(Texture);
		//if we need to draw rotated but the sprite is not the full sprite need to first draw the sprite to seperate tile to only grab the part we need)
		if ((ImageTz.x != srcW) || (ImageTz.y != srcH))
		{
			LCDBitmap* tmp = pd->graphics->newBitmap(srcW, srcH, kColorClear);
			pd->graphics->pushContext(tmp);
			DrawBitmapSrcRec(Texture, 0, 0, srcX, srcY, srcW, srcH, flip);
			pd->graphics->popContext();
			//pd->graphics->drawBitmap(tmp, (int)(Pos->x), (int)(Pos->y),kBitmapUnflipped);
			pd->graphics->drawRotatedBitmap(tmp, (int)Pos->x, (int)Pos->y, Angle, 0.5f, 0.5f, (float)Dst.w / srcW, (float)Dst.h / srcH);
			pd->graphics->freeBitmap(tmp);
		}
		else
			pd->graphics->drawRotatedBitmap(Texture,(int)(Dst.x + ((dstW) / 2)),(int)(Dst.y + ((dstH) / 2)), Angle, 0.5f, 0.5f, (float)Dst.w / srcW, (float)Dst.h / srcH);
	}
	else
	{
		if ((fabsf(Scale->x) <= 1.0f - epsilion) || (fabsf(Scale->x) >= 1.0f + epsilion) || (fabsf(Scale->y) <= 1.0f - epsilion) || (fabsf(Scale->y)>= 1.0f + epsilion))
		{
			DrawBitmapScaledSrcRec(Texture, Scale->x, Scale->y, Dst.x, Dst.y, srcX, srcY, srcW, srcH);
		}
		else
		{
			if ((Dst.w == srcW) && (Dst.h == srcH))
			{
				DrawBitmapSrcRec(Texture, Dst.x, Dst.y, srcX, srcY, srcW, srcH, flip);
			}
			else
			{
				DrawBitmapScaledSrcRec(Texture, (float)Dst.w / srcW, (float)Dst.h / srcH, Dst.x, Dst.y, srcX, srcY, srcW, srcH);
			}

		}
	}

}

SDL_Point CImage_ImageSize(int GFXID)
{
	SDL_Point Result = {-1, -1};
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return Result;

	return CImage_ImageSizeTex(CImage_Images[GFXID]->Img);
}


SDL_Point CImage_ImageSizeTex(LCDBitmap *Texture)
{
	SDL_Point Result = {-1, -1};
	if(!CImage_ImgEnabled)
		return Result;

	if(Texture == NULL)
		return Result;

	SDL_Point size;
	pd->graphics->getBitmapData(Texture, &size.x, &size.y, NULL, NULL, NULL);
	return size;
}

int CImage_ImageSlotsUsed()
{
	int c = 0;
	for (int i=0; i < GFX_Max; i++)
	{
		if(CImage_Images[i] != NULL)
			c++;
	}
	return c;
}

int CImage_ImageSlotsMax()
{
	return GFX_Max;
}

LCDBitmap* CImage_GetImage(int GFXID)
{
	if((GFXID < 0) || (GFXID >= GFX_Max))
		return NULL;
	
	if (CImage_Images[GFXID] == NULL)
		return NULL;

	return CImage_Images[GFXID]->Img;
}