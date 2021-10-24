#include "Tank.h"
#include "Image.h"
#include "Collider.h"
#include "Ammo.h"

HRESULT Tank::Init(eCollisionTag colTag, eTankType type, TANK_INFO info, eTankColor color, POINTFLOAT pos, Collider* collider)
{
	mImage = IMG_MGR->FindImage(eImageTag::Tank);

	mCollisionTag = colTag;
	mType = type;
	mInfo = info;
	mColor = color;

	mPos = pos;

	mCollider = collider;

	return S_OK;
}

void Tank::Release()
{
	GameObject::Release();
}

void Tank::Update()
{
	if (mbIsCanFire) { return; }
	mElapsedFireTime += DELTA_TIME;
	if (mElapsedFireTime >= mInfo.AttackSpeed)
	{
		mElapsedFireTime = 0.0f;
		mbIsCanFire = true;
	}
}

void Tank::Render(HDC hdc)
{
	if (mbIsDead) { return; }
	mImage->Render(hdc, mPos.x, mPos.y,
		COLOR_START_FRAME[(int)mColor].x + (int)mDir * 2 + mCurAnim,
		COLOR_START_FRAME[(int)mColor].y + (int)mType + mStarCount);
}

void Tank::Move(eDir dir)
{
	mElapsedAnimTime += DELTA_TIME;
	if (mElapsedAnimTime >= MAX_ANIM_TIME)
	{
		mElapsedAnimTime -= MAX_ANIM_TIME;
		mCurAnim ^= 0x1;
	}
	mDir = dir;
	// dir을 활용한 이동
	mPos.x = mPos.x + DIR_VALUE[(int)mDir].x * mInfo.MoveSpeed * DELTA_TIME;
	mPos.y = mPos.y + DIR_VALUE[(int)mDir].y * mInfo.MoveSpeed * DELTA_TIME;
	//mCollider->MoveTo(DIR_VALUE[(int)mDir], mInfo.MoveSpeed * DELTA_TIME);
	//mPos =  mCollider->GetPlayerPos();
}

void Tank::MoveForward()
{
	Move(mDir);
}

void Tank::OnCollided(eCollisionDir dir, eCollisionTag tag)
{
	if ((mCollisionTag == eCollisionTag::PlayerTank && tag == eCollisionTag::EnemyAmmo) ||
		(mCollisionTag == eCollisionTag::EnemyTank && tag == eCollisionTag::PlayerAmmo))
	{
		--mInfo.Health;
		if (mInfo.Health == 0) { mbIsDead = true; }
	}
}

void Tank::AddAmmo(Ammo* ammo)
{
	mVecAmmo.push_back(ammo);
	ammo->SetOwner(this);
	ammo->SetPosition(mPos);
}

void Tank::OnAmmoCollided(Ammo* ammo)
{
	mVecAmmo.erase(find(mVecAmmo.begin(), mVecAmmo.end(), ammo));
	ammo->SetOwner(nullptr);
}
