#include "Config.h"
#include "Tile.h"
#include "Collider.h"
#include "Image.h"

HRESULT Tile::Init(TagTile info, Collider* collider, POINT pos)
{
	mImg = IMG_MGR->FindImage(eImageTag::Tile);
	mTileInfo = info;
	mCollider = collider;
	mPos = pos;

	return S_OK;
}

void Tile::Release()
{
}

void Tile::Update()
{
}

void Tile::Render(HDC hdc)
{
	if (mbIsBroken) { return; }
	mImg->Render(hdc, mPos.x, mPos.y, mTileInfo.TilePos.x, mTileInfo.TilePos.y);
}

void Tile::OnCollided(eCollisionDir dir, int tag)
{
	switch (tag)
	{
	case (int)eCollisionTag::EnemyAmmo:
	case (int)eCollisionTag::PlayerAmmo:
		CalcBroken(dir);
		break;
	}
}

void Tile::CalcBroken(eCollisionDir dir)
{
	int brokenStyle = 0;
	switch (dir)
	{
	case eCollisionDir::Left: brokenStyle = BROKEN_LEFT; break;
	case eCollisionDir::Right: brokenStyle = BROKEN_RIGHT; break;
	case eCollisionDir::Top: brokenStyle = BROKEN_TOP; break;
	case eCollisionDir::Bottom: brokenStyle = BROKEN_BOTTOM; break;
	}
	if (mTileInfo.Terrain == eTerrain::Wall)
	{
		mTileInfo.CollisionCount++;
		if (mTileInfo.CollisionCount == 3)
		{
			mTileInfo.TileState = 0x1111;
			mbIsBroken = true;
			return;
		}
		else if ((mTileInfo.TileState & brokenStyle) == brokenStyle)
		{
			mTileInfo.TileState = 0x1111;
			mbIsBroken = true;
			return;
		}
		else if ((~brokenStyle) == mTileInfo.TileState)
		{
			mTileInfo.TileState = 0x1111;
			mbIsBroken = true;
			return;
		}
		else { mTileInfo.TileState |= brokenStyle; }


		if (mTileInfo.TileState == 0x1110) { mTileInfo.TilePos.x = 2; mTileInfo.TilePos.y = 4; }
		else if (mTileInfo.TileState == 0x0111) { mTileInfo.TilePos.x = 3; mTileInfo.TilePos.y = 4; }
		else if (mTileInfo.TileState == 0x1011) { mTileInfo.TilePos.x = 3; mTileInfo.TilePos.y = 5; }
		else if (mTileInfo.TileState == 0x1101) { mTileInfo.TilePos.x = 2; mTileInfo.TilePos.y = 5; }

		else if (mTileInfo.TileState == 0x1100) { mTileInfo.TilePos.x = 0; mTileInfo.TilePos.y = 4; }
		else if (mTileInfo.TileState == 0x0110) { mTileInfo.TilePos.x = 1; mTileInfo.TilePos.y = 5; }
		else if (mTileInfo.TileState == 0x0011) { mTileInfo.TilePos.x = 0; mTileInfo.TilePos.y = 5; }
		else if (mTileInfo.TileState == 0x1001) { mTileInfo.TilePos.x = 1; mTileInfo.TilePos.y = 4; }
	}
}
