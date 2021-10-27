#include "Config.h"
#include "TankSpawner.h"
#include "Tank.h"
#include "TankController.h"
#include "GameManager.h"
#include "Subject.h"

HRESULT TankSpawner::Init(GameManager* gameManager, SPAWN_INFO info, int leftCount, POINTFLOAT spawnPos)
{
	mGameManager = gameManager;
	mInfo = info;
	mLeftCount = leftCount;
	mSpawnPosition = spawnPos;
	mSubject = new Subject();
	PART_MGR->CreateParticle(eParticleTag::Spawn, mSpawnPosition);
	return S_OK;
}

void TankSpawner::Release()
{
	SAFE_DELETE(mSubject);
	SAFE_RELEASE(mController);
	if (mCurTank)
	{
		mGameManager->DestroyCollider(mCurTank->GetCollider());
		SAFE_RELEASE(mCurTank);
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
			PART_MGR->CreateParticle(eParticleTag::BigBoom, mCurTank->GetPosition());
			mGameManager->DestroyCollider(mCurTank->GetCollider());
			SAFE_RELEASE(mCurTank);
			mCurTank = nullptr;
			if (mController) { mController->SetTank(nullptr); }
			mElapsedSpawnTime = 0.0f;
			if (mLeftCount <= 0 && !mbIsSpawnEnd)
			{
				mbIsSpawnEnd = true;
				if (mSubject) { mSubject->Notify(this, eSubjectTag::Player, eEventTag::PlayerDown); }
			}
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
			mCurTank->Init(mInfo.CollisionTag,
				mInfo.Type,
				mInfo.TankInfo,
				mInfo.Color,
				mSpawnPosition,
				mGameManager->CreateCollider(mSpawnPosition, TANK_BODY_SIZE, mCurTank, mInfo.CollisionTag));
			mCurTank->ChangeToInvencible();
			if (mController) { mController->SetTank(mCurTank); }
		}
	}
}

void TankSpawner::Render(HDC hdc)
{
	SAFE_RENDER(mCurTank);
}