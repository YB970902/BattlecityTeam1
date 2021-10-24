#include "Config.h"
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

	mPhyscis = new Physcis;

	mbLoadMap = true;
	mEnemyInfo = new TagEnemyInfo;

	LoadMap();
	LoadEnemyOrder();

	return S_OK;
}

void BattleScene::Update()
{
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
				mPhyscis->DestroyCollider(temp->GetCollider());
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

	if (KEY_MGR->IsStayKeyDown('A'))
	{
		mScale -= 10.0f * DELTA_TIME;
	}
	if (KEY_MGR->IsStayKeyDown('D'))
	{
		mScale += 10.0f * DELTA_TIME;
	}

	if (KEY_MGR->IsStayKeyDown('O'))
	{
		mbItemBlock = true;
	}

	if (mbItemBlock)
	{
		mItemTime += DELTA_TIME;
		for (map<int, map<int, Tile*>>::iterator itX = mMapTile.begin(); itX != mMapTile.end(); itX++)
		{
			for (map<int, Tile*>::iterator itY = (*itX).second.begin(); itY != (*itX).second.end(); itY++)
			{
				if ((*itY).second->GetTileInfo()->NexusAroundTile == true)
				{
					POINT tempPos = { 0,2 };
					(*itY).second->GetTileInfo()->Terrain = eTerrain::UnbreakableWall;
					(*itY).second->SetImagePos(tempPos);

					if (mItemTime >= 3.55f)
					{
						POINT tempPos = { 0,0 };
						(*itY).second->GetTileInfo()->Terrain = eTerrain::Wall;
						(*itY).second->SetImagePos(tempPos);
					}
				}
			}
		}
		if (mItemTime >= 3.55f)
		{
			mbItemBlock = false;
			mItemTime = 0;
		}
	}
}

void BattleScene::Render(HDC hdc)
{
	mBackgroundGray->Render(hdc);

	mResizeHDC = mForResize->GetMemDC();
	mBackgroundBlack->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 2);

	for (map<int, map<int, Tile*>>::iterator itX = mMapTile.begin(); itX != mMapTile.end(); itX++)
	{
		for (map<int, Tile*>::iterator itY = (*itX).second.begin(); itY != (*itX).second.end(); itY++)
		{
			(*itY).second->Render(hdc);
		}
	}
}

void BattleScene::Release()
{
	SAFE_RELEASE(mPhyscis);
	SAFE_RELEASE(mForResize);

	if (mEnemyInfo)
	{
		delete mEnemyInfo;
		mEnemyInfo = nullptr;
	}
}

void BattleScene::LoadMap(int loadIndex)
{
	TagTile arrTile[TILE_COUNT_X * TILE_COUNT_Y];

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
	if (ReadFile(hFile, arrTile, mapSaveTileInfo,
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

				mMapTile[y][x] = newTile;
			}
		}
	}

	CloseHandle(hFile);
}

void BattleScene::ClearTileMap()
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
void BattleScene::LoadEnemyOrder(int loadIndex)
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

