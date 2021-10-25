#include "Config.h"
#include "Ammo.h"
#include "Image.h"
#include "Tank.h"
#include "Collider.h"

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
	mImage = nullptr;
	mbIsDead = true;
}

void Ammo::Update()
{
	if (mbIsFire)
	{
		mCollider->MoveTo(mDirPoint, mSpeed * DELTA_TIME);
		mPos.x += mDirPoint.x * mSpeed * DELTA_TIME;
		mPos.y += mDirPoint.y * mSpeed * DELTA_TIME;
	}
}

void Ammo::Render(HDC hdc)
{
	if (mbIsFire) 
	{
		mImage->Render(hdc, mPos.x, mPos.y);
	}
}

void Ammo::OnCollided(eCollisionDir dir, int tag)
{
	Release();
	PART_MGR->CreateParticle(eParticleTag::SmallBoom, mPos);
}

void Ammo::OnNotify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag)
{
}