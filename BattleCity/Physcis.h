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
	map<int, map<int, vector<Collider*>>> mGridMap;

	POINT mCheckGrid[4];

public:
	HRESULT Init();
	Collider* CreateCollider(POINTFLOAT pos, float bodySize, CollisionChecker* obj, eCollisionTag tag);
	void DestroyCollider(Collider* col);
	void CheckCollider(Collider* col, POINTFLOAT dir, POINTFLOAT oldPos);
	bool IsCollided(Collider* col1, Collider* col2);
	bool IsCollided(Collider* col);
	// 충돌하지 않으면 0, 충돌은 했지만 보정을 안한다면 1, 그 외엔 충돌한 상대의 태그 반환
	int PreventOverlapped(Collider* col1, Collider* col2, POINTFLOAT& addedForce, POINTFLOAT dir, POINTFLOAT& oldOverlapped);

	void Render(HDC hdc)override;
	void Release()override;

	virtual ~Physcis() {}
};