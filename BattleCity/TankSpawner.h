#pragma once
#include "GameEntity.h"

class Tank;
class Physcis;
class TankController;
class TankSpawner : public GameEntity
{
protected:
	Physcis* mPhysics = nullptr;
	TankController* mController = nullptr;

	Tank* mCurTank = nullptr;
	SPAWN_INFO mInfo = {};
	int mLeftCount = 0;

	float mElapsedSpawnTime = 0.0f;

	POINTFLOAT mSpawnPosition = {};

	bool mbIsSpawnEnd = false;

public:
	virtual ~TankSpawner() {}
	HRESULT Init(Physcis* physics, SPAWN_INFO info, int leftCount, POINTFLOAT spawnPos);
	void Release();
	void Update();
	void Render(HDC hdc);

	inline bool IsSpawnEnd() { return mbIsSpawnEnd; }

	inline void SetController(TankController* controller) { mController = controller; }
};