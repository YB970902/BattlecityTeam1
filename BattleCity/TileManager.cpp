#include "Config.h"
#include "TileManager.h"
#include "Tile.h"
#include "MapEditor.h"
#include "Image.h"
#include "Physcis.h"
#include "Collider.h"

HRESULT TileManager::Init(POINT startPos, POINT backgroundSize)
{
	mStartPos = startPos;
	mBackgroundSize = backgroundSize;

	mbLoadMap = true;
	mEnemyInfo = new TagEnemyInfo;

	return S_OK;
}

void TileManager::Release()
{
	ClearTileMap();
	SAFE_DELETE(mEnemyInfo);
}

void TileManager::Update()
{
	TileDebugUpdate();

	for (map<int, map<int, Tile*>>::iterator itX = mMapTile.begin(); itX != mMapTile.end();)
	{
		for (map<int, Tile*>::iterator itY = (*itX).second.begin(); itY != (*itX).second.end();)
		{
			if ((*itY).second->IsBroken())
			{
				Tile* temp = (*itY).second;
				itY = (*itX).second.erase(itY);
				mPhysics->DestroyCollider(temp->GetCollider());
				SAFE_RELEASE(temp);
			}
			else { itY++; }
		}
		if ((*itX).second.size() == 0) { itX = mMapTile.erase(itX); }
		else { itX++; }
	}

	mChangeWaterTileTime += TIMER_MGR->GetDeltaTime();

	for (map<int, map<int, Tile*>>::iterator itX = mMapTile.begin(); itX != mMapTile.end(); itX++)
	{
		for (map<int, Tile*>::iterator itY = (*itX).second.begin(); itY != (*itX).second.end(); itY++)
		{
			if ((*itY).second->IsWater())
			{
				if (mChangeWaterTileTime >= CHANGE_WATER_TILE_TIME)
				{
					if ((*itY).second->GetTileInfo()->TilePos.x < 6) {
						(*itY).second->GetTileInfo()->TilePos.x = (*itY).second->GetTileInfo()->TilePos.x + 2;
					}
					else
					{
						(*itY).second->GetTileInfo()->TilePos.x = (*itY).second->GetTileInfo()->TilePos.x - 2;
					}
				}
			}
		}
	}

	if (mChangeWaterTileTime >= CHANGE_WATER_TILE_TIME) mChangeWaterTileTime = 0;

	if (mbItemBlock)
	{
		mCurSparkleElapsedTime += DELTA_TIME;
		if (mCurSparkleElapsedTime >= mMaxSparkleDurationTime)
		{
			mbIsOnProtectBlock = !mbIsOnProtectBlock;
			mCurSparkleElapsedTime = 0.0f;
			mMaxSparkleDurationTime = 0.25f;
			TurnProtectBlockImage(mbIsOnProtectBlock);
		}
		mCurProtectElapsedTime += DELTA_TIME;
		if (mCurProtectElapsedTime >= PROTECT_ITEM_DURATION_TIME)
		{
			mCurProtectElapsedTime = 0.0f;
			mbItemBlock = false;
			TurnProtectBlockImage(false);
			POINT tileIndex;
			for (int i = 0; i < mVecAroundTileInfo.size(); ++i)
			{
				tileIndex = mVecAroundTileInfo[i].second;
				mMapTile[tileIndex.x][tileIndex.y]->GetCollider()->SetTag(eCollisionTag::Block);
			}
		}
	}
}

void TileManager::Render(HDC hdc)
{
	for (map<int, map<int, Tile*>>::iterator itX = mMapTile.begin(); itX != mMapTile.end(); itX++)
	{
		for (map<int, Tile*>::iterator itY = (*itX).second.begin(); itY != (*itX).second.end(); itY++)
		{
			(*itY).second->Render(hdc);
		}
	}
}

