#pragma once
#include "GameEntity.h"

typedef struct TankSpawnInfo
{
	TankSpawnInfo() {}
	TankSpawnInfo(eCollisionTag collisionTag, eTankType type, eTankColor color, TANK_INFO tankInfo) :
		CollisionTag(collisionTag), Type(type), Color(color), TankInfo(tankInfo) { }

	eCollisionTag CollisionTag = eCollisionTag::Block;
	eTankType Type = eTankType::Player;
	eTankColor Color = eTankColor::Yellow;
	TANK_INFO TankInfo = NORMAL_TANK_INFO;
} SPAWN_INFO;

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
	float mMaxSpawnTime = 0.0f;

	POINTFLOAT mSpawnPosition = {};

	bool mbIsSpawnEnd = false;

public:
	HRESULT Init(Physcis* physics, SPAWN_INFO info, int leftCount, float maxSpawnTime, POINTFLOAT spawnPos);
	void Release();
	void Update();
	void Render(HDC hdc);

	inline bool IsSpawnEnd() { return mbIsSpawnEnd; }

	inline void SetController(TankController* controller) { mController = controller; }
};