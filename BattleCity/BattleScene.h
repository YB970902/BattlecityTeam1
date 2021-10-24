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
	Image* mBackgroundGray = nullptr;
	Image* mBackgroundBlack = nullptr;

	Image* mForResize = nullptr;
	Physcis* mPhyscis = nullptr;

	HDC mResizeHDC = {};
	float mScale = 1.0f;

	/*TagTile mBattleTile[TILE_COUNT_X * TILE_COUNT_Y];*/
	
	POINT mStartPos = {};

	bool mbLoadMap = false;
	bool mbChangeBlock = false;
	float  mChangeWaterTileTime = 0.0f;

	// 콜라이더
	//Collider* mCollider;

	map<int, map<int, Tile*>> mMapTile;

	bool mDebugMode;

public:
	virtual ~BattleScene() = default;

	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;

	void LoadMap(int loadIndex = 0);

	void ChangeBlock();
};

