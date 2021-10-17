#pragma once
#include "GameEntity.h"

class Physcis;
class CollisionChecker;
class Collider : public GameEntity
{
private:
	CollisionChecker* pObj;
	eCollisionTag mTag;

	
	void (CollisionChecker::* pFunc)(eCollisionDir dir, eCollisionTag tag);

	POINTFLOAT mPlayerPos;
	RECT mPlayerBody;
	float mPlayerBodySize;
	Physcis* mPhyscis;

public:
	HRESULT Init(POINTFLOAT pos, float bodySize, Physcis* physcis, CollisionChecker* obj, eCollisionTag tag);

	void UpdateBodySize();
	void Render(HDC hdc) override;
	void SetPlayerPos(POINTFLOAT movePos);
	void AddPlayerPos(POINTFLOAT addPos);
	void MoveTo(POINTFLOAT dir, float moveSpeed);

	void OnCollided(eCollisionDir dir, eCollisionTag tag);

	inline eCollisionTag GetTag() { return this->mTag; }
	inline POINTFLOAT GetPlayerPos() { return this->mPlayerPos; }
	inline float GetPlayerBodySize() { return this->mPlayerBodySize; }
	inline RECT GetPlayerBody() { return this->mPlayerBody; }
};