#include "Config.h"
#include "GameObject.h"
#include "AITankController.h"
#include "Tank.h"
#include "AmmoSpawner.h"

HRESULT AITankController::Init()
{
	InitTimer();
	return S_OK;
}

void AITankController::Release()
{
}

void AITankController::Update()
{
	mTank->Update();
	if (mTank->IsCanFire())
	{
		mTank->AddAmmo(mAmmoSpawner->Fire(mTank->GetDirection(), mTank->GetCollisionTag(), mTank->GetInfo().MoveSpeed));
	}

	mElapsedRotateTime += DELTA_TIME;
	if (mElapsedRotateTime >= mMaxRotateTime)
	{
		mElapsedRotateTime -= mMaxRotateTime;
		InitTimer();
		Rotate();
	}
	mTank->MoveForward();
}

void AITankController::Render(HDC hdc)
{
}

void AITankController::OnCollided(eCollisionDir dir, eCollisionTag tag)
{
	// 방향 전환
	Rotate();
	mTank->OnCollided(dir, tag);
}

void AITankController::InitTimer()
{
	mMaxRotateTime = RANDOM(25, 35) * 0.1f;
}

void AITankController::Rotate()
{
	int dir = RANDOM(0, 2);
	if (dir >= (int)mTank->GetDirection()) { ++dir; }
	mTank->SetDirection((eDir)dir);
}
