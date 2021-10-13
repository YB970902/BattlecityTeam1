#pragma once
#include "GameEntity.h"

class Collider;
class Physcis : public GameEntity
{
private:
	POINTFLOAT mPos;
	float mBodySize;

	POINTFLOAT mDistance;


	vector<Collider*> mVecCollider;

public:
	Collider* CreateCollider(POINTFLOAT pos, float bodySize);
	void CheckCollider(Collider* col1);
	bool IsCollided(Collider* col1, Collider* col2);
	void PreventOverlapped(Collider* col1, Collider* col2);

	void Render(HDC hdc);
};