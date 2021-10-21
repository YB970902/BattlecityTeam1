#include "BattleScene.h"
#include "Image.h"
#include "Physcis.h"
#include "Collider.h"
#include "CollisionChecker.h"
#include "MapEditor.h"
#include "Tile.h"


HRESULT BattleScene::Init()
{
	mBackgroundGray = IMG_MGR->AddImage(eImageTag::BattleSceneGrayBG, "Image/background.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	mBackgroundBlack = IMG_MGR->AddImage(eImageTag::BattleSceneBlackBG, "Image/background2.bmp", 16 * 26, 16 * 26);

	mForResize = new Image();
	mForResize->Init(16 * 26, 16 * 26);

	mStartPos = { (mBackgroundGray->GetWidth() - mBackgroundBlack->GetWidth()) / 2 ,(mBackgroundGray->GetHeight() - mBackgroundBlack->GetHeight()) / 2 };
	//mCollider = new Collider;
	mPhyscis = new Physcis;

	LoadMap(0);

	mbLoadMap = true;
	mDebugMode = false;

	return S_OK;
}

void BattleScene::Update()
{
	//for (int i = 0; i < TILE_COUNT_Y; i++)
	//{
	//	for (int j = 0; j < TILE_COUNT_X; j++)
	//	{
	//		if (tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileState == 0x1111)
	//		{
	//			/*mPhyscis->DestoryCollider();*/
	//		}
	//	}
	//}

	//if (mbChangeBlock){ ChangeBlock(); }
	
	if (KEY_MGR->IsOnceKeyDown(VK_TAB))
	{
		mDebugMode ? mDebugMode = false : mDebugMode = true;
	}
	eCollisionDir dirTag = (eCollisionDir)-1;
	if (KEY_MGR->IsStayKeyDown('Q')) { dirTag = eCollisionDir::Left; }
	if (KEY_MGR->IsStayKeyDown('W')) { dirTag = eCollisionDir::Top; }
	if (KEY_MGR->IsStayKeyDown('E')) { dirTag = eCollisionDir::Right; }
	if (KEY_MGR->IsStayKeyDown('R')) { dirTag = eCollisionDir::Bottom; }

	if (KEY_MGR->IsOnceKeyDown(VK_LBUTTON))
	{
		if ((int)dirTag != -1 && g_ptMouse.x > mStartPos.x &&
			g_ptMouse.x <(mStartPos.x + mBackgroundBlack->GetWidth()) &&
			g_ptMouse.y > mStartPos.y &&
			g_ptMouse.y <(mStartPos.x + mBackgroundBlack->GetHeight()) &&
			mMapTile.find((g_ptMouse.x - mStartPos.x) / TILE_SIZE) != mMapTile.end() &&
			mMapTile[(g_ptMouse.x - mStartPos.x) / TILE_SIZE].find((g_ptMouse.y - mStartPos.y) / TILE_SIZE) != mMapTile[(g_ptMouse.x - mStartPos.x) / TILE_SIZE].end())
		{
			mMapTile[(g_ptMouse.x - mStartPos.x) / TILE_SIZE][(g_ptMouse.y - mStartPos.y) / TILE_SIZE]->OnCollided(dirTag, eCollisionTag::PlayerAmmo);
		}
	}
	
	for (map<int, map<int, Tile*>>::iterator itX = mMapTile.begin(); itX != mMapTile.end();)
	{
		for (map<int, Tile*>::iterator itY = (*itX).second.begin(); itY != (*itX).second.end();)
		{
			if ((*itY).second->IsBroken())
			{
				Tile* temp = (*itY).second;
				itY = (*itX).second.erase(itY);
				SAFE_RELEASE(temp);
			}
			else { itY++; }
		}
		if ((*itX).second.size() == 0) { itX = mMapTile.erase(itX); }
		else { itX++; }
	}

	mChangeWaterTileTime += TIMER_MGR->GetDeltaTime();

	//for (int i = 0; i < TILE_COUNT_Y; i++)
	//{
	//	for (int j = 0; j < TILE_COUNT_X; j++)
	//	{
	//		if (tile->GetmTileInfo()[i * TILE_COUNT_X + j].Terrain == eTerrain::Water)
	//		{
	//			if (mChangeWaterTileTime >= CHANGE_WATER_TILE_TIME)
	//			{
	//				if (tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.x < 6) {
	//					tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.x = tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.x + 2;
	//				}
	//				else
	//				{
	//					tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.x = tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.x - 2;
	//				}
	//			}
	//		}
	//	}
	//}

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

	//if (mbLoadMap)
	//{
	//	for (int i = 0; i < TILE_COUNT_Y; i++)
	//	{
	//		for (int j = 0; j < TILE_COUNT_X; j++)
	//		{
	//			SetRect(&(tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileShape),
	//				mStartPos.x + TILE_SIZE * j,
	//				mStartPos.y + TILE_SIZE * i,
	//				mStartPos.x + TILE_SIZE * j + TILE_SIZE,
	//				mStartPos.y + TILE_SIZE * i + TILE_SIZE);
	//
	//			if (!(tile->GetmTileInfo()[i * TILE_COUNT_X + j].Terrain == eTerrain::None))
	//			{
	//				tile->GetmPhysics()->CreateCollider({ tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileShape.left + TILE_SIZE * 0.5f, tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileShape.top + TILE_SIZE * 0.5f },
	//					TILE_SIZE, (CollisionChecker*)&(tile->GetmTileInfo()[i * TILE_COUNT_X + j]), eCollisionTag::Block);
	//			}
	//
	//		}
	//	}
	//	mbLoadMap = false;
	//	mbChangeBlock = false;
	//}

	// 임시 콜라이더 삭제 코드


	//if (KEY_MGR->IsStayKeyDown('Q'))
	//{
	//	if (KEY_MGR->IsOnceKeyDown(VK_LBUTTON))
	//	{
	//		if (g_ptMouse.x < mStartPos.x ||
	//			g_ptMouse.x >(mStartPos.x + mBackgroundBlack->GetWidth()) ||
	//			g_ptMouse.y < mStartPos.y ||
	//			g_ptMouse.y >(mStartPos.x + mBackgroundBlack->GetHeight())) return;
	//
	//		/*OnCollided(eCollisionDir::Left, eCollisionTag::EnemyAmmo);*/
	//	}
	//}
	/*
	if (KEY_MGR->IsStayKeyDown('W'))
	{
		int brokenStyle = BROKEN_TOP;
		if (KEY_MGR->IsOnceKeyDown(VK_LBUTTON))
		{
			if (g_ptMouse.x < mStartPos.x ||
				g_ptMouse.x >(mStartPos.x + mBackgroundBlack->GetWidth()) ||
				g_ptMouse.y < mStartPos.y ||
				g_ptMouse.y >(mStartPos.x + mBackgroundBlack->GetHeight())) return;

			POINT tileIndex{ (g_ptMouse.x - mStartPos.x) / TILE_SIZE , (g_ptMouse.y - mStartPos.y) / TILE_SIZE };
			int arrayIndex = tileIndex.y * TILE_COUNT_X + tileIndex.x;

			if (mBattleTile[arrayIndex].Terrain == eTerrain::Wall)
			{
				mBattleTile[arrayIndex].CollisionCount++;

				if (mBattleTile[arrayIndex].CollisionCount == 3)
				{
					mBattleTile[arrayIndex].TileState = 0x1111;
				}
				else if ((mBattleTile[arrayIndex].TileState & brokenStyle) == brokenStyle)
				{
					mBattleTile[arrayIndex].TileState = 0x1111;
				}
				else if (~brokenStyle == mBattleTile[arrayIndex].TileState)
				{
					mBattleTile[arrayIndex].TileState = 0x1111;
				}
				else
				{
					mBattleTile[arrayIndex].TileState |= brokenStyle;
				}
				mbChangeBlock = true;
			}
		}
	}
	if (KEY_MGR->IsStayKeyDown('E'))
	{
		int brokenStyle = BROKEN_RIGHT;
		if (KEY_MGR->IsOnceKeyDown(VK_LBUTTON))
		{
			if (g_ptMouse.x < mStartPos.x ||
				g_ptMouse.x >(mStartPos.x + mBackgroundBlack->GetWidth()) ||
				g_ptMouse.y < mStartPos.y ||
				g_ptMouse.y >(mStartPos.x + mBackgroundBlack->GetHeight())) return;

			POINT tileIndex{ (g_ptMouse.x - mStartPos.x) / TILE_SIZE , (g_ptMouse.y - mStartPos.y) / TILE_SIZE };
			int arrayIndex = tileIndex.y * TILE_COUNT_X + tileIndex.x;

			if (mBattleTile[arrayIndex].Terrain == eTerrain::Wall)
			{
				mBattleTile[arrayIndex].CollisionCount++;

				if (mBattleTile[arrayIndex].CollisionCount == 3)
				{
					mBattleTile[arrayIndex].TileState = 0x1111;
				}
				else if ((mBattleTile[arrayIndex].TileState & brokenStyle) == brokenStyle)
				{
					mBattleTile[arrayIndex].TileState = 0x1111;
				}
				else if (~brokenStyle == mBattleTile[arrayIndex].TileState)
				{
					mBattleTile[arrayIndex].TileState = 0x1111;
				}
				else
				{
					mBattleTile[arrayIndex].TileState |= brokenStyle;
				}
				mbChangeBlock = true;
			}
		}
	}
	if (KEY_MGR->IsStayKeyDown('R'))
	{
		int brokenStyle = BROKEN_BOTTOM;
		if (KEY_MGR->IsOnceKeyDown(VK_LBUTTON))
		{
			if (g_ptMouse.x < mStartPos.x ||
				g_ptMouse.x >(mStartPos.x + mBackgroundBlack->GetWidth()) ||
				g_ptMouse.y < mStartPos.y ||
				g_ptMouse.y >(mStartPos.x + mBackgroundBlack->GetHeight())) return;

			POINT tileIndex{ (g_ptMouse.x - mStartPos.x) / TILE_SIZE , (g_ptMouse.y - mStartPos.y) / TILE_SIZE };
			int arrayIndex = tileIndex.y * TILE_COUNT_X + tileIndex.x;

			if (mBattleTile[arrayIndex].Terrain == eTerrain::Wall)
			{
				mBattleTile[arrayIndex].CollisionCount++;

				if (mBattleTile[arrayIndex].CollisionCount == 3)
				{
					mBattleTile[arrayIndex].TileState = 0x1111;
				}
				else if ((mBattleTile[arrayIndex].TileState & brokenStyle) == brokenStyle)
				{
					mBattleTile[arrayIndex].TileState = 0x1111;
				}
				else if (~brokenStyle == mBattleTile[arrayIndex].TileState)
				{
					mBattleTile[arrayIndex].TileState = 0x1111;
				}
				else
				{
					mBattleTile[arrayIndex].TileState |= brokenStyle;
				}
				mbChangeBlock = true;
			}
		}
	}*/

	if (KEY_MGR->IsOnceKeyDown('1'))
	{
		LoadMap(0);
		mbLoadMap = true;
	}
	else if (KEY_MGR->IsOnceKeyDown('2'))
	{
		LoadMap(2);
		mbLoadMap = true;
	}
	else if (KEY_MGR->IsOnceKeyDown('3'))
	{
		LoadMap(3);
		mbLoadMap = true;
	}

	if (KEY_MGR->IsStayKeyDown('A'))
	{
		mScale -= 10.0f * DELTA_TIME;
	}
	if (KEY_MGR->IsStayKeyDown('D'))
	{
		mScale += 10.0f * DELTA_TIME;
	}
}

void BattleScene::Render(HDC hdc)
{
	mBackgroundGray->Render(hdc);

	mResizeHDC = mForResize->GetMemDC();
	mBackgroundBlack->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 2);

	//for (int i = 0; i < TILE_COUNT_Y; i++)
	//{
	//	for (int j = 0; j < TILE_COUNT_X; j++)
	//	{
	//		IMG_MGR->FindImage(eImageTag::Tile)->Render(hdc,
	//			tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileShape.left + TILE_SIZE / 2,
	//			tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileShape.top + TILE_SIZE / 2,
	//			tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.x,
	//			tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.y);
	//	}
	//}

	//tile->GetmPhysics()->Render(hdc);

	for (map<int, map<int, Tile*>>::iterator itX = mMapTile.begin(); itX != mMapTile.end(); itX++)
	{
		for (map<int, Tile*>::iterator itY = (*itX).second.begin(); itY != (*itX).second.end(); itY++)
		{
			(*itY).second->Render(hdc);
		}
	}
	if (mDebugMode)
	{
		mPhyscis->Render(hdc);
	}

	

	//mForResize->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 2, mScale);

}

