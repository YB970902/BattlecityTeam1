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
	Image* mImage;

	POINTFLOAT mPos;
	eDir mDir;

	bool mbIsDead = false;

public:
	GameObject();
	~GameObject();

	eDir GetDirection() { return mDir; }
	bool IsDead() { return mbIsDead; }

	virtual void OnCollided(eCollisionDir dir, eCollisionTag tag) override;
};