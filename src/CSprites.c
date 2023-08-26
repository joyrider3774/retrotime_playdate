#include <pd_api.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "CSprites.h"
#include "Vec2F.h"
#include "Common.h"
#include "pd_helperfuncs.h"
#include "SDL_HelperTypes.h"

#define SPR_Max 500

#define SavedScalingsMax 500

int SavedScalingsCount = 0;
struct SavedScalingsStruct {
	int gfxid;
	Vec2F scale;
};
typedef struct SavedScalingsStruct SavedScalingsStruct;

SavedScalingsStruct CSprites_SavedScalings[SavedScalingsMax];


CSprite* CSprites_Sprites[SPR_Max];
int CSprites_UpdateImageResets;
int CSprites_SpritesDrawn;
bool CSprites_ForceShowCollisionShape;
bool CSprites_needSpriteSorting;

void CSprites_Init()
{
	CSprites_UpdateImageResets = 0;
	CSprites_ForceShowCollisionShape = false;
	CSprites_needSpriteSorting = false;
	CSprites_SpritesDrawnReset();
	for (int i=0; i < SPR_Max; i++)
	{
		CSprites_Sprites[i] = NULL;
	}
}

void CSprites_DeInit()
{
	for (int i=0; i < SPR_Max; i++)
	{
		CSprites_RemoveSprite(CSprites_Sprites[i]);
	}
}

void CSprites_SetForceShowCollisionShape(bool val)
{
	CSprites_ForceShowCollisionShape = val;
}

int CSprites_UpdateImageResetsCount()
{
	return  CSprites_UpdateImageResets;
}

int CSprites_SpritesDrawnCount()
{
	return CSprites_SpritesDrawn;
}

void CSprites_SpritesDrawnReset()
{
	CSprites_SpritesDrawn = 0;
}

CSprite* CSprites_CreateSprite()
{
	for (int i= 0; i < SPR_Max; i++)
	{
		if(CSprites_Sprites[i] == NULL)
		{
			CSprite* Spr = (CSprite*) malloc(sizeof(CSprite));
			Spr->index = i;
			Spr->animInc = 0;
			Spr->animTimer = 0;
			Spr->rotation = 0;
			Spr->imageID = NULL;
			Spr->sxscale = 1;
			Spr->syscale = 1;
			Spr->prevxscale = -1;
			Spr->prevyscale = -1;
			Spr->xscale_speed = 0;
			Spr->yscale_speed = 0;
			Spr->xspeed = 0;
			Spr->yspeed = 0;
			Spr->tileSizeX = 0;
			Spr->tileSizeY = 0;
			Spr->animStartTile = 0;
			Spr->animEndTile = 0;
			Spr->animSpeed = 0;
			Spr->animTile = 0;
			Spr->collisionShape = SHAPE_BOX;
			Spr->collisionAngle = 0;
			Spr->collisionWidth = 0;
			Spr->collisionHeight = 0;
			Spr->collisionxoffset = 0;
			Spr->collisionyoffset = 0;
			Spr->depth = 0;
			Spr->show = true;
			Spr->r = 1.0;
			Spr->g = 1.0;
			Spr->b = 1.0;
			Spr->a = 1.0;
			Spr->tilesX = 1;
			Spr->tilesY = 1;
			Spr->rotation_speed = 0.0;
			Spr->Img = NULL;
			Spr->x = 0.0f;
			Spr->y = 0.0f;
			Spr->show_collision_shape = false;
			CSprites_Sprites[i] = Spr;

			return Spr;
		}
	}
	return NULL;
}

void CSprites_RemoveSprite(CSprite* Spr)
{
	if(Spr == NULL)
		return;

	if((Spr->index < 0) || (Spr->index >= SPR_Max))
		return;
	
	//dumped scaled bitmaps are handled in images
	if(!loadDumpedScaledBitmaps)
		if(CSprites_Sprites[Spr->index]->Img != NULL)
		{
			pd->graphics->freeBitmap(CSprites_Sprites[Spr->index]->Img);
			CSprites_Sprites[Spr->index]->Img = NULL;
		}
	
	CSprites_Sprites[Spr->index] = NULL;
		
	free(Spr);
}

