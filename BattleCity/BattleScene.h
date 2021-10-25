#pragma once
#include "GameEntity.h"

class Tile;
class Physcis;
class TileManager;
class TankController;
class TankSpawner;
class BattleScene : public GameEntity
{
private:
	Image* mBackgroundGray;
	Image* mBackgroundBlack;

	Physcis* mPhysics = nullptr;
	TileManager* mTileManager;

	TankController* mFirstPlayerController = nullptr;
	TankSpawner* mFirstPlayerSpawner = nullptr;

	TankController* mSecondPlayerController = nullptr;
	TankSpawner* mSecondPlayerSpawner = nullptr;

	POINT mStartPos = {};

public:
	virtual ~BattleScene() = default;

	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;
};