void TileManager::LoadMap(int loadIndex)
{
	string loadFileName = "Save/saveMapData_" + to_string(loadIndex);
	loadFileName += ".map";
	DWORD mapSaveTileInfo = sizeof(TagTile) * TILE_COUNT_X * TILE_COUNT_Y;

	HANDLE hFile = CreateFile(loadFileName.c_str(),
		GENERIC_READ,
		0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD readByte;
	if (ReadFile(hFile, mArrTile, mapSaveTileInfo,
		&readByte, NULL) == false)
	{
		MessageBox(g_hWnd, "맵 데이터 로드에 실패했습니다.", "에러", MB_OK);
	}

	Tile* newTile = nullptr;
	eCollisionTag tileTag = eCollisionTag::Block;
	for (int y = 0; y < TILE_COUNT_Y; y++)
	{
		for (int x = 0; x < TILE_COUNT_X; x++)
		{
			cout << (int)mArrTile[y * TILE_COUNT_X + x].NexusAroundTile;
			if (mArrTile[y * TILE_COUNT_X + x].Terrain != eTerrain::None)
			{
				switch (mArrTile[y * TILE_COUNT_X + x].Terrain)
				{
				case eTerrain::Water:
					tileTag = eCollisionTag::Water;
					break;
				default:
					tileTag = eCollisionTag::Block;
					break;
				}
				newTile = new Tile;
				newTile->Init(mArrTile[y * TILE_COUNT_X + x],
					mPhysics->CreateCollider({ mStartPos.x + mArrTile[y * TILE_COUNT_X + x].TileShape.left + TILE_SIZE * 0.5f,
						mStartPos.y + mArrTile[y * TILE_COUNT_X + x].TileShape.top + TILE_SIZE * 0.5f },
						TILE_SIZE,
						newTile,
						tileTag),
					{ mStartPos.x + mArrTile[y * TILE_COUNT_X + x].TileShape.left + (int)(TILE_SIZE * 0.5f),
						mStartPos.y + mArrTile[y * TILE_COUNT_X + x].TileShape.top + static_cast<int>(TILE_SIZE * 0.5f) });

				if (mArrTile[y * TILE_COUNT_X + x].NexusAroundTile)
				{
					mVecAroundTileInfo.push_back(make_pair<POINTFLOAT, POINT>(POINTFLOAT{ (float)newTile->GetPosition().x, (float)newTile->GetPosition().y }, POINT{ x, y }));
				}
				mMapTile[x][y] = newTile;
			}
		}
		cout << endl;
	}

	CloseHandle(hFile);
}

void TileManager::LoadEnemyOrder(int loadIndex)
{
	string loadFileName = "Save/saveEnemyOrderData_" + to_string(loadIndex);
	loadFileName += ".map";

	HANDLE hFile = CreateFile(loadFileName.c_str(),
		GENERIC_READ,
		0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD readByte;
	if (ReadFile(hFile, mEnemyInfo, sizeof(TagEnemyInfo),
		&readByte, NULL) == false)
	{
		MessageBox(g_hWnd, "에너미 데이터 로드에 실패했습니다.", "에러", MB_OK);
	}

	CloseHandle(hFile);
}

void TileManager::ProtectNexus()
{
	mbItemBlock = true;
	mCurProtectElapsedTime = 0.0f;

	POINT tileIndex;
	for (int i = 0; i < mVecAroundTileInfo.size(); ++i)
	{
		tileIndex = mVecAroundTileInfo[i].second;
		if (mMapTile.find(tileIndex.x) == mMapTile.end() ||
			mMapTile[tileIndex.x].find(tileIndex.y) == mMapTile[tileIndex.x].end() ||
			mMapTile[tileIndex.x][tileIndex.y] == nullptr)
		{
			mMapTile[tileIndex.x][tileIndex.y] = new Tile();
			mMapTile[tileIndex.x][tileIndex.y]->Init(mArrTile[tileIndex.y * TILE_COUNT_X + tileIndex.x],
				mPhysics->CreateCollider(mVecAroundTileInfo[i].first,
					TILE_SIZE,
					mMapTile[tileIndex.x][tileIndex.y],
					eCollisionTag::NexusBlock),
				POINT{ (long)mVecAroundTileInfo[i].first.x, (long)mVecAroundTileInfo[i].first.y });
		}

		mMapTile[tileIndex.x][tileIndex.y]->GetCollider()->SetTag(eCollisionTag::NexusBlock);
		TurnProtectBlockImage(true);
		mbIsOnProtectBlock = true;
	}
	mCurSparkleElapsedTime = 0.0f;
	mMaxSparkleDurationTime = 15.0f;
}

void TileManager::ClearTileMap()
{
	for (map<int, map<int, Tile*>>::iterator itX = mMapTile.begin(); itX != mMapTile.end();)
	{
		for (map<int, Tile*>::iterator itY = (*itX).second.begin(); itY != (*itX).second.end();)
		{
			Tile* temp = (*itY).second;
			itY = (*itX).second.erase(itY);
			SAFE_RELEASE(temp);
		}
		itX = mMapTile.erase(itX);
	}
}

void TileManager::TileDebugUpdate()
{
	eCollisionDir dirTag = (eCollisionDir)-1;
	if (KEY_MGR->IsStayKeyDown('Q')) { dirTag = eCollisionDir::Left; }
	if (KEY_MGR->IsStayKeyDown('W')) { dirTag = eCollisionDir::Top; }
	if (KEY_MGR->IsStayKeyDown('E')) { dirTag = eCollisionDir::Right; }
	if (KEY_MGR->IsStayKeyDown('R')) { dirTag = eCollisionDir::Bottom; }

	if (KEY_MGR->IsOnceKeyDown(VK_LBUTTON))
	{
		if ((int)dirTag != -1 && g_ptMouse.x > mStartPos.x &&
			g_ptMouse.x <(mStartPos.x + mBackgroundSize.x) &&
			g_ptMouse.y > mStartPos.y &&
			g_ptMouse.y < (mStartPos.x + mBackgroundSize.y) &&
			mMapTile.find((g_ptMouse.x - mStartPos.x) / TILE_SIZE) != mMapTile.end() &&
			mMapTile[(g_ptMouse.x - mStartPos.x) / TILE_SIZE].find((g_ptMouse.y - mStartPos.y) / TILE_SIZE) != mMapTile[(g_ptMouse.x - mStartPos.x) / TILE_SIZE].end())
		{
			mMapTile[(g_ptMouse.x - mStartPos.x) / TILE_SIZE][(g_ptMouse.y - mStartPos.y) / TILE_SIZE]->OnCollided(dirTag, (int)eCollisionTag::FirstPlayerAmmo);
		}
	}

	if (KEY_MGR->IsOnceKeyDown('1'))
	{
		ClearTileMap();
		LoadMap(1);
		LoadEnemyOrder(1);
		mbLoadMap = true;
	}
	else if (KEY_MGR->IsOnceKeyDown('2'))
	{
		ClearTileMap();
		LoadMap(2);
		LoadEnemyOrder(2);
		mbLoadMap = true;
	}
	else if (KEY_MGR->IsOnceKeyDown('3'))
	{
		ClearTileMap();
		LoadMap(3);
		LoadEnemyOrder(3);
		mbLoadMap = true;
	}
}

void TileManager::TurnProtectBlockImage(bool set)
{
	POINT tileIndex;
	for (int i = 0; i < mVecAroundTileInfo.size(); ++i)
	{
		tileIndex = mVecAroundTileInfo[i].second;
		mMapTile[tileIndex.x][tileIndex.y]->SetImagePos(POINT{ 0, (int)set * 2 });
	}
}
