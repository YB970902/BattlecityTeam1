#pragma once
#include "GameObject.h"
#include "CollisionChecker.h"

class Tank;
class Ammo : public GameObject, public CollisionChecker
{
protected:
	Tank* mOwner = nullptr;

	float mSpeed = 0.0f;
	POINTFLOAT mDirPoint = {};

	bool mbIsFire = false;

public:
	HRESULT Init(eDir dir, eCollisionTag tag, float speed);
	void Release();
	void Update();
	void Render(HDC hdc);

	virtual void OnCollided(eCollisionDir dir, eCollisionTag tag) override;

	inline bool IsFire() { return mbIsFire; }
	inline void SetIsFire(bool set) { mbIsFire = set; }

	inline void SetOwner(Tank* owner) { mOwner = owner; }
};

