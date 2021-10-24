#pragma once
#include "GameEntity.h"
#include "CollisionChecker.h"

enum class eDir
{
	Up = 0,
	Left = 1,
	Down = 2,
	Right = 3,
};

class Image;
class GameObject : public GameEntity, public CollisionChecker
{
protected:
	Image* mImage = nullptr;

	POINTFLOAT mPos = {};
	eDir mDir = eDir::Up;
	const POINTFLOAT DIR_VALUE[4] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0} };
	eCollisionTag mCollisionTag = eCollisionTag::Block;

	bool mbIsDead = false;

public:
	GameObject();
	~GameObject();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	inline POINTFLOAT GetPosition() { return mPos; }
	inline void SetPosition(POINTFLOAT pos) { mPos = pos; }
	inline eDir GetDirection() { return mDir; }
	inline void SetDirection(eDir dir) { mDir = dir; }
	inline eCollisionTag GetCollisionTag() { return mCollisionTag; }
	inline void SetCollisionTag(eCollisionTag tag) { mCollisionTag = tag; }
	inline bool IsDead() { return mbIsDead; }

	virtual void OnCollided(eCollisionDir dir, eCollisionTag tag) override;
};