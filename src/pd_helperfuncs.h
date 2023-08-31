#ifndef PD_HELPERFUNCS_H
#define PD_HELPERFUNCS_H

#include <stdbool.h>
#include "pd_api.h"

#ifndef DrawTextColorBitmapCacheCount
#define DrawTextColorBitmapCacheCount 10
#endif

#ifndef DrawTextColorBitmapCacheMaxTextSize
#define DrawTextColorBitmapCacheMaxTextSize 850
#endif

extern LCDPattern kColorGrey;
extern PlaydateAPI* pd;
extern long int DrawTextColorBitmapCacheMisses;
void drawTextColor(bool IgnoreBitmapContext, LCDBitmap* BitmapContext, LCDFont* font, const void* text, size_t len, PDStringEncoding encoding, int x, int y, LCDColor color, bool inverted);
void DrawBitmapScaledSrcRec(LCDBitmap* Bitmap, float scalex, float scaley, int dstX, int dstY, int srcX, int srcY, int srcW, int srcH);
void DrawBitmapSrcRec(LCDBitmap* Bitmap, int dstX, int dstY, int srcX, int srcY, int srcW, int srcH, LCDBitmapFlip FlipMode);
LCDFont* loadFontAtPath(const char* path);
LCDBitmap* loadImageAtPath(const char* path);
LCDBitmapTable* loadBitmapTableAtPath(const char* path);
unsigned int logPower(const char* filename, unsigned int logIntervalSeconds, unsigned int prevLogTime);
void setPDPtr(PlaydateAPI* playdate);
void pdDelay(unsigned int milliseconds);
#endif