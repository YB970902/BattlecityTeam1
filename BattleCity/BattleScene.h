#pragma once
#include "GameEntity.h"

class Tile;
class Physcis;
class TileManager;
class TankController;
class TankSpawner;
class AITankSpawner;
class AmmoSpawner;
class ItemManager;
class GameManager;
class BattleScene : public GameEntity
{
private:
	GameManager* mGameManager;

public:
	virtual ~BattleScene() = default;

	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;
};

