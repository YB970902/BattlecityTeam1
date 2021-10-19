#pragma once
#include "GameEntity.h"

#include "CollisionChecker.h"

class Image;
class Collider;
class Physcis;

class PhyscisScene : public GameEntity
{
private:
	Image* mBackGround;
	Collider* mCurCollider;

	const POINTFLOAT DIR_LEFT{ -1, 0 };
	const POINTFLOAT DIR_RIGHT{ 1, 0 };
	const POINTFLOAT DIR_UP{ 0, -1 };
	const POINTFLOAT DIR_DOWN{ 0, 1 };

	float mMoveSpeed = 150.0f;

	Physcis* mPhyscis;
public:
	HRESULT Init()override;
	void Release()override;
	void Render(HDC hdc)override;
	void Update()override;
};

