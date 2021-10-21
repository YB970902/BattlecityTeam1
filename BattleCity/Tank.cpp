#include "Tank.h"
#include "Image.h"

HRESULT Tank::Init(eCollisionTag colTag, eTankType type, TANK_INFO info, eTankColor color, float x, float y)
{
	mImage = IMG_MGR->FindImage(eImageTag::Tank);

	mColTag = colTag;
	mType = type;
	mInfo = info;
	mColor = color;

	mPos.x = x;
	mPos.y = y;

	return S_OK;
}

void Tank::Release()
{
}

void Tank::Update()
{
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
	mPos.x += DIR_VALUE[(int)dir].x * mInfo.MoveSpeed * DELTA_TIME;
	mPos.y += DIR_VALUE[(int)dir].y * mInfo.MoveSpeed * DELTA_TIME;
}

void Tank::OnCollided(eCollisionDir dir, eCollisionTag tag)
{
	if ((mColTag == eCollisionTag::PlayerTank && tag == eCollisionTag::EnemyAmmo) || 
		(mColTag == eCollisionTag::EnemyTank && tag == eCollisionTag::PlayerAmmo))
	{
		mInfo.Health--;
		if (mInfo.Health == 0) { mbIsDead = true; }
	}
}
