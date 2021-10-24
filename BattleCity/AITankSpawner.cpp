#include "Config.h"
#include "Tank.h"
#include "TankSpawner.h"
#include "AITankSpawner.h"
#include "Physcis.h"
#include "AITankController.h"
#include "AmmoSpawner.h"

HRESULT AITankSpawner::Init(Physcis* physics, float maxSpawnTime, int maxCountInScreen)
{
	mPhysics = physics;
	mMaxSpawnTime = maxSpawnTime;
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
	for (int i = 0; i < mVecTankController.size(); ++i)
	{
		mVecTankController[i]->Update();
	}

	// 탱크 테스트중 맵 밖으로 나가는거 막기위한 코드

	for (int i = 0; i < mVecTank.size(); i++)
	{
		if (mVecTank[i]->GetPosition().x < 0) { mVecTankController[i]->OnCollided(eCollisionDir::Left, eCollisionTag::Block); }
		else if (mVecTank[i]->GetPosition().x > WIN_SIZE_X) { mVecTankController[i]->OnCollided(eCollisionDir::Right, eCollisionTag::Block); }
		else if (mVecTank[i]->GetPosition().y < 0) { mVecTankController[i]->OnCollided(eCollisionDir::Top, eCollisionTag::Block); }
		else if (mVecTank[i]->GetPosition().y > WIN_SIZE_Y) { mVecTankController[i]->OnCollided(eCollisionDir::Bottom, eCollisionTag::Block); }
	}

	// 테스트 끝나면 꼭 지웁시당

	for(vector<Tank*>::iterator tankIt = mVecTank.begin(); tankIt != mVecTank.end();)
	{
		if ((*tankIt)->IsDead())
		{
			Tank* delTank = (*tankIt);
			mPhysics->DestroyCollider((*tankIt)->GetCollider());
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

	if (mbIsSpawning)
	{
		mElapsedSpawnTime += DELTA_TIME;
		if (mElapsedSpawnTime >= mMaxSpawnTime)
		{
			mbIsSpawning = false;
			mElapsedSpawnTime = 0.0f;
			AITankController* controller = new AITankController();
			controller->Init();

			Tank* newTank = new Tank();
			SPAWN_INFO info = mInfo.front();
			mInfo.erase(mInfo.begin());
			newTank->Init(info.CollisionTag, info.Type, info.TankInfo, info.Color, mArrSpawnPosition[mCurSpawnPositionIndex],
				mPhysics->CreateCollider(mArrSpawnPosition[mCurSpawnPositionIndex], TANK_BODY_SIZE, controller, info.CollisionTag));
			controller->SetTank(newTank);
			controller->SetAmmoSpawner(mAmmoSpawner);

			mVecTank.push_back(newTank);
			mVecTankController.push_back(controller);

			if (++mCurSpawnPositionIndex >= mMaxSpawnPosition) { mCurSpawnPositionIndex = 0; }
		}
	}
	else if (mVecTank.size() < mMaxCountInScreen && mInfo.size() > 0)
	{
		mbIsSpawning = true;
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
