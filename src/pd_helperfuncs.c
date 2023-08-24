#include <string.h>
#include "pd_helperfuncs.h"
#include "Common.h"
#include "pd_api.h"

PlaydateAPI* pd;

LCDPattern kColorGrey = {
	// Bitmap
	0b10101010,
	0b01010101,
	0b10101010,
	0b01010101,
	0b10101010,
	0b01010101,
	0b10101010,
	0b01010101,

	// Mask
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
};

void drawTextColor(bool IgnoreBitmapContext, LCDBitmap* BitmapContext, LCDFont* font, const void* text, size_t len, PDStringEncoding encoding, int x, int y, LCDColor color, bool inverted)
{
	//grab width & height of our to be rendered text
	if(!IgnoreBitmapContext)
		pd->graphics->pushContext(BitmapContext);
	int h = pd->graphics->getFontHeight(font);

	pd->graphics->setFont(font);
	int w = pd->graphics->getTextWidth(font, text, len, encoding, 0);

	//create new bitmap and fillrect with our color / pattern
	LCDBitmap* bitmap = pd->graphics->newBitmap(w, h, kColorClear);
	if (inverted)
		pd->graphics->setDrawMode(kDrawModeInverted);
	pd->graphics->pushContext(bitmap);
	pd->graphics->fillRect(0, 0, w, h, color);
	pd->graphics->popContext();

	//create mask with black background and draw text in white on the mask 
	LCDBitmap* bitmapmask = pd->graphics->newBitmap(w, h, kColorBlack);
	pd->graphics->pushContext(bitmapmask);
	pd->graphics->setDrawMode(kDrawModeFillWhite);
	pd->graphics->setFont(font);
	pd->graphics->drawText(text, len, encoding, 0, 0);
	pd->graphics->popContext();

	//set the mask to the bitmap with our pattern, this will make sure only the text
	//part (white in mask is drawn from the bitmap)
	pd->graphics->setBitmapMask(bitmap, bitmapmask);

	//now draw the bitmap containing our text to the x & y position
	pd->graphics->drawBitmap(bitmap, x, y, kBitmapUnflipped);
	pd->graphics->freeBitmap(bitmap);
	pd->graphics->freeBitmap(bitmapmask);
	if (!IgnoreBitmapContext)
		pd->graphics->popContext();
}



LCDBitmap* loadImageAtPath(const char* path)
{
	const char* outErr = NULL;
	LCDBitmap* img = pd->graphics->loadBitmap(path, &outErr);
	if (outErr != NULL) {
		pd->system->logToConsole("Error loading image at path '%s': %s", path, outErr);
	}
	return img;
}

LCDBitmapTable* loadBitmapTableAtPath(const char* path)
{
	const char* outErr = NULL;
	LCDBitmapTable* table = pd->graphics->loadBitmapTable(path, &outErr);
	if (outErr != NULL) {
		pd->system->logToConsole("Error loading image at path '%s': %s", path, outErr);
	}
	return table;
}

LCDFont* loadFontAtPath(const char* path)
{
	const char* outErr = NULL;
	LCDFont* fnt = pd->graphics->loadFont(path, &outErr);
	if (outErr != NULL) {
		pd->system->logToConsole("Error loading font at path '%s': %s", path, outErr);
	}
	return fnt;
}

void DrawBitmapSrcRec(LCDBitmap* Bitmap, int dstX, int dstY, int srcX, int srcY, int srcW, int srcH, LCDBitmapFlip FlipMode)
{
	pd->graphics->pushContext(Bitmap);
	pd->graphics->setClipRect(srcX, srcY, srcW, srcH);
	pd->graphics->pushContext(NULL);
	pd->graphics->setClipRect(dstX, dstY, srcW, srcH);
	pd->graphics->setDrawOffset(-srcX + dstX, -srcY + dstY);
	pd->graphics->drawBitmap(Bitmap, 0, 0, FlipMode);
	pd->graphics->setDrawOffset(0, 0);
	pd->graphics->clearClipRect();
	pd->graphics->popContext();
	pd->graphics->popContext();
}

void DrawBitmapScaledSrcRec(LCDBitmap* Bitmap, float scalex, float scaley, int dstX, int dstY, int srcX, int srcY, int srcW, int srcH)
{
	pd->graphics->setClipRect(dstX, dstY, (int)(srcW * scalex), (int)(srcH * scaley));
	pd->graphics->setDrawOffset((int)(-srcX * scalex) + dstX, (int)(-srcY * scaley) + dstY);
	pd->graphics->drawScaledBitmap(Bitmap, 0, 0, scalex, scaley);
	pd->graphics->setDrawOffset(0, 0);
	pd->graphics->clearClipRect();
}

unsigned int logPower(const char* filename, unsigned int logIntervalSeconds, unsigned int prevLogTime)
{
	unsigned int s = pd->system->getSecondsSinceEpoch(NULL);
	if(s - prevLogTime >= logIntervalSeconds)
	{
		float p = pd->system->getBatteryPercentage();
		float v = pd->system->getBatteryVoltage();
		SDFile* file = pd->file->open(filename, kFileAppend);
		if (file)
		{
			char* line;
			pd->system->formatString(&line, "%d, %f, %f\n", s, p, v);
			pd->file->write(file, line, (unsigned int)strlen(line));
			pd->system->realloc(line, 0);
			//simulator crashes on windows when calling flush
#ifndef _WIN32
			pd->file->flush(file);
#endif
			pd->file->close(file);
		}
		return s;
	}
	return prevLogTime;
}


void setPDPtr(PlaydateAPI* playdate)
{
	pd = playdate;
}

void pdDelay(unsigned int milliseconds)
{
	unsigned int start = pd->system->getCurrentTimeMilliseconds();
	while (start + milliseconds < pd->system->getCurrentTimeMilliseconds());
}

bool pdFileExists(char* Path)
{
	bool result = false;
	SDFile* File = pd->file->open(Path, kFileRead);
	if (File)
	{
		result = true;
		pd->file->close(File);
	}

	return result;
}