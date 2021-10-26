#pragma once
#include "GameEntity.h"

class Tank;
class Image;
class AmmoSpawner;
class TankController;
class TankSpawner;
class AITankSpawner;
class Physcis;
class TankScene : public GameEntity
{
private:
	Image* mBackground = nullptr;

	AmmoSpawner* mAmmoSpawner = nullptr;
	TankController* mFirstPlayerController = nullptr;
	TankSpawner* mFirstPlayerTankSpawner = nullptr;

	TankController* mSecondPlayerController = nullptr;
	TankSpawner* mSecondPlayerTankSpawner = nullptr;
	AITankSpawner* mAISpawner = nullptr;

	Physcis* mPhysics = nullptr;

	bool mbIsDebugMode = false;
public:
	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
};

