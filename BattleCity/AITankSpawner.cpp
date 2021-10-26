#include "Config.h"
#include "Tank.h"
#include "TankSpawner.h"
#include "AITankSpawner.h"
#include "Physcis.h"
#include "AITankController.h"
#include "AmmoSpawner.h"
#include "ItemManager.h"
#include "Subject.h"

HRESULT AITankSpawner::Init(Physcis* physics, int maxCountInScreen)
{
	mPhysics = physics;
	mMaxCountInScreen = maxCountInScreen;
	return S_OK;
}

void AITankSpawner::Release()
{
	if (mArrSpawnPosition)
	{
		if (mMaxSpawnPosition > 1) { delete[] mArrSpawnPosition; }
		else { delete mArrSpawnPosition; }
	}

	Tank* tank;
	for (vector<Tank*>::iterator it = mVecTank.begin(); it != mVecTank.end();)
	{
		tank = (*it);
		it = mVecTank.erase(it);
		mPhysics->DestroyCollider(tank->GetCollider());
		SAFE_RELEASE(tank);
	}

	mInfo.clear();

	AITankController* controller;
	for (vector<AITankController*>::iterator it = mVecTankController.begin(); it != mVecTankController.end();)
	{
		controller = (*it);
		it = mVecTankController.erase(it);
		SAFE_RELEASE(controller);
	}
}

void AITankSpawner::Update()
{
	if (!mbIsPause)
	{
		for (int i = 0; i < mVecTankController.size(); ++i)
		{
			mVecTankController[i]->Update();
		}
	}

	for (vector<Tank*>::iterator tankIt = mVecTank.begin(); tankIt != mVecTank.end();)
	{
		if ((*tankIt)->IsDead())
		{
			Tank* delTank = (*tankIt);
			PART_MGR->CreateParticle(eParticleTag::BigBoom, delTank->GetPosition());
			mPhysics->DestroyCollider(delTank->GetCollider());
			tankIt = mVecTank.erase(tankIt);

			for (vector<AITankController*>::iterator ctrlIt = mVecTankController.begin(); ctrlIt != mVecTankController.end(); ++ctrlIt)
			{
				if ((*ctrlIt)->GetTank() == delTank)
				{
					AITankController* controller = (*ctrlIt);
					mVecTankController.erase(ctrlIt);
					SAFE_RELEASE(controller);
					break;
				}
			}

			SAFE_RELEASE(delTank);
		}
		else { ++tankIt; }
	}

	if (mbIsPause)
	{
		mElapsedPauseTime += DELTA_TIME;
		if (mElapsedPauseTime >= MAX_PAUSE_TIME)
		{
			mElapsedPauseTime = 0.0f;
			mbIsPause = false;
		}
		else { return; }
	}

	if (mbIsSpawning)
	{
		mElapsedSpawnTime += DELTA_TIME;
		if (mElapsedSpawnTime >= TANK_SPAWNING_TIME)
		{
			mbIsSpawning = false;
			mElapsedSpawnTime = 0.0f;
			AITankController* controller = new AITankController();
			controller->Init();

			Tank* newTank = new Tank();
			SPAWN_INFO info = mInfo.front();
			mInfo.erase(mInfo.begin());
			if (mInfo.size() <= 0) { mbIsSpawnEnd = true; }
			newTank->Init(info.CollisionTag, info.Type, info.TankInfo, info.Color, mArrSpawnPosition[mCurSpawnPositionIndex],
				mPhysics->CreateCollider(mArrSpawnPosition[mCurSpawnPositionIndex], TANK_BODY_SIZE, controller, eCollisionTag::PassedEnemyTank));
			controller->SetTank(newTank);
			controller->SetAmmoSpawner(mAmmoSpawner);
			switch (++mSpawnedCount)
			{
			case 4:
			case 11:
			case 18:
				newTank->GetSubject()->AddObserver(this);
				newTank->SetIsHaveItem(true);
				break;
			}

			mVecTank.push_back(newTank);
			mVecTankController.push_back(controller);

			if (++mCurSpawnPositionIndex >= mMaxSpawnPosition) { mCurSpawnPositionIndex = 0; }
		}
	}
	else if (mVecTank.size() < mMaxCountInScreen && mInfo.size() > 0)
	{
		mbIsSpawning = true;
		mElapsedSpawnTime = 0.0f;
		PART_MGR->CreateParticle(eParticleTag::Spawn, mArrSpawnPosition[mCurSpawnPositionIndex]);
	}
}

void AITankSpawner::Render(HDC hdc)
{
	for (int i = 0; i < mVecTank.size(); ++i)
	{
		mVecTank[i]->Render(hdc);
	}
}

void AITankSpawner::SetSpawnPosition(POINTFLOAT* arrPos, int maxCount)
{
	if (mArrSpawnPosition)
	{
		if (mMaxSpawnPosition > 1) { delete[] mArrSpawnPosition; }
		else { delete mArrSpawnPosition; }
	}

	mArrSpawnPosition = arrPos;
	mMaxSpawnPosition = maxCount;
}

void AITankSpawner::AddTankSpawnInfo(TankSpawnInfo info)
{
	mInfo.push_back(info);
}

void AITankSpawner::PauseAll()
{
	mbIsPause = true;
	mElapsedPauseTime = 0.0f;
}

void AITankSpawner::DestroyAll()
{
	Tank* tank;
	for (vector<Tank*>::iterator it = mVecTank.begin(); it != mVecTank.end();)
	{
		tank = (*it);
		PART_MGR->CreateParticle(eParticleTag::SmallBoom, tank->GetPosition());
		PART_MGR->CreateParticle(eParticleTag::BigBoom, tank->GetPosition());
		it = mVecTank.erase(it);
		mPhysics->DestroyCollider(tank->GetCollider());
		SAFE_RELEASE(tank);
	}

	AITankController* controller;
	for (vector<AITankController*>::iterator it = mVecTankController.begin(); it != mVecTankController.end();)
	{
		controller = (*it);
		it = mVecTankController.erase(it);
		SAFE_RELEASE(controller);
	}
}

void AITankSpawner::OnNotify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag)
{
	if (subjectTag == eSubjectTag::Tank && eventTag == eEventTag::DropItem)
	{
		mItemManager->CreateItem((eItemTag)RANDOM(1, 6));
	}
}