#ifndef CIMAGE_H
#define CIMAGE_H

#include <pd_api.h>
#include <stdbool.h>
#include <stdint.h>
#include "Vec2F.h"
#include "SDL_HelperTypes.h"


struct CTexture {
	LCDBitmap* Img;
	char BaseFilename[1000];
	int whiteThreshold;
	int bayerversion;
	bool cansave;
};

typedef struct CTexture CTexture;

void CImage_Init(bool ADebugInfo);
void CImage_DeInit();
LCDBitmap *CImage_LoadScaledImage(int GFXID, Vec2F Scale);
int CImage_LoadImage(const char* FileName);
int CImage_LoadImageEx(const char* FileName, int bayerver, int whitethreshold, bool cansave);
void CImage_UnLoadImage(int GFXID);
void CImage_UnloadImages();
void CImage_DrawImage(int GFXID, SDL_Rect* Src, SDL_Rect* Dst);
void CImage_DrawImageTex(LCDBitmap*, SDL_Rect* Src, SDL_Rect* Dst);
//tint & alpha value between 0.0 & 1.0
void CImage_DrawImageFuzeSrcRectTintFloat(int GFXID, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale,
	float TintR, float TintG, float TintB, float Alpha);
void CImage_DrawImageFuzeSrcRectTintFloatTex(LCDBitmap* Texture, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale, 
	float TintR, float TintG, float TintB, float Alpha);
//tint & alpha value between 0.0 & 1.0
void CImage_DrawImageFuzeTintFloatTex(LCDBitmap *Texture, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale,
	float TintR, float TintG, float TintB, float Alpha);
//tint & alpha value between 0.0 & 1.0
void CImage_DrawImageFuzeTintFloat(int GFXID, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale,
	float TintR, float TintG, float TintB, float Alpha);
void CImage_DrawImageFuze(int GFXID, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale,
	uint8_t TintR, uint8_t TintG, uint8_t TintB, uint8_t Alpha);
void CImage_DrawImageFuzeTex(LCDBitmap *Texture, SDL_Rect *SrcRect, bool CenterImagePos, SDL_Point* Pos, float Angle, Vec2F* Scale,
	uint8_t TintR, uint8_t TintG, uint8_t TintB, uint8_t Alpha);
SDL_Point CImage_ImageSize(int GFXID);
SDL_Point CImage_ImageSizeTex(LCDBitmap *Texture);
int CImage_ImageSlotsUsed();
int CImage_ImageSlotsMax();
int CImage_ScaledImagesLoadedCount();
int CImage_ScaledImagesLoadedMax();
LCDBitmap* CImage_GetImage(int GFXID);

#endif