void CSprites_UpdateSprites()
{
	CSprites_SortSprites();
	for (int i = 0; i < SPR_Max; i++)
	{
		if (CSprites_Sprites[i] == NULL)
			continue;

		if (CSprites_Sprites[i]->animSpeed != 0)
		{
			if (pd->system->getCurrentTimeMilliseconds() > CSprites_Sprites[i]->animTimer)
			{
				CSprites_Sprites[i]->animTile += CSprites_Sprites[i]->animInc;
				if (CSprites_Sprites[i]->animInc > 0)
				{
					if(CSprites_Sprites[i]->animTile > CSprites_Sprites[i]->animEndTile)
						CSprites_Sprites[i]->animTile = CSprites_Sprites[i]->animStartTile;
				}
				else
				{
					if (CSprites_Sprites[i]->animInc < 0)
					{
						if(CSprites_Sprites[i]->animTile < CSprites_Sprites[i]->animEndTile)
							CSprites_Sprites[i]->animTile = CSprites_Sprites[i]->animStartTile;
					}
				}
				CSprites_Sprites[i]->animTimer = pd->system->getCurrentTimeMilliseconds() + (int)floor(1000 / CSprites_Sprites[i]->animSpeed);
			}
		}

		CSprites_Sprites[i]->x += CSprites_Sprites[i]->xspeed;
		CSprites_Sprites[i]->y += CSprites_Sprites[i]->yspeed;
		CSprites_Sprites[i]->sxscale += CSprites_Sprites[i]->xscale_speed;
		CSprites_Sprites[i]->syscale += CSprites_Sprites[i]->yscale_speed;
		CSprites_Sprites[i]->rotation += CSprites_Sprites[i]->rotation_speed;
		CSprites_Sprites[i]->collisionAngle += CSprites_Sprites[i]->rotation_speed;
		CSprites_Sprites[i]->collisionWidth += CSprites_Sprites[i]->xscale_speed;
		CSprites_Sprites[i]->collisionHeight += CSprites_Sprites[i]->yscale_speed;

		CSprites_UpdateImage(CSprites_Sprites[i]);
	}
}

// function to swap elements https://www.programiz.com/dsa/quick-sort

void CSprites_swap(CSprite* array[], int ai, int bi) {
	CSprite* t = array[ai];
	array[ai] = array[bi];
	if (array[ai])
		array[ai]->index = ai;
	array[bi] = t;
	if (array[bi])
		array[bi]->index = bi;
}

// function to find the partition position https://www.programiz.com/dsa/quick-sort

int CSprites_partition(CSprite* array[], int low, int high) {

	// select the rightmost element as pivot
	int pivot = array[high] == NULL ? INT_MAX: array[high]->depth;

	// pointer for greater element
	int i = (low - 1);

	// traverse each element of the array
	// compare them with the pivot
	for (int j = low; j < high; j++) {
		if ((array[j] == NULL ? INT_MAX : array[j]->depth) < pivot) {

			// if element smaller than pivot is found
			// swap it with the greater element pointed by i
			i++;

			// swap element at i with element at j
			CSprites_swap(array, i, j);
		}
	}

	// swap the pivot element with the greater element at i
	CSprites_swap(array, i + 1, high);

	// return the partition point
	return (i + 1);
}

//https://www.programiz.com/dsa/quick-sort
void CSprites_quickSort(CSprite* array[], int low, int high) {
	if (low < high) {

		// find the pivot element such that
		// elements smaller than pivot are on left of pivot
		// elements greater than pivot are on right of pivot
		int pi = CSprites_partition(array, low, high);

		// recursive call on the left of pivot
		CSprites_quickSort(array, low, pi - 1);

		// recursive call on the right of pivot
		CSprites_quickSort(array, pi + 1, high);
	}
}

