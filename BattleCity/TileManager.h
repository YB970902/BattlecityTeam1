#pragma once
#include "GameEntity.h"

enum class eBrokenStyle
{
	None,
	BrokenLeft,
	BrokenTop,
	BrokenRight,
	BrokenBottom
};

class Physcis;
class Tile;
struct TagEnemyInfo;
class TileManager : public GameEntity
{
private:
	const int CHANGE_WATER_TILE_TIME = 1;

	Physcis* mPhysics = nullptr;

	map<int, map<int, Tile*>> mMapTile = {};

	POINT mStartPos = {};
	POINT mBackgroundSize = {};

	bool mbLoadMap = false;
	bool mbChangeBlock = false;
	float mChangeWaterTileTime = 0.0f;

	bool mbItemBlock = false;
	bool mbIsOnProtectBlock = false;
	float mCurProtectElapsedTime = 0.0f;
	float mCurSparkleElapsedTime = 0.0f;
	float mMaxSparkleDurationTime = 0.0f;
	vector<pair<POINTFLOAT, POINT>> mVecAroundTileInfo;
	TagTile mArrTile[TILE_COUNT_X * TILE_COUNT_Y];

	TagEnemyInfo* mEnemyInfo;
public:
	HRESULT Init(POINT startPos, POINT backgroundSize);
	void Release();
	void Update();
	void Render(HDC hdc);

	void LoadMap(int loadIndex = 0);
	void LoadEnemyOrder(int loadIndex = 0);

	void SetPhysics(Physcis* physcis) { mPhysics = physcis; }

	void ProtectNexus();

	void CreateEdgeBlock();

	vector<TankSpawnInfo>* GetEnemyList();
	POINTFLOAT* GetEnemySpawnPosition(int& size);
	POINTFLOAT GetFirstPlayerSpawnPosition();
	POINTFLOAT GetSecondPlayerSpawnPosition();
	POINTFLOAT GetItemSpawnPosition();
private:
	void ClearTileMap();
	void TileDebugUpdate();
	void TurnProtectBlockImage(bool set);

	int GetFlagDetectInfo(int x, int y, eTerrain terrain);
};

