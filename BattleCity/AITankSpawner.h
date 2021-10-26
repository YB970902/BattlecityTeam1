#pragma once
#include "GameEntity.h"

class Physcis;
class Tank;
class AITankController;
class AmmoSpawner;
class AITankSpawner : public GameEntity
{
protected:
	const float MAX_PAUSE_TIME = 10.0f;

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

	bool mbIsPause = false;
	float mElapsedPauseTime = 0.0f;
public:
	virtual ~AITankSpawner() {}
	HRESULT Init(Physcis* physics, int maxCountInScreen);
	void Release();
	void Update();
	void Render(HDC hdc);

	void SetAmmoSpawner(AmmoSpawner* spawner) { mAmmoSpawner = spawner; }
	void SetSpawnPosition(POINTFLOAT* arrPos, int maxCount);
	void AddTankSpawnInfo(TankSpawnInfo info);

	void PauseAll();
	void DestroyAll();

	inline bool IsSpawnEnd() { return mbIsSpawnEnd; }
};

