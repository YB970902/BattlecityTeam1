#include "Config.h"
#include "TileManager.h"
#include "Tile.h"
#include "NexusTile.h"
#include "MapEditor.h"
#include "Image.h"
#include "Collider.h"
#include "GameManager.h"
#include "Subject.h"

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
				mGameManager->DestroyCollider(temp->GetCollider());
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
				if (mMapTile.find(tileIndex.x) != mMapTile.end() && mMapTile[tileIndex.x].find(tileIndex.y) != mMapTile[tileIndex.x].end() && mMapTile[tileIndex.x][tileIndex.y] != nullptr)
				{
					mMapTile[tileIndex.x][tileIndex.y]->GetCollider()->SetTag(eCollisionTag::Block);
				}
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
		MessageBox(g_hWnd, "?? ?????? ?????? ????????????.", "????", MB_OK);
	}

	Tile* newTile = nullptr;
	eCollisionTag tileTag = eCollisionTag::Block;
	for (int y = 0; y < TILE_COUNT_Y; y++)
	{
		for (int x = 0; x < TILE_COUNT_X; x++)
		{
			eTerrain curTerrain = mArrTile[y * TILE_COUNT_X + x].Terrain;
			if (curTerrain != eTerrain::None && curTerrain != eTerrain::FlagEnemy && curTerrain != eTerrain::FlagFirstPlayer && curTerrain != eTerrain::FlagSecondPlayer)
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
				if (mArrTile[y * TILE_COUNT_X + x].Terrain == eTerrain::Nexus)
				{
					newTile = new NexusTile;
					newTile->Init(mArrTile[y * TILE_COUNT_X + x],
						mGameManager->CreateCollider({ mStartPos.x + mArrTile[y * TILE_COUNT_X + x].TileShape.left + TILE_SIZE * 0.5f,
							mStartPos.y + mArrTile[y * TILE_COUNT_X + x].TileShape.top + TILE_SIZE * 0.5f },
							TILE_SIZE,
							newTile,
							tileTag),
						{ mStartPos.x + mArrTile[y * TILE_COUNT_X + x].TileShape.left + (int)(TILE_SIZE * 0.5f),
							mStartPos.y + mArrTile[y * TILE_COUNT_X + x].TileShape.top + static_cast<int>(TILE_SIZE * 0.5f) });
					dynamic_cast<NexusTile*>(newTile)->GetSubject()->AddObserver(mGameManager);
					mVecNexusTile.push_back(static_cast<NexusTile*>(newTile));
				}
				else
				{
					newTile = new Tile;
					newTile->Init(mArrTile[y * TILE_COUNT_X + x],
						mGameManager->CreateCollider({ mStartPos.x + mArrTile[y * TILE_COUNT_X + x].TileShape.left + TILE_SIZE * 0.5f,
							mStartPos.y + mArrTile[y * TILE_COUNT_X + x].TileShape.top + TILE_SIZE * 0.5f },
							TILE_SIZE,
							newTile,
							tileTag),
						{ mStartPos.x + mArrTile[y * TILE_COUNT_X + x].TileShape.left + (int)(TILE_SIZE * 0.5f),
							mStartPos.y + mArrTile[y * TILE_COUNT_X + x].TileShape.top + static_cast<int>(TILE_SIZE * 0.5f) });
				}

				if (mArrTile[y * TILE_COUNT_X + x].NexusAroundTile)
				{
					mVecAroundTileInfo.push_back(make_pair<POINTFLOAT, POINT>(POINTFLOAT{ (float)newTile->GetPosition().x, (float)newTile->GetPosition().y }, POINT{ x, y }));
				}

				if (mArrTile[y * TILE_COUNT_X + x].Terrain == eTerrain::Grass)
				{
					mGameManager->DestroyCollider(newTile->GetCollider());
					newTile->SetCollider(nullptr);
					mVecBush.push_back(newTile);
				}
				mMapTile[x][y] = newTile;
			}
		}
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
		MessageBox(g_hWnd, "?????? ?????? ?????? ????????????.", "????", MB_OK);
	}

	CloseHandle(hFile);
}

