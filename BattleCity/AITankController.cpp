#include "Config.h"
#include "GameObject.h"
#include "AITankController.h"
#include "Tank.h"
#include "Ammo.h"
#include "AmmoSpawner.h"
#include "Collider.h"

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
	if (mbIsPassedTank && mTank->GetCollider()->IsCollided() == false)
	{
		mbIsPassedTank = false;
		mTank->GetCollider()->SetTag(mTank->GetCollisionTag());
	}
	mTank->Update();
	if (mTank->IsCanFire())
	{
		mAmmoSpawner->Fire(mTank->GetDirection(),
			eCollisionTag::EnemyAmmo,
			mTank->GetInfo().AmmoSpeed, mTank->GetBarrelPosition())->AddObserver(mTank);
		mTank->SetIsCanFire(false);
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

void AITankController::OnCollided(eCollisionDir dir, int tag)
{
	if (!IS_PLAYER_AMMO(tag)) { Rotate(); }
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