// function to print array elements
void CSprites_printArray(CSprite* array[], int size) {
	for (int i = 0; i < size; ++i) {
		pd->system->logToConsole("%d  ", array[i] == NULL ? INT_MAX : array[i]->depth);
	}
	pd->system->logToConsole("\n\n\n\n\n");
}


void CSprites_SortSprites()
{
	if (CSprites_needSpriteSorting)
	{
		CSprites_quickSort(CSprites_Sprites, 0, SPR_Max - 1);
		//CSprites_printArray(CSprites_Sprites, SPR_Max - 1);
		CSprites_needSpriteSorting = false;
	}

}

void CSprites_DrawSprite(CSprite* Spr)
{
	if (Spr == NULL)
		return;

	CSprites_SortSprites();

	if (Spr->show && ((*Spr->imageID > -1) && (Spr->Img != NULL) && (*Spr->imageID < CImage_ImageSlotsMax())))
	{
		CSprites_SpritesDrawn++;
		SDL_Point pos = {(int)(Spr->x), (int)(Spr->y)};
	
		Vec2F scale = {Spr->sxscale, Spr->syscale};
		//multiply is to get the sign
		scale.x = 1.0f * (Spr->sxscale / fabsf(Spr->sxscale));
		scale.y = 1.0f * (Spr->syscale/fabsf(Spr->syscale));
		int AnimTile = Spr->animTile;
		int y = (int)floor(AnimTile / Spr->tilesX);
		int x = AnimTile - (y * Spr->tilesX);
		
		SDL_Rect SrcRect = {(int)(x * Spr->tileSizeX* fabsf(Spr->sxscale)), (int)(y* Spr->tileSizeY* fabsf(Spr->syscale)), (int)(Spr->tileSizeX* fabsf(Spr->sxscale)),(int)(Spr->tileSizeY* fabsf(Spr->syscale))};
		
		
		CImage_DrawImageFuzeSrcRectTintFloatTex(Spr->Img, &SrcRect, true, &pos, Spr->rotation, &scale, Spr->r, Spr->g, Spr->b, Spr->a);
		

		if (Spr->show_collision_shape || CSprites_ForceShowCollisionShape)
		{
			switch(Spr->collisionShape)
			{
				case SHAPE_BOX:
				{
					const SDL_Rect rect = {(int)(Spr->x + Spr->collisionxoffset - (Spr->collisionWidth * fabsf(Spr->sxscale) / 2)), (int)(Spr->y + Spr->collisionyoffset - (Spr->collisionHeight * fabsf(Spr->syscale) / 2)), (int)(Spr->collisionWidth * fabsf(Spr->sxscale)), (int)(Spr->collisionHeight * fabsf(Spr->syscale))};
					pd->graphics->drawRect(rect.x, rect.y, rect.w, rect.h, kColorXOR);
					break;
				}
				case SHAPE_CIRCLE:
				{
					if ((Spr->collisionWidth == Spr->collisionHeight) && (Spr->sxscale == Spr->syscale))
						pd->graphics->drawEllipse((int)(Spr->x + Spr->collisionxoffset - (Spr->collisionWidth * fabsf(Spr->sxscale) / 2)),(int)(Spr->y + Spr->collisionyoffset - (Spr->collisionHeight * fabsf(Spr->syscale) / 2)),(int) (Spr->collisionWidth * fabsf(Spr->sxscale)), (int)(Spr->collisionWidth * fabsf(Spr->syscale)),1,0,360, kColorXOR);
					break;
				}
				default:
					break;
			}
		}
	}
}

void CSprites_ResetDrawTargets()
{
	for (int i = 0; i < SPR_Max; i++)
	{
		if (CSprites_Sprites[i] == NULL)
			continue;

		//by reseting prevxscale
		//   it will be redrawn
		CSprites_Sprites[i]->prevxscale = 0;
		CSprites_Sprites[i]->prevyscale = 0;
	}
}