void BattleScene::Release()
{
	/*mMapEditorInfo->Release();*/
}

void BattleScene::LoadMap(int loadIndex)
{
	TagTile arrTile[TILE_COUNT_X * TILE_COUNT_Y];

	string loadFileName = "Save/saveMapData_" + to_string(loadIndex);
	loadFileName += ".map";
	DWORD mapSaveTileInfo = sizeof(TagTile) * TILE_COUNT_X * TILE_COUNT_Y;

	HANDLE hFile = CreateFile(loadFileName.c_str(),
		GENERIC_READ,           // 읽기, 쓰기
		0, NULL,                // 공유, 보안 모드
		OPEN_EXISTING,          // 파일 만들거나 읽을 때 옵션
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성(읽기 전용, 숨김 등등)
		NULL);

	// 읽기
	DWORD readByte;
	if (ReadFile(hFile, arrTile, mapSaveTileInfo,
		&readByte, NULL) == false)
	{
		MessageBox(g_hWnd, "맵 데이터 로드에 실패했습니다.", "에러", MB_OK);
	}

	Tile* newTile = nullptr;
	eCollisionTag tileTag = eCollisionTag::Block;
	for (int x = 0; x < TILE_COUNT_X; x++)
	{
		for (int y = 0; y < TILE_COUNT_Y; y++)
		{
			if (arrTile[y * TILE_COUNT_X + x].Terrain != eTerrain::None)
			{
				switch (arrTile[y * TILE_COUNT_X + x].Terrain)
				{
				case eTerrain::Wall:
					tileTag = eCollisionTag::Block;
					break;
				case eTerrain::Water:
					tileTag = eCollisionTag::Water;
					break;
				default:
					tileTag = eCollisionTag::Block;
					break;
				}
				newTile = new Tile;
				newTile->Init(arrTile[y * TILE_COUNT_X + x],
					mPhyscis->CreateCollider({ mStartPos.x + arrTile[y * TILE_COUNT_X + x].TileShape.left + TILE_SIZE * 0.5f,
						mStartPos.y + arrTile[y * TILE_COUNT_X + x].TileShape.top + TILE_SIZE * 0.5f },
						TILE_SIZE,
						newTile,
						tileTag),
					{ mStartPos.x + arrTile[y * TILE_COUNT_X + x].TileShape.left + (int)(TILE_SIZE * 0.5f),
						mStartPos.y + arrTile[y * TILE_COUNT_X + x].TileShape.top + static_cast<int>(TILE_SIZE * 0.5f) });

				mMapTile[x][y] = newTile;
			}
		}
	}

	CloseHandle(hFile);
}