void TileManager::BushRender(HDC hdc)
{
	for (int i = 0; i < mVecBush.size(); ++i)
	{
		mVecBush[i]->Render(hdc);
	}
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
				mGameManager->CreateCollider(mVecAroundTileInfo[i].first,
					TILE_SIZE,
					mMapTile[tileIndex.x][tileIndex.y],
					eCollisionTag::NexusBlock),
				POINT{ (long)mVecAroundTileInfo[i].first.x, (long)mVecAroundTileInfo[i].first.y });
		}

		mMapTile[tileIndex.x][tileIndex.y]->GetCollider()->SetTag(eCollisionTag::NexusBlock);
		mMapTile[tileIndex.x][tileIndex.y]->GetTileInfo()->TileState = 0;
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

POINTFLOAT* TileManager::GetEnemySpawnPosition(int& size)
{
	map<int, bool> mapDetected;

	for (int x = 0; x < TILE_COUNT_X; ++x)
	{
		for (int y = 0; y < TILE_COUNT_Y; ++y)
		{
			if (mapDetected[y * TILE_COUNT_X + x]) continue;
			mapDetected[y * TILE_COUNT_X + x] = false;
			if (mArrTile[y * TILE_COUNT_X + x].Terrain == eTerrain::FlagEnemy)
			{
				// ???????? ?????? ???? ????????
				// (C, L, R, T, B, LT, LB, RT, RB)
				int bit = GetFlagDetectInfo(x, y, eTerrain::FlagEnemy);
				if ((bit & 0b1) == 0b1) { mapDetected[(y + 1) * TILE_COUNT_X + (x + 1)] = true; }
				if ((bit & 0b10) == 0b10) { mapDetected[(y - 1) * TILE_COUNT_X + (x + 1)] = true; }
				if ((bit & 0b100) == 0b100) { mapDetected[(y + 1) * TILE_COUNT_X + (x - 1)] = true; }
				if ((bit & 0b1000) == 0b1000) { mapDetected[(y - 1) * TILE_COUNT_X + (x - 1)] = true; }
				if ((bit & 0b10000) == 0b10000) { mapDetected[(y + 1) * TILE_COUNT_X + x] = true; }
				if ((bit & 0b100000) == 0b100000) { mapDetected[(y - 1) * TILE_COUNT_X + x] = true; }
				if ((bit & 0b1000000) == 0b1000000) { mapDetected[y * TILE_COUNT_X + (x + 1)] = true; }
				if ((bit & 0b10000000) == 0b10000000) { mapDetected[y * TILE_COUNT_X + (x - 1)] = true; }
				if ((bit & 0b100000000) == 0b100000000) { mapDetected[y * TILE_COUNT_X + x] = true; }
			}
		}
	}

	vector<POINTFLOAT> vecTile;

	for (int x = 0; x < TILE_COUNT_X; ++x)
	{
		for (int y = 0; y < TILE_COUNT_Y; ++y)
		{
			if (mapDetected[y * TILE_COUNT_X + x])
			{
				if ((x == 0 || mapDetected[y * TILE_COUNT_X + (x - 1)] == false) &&
					(y == 0 || mapDetected[(y - 1) * TILE_COUNT_X + x] == false))
				{
					// ?????? ???? ?? ????
					vecTile.push_back(POINTFLOAT{ (float)mStartPos.x + x * TILE_SIZE + TILE_SIZE, (float)mStartPos.y + y * TILE_SIZE + TILE_SIZE });
				}
			}
		}
	}

	POINTFLOAT* result = new POINTFLOAT[vecTile.size()];
	for (int i = 0; i < vecTile.size(); ++i)
	{
		result[i] = vecTile[i];
	}

	size = vecTile.size();

	return result;
}

