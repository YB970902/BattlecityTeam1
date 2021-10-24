#include "Config.h"
#include "Ammo.h"
#include "Image.h"
#include "Tank.h"

HRESULT Ammo::Init(eDir dir, eCollisionTag tag, float speed)
{
	mDir = dir;
	mCollisionTag = tag;
	mSpeed = speed;

	mbIsFire = true;

	switch (dir)
	{
	case eDir::Left: mImage = IMG_MGR->FindImage(eImageTag::AmmoLeft); break;
	case eDir::Right: mImage = IMG_MGR->FindImage(eImageTag::AmmoRight); break;
	case eDir::Up: mImage = IMG_MGR->FindImage(eImageTag::AmmoUp); break;
	case eDir::Down: mImage = IMG_MGR->FindImage(eImageTag::AmmoDown); break;
	}

	mDirPoint = DIR_VALUE[(int)mDir];

	return S_OK;
}

void Ammo::Release()
{
	if (mOwner)
	{
		mOwner->OnAmmoCollided(this);
		mOwner = nullptr;
	}
	mbIsFire = false;
}

void Ammo::Update()
{
	if (mbIsFire)
	{
		mPos.x += mDirPoint.x * mSpeed * DELTA_TIME;
		mPos.y += mDirPoint.y * mSpeed * DELTA_TIME;

		// 맵 밖으로 나가는 아모 삭제를 위한 코드

		if (mPos.x < 0 || mPos.x > WIN_SIZE_X ||
			mPos.y < 0 || mPos.y > WIN_SIZE_Y)
		{
			OnCollided(eCollisionDir::Left, eCollisionTag::Block);
		}

		// 테스트 끝나면 꼭 지웁시당
	}
}

void Ammo::Render(HDC hdc)
{
	if (mbIsFire) { mImage->Render(hdc, mPos.x, mPos.y); }
}

void Ammo::OnCollided(eCollisionDir dir, eCollisionTag tag)
{
	Release();
}
