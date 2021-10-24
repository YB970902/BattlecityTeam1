#pragma once
#include "GameObject.h"
#include "CollisionChecker.h"

const float AMMO_BODY_SIZE = 16.0f;

class Tank;
class Ammo : public GameObject, public CollisionChecker
{
protected:
	Tank* mOwner = nullptr;

	float mSpeed = 0.0f;
	POINTFLOAT mDirPoint = {};

	bool mbIsFire = false;

public:
	virtual HRESULT Init(eDir dir, eCollisionTag tag, float speed);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual void OnCollided(eCollisionDir dir, int tag) override;

	inline bool IsFire() { return mbIsFire; }
	inline void SetIsFire(bool set) { mbIsFire = set; }

	inline void SetOwner(Tank* owner) { mOwner = owner; }

	inline void SetCollider(Collider* collider) { mCollider = collider; };
};