void TileManager::CreateEdgeBlock()
{
	for (int x = 0; x < TILE_COUNT_X; ++x)
	{
		mGameManager->CreateCollider(POINTFLOAT{ mStartPos.x + (float)x * TILE_SIZE + (TILE_SIZE * 0.5f), (float)mStartPos.y - (TILE_SIZE * 0.5f) }, TILE_SIZE, nullptr, eCollisionTag::Block);
		mGameManager->CreateCollider(POINTFLOAT{ mStartPos.x + (float)x * TILE_SIZE + (TILE_SIZE * 0.5f), (float)mStartPos.y + (float)mBackgroundSize.y + (TILE_SIZE * 0.5f) }, TILE_SIZE, nullptr, eCollisionTag::Block);
	}

	for (int y = 0; y < TILE_COUNT_Y; ++y)
	{
		mGameManager->CreateCollider(POINTFLOAT{ (float)mStartPos.x - (TILE_SIZE * 0.5f), mStartPos.y + (float)y * TILE_SIZE + (TILE_SIZE * 0.5f) }, TILE_SIZE, nullptr, eCollisionTag::Block);
		mGameManager->CreateCollider(POINTFLOAT{ (float)mStartPos.x + (float)mBackgroundSize.x + (TILE_SIZE * 0.5f), mStartPos.y + (float)y * TILE_SIZE + (TILE_SIZE * 0.5f) }, TILE_SIZE, nullptr, eCollisionTag::Block);
	}
}

void TileManager::ChangeNexusImageToFlag()
{
	for (int i = 0; i < mVecNexusTile.size(); ++i)
	{
		mVecNexusTile[i]->SetImagePos(POINT{ mVecNexusTile[i]->GetTileInfo()->TilePos.x + 2, mVecNexusTile[i]->GetTileInfo()->TilePos.y });
	}
}

pair<TankSpawnInfo*, int> TileManager::GetEnemyList()
{
	TankSpawnInfo* result = new TankSpawnInfo[mEnemyInfo->mEnemyOrderCount];
	for (int i = 0; i < mEnemyInfo->mEnemyOrderCount; ++i)
	{
		switch (mEnemyInfo->mEnemyOrderType[i])
		{
		case eTankType::NormalEnemy:
			result[i] = (TankSpawnInfo(eCollisionTag::EnemyTank, mEnemyInfo->mEnemyOrderType[i], eTankColor::White, NORMAL_TANK_INFO));
			break;
		case eTankType::QuickEnemy:
			result[i] = (TankSpawnInfo(eCollisionTag::EnemyTank, mEnemyInfo->mEnemyOrderType[i], eTankColor::White, QUICK_TANK_INFO));
			break;
		case eTankType::RapidFireEnemy:
			result[i] = (TankSpawnInfo(eCollisionTag::EnemyTank, mEnemyInfo->mEnemyOrderType[i], eTankColor::White, RAPID_FIRE_TANK_INFO));
			break;
		case eTankType::DefenceEnemy:
			result[i] = (TankSpawnInfo(eCollisionTag::EnemyTank, mEnemyInfo->mEnemyOrderType[i], eTankColor::White, DEFENCE_TANK_INFO));
			break;
		}
	}

	return pair<TankSpawnInfo*, int>(result, mEnemyInfo->mEnemyOrderCount);
}

POINTFLOAT TileManager::GetFirstPlayerSpawnPosition()
{
	for (int x = 0; x < TILE_COUNT_X; ++x)
	{
		for (int y = 0; y < TILE_COUNT_Y; ++y)
		{
			if (mArrTile[y * TILE_COUNT_X + x].Terrain == eTerrain::FlagFirstPlayer)
			{
				return POINTFLOAT{ (float)mStartPos.x + x * TILE_SIZE + TILE_SIZE, (float)mStartPos.y + y * TILE_SIZE + TILE_SIZE };
			}
		}
	}

	// ????!
	return POINTFLOAT{ 0, 0 };
}

