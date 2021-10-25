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

	bool mbIsPassedTank = true;
public:
	virtual ~AITankController() {}
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	Tank* GetTank() { return mTank; }
	void SetTank(Tank* tank) { mTank = tank; if (mTank) { Rotate(); } }

	void OnCollided(eCollisionDir dir, int tag) override;
	void SetAmmoSpawner(AmmoSpawner* spawner) { mAmmoSpawner = spawner; }

private:
	void InitTimer();
	void Rotate();
};

