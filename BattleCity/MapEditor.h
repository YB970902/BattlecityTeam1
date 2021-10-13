#pragma once
#include "GameEntity.h"


#define TILE_SIZE	16

#define TILE_COUNT_X 26
#define TILE_COUNT_Y 26

#define SAMPLETILE_COUNT_X 11
#define SAMPLETILE_COUNT_Y 11

enum class eTerrain { None, };

struct SampleAreaMouse
{
	POINT Start;
	//POINT Current;
	POINT End;

	SampleAreaMouse()
	{
		Start	= { 0 , 0 };
		//Current;
		End		= { 0 , 0 };
	}
};

struct TagTile
{
	RECT		TileShape;
	POINT		TilePos;
	eTerrain	Terrain;
};

class Image;
class MapEditor : public GameEntity
{
private:
	Image*	mTileImage;
	TagTile mTileInfo[TILE_COUNT_X * TILE_COUNT_Y];
	TagTile mTileInfoSample[TILE_COUNT_X * TILE_COUNT_Y];

	RECT mDrawingArea;
	POINT mClickedStartIndex;
	POINT mClickedArea;

	bool mbIsClicked = false;

	RECT mSampleAreaShape;
	SampleAreaMouse sampleAreaMouse;

public:
	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
};