void CSprites_DrawSprites()
{
	CSprites_SpritesDrawnReset();
	for (int i = 0; i < SPR_Max; i++)
	{
		if (CSprites_Sprites[i] == NULL)
			continue;

		CSprites_DrawSprite(CSprites_Sprites[i]);
	}
}

void CSprites_SetSpriteRotationSpeed(CSprite* Spr, float rotationSpeed)
{
	Spr->rotation_speed = rotationSpeed;
}

void CSprites_SetSpriteColour (CSprite* Spr, float red, float green, float blue, float alpha)
{
	Spr->r = red;
	Spr->g = green;
	Spr->b = blue;
	Spr->a = alpha;
}

void CSprites_SetSpriteVisibility(CSprite* Spr, bool visibility)
{
	Spr->show = visibility;
}

Vec2F CSprites_GetSpriteLocation(CSprite* Spr)
{
	Vec2F Result = {Spr->x, Spr->y};
	return Result;
}

void CSprites_SetSpriteImage(CSprite* Spr, int *AImageID)
{
	CSprites_SetSpriteImageTiles(Spr, AImageID, 1, 1);
}

void CSprites_UpdateImage(CSprite* Spr) 
{
	if(Spr == NULL)
		return;

	if(Spr->imageID == NULL)
		return;

	if((fabsf(Spr->syscale) == fabsf(Spr->prevyscale)) &&
		(fabsf(Spr->sxscale) == fabsf(Spr->prevxscale)))
		return;

	if(loadDumpedScaledBitmaps)
	{
		Vec2F Scale = {Spr->sxscale,Spr->syscale};
		Spr->Img = CImage_LoadScaledImage(*Spr->imageID, Scale);
		//remember current scale
		Spr->prevyscale = Spr->syscale;
		Spr->prevxscale = Spr->sxscale;
		CSprites_UpdateImageResets++;
	}
	else
	{
		LCDBitmap *tex = CImage_GetImage(*Spr->imageID);

		if (tex == NULL)
			return;


		int w, h;

		pd->graphics->getBitmapData(tex, &w,&h, NULL, NULL, NULL);

		if (Spr->Img != NULL)
			pd->graphics->freeBitmap(Spr->Img);

		// Create a new texture with the same properties as the one we are duplicating
		Spr->Img = pd->graphics->newBitmap((int)(w * fabsf(Spr->sxscale)),(int)(h * fabsf(Spr->syscale)), kColorClear);

		
		
		//calculate dest size
		SDL_Rect TmpR;
		TmpR.x = 0;
		TmpR.y = 0;
		TmpR.w = (int)(w * fabsf(Spr->sxscale));
		TmpR.h = (int)(h * fabsf(Spr->syscale));

		pd->graphics->pushContext(Spr->Img);
		DrawBitmapScaledSrcRec(tex, (float)TmpR.w / w,(float)TmpR.h / h, 0, 0, 0, 0, w, h);
		pd->graphics->popContext();
		//remember current scale
		Spr->prevyscale = Spr->syscale;
		Spr->prevxscale = Spr->sxscale;
		CSprites_UpdateImageResets++;
		
	}
}

void CSprites_SetSpriteImageTiles(CSprite* Spr, int *AImageID, int TilesX, int TilesY)
{
	bool needCSprites_UpdateImage = Spr->imageID != AImageID;
	Spr->imageID = AImageID;
	SDL_Point Tz = CImage_ImageSize(*AImageID);
	if(needCSprites_UpdateImage)
	{
		//to force an update
		Spr->prevyscale = 0;
		Spr->prevxscale = 0;
		CSprites_UpdateImage(Spr);
	}
	Spr->tileSizeX = (int)floor(Tz.x / TilesX);
	Spr->tileSizeY = (int)floor(Tz.y / TilesY);
	Spr->tilesX = TilesX;
	Spr->tilesY = TilesY;
	if ((Spr->collisionHeight == 0) && (Spr->collisionWidth == 0))
	{
		Spr->collisionHeight = (float)Spr->tileSizeY;
		Spr->collisionWidth = (float)Spr->tileSizeX;
	}
}

