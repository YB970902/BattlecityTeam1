#pragma once
#include "GameEntity.h"

class Tank;
class AmmoSpawner;
class TankController : public GameEntity
{
private:
	AmmoSpawner* mAmmoSpawner;
	Tank* mTank = nullptr;

	int mLeftKey = 0;
	int mRightKey = 0;
	int mUpKey = 0;
	int mDownKey = 0;
	int mFireKey = 0;
public:
	HRESULT Init(int leftKey, int rightKey, int upKey, int downKey, int fireKey);
	void Release();
	void Update();
	void Render(HDC hdc);

	inline void SetTank(Tank* tank) { mTank = tank; }
	inline void SetAmmoSpawner(AmmoSpawner* spawner) { mAmmoSpawner = spawner; }
};

