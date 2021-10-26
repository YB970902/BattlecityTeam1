#pragma once
#include "GameEntity.h"
#include "Observer.h"

class Physcis;
class Tank;
class AITankController;
class AmmoSpawner;
class ItemManager;
class AITankSpawner : public GameEntity, public Observer
{
protected:
	const float MAX_PAUSE_TIME = 10.0f;

	Physcis* mPhysics = nullptr;
	AmmoSpawner* mAmmoSpawner = nullptr;
	ItemManager* mItemManager = nullptr;
	int mMaxCountInScreen = 0;
	int mSpawnedCount = 0;

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

	void SetItemManager(ItemManager* itemManager) { mItemManager = itemManager; }

	inline bool IsSpawnEnd() { return mbIsSpawnEnd; }

	virtual void OnNotify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag) override;
};

