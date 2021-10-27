#pragma once
#include "GameEntity.h"
#include "Observer.h"

class Tank;
class AITankController;
class GameManager;
class AITankSpawner : public GameEntity, public Observer
{
protected:
	const float MAX_PAUSE_TIME = 10.0f;

	GameManager* mGameManager = nullptr;
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
	HRESULT Init(GameManager* gameManager, int maxCountInScreen);
	void Release();
	void Update();
	void Render(HDC hdc);

	void SetSpawnPosition(POINTFLOAT* arrPos, int maxCount);
	void AddTankSpawnInfo(TankSpawnInfo info);

	void PauseAll();
	void DestroyAll();

	inline bool IsSpawnEnd() { return mbIsSpawnEnd; }

	virtual void OnNotify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag) override;
};

