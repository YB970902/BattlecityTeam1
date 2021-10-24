#include "Config.h"
#include "Tank.h"
#include "Collider.h"
#include "Image.h"
#include "Ammo.h"
#include "Particle.h"

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
	for (int i = 0; i < mVecAmmo.size(); ++i)
	{
		mVecAmmo[i]->SetOwner(nullptr);
	}

	for (int i = 0; i < mVecParticle.size(); ++i)
	{
		mVecParticle[i]->SetIsEnd(true);
	}
	GameObject::Release();
}

void Tank::Update()
{
	if (KEY_MGR->IsOnceKeyDown('Q')) { TurnOnStun(); }
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
	mImage->Render(hdc, mPos.x, mPos.y,
		COLOR_START_FRAME[(int)mColor].x + (int)mDir * 2 + mCurAnim,
		COLOR_START_FRAME[(int)mColor].y + (int)mType + mStarCount);
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
		--mInfo.Health;
		if (mInfo.Health == 0) { mbIsDead = true; }
	}
	else if ((mCollisionTag == eCollisionTag::FirstPlayerTank && tag == (int)eCollisionTag::SecondPlayerAmmo) ||
		(mCollisionTag == eCollisionTag::SecondPlayerTank && tag == (int)eCollisionTag::FirstPlayerAmmo))
	{
		TurnOnStun();
	}
}

void Tank::AddAmmo(Ammo* ammo)
{
	mVecAmmo.push_back(ammo);
	ammo->SetOwner(this);
}

void Tank::OnAmmoCollided(Ammo* ammo)
{
	mVecAmmo.erase(find(mVecAmmo.begin(), mVecAmmo.end(), ammo));
	ammo->SetOwner(nullptr);
}

void Tank::OnParticleEnded(Particle* particle)
{
	for (vector<Particle*>::iterator it = mVecParticle.begin(); it != mVecParticle.end(); ++it)
	{
		if ((*it) == particle)
		{
			mVecParticle.erase(it);
			return;
		}
	}
}

void Tank::TurnOnInvencible()
{
	mbIsInvincible = true;
	mElapsedInvencibleTime = 0.0f;
	PART_MGR->CreateParticle(eParticleTag::Shield, this);
}

void Tank::TurnOnStun()
{
	mbIsStun = true;
	mbIsSparkle = true;
	mElapsedStunTime = 0.0f;
	mElapsedSparkleTime = 0.0f;
}
