#include "Config.h"
#include "TankSpawner.h"
#include "Tank.h"
#include "Physcis.h"
#include "TankController.h"
#include "Collider.h"

HRESULT TankSpawner::Init(Physcis* physics, SPAWN_INFO info, int leftCount, POINTFLOAT spawnPos)
{
	mPhysics = physics;
	mInfo = info;
	mLeftCount = leftCount;
	mSpawnPosition = spawnPos;
	PART_MGR->CreateParticle(eParticleTag::Spawn, mSpawnPosition);
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
			mElapsedSpawnTime = 0.0f;
			if (mLeftCount <= 0) { mbIsSpawnEnd = true; }
			else { PART_MGR->CreateParticle(eParticleTag::Spawn, mSpawnPosition); }
		}
	}
	else
	{
		if (mbIsSpawnEnd) { return; }
		mElapsedSpawnTime += DELTA_TIME;
		if (mElapsedSpawnTime >= TANK_SPAWNING_TIME)
		{
			--mLeftCount;
			mElapsedSpawnTime = 0.0f;
			mCurTank = new Tank();
			Collider* collider = mPhysics->CreateCollider(mSpawnPosition, TANK_BODY_SIZE, mCurTank, mInfo.CollisionTag);
			mCurTank->Init(mInfo.CollisionTag,
				mInfo.Type,
				mInfo.TankInfo,
				mInfo.Color,
				mSpawnPosition,
				collider);
			mCurTank->TurnOnInvencible();
			if (mController) { mController->SetTank(mCurTank); }
		}
	}
}

void TankSpawner::Render(HDC hdc)
{
	SAFE_RENDER(mCurTank);
}