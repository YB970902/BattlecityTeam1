#pragma once
#include "GameEntity.h"

class Tile;
class Physcis;
class TileManager;
class BattleScene : public GameEntity
{
private:
	Image* mBackgroundGray;
	Image* mBackgroundBlack;

	Physcis* mPhyscis = nullptr;
	TileManager* mTileManager;

	POINT mStartPos = {};

public:
	virtual ~BattleScene() = default;

	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;
};