POINTFLOAT TileManager::GetSecondPlayerSpawnPosition()
{
	for (int x = 0; x < TILE_COUNT_X; ++x)
	{
		for (int y = 0; y < TILE_COUNT_Y; ++y)
		{
			if (mArrTile[y * TILE_COUNT_X + x].Terrain == eTerrain::FlagSecondPlayer)
			{
				return POINTFLOAT{ (float)mStartPos.x + x * TILE_SIZE + TILE_SIZE, (float)mStartPos.y + y * TILE_SIZE + TILE_SIZE };
			}
		}
	}

	// ????!
	return POINTFLOAT{ 0, 0 };
}

POINTFLOAT TileManager::GetItemSpawnPosition()
{
	int x = 0;
	int y = 0;

	while (true)
	{
		x = RANDOM(0, TILE_COUNT_X);
		y = RANDOM(0, TILE_COUNT_X);

		if (x > 0)
		{
			if (mMapTile.find(x - 1) == mMapTile.end()) { break; }
			if (y > 0 && mMapTile[x - 1].find(y - 1) == mMapTile[x - 1].end()) { break; }
			if (y < TILE_COUNT_Y && mMapTile[x - 1].find(y) == mMapTile[x - 1].end()) { break; }
		}

		if (x < TILE_COUNT_X)
		{
			if (mMapTile.find(x) == mMapTile.end()) { break; }
			if (y > 0 && mMapTile[x].find(y - 1) == mMapTile[x].end()) { break; }
			if (y < TILE_COUNT_Y && mMapTile[x].find(y) == mMapTile[x].end()) { break; }
		}
	}

	return POINTFLOAT{ (float)mStartPos.x + x * TILE_SIZE, (float)mStartPos.y + y * TILE_SIZE };

}

int TileManager::GetFlagDetectInfo(int x, int y, eTerrain terrain)
{
	// bit???? ??????
	// (C, L, R, T, B, LT, LB, RT, RB)
	// (0b10000000) -> Left?? true??????
	int resultBit = 0;
	for (int i = -1; i < 2; ++i)
	{
		for (int j = -1; j < 2; ++j)
		{
			// x?? y?? ???? ?????? ????
			if (x + i >= 0 && x + i < TILE_COUNT_X &&
				y + j >= 0 && y + j < TILE_COUNT_Y)
			{
				if (mArrTile[(y + j) * TILE_COUNT_X + (x + i)].Terrain == terrain)
				{
					if (i == -1)
					{
						if (j == -1) { resultBit |= 0b1000; }
						else if (j == 0) { resultBit |= 0b10000000; }
						else if (j == 1) { resultBit |= 0b100; }
					}
					else if (i == 0)
					{
						if (j == -1) { resultBit |= 0b100000; }
						else if (j == 0) { resultBit |= 0b100000000; }
						else if (j == 1) { resultBit |= 0b10000; }
					}
					else if (i == 1)
					{
						if (j == -1) { resultBit |= 0b10; }
						else if (j == 0) { resultBit |= 0b1000000; }
						else if (j == 1) { resultBit |= 0b1; }
					}

				}
			}
		}
	}

	return resultBit;
}

void TileManager::TurnProtectBlockImage(bool set)
{
	POINT tileIndex;
	for (int i = 0; i < mVecAroundTileInfo.size(); ++i)
	{
		tileIndex = mVecAroundTileInfo[i].second;
		if (mMapTile.find(tileIndex.x) != mMapTile.end() && mMapTile[tileIndex.x].find(tileIndex.y) != mMapTile[tileIndex.x].end() && mMapTile[tileIndex.x][tileIndex.y] != nullptr)
		{
			mMapTile[tileIndex.x][tileIndex.y]->SetImagePos(POINT{ 0, (int)set * 2 });
		}
	}
}