void CSprites_SetSpriteScale(CSprite* Spr, Vec2F AScale)
{
	Spr->sxscale = AScale.x;
	Spr->syscale = AScale.y;
	CSprites_UpdateImage(Spr);
}

void CSprites_SetSpriteRotation(CSprite* Spr, float AAngle)
{
	float diffAngle = AAngle - Spr->rotation;
	Spr->collisionAngle += diffAngle;
	Spr->rotation = AAngle;
}

void CSprites_SetSpriteAnimation(CSprite* Spr, int StartTile, int EndTile, int animSpeed)
{
	Spr->animStartTile = StartTile;
	Spr->animEndTile = EndTile;
	Spr->animSpeed = animSpeed;
	Spr->animTile = StartTile;
	if (animSpeed != 0)
	{
		Spr->animTimer = pd->system->getCurrentTimeMilliseconds() + (int)floor(1000 / animSpeed);

		if(EndTile < StartTile)
			Spr->animInc = -1;
		else
		{
			if(EndTile > StartTile)
				Spr->animInc = 1;
			else
				Spr->animInc = 0;
		}
	}
}

void CSprites_SetSpriteCollisionShape(CSprite* Spr, ECollisionShape shape, float width, float height, float rotation, float xoffset, float yoffset)
{
	Spr->collisionShape = shape;
	Spr->collisionWidth = width;
	Spr->collisionHeight = height;
	Spr->collisionAngle = rotation;
	Spr->collisionxoffset = xoffset;
	Spr->collisionyoffset = yoffset;
}

void CSprites_SetSpriteLocation(CSprite* Spr, Vec2F pos )
{
	Spr->x = pos.x;
	Spr->y = pos.y;
}

void CSprites_SetSpriteDepth(CSprite* Spr, int depth)
{
	Spr->depth = depth;
	CSprites_needSpriteSorting = true;
}

int CSprites_GetSpriteAnimFrameCount(CSprite* Spr)
{
	return (Spr->animEndTile > Spr->animStartTile ? Spr->animEndTile : Spr->animStartTile) - (Spr->animEndTile < Spr->animStartTile ? Spr->animEndTile : Spr->animStartTile) + 1;
}

int CSprites_GetSpriteAnimFrame(CSprite* Spr)
{
	return Spr->animTile - (Spr->animEndTile < Spr->animStartTile ? Spr->animEndTile : Spr->animStartTile);
}

SDL_Point CSprites_TileSize(CSprite* Spr)
{
	SDL_Point Result = {Spr->tileSizeX, Spr->tileSizeY};
	return Result;
}

//https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-Detection#
bool CSprites_DetectRectCircleCollsion(CSprite* SprRect, CSprite* SprCircle)
{
	Vec2F center = {SprCircle->x + SprCircle->collisionxoffset /2.0f, SprCircle->y + SprCircle->collisionyoffset / 2.0f};
	// calculate AABB info (center, half-extents)
	Vec2F aabb_half_extents = {fabsf(SprRect->collisionWidth) * fabsf(SprRect->sxscale) / 2.0f, fabsf(SprRect->collisionHeight) * fabsf(SprRect->syscale) / 2.0f};
	Vec2F aabb_center = {SprRect->x + SprRect->collisionxoffset / 2.0f, SprRect->y + SprRect->collisionyoffset / 2.0f};

	// get difference vector between both centers
	Vec2F difference;
	difference.x = center.x - aabb_center.x;
	difference.y = center.y - aabb_center.y;
	Vec2F neg_half_extends;
	neg_half_extends.x = -aabb_half_extents.x;
	neg_half_extends.y = -aabb_half_extents.y;
	Vec2F clamped = clampVec2F(difference, neg_half_extends, aabb_half_extents);
	// add clamped value to AABB_center and we get the value of box closest to circle
	Vec2F closest;
	closest.x = aabb_center.x + clamped.x;
	closest.y = aabb_center.y + clamped.y;
	// retrieve vector between center circle and closest point AABB and check if length <= radius
	difference.x = closest.x - center.x;
	difference.y = closest.y - center.y;

	return length(difference) < (fabsf(SprCircle->collisionWidth) * fabsf(SprCircle->sxscale) / 2.0f);
}

