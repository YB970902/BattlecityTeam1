#pragma once
#include "GameEntity.h"
#include "CollisionChecker.h"

class Collider;
class Image;
class Subject;
class Item : public GameEntity, public CollisionChecker
{
private:
	Collider* mCollider = nullptr;
	eItemTag mItemTag = eItemTag::None;
	Image* mImage = nullptr;
	Subject* mSubject = nullptr;

	POINTFLOAT mPos = POINTFLOAT{ 0, 0 };

	eCollisionTag mCollidedObjectTag = eCollisionTag::Block;

	float mDurationTime = 20.0f;
	float mElapsedTime = 0.0f;
	float mDurationSparkleTime = 0.125f;
	float mElapsedSparkleTime = 0.0f;
	bool mbIsSparkle = false;

	bool mbIsDead = false;
public:
	HRESULT Init(Collider* collider, eImageTag imageTag, eItemTag itemTag);
	void Release();
	void Update();
	void Render(HDC hdc);

	Collider* GetCollider() { return mCollider; }
	Subject* GetSubject() { return mSubject; }
	eItemTag GetItemTag() { return mItemTag; }
	eCollisionTag GetCollidedTag() { return mCollidedObjectTag; }
	POINTFLOAT GetPosition() { return mPos; }
	void SetPosition(POINTFLOAT pos) { mPos = pos; }
	bool IsDead() { return mbIsDead; }

	virtual void OnCollided(eCollisionDir dir, int tag) override;
};