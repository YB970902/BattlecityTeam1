#pragma once
#include "GameEntity.h"

class Tank;
class GameManager;
class TankController;
class Subject;
class TankSpawner : public GameEntity
{
protected:
	GameManager* mGameManager = nullptr;
	TankController* mController = nullptr;
	Subject* mSubject = nullptr;

	Tank* mCurTank = nullptr;
	SPAWN_INFO mInfo = {};
	int mLeftCount = 0;

	float mElapsedSpawnTime = 0.0f;

	POINTFLOAT mSpawnPosition = {};

	bool mbIsSpawnEnd = false;

public:
	virtual ~TankSpawner() {}
	HRESULT Init(GameManager* gameManager, SPAWN_INFO info, int leftCount, POINTFLOAT spawnPos);
	void Release();
	void Update();
	void Render(HDC hdc);

	inline Subject* GetSubject() { return mSubject; }
	inline int GetLeftLife() { return mLeftCount; }
	inline void AddLife() { mLeftCount++; }
	inline POINTFLOAT GetSpawnPosition() { return mSpawnPosition; }
	inline bool IsSpawnEnd() { return mbIsSpawnEnd; }

	inline void SetController(TankController* controller) { mController = controller; }
};