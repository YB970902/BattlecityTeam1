#pragma once
#include "GameEntity.h"
#include "CollisionChecker.h"

class Tank;
class TankController : public GameEntity, public CollisionChecker
{
private:
	Tank* mTank = nullptr;

	int mLeftKey = 0;
	int mRightKey = 0;
	int mUpKey = 0;
	int mDownKey = 0;
public:
	HRESULT Init(Tank* tank, int leftKey, int rightKey, int upKey, int downKey);
	void Release();
	void Update();
	void Render(HDC hdc);

	virtual void OnCollided(eCollisionDir dir, eCollisionTag tag) override;
};

