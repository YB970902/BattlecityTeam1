#include "Config.h"
#include "Tank.h"
#include "Collider.h"
#include "Image.h"
#include "Ammo.h"
#include "Subject.h"

HRESULT Tank::Init(eCollisionTag colTag, eTankType type, TANK_INFO info, eTankColor color, POINTFLOAT pos, Collider* collider)
{
	mImage = IMG_MGR->FindImage(eImageTag::Tank);

	mCollisionTag = colTag;
	mType = type;
	mInfo = info;
	mColor = color;
	mDefaultColor = color;

	mPos = pos;

	mCollider = collider;

	mSubject = new Subject();

	if (mInfo.Health > 1)
	{
		mbIsChangeColor = true;
		mMainColor = eTankColor::Green;
		mSubColor = eTankColor::White;
	}

	return S_OK;
}

void Tank::Release()
{
	mSubject->Notify(this, eSubjectTag::Tank, eEventTag::Released);
	SAFE_DELETE(mSubject);

	GameObject::Release();
}

void Tank::Update()
{
	if (mbIsChangeColor)
	{
		mElapsedChangeColorTime += DELTA_TIME;
		if (mElapsedChangeColorTime >= MAX_CHANGE_COLOR_TIME)
		{
			mElapsedChangeColorTime -= MAX_CHANGE_COLOR_TIME;
			swap<eTankColor>(mMainColor, mSubColor);
		}
	}

	if (mbIsHaveItem)
	{
		mElapsedItemSparkleTime += DELTA_TIME;
		if (mElapsedItemSparkleTime >= MAX_ITEM_SPARKLE_TIME)
		{
			mElapsedItemSparkleTime -= MAX_ITEM_SPARKLE_TIME;
			swap<eTankColor>(mDefaultColor, mItemColor);
		}
	}

	if (mbIsInvincible)
	{
		mElapsedInvencibleTime += DELTA_TIME;
		if (mElapsedInvencibleTime >= INVENCIBLE_ITEM_DURATION_TIME)
		{
			mElapsedAnimTime = 0.0f;
			mbIsInvincible = false;
		}
	}

	if (mbIsStun)
	{
		mElapsedSparkleTime += DELTA_TIME;
		if (mElapsedSparkleTime >= MAX_SPARKLE_TIME)
		{
			mElapsedSparkleTime -= MAX_SPARKLE_TIME;
			mbIsSparkle = !mbIsSparkle;
		}
		mElapsedStunTime += DELTA_TIME;
		if (mElapsedStunTime >= MAX_STUN_TIME)
		{
			mbIsStun = false;
			mbIsSparkle = false;
		}
		return;
	}

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
	if (mbIsSparkle) { return; }
	if (mbIsHaveItem)
	{
		mImage->Render(hdc, mPos.x, mPos.y,
			COLOR_START_FRAME[(int)mItemColor].x + (int)mDir * 2 + mCurAnim,
			COLOR_START_FRAME[(int)mItemColor].y + (int)mType + mStarCount);
	}
	else if (mbIsChangeColor)
	{
		mImage->Render(hdc, mPos.x, mPos.y,
			COLOR_START_FRAME[(int)mMainColor].x + (int)mDir * 2 + mCurAnim,
			COLOR_START_FRAME[(int)mMainColor].y + (int)mType + mStarCount);
	}
	else
	{
		mImage->Render(hdc, mPos.x, mPos.y,
			COLOR_START_FRAME[(int)mColor].x + (int)mDir * 2 + mCurAnim,
			COLOR_START_FRAME[(int)mColor].y + (int)mType + mStarCount);
	}
	
}

void Tank::Move(eDir dir)
{
	if (mbIsStun) { return; }
	mElapsedAnimTime += DELTA_TIME;
	if (mElapsedAnimTime >= MAX_ANIM_TIME)
	{
		mElapsedAnimTime -= MAX_ANIM_TIME;
		mCurAnim ^= 0x1;
	}
	mDir = dir;
	mCollider->MoveTo(DIR_VALUE[(int)dir], mInfo.MoveSpeed * DELTA_TIME);
	this->mPos.x = mCollider->GetPlayerPos().x;
	this->mPos.y = mCollider->GetPlayerPos().y;
}

void Tank::MoveForward()
{
	Move(mDir);
}

void Tank::OnCollided(eCollisionDir dir, int tag)
{
	if (mbIsInvincible) { return; }

	if ((mCollisionTag == eCollisionTag::FirstPlayerTank && tag == (int)eCollisionTag::EnemyAmmo) ||
		(mCollisionTag == eCollisionTag::SecondPlayerTank && tag == (int)eCollisionTag::EnemyAmmo) ||
		(mCollisionTag == eCollisionTag::EnemyTank && tag == (int)eCollisionTag::FirstPlayerAmmo) ||
		(mCollisionTag == eCollisionTag::EnemyTank && tag == (int)eCollisionTag::SecondPlayerAmmo))
	{
		mCollidedTag = (eCollisionTag)tag;
		OnDamaged();
	}
	else if ((mCollisionTag == eCollisionTag::FirstPlayerTank && tag == (int)eCollisionTag::SecondPlayerAmmo) ||
		(mCollisionTag == eCollisionTag::SecondPlayerTank && tag == (int)eCollisionTag::FirstPlayerAmmo))
	{
		ChangeToStun();
	}
}

void Tank::ChangeToInvencible()
{
	mbIsInvincible = true;
	mElapsedInvencibleTime = 0.0f;
	PART_MGR->CreateParticle(eParticleTag::Shield, this);
}

void Tank::ChangeToStun()
{
	mbIsStun = true;
	mbIsSparkle = true;
	mElapsedStunTime = 0.0f;
	mElapsedSparkleTime = 0.0f;
}

void Tank::OnDamaged()
{
	if (mbIsHaveItem) { mSubject->Notify(this, eSubjectTag::Tank, eEventTag::DropItem); }
	--mInfo.Health;
	if (mInfo.Health == 0) { mbIsDead = true; }
	else
	{
		switch (mInfo.Health)
		{
		case 3:
			mMainColor = eTankColor::Yellow;
			mSubColor = eTankColor::White;
			break;
		case 2:
			mMainColor = eTankColor::Yellow;
			mSubColor = eTankColor::Green;
			break;
		case 1:
			mbIsChangeColor = false;
			break;
		}
	}
}

void Tank::OnNotify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag)
{
	switch (subjectTag)
	{
	case eSubjectTag::Particle:
		switch (eventTag)
		{
		case eEventTag::Added:
			mSubject->AddObserver(dynamic_cast<Observer*>(obj));
			break;
		case eEventTag::Released:
			mSubject->RemoveObserver(dynamic_cast<Observer*>(obj));
			break;
		}
		break;
	case eSubjectTag::Ammo:
		switch (eventTag)
		{
		case eEventTag::Added:
			mSubject->AddObserver(dynamic_cast<Observer*>(obj));
			++mFiredAmmoCount;
			break;
		case eEventTag::Released:
			mSubject->RemoveObserver(dynamic_cast<Observer*>(obj));
			--mFiredAmmoCount;
			break;
		}
		break;
	}
}
