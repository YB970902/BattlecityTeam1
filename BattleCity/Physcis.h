#pragma once
#include "GameEntity.h"

class Collider;
class CollisionChecker;
class Physcis : public GameEntity
{
private:
	POINTFLOAT mPos;
	float mBodySize;

	vector<Collider*> mVecCollider;

public:
	Collider* CreateCollider(POINTFLOAT pos, float bodySize, CollisionChecker* obj, eCollisionTag tag);
	void DestoryCollider(Collider* col1);
	void CheckCollider(Collider* col, POINTFLOAT dir, POINTFLOAT oldPos);
	bool IsCollided(Collider* col1, Collider* col2);
	bool IsCollided(Collider* col);
	void PreventOverlapped(Collider* col1, Collider* col2, POINTFLOAT& addedForce, POINTFLOAT dir, POINTFLOAT& oldOverlapped);

	void Render(HDC hdc);
};