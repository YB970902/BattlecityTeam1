#pragma once
#include "GameEntity.h"

class Tank;
class Image;
class TankController;
class TankScene : public GameEntity
{
private:
	Image* mBackground = nullptr;

	Tank* mPlayer1Tank = nullptr;
	Tank* mPlayer2Tank = nullptr;

	Tank* mEnemyTank = nullptr;

	TankController* mPlayerController = nullptr;

public:
	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
};

