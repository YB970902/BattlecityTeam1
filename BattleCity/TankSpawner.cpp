#include "Tank.h"
#include "TankSpawner.h"
#include "Physcis.h"
#include "TankController.h"

HRESULT TankSpawner::Init(Physcis* physics, SPAWN_INFO info, int leftCount, float maxSpawnTime, POINTFLOAT spawnPos)
{
	mPhysics = physics;
	mInfo = info;
	mLeftCount = leftCount;
	mMaxSpawnTime = maxSpawnTime;
	mSpawnPosition = spawnPos;
	return S_OK;
}

void TankSpawner::Release()
{
	SAFE_RELEASE(mController);
	if (mCurTank)
	{
		mPhysics->DestroyCollider(mCurTank->GetCollider());
		SAFE_DELETE(mCurTank);
	}
}

void TankSpawner::Update()
{
	if (mbIsSpawnEnd) { return; }
	
	if (mCurTank)
	{
		SAFE_UPDATE(mController);
		if (mCurTank->IsDead())
		{
			mPhysics->DestroyCollider(mCurTank->GetCollider());
			SAFE_RELEASE(mCurTank);
			mCurTank = nullptr;
			if (mController) { mController->SetTank(nullptr); }
		}
	}
	else
	{
		if (mLeftCount <= 0)
		{
			mbIsSpawnEnd = true;
			return;
		}
		mElapsedSpawnTime += DELTA_TIME;
		if (mElapsedSpawnTime >= mMaxSpawnTime)
		{
			--mLeftCount;
			mElapsedSpawnTime = 0.0f;
			mCurTank = new Tank();
			mCurTank->Init(mInfo.CollisionTag,
				mInfo.Type,
				mInfo.TankInfo,
				mInfo.Color,
				mSpawnPosition,
				mPhysics->CreateCollider(mSpawnPosition, TANK_BODY_SIZE, mCurTank, mInfo.CollisionTag));
			if (mController) { mController->SetTank(mCurTank); }
		}
	}
}

void TankSpawner::Render(HDC hdc)
{
	SAFE_RENDER(mCurTank);
}