#pragma once
#include "GameEntity.h"

#define CHANGE_WATER_TILE_TIME 1

class Tile;
enum class eBrokenStyle
{
	None,
	BrokenLeft,
	BrokenTop,
	BrokenRight,
	BrokenBottom
};


class Image;
class Physcis;
class MapEditor;
struct TagEnemyInfo;
class BattleScene : public GameEntity
{
private:
	Image* mBackgroundGray;
	Image* mBackgroundBlack;
	Image* mForResize;

	HDC mResizeHDC;
	float mScale = 1.0f;
	
	POINT mStartPos = {};

	bool mbLoadMap = false;
	bool mbChangeBlock = false;
	float  mChangeWaterTileTime = 0.0f;

	Physcis* mPhyscis = nullptr;

	map<int, map<int, Tile*>> mMapTile = {};

	bool mbItemBlock = 0;
	float mItemTime = 0;

	TagEnemyInfo* mEnemyInfo;


public:
	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;

	void LoadMap(int loadIndex = 0);

	void ClearTileMap();

	void LoadEnemyOrder(int loadIndex = 0);
};

