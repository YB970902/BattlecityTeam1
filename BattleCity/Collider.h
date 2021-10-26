#pragma once
#include "GameEntity.h"

class Physcis;
class CollisionChecker;
class Collider : public GameEntity
{
private:
	CollisionChecker* pObj;
	eCollisionTag mTag;


	void (CollisionChecker::* pFunc)(eCollisionDir dir, int tag);

	POINTFLOAT mPlayerPos;
	RECT mPlayerBody;
	float mPlayerBodySize;
	Physcis* mPhyscis;

	POINT mPointGrid[4];

public:
	HRESULT Init(POINTFLOAT pos, float bodySize, Physcis* physcis, CollisionChecker* obj, eCollisionTag tag);

	void UpdateBodySize();
	void UpdatePointGrid();
	void Render(HDC hdc) override;
	void SetPlayerPos(POINTFLOAT movePos);
	void AddPlayerPos(POINTFLOAT addPos);
	void MoveTo(POINTFLOAT dir, float moveSpeed);

	void OnCollided(eCollisionDir dir, int tag);
	bool IsCollided();

	inline eCollisionTag GetTag() { return this->mTag; }
	inline void SetTag(eCollisionTag tag) { this->mTag = tag; }
	inline POINTFLOAT GetPlayerPos() { return this->mPlayerPos; }
	inline float GetPlayerBodySize() { return this->mPlayerBodySize; }
	inline RECT GetPlayerBody() { return this->mPlayerBody; }
	inline POINT* GetPointGrid() { return this->mPointGrid; }

	virtual ~Collider() {}
};