void BattleScene::ChangeBlock()
{
	//for (int i = 0; i < TILE_COUNT_Y; i++)
	//{
	//	for (int j = 0; j < TILE_COUNT_X; j++)
	//	{
	//		if (tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileState == 0x0000) {}
	//		else if (tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileState == 0x1111) { tile->GetmTileInfo()[i * TILE_COUNT_X + j].Terrain = eTerrain::None; tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.x = 4; tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.y = 4; tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileState = 0; }
	//
	//		else if (tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileState == 0x1110) { tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.x = 2; tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.y = 4; }
	//		else if (tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileState == 0x0111) { tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.x = 3; tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.y = 4; }
	//		else if (tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileState == 0x1011) { tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.x = 3; tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.y = 5; }
	//		else if (tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileState == 0x1101) { tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.x = 2; tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.y = 5; }
	//
	//		else if (tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileState == 0x1100) { tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.x = 0; tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.y = 4; }
	//		else if (tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileState == 0x0110) { tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.x = 1; tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.y = 5; }
	//		else if (tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileState == 0x0011) { tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.x = 0; tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.y = 5; }
	//		else if (tile->GetmTileInfo()[i * TILE_COUNT_X + j].TileState == 0x1001) { tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.x = 1; tile->GetmTileInfo()[i * TILE_COUNT_X + j].TilePos.y = 4; }
	//		
	//		mbChangeBlock = false;
	//	}
	//}
}
//
//void BattleScene::OnCollided(eCollisionDir dir, eCollisionTag tag)
//{
//	switch (tag)
//	{
//	case eCollisionTag::EnemyAmmo:
//		switch (dir)
//		{
//		case eCollisionDir::Left:
//			int brokenStyle = BROKEN_LEFT;
//			POINT tileIndex{ (g_ptMouse.x - mStartPos.x) / TILE_SIZE , (g_ptMouse.y - mStartPos.y) / TILE_SIZE };
//			int arrayIndex = tileIndex.y * TILE_COUNT_X + tileIndex.x;
//
//			cout << (int)mBattleTile[arrayIndex].Terrain << endl;
//
//			if (mBattleTile[arrayIndex].Terrain == eTerrain::Wall)
//			{
//				mBattleTile[arrayIndex].CollisionCount++;
//
//				if (mBattleTile[arrayIndex].CollisionCount == 3)
//				{
//					mBattleTile[arrayIndex].TileState = 0x1111;
//				}
//				else if ((mBattleTile[arrayIndex].TileState & brokenStyle) == brokenStyle)
//				{
//					mBattleTile[arrayIndex].TileState = 0x1111;
//				}
//				else if ((~brokenStyle) == mBattleTile[arrayIndex].TileState)
//				{
//					mBattleTile[arrayIndex].TileState = 0x1111;
//				}
//				else
//				{
//					mBattleTile[arrayIndex].TileState |= brokenStyle;
//				}
//				mbChangeBlock = true;
//			}
//		}
//		break;
//	}
//}