bool CSprites_DetectRectRectCollsion(CSprite* Spr, CSprite* SprOther)
{
	float widthA = (fabsf(Spr->collisionWidth) * fabsf(Spr->sxscale));
	float heightA = (fabsf(Spr->collisionHeight) * fabsf(Spr->syscale));
	float minAx = Spr->x + Spr->collisionxoffset - (fabsf(Spr->collisionWidth) * fabsf(Spr->sxscale) / 2);
	float minAy = Spr->y + Spr->collisionyoffset - (fabsf(Spr->collisionHeight) * fabsf(Spr->syscale) / 2);

	float widthB = (fabsf(SprOther->collisionWidth) * fabsf(SprOther->sxscale));
	float heightB = (fabsf(SprOther->collisionHeight) * fabsf(SprOther->syscale));
	float minBx = SprOther->x + SprOther->collisionxoffset - (fabsf(SprOther->collisionWidth) * fabsf(SprOther->sxscale) / 2);
	float minBy = SprOther->y + SprOther->collisionyoffset - (fabsf(SprOther->collisionHeight) * fabsf(SprOther->syscale) / 2);

	bool xOverlap = ((minAx >= minBx) && (minAx <= minBx + widthB)) ||
					((minBx >= minAx) && (minBx <= minAx + widthA));

	bool yOverlap = ((minAy >= minBy) && (minAy <= minBy + heightB)) ||
					((minBy >= minAy) && (minBy <= minAy + heightA));

	return xOverlap && yOverlap;
}

//takes no rotations into account !
bool CSprites_DetectSpriteCollision(CSprite* Spr, CSprite* SprOther)
{
	if((Spr == NULL) || (SprOther == NULL))
		return false;

	switch(Spr->collisionShape)
	{
		case SHAPE_BOX:
		{
			switch(SprOther->collisionShape)
			{
				case SHAPE_BOX:
					return CSprites_DetectRectRectCollsion(Spr, SprOther);
					break;
				case SHAPE_CIRCLE:
					//only works for true circles not ovals!
					if ((SprOther->collisionWidth == SprOther->collisionHeight) && (SprOther->sxscale == SprOther->syscale))
					{	// check normal rect first
						//if (CSprites_DetectRectRectCollsion(Spr, SprOther))
							return CSprites_DetectRectCircleCollsion(Spr, SprOther);
						//else
						//	return false;
					}
					else
						return false;
					break;
				default:
					return false;
			}
			break;
		}
		case SHAPE_CIRCLE:
		{
			switch(SprOther->collisionShape)
			{
				case SHAPE_BOX:
					//only works for true circles not ovals!
					if ((Spr->collisionWidth == Spr->collisionHeight) && (Spr->sxscale == Spr->syscale))
					{
						// check normal rect first
						//if (CSprites_DetectRectRectCollsion(Spr, SprOther))
							return CSprites_DetectRectCircleCollsion(SprOther, Spr);
						//else
						//	return false;
					}
					else
						return false;
					break;
				default:
					return false;
			}
			break;
		}
		default:
			return false;
	}
}

int CSprites_SpriteSlotsUsed()
{
	int c = 0;
	for (int i=0; i < SPR_Max; i++)
	{
		if(CSprites_Sprites[i] != NULL)
			c++;
	}
	return c;
}

int CSprites_SpriteSlotsMax()
{
	return SPR_Max;
}

