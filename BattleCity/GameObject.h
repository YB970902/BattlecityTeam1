#pragma once
#include "GameEntity.h"
#include "CollisionChecker.h"

class Image;
class Collider;
class GameObject : public GameEntity, public CollisionChecker
{
protected:
	Image* mImage = nullptr;
	Collider* mCollider = nullptr;

	POINTFLOAT mPos = {};
	eDir mDir = eDir::Up;
	const POINTFLOAT DIR_VALUE[4] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0} };
	eCollisionTag mCollisionTag = eCollisionTag::Block;

	bool mbIsDead = false;

public:
	GameObject();
	~GameObject();

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	inline Collider* GetCollider() { return mCollider; }
	inline POINTFLOAT GetPosition() { return mPos; }
	inline void SetPosition(POINTFLOAT pos) { mPos = pos; }
	inline eDir GetDirection() { return mDir; }
	inline void SetDirection(eDir dir) { mDir = dir; }
	inline eCollisionTag GetCollisionTag() { return mCollisionTag; }
	inline void SetCollisionTag(eCollisionTag tag) { mCollisionTag = tag; }
	inline bool IsDead() { return mbIsDead; }
	inline void SetIsDead(bool set) { mbIsDead = set; }

	virtual void OnCollided(eCollisionDir dir, int tag) override;
};