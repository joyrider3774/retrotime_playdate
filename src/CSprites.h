#ifndef CSPRITES_H
#define CSPRITES_H

#include <pd_api.h>
#include <stdbool.h>
#include <string.h>
#include "CImage.h"
#include "Vec2F.h"


enum ECollisionShape
{
	SHAPE_BOX,
	SHAPE_TRIANGLE,
	SHAPE_CIRCLE
};
typedef enum ECollisionShape ECollisionShape;

struct CSprite {
		LCDBitmap *Img;
		int animInc;
		uint32_t animTimer;
		int index;
		int* imageID;
		float x;
		float y;
		float prevxscale;
		float prevyscale;
		float sxscale;
		float syscale;
		float xspeed;
		float yspeed;
		float xscale_speed;
		float yscale_speed;
		float rotation;
		int tileSizeX;
		int tileSizeY;
		int tilesX;
		int tilesY;
		int animTile;
		int animStartTile;
		int animEndTile;
		int animSpeed;
		ECollisionShape collisionShape;
		float collisionAngle;
		float collisionWidth;
		float collisionHeight;
		float collisionxoffset;
		float collisionyoffset;
		int depth;
		bool show;
		float r;
		float g;
		float b;
		float a;
		float rotation_speed;
		bool show_collision_shape;		
};
typedef struct CSprite CSprite;

void CSprites_SortSprites();
bool CSprites_DetectRectRectCollsion(CSprite* Spr, CSprite* SprOther);
bool CSprites_DetectRectCircleCollsion(CSprite* SprRect, CSprite* SprCircle);
void CSprites_Init();
void CSprites_DeInit();
void CSprites_ResetDrawTargets();
void CSprites_UpdateImage(CSprite* Spr);
CSprite* CSprites_CreateSprite();
void CSprites_RemoveSprite(CSprite* Spr);
Vec2F CSprites_GetSpriteLocation(CSprite* Spr);
void CSprites_SetForceShowCollisionShape(bool val);
void CSprites_UpdateSprites();
void CSprites_DrawSprite(CSprite* Spr);
void CSprites_DrawSprites();
int CSprites_GetSpriteAnimFrame(CSprite* Spr);
int CSprites_GetSpriteAnimFrameCount(CSprite* Spr);
void CSprites_SetSpriteImage(CSprite* Spr, int *AImageID);
void CSprites_SetSpriteImageTiles(CSprite* Spr, int *AImageID, int TilesX, int TilesY);
void CSprites_SetSpriteRotation(CSprite* Spr, float AAngle);
void CSprites_SetSpriteScale(CSprite* Spr, Vec2F AScale);
void CSprites_SetSpriteAnimation(CSprite* Spr, int StartTile, int EndTile, int animSpeed);
void CSprites_SetSpriteCollisionShape(CSprite* Spr, ECollisionShape shape, float width, float height, float rotation, float xoffset, float yoffset);
void CSprites_SetSpriteLocation(CSprite* Spr, Vec2F pos);
void CSprites_SetSpriteDepth(CSprite* Spr, int depth);
void CSprites_SetSpriteColour(CSprite* Spr, float red, float green, float blue, float alpha);
void CSprites_SetSpriteVisibility(CSprite* Spr, bool visibility);
void CSprites_SetSpriteRotationSpeed(CSprite* Spr, float rotationSpeed);
bool CSprites_DetectSpriteCollision(CSprite* Spr, CSprite* SprOther);
SDL_Point CSprites_TileSize(CSprite* Spr);
int CSprites_SpriteSlotsUsed();
int CSprites_SpriteSlotsMax();
int CSprites_UpdateImageResetsCount();
int CSprites_SpritesDrawnCount();
void CSprites_SpritesDrawnReset();
void CSprites_quickSort(CSprite* array[], int low, int high);
int CSprites_partition(CSprite* array[], int low, int high);
void CSprites_swap(CSprite* array[], int ai, int bi);

#endif