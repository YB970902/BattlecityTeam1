#pragma once
#include "GameEntity.h"
#include "CollisionChecker.h"

class Tank;
class AmmoSpawner;
class AITankController : public GameEntity, public CollisionChecker
{
private:
	Tank* mTank = nullptr;
	AmmoSpawner* mAmmoSpawner = nullptr;

	float mElapsedRotateTime = 0.0f;
	float mMaxRotateTime = 0.0f;
public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	Tank* GetTank() { return mTank; }
	void SetTank(Tank* tank) { mTank = tank; }

	void OnCollided(eCollisionDir dir, eCollisionTag tag) override;
	void SetAmmoSpawner(AmmoSpawner* spawner) { mAmmoSpawner = spawner; }

private:
	void InitTimer();
	void Rotate();
};

