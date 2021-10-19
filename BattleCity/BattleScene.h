#pragma once
#include "GameEntity.h"
#include "MapEditor.h"

#define BROKEN_LEFT   0x1100
#define BROKEN_TOP    0x0110
#define BROKEN_RIGHT  0x0011
#define BROKEN_BOTTOM 0x1001

#define CHANGE_WATER_TILE_TIME 1

enum class eBrokenStyle
{
	None,
	BrokenLeft,
	BrokenTop,
	BrokenRight,
	BrokenBottom
};

class Image;
class MapEditor;
class BattleScene : public GameEntity
{
private:
	Image* mBackgroundGray;
	Image* mBackgroundBlack;

	TagTile mBattleTile[TILE_COUNT_X * TILE_COUNT_Y];
	
	POINT mStartPos;

	bool mbLoadMap;
	bool mbChangeBlock;
	float  mChangeWaterTileTime;

public:
	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;

	void LoadMap(int loadIndex = 0);

	void ChangeBlock();
};

