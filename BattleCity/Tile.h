#pragma once
#include "CollisionChecker.h"
#include "GameEntity.h"

#define BROKEN_LEFT   0x1100
#define BROKEN_TOP    0x0110
#define BROKEN_RIGHT  0x0011
#define BROKEN_BOTTOM 0x1001

class Collider;
class Image;
class Tile : public GameEntity, public CollisionChecker
{
private:
	Image* mImg;
	TagTile mTileInfo;
	Collider* mCollider;
	POINT mPos;

	bool mbIsBroken = false;

public:
	HRESULT Init(TagTile info, Collider* collider, POINT pos);
	void Release();
	void Update();
	void Render(HDC hdc);

	void OnCollided(eCollisionDir dir, eCollisionTag tag) override;

	Collider* GetCollider() { return this->mCollider; }
	POINT GetPosition() { return this->mPos; }

	bool IsWater() { return mTileInfo.Terrain == eTerrain::Water; }
	bool IsBroken() { return mbIsBroken; }

	TagTile* GetTileInfo() { return &mTileInfo; }
private:
	void CalcBroken(eCollisionDir dir);
};
