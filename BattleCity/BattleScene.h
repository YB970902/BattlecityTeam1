#pragma once
#include "GameEntity.h"

class Tile;
class Physcis;
class TileManager;
class TankController;
class TankSpawner;
class AITankSpawner;
class AmmoSpawner;
class BattleScene : public GameEntity
{
private:
	Image* mBackgroundGray;
	Image* mBackgroundBlack;

	Physcis* mPhysics = nullptr;
	TileManager* mTileManager = nullptr;
	AmmoSpawner* mAmmoSpawner = nullptr;

	TankController* mFirstPlayerController = nullptr;
	TankSpawner* mFirstPlayerSpawner = nullptr;

	TankController* mSecondPlayerController = nullptr;
	TankSpawner* mSecondPlayerSpawner = nullptr;

	AITankSpawner* mAISpawner = nullptr;

	POINT mStartPos = {};

	bool mbIsDebugMode = false;

public:
	virtual ~BattleScene() = default;

	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;
};

