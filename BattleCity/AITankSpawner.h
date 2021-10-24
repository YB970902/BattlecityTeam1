#pragma once
#include "GameEntity.h"

class Physcis;
class Tank;
class AITankController;
class AmmoSpawner;
class AITankSpawner : public GameEntity
{
protected:
	Physcis* mPhysics = nullptr;
	AmmoSpawner* mAmmoSpawner = nullptr;
	int mMaxCountInScreen = 0;

	vector<Tank*> mVecTank;
	vector<SPAWN_INFO> mInfo;
	vector<AITankController*> mVecTankController;

	float mElapsedSpawnTime = 0.0f;

	POINTFLOAT* mArrSpawnPosition = nullptr;
	int mMaxSpawnPosition = 0;
	int mCurSpawnPositionIndex = 0;

	bool mbIsSpawning = false;
	bool mbIsSpawnEnd = false;

public:
	HRESULT Init(Physcis* physics, int maxCountInScreen);
	void Release();
	void Update();
	void Render(HDC hdc);

	void SetAmmoSpawner(AmmoSpawner* spawner) { mAmmoSpawner = spawner; }
	void SetSpawnPosition(POINTFLOAT* arrPos, int maxCount);
	void AddTankSpawnInfo(TankSpawnInfo info);

	inline bool IsSpawnEnd() { return mbIsSpawnEnd; }
};

