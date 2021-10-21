#pragma once
#include "GameEntity.h"
#include "CollisionChecker.h"

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
class MapEditor;
class Physcis;
class BattleScene : public GameEntity
{
private:
	Image* mBackgroundGray;
	Image* mBackgroundBlack;

	Image* mForResize;
	HDC mResizeHDC;
	float mScale = 1.0f;

	/*TagTile mBattleTile[TILE_COUNT_X * TILE_COUNT_Y];*/
	
	POINT mStartPos;

	bool mbLoadMap;
	bool mbChangeBlock;
	float  mChangeWaterTileTime;

	// 콜라이더
	//Collider* mCollider;
	Physcis* mPhyscis;

	map<int, map<int, Tile*>> mMapTile;

	bool mDebugMode;

public:
	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;

	void LoadMap(int loadIndex = 0);

	void ChangeBlock();
};

