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
	// �浹���� ������ 0, �浹�� ������ ������ ���Ѵٸ� 1, �� �ܿ� �浹�� ����� �±� ��ȯ
	int PreventOverlapped(Collider* col1, Collider* col2, POINTFLOAT& addedForce, POINTFLOAT dir, POINTFLOAT& oldOverlapped);

	void Render(HDC hdc)override;
	void Release()override;

	virtual ~Physcis() {}
};