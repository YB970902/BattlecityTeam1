#include "Config.h"
#include "Item.h"
#include "Subject.h"
#include "Image.h"

HRESULT Item::Init(Collider* collider, eImageTag imageTag, eItemTag itemTag)
{
	mCollider = collider;
	mImage = IMG_MGR->FindImage(imageTag);
	mItemTag = itemTag;
	mSubject = new Subject;
	return S_OK;
}

void Item::Release()
{
	SAFE_DELETE(mSubject);
}

void Item::Update()
{
}

void Item::Render(HDC hdc)
{
	if (mbIsDead) { return; }
	mImage->Render(hdc, mPos.x, mPos.y);
}

void Item::OnCollided(eCollisionDir dir, int tag)
{
	if (mbIsDead) { return; }

	if (IS_PLAYER_TANK(tag))
	{
		mbIsDead = true;
		mCollidedObjectTag = (eCollisionTag)tag;
		mSubject->Notify(this, eSubjectTag::Item, eEventTag::Collided);
	}
}
