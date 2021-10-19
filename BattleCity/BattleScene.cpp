#include "BattleScene.h"
#include "Image.h"


HRESULT BattleScene::Init()
{
	mBackgroundGray = IMG_MGR->AddImage(eImageTag::BattleSceneGrayBG, "Image/background.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	mBackgroundBlack = IMG_MGR->AddImage(eImageTag::BattleSceneBlackBG, "Image/background2.bmp", 16 * 26, 16 * 26);

	mStartPos = { (mBackgroundGray->GetWidth() - mBackgroundBlack->GetWidth()) / 2 ,(mBackgroundGray->GetHeight() - mBackgroundBlack->GetHeight()) / 2 };

	LoadMap(0);

	mbLoadMap = true;
	
	return S_OK;
}

void BattleScene::Update()
{
	if (mbChangeBlock){ ChangeBlock(); }

	mChangeWaterTileTime += TIMER_MGR->GetDeltaTime();

	for (int i = 0; i < TILE_COUNT_Y; i++)
	{
		for (int j = 0; j < TILE_COUNT_X; j++)
		{
			if (mBattleTile[i * TILE_COUNT_X + j].Terrain == eTerrain::Water)
			{
				if (mChangeWaterTileTime >= CHANGE_WATER_TILE_TIME)
				{
					if (mBattleTile[i * TILE_COUNT_X + j].TilePos.x < 6) {
						mBattleTile[i * TILE_COUNT_X + j].TilePos.x = mBattleTile[i * TILE_COUNT_X + j].TilePos.x + 2;
					}
					else
					{
						mBattleTile[i * TILE_COUNT_X + j].TilePos.x = mBattleTile[i * TILE_COUNT_X + j].TilePos.x - 2;
					}
				}
			}
		}
	}

	if(mChangeWaterTileTime >= CHANGE_WATER_TILE_TIME) mChangeWaterTileTime = 0;

	cout << mChangeWaterTileTime << endl;

	if(mbLoadMap)
	{
		for (int i = 0; i < TILE_COUNT_Y; i++)
		{
			for (int j = 0; j < TILE_COUNT_X; j++)
			{
				SetRect(&(mBattleTile[i * TILE_COUNT_X + j].TileShape),
					mStartPos.x + TILE_SIZE * j,
					mStartPos.y + TILE_SIZE * i,
					mStartPos.x + TILE_SIZE * j + TILE_SIZE,
					mStartPos.y + TILE_SIZE * i + TILE_SIZE);
			}
		}
		mbLoadMap = false;
		mbChangeBlock = false;
	}

	if (KEY_MGR->IsStayKeyDown('Q'))
	{
		int brokenStyle = BROKEN_LEFT;
		if (KEY_MGR->IsOnceKeyDown(VK_LBUTTON))
		{
			if (g_ptMouse.x < mStartPos.x ||
				g_ptMouse.x >(mStartPos.x + mBackgroundBlack->GetWidth()) ||
				g_ptMouse.y < mStartPos.y ||
				g_ptMouse.y >(mStartPos.x + mBackgroundBlack->GetHeight())) return;


			POINT tileIndex{ (g_ptMouse.x - mStartPos.x) / TILE_SIZE , (g_ptMouse.y - mStartPos.y) / TILE_SIZE };
			int arrayIndex = tileIndex.y * TILE_COUNT_X + tileIndex.x;

			cout << (int)mBattleTile[arrayIndex].Terrain << endl;

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
				else if ((~brokenStyle) == mBattleTile[arrayIndex].TileState)
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
	}
	
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
}

void BattleScene::Render(HDC hdc)
{
	mBackgroundGray->Render(hdc);
	mBackgroundBlack->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 2);

	for (int i = 0; i < TILE_COUNT_Y; i++)
	{
		for (int j = 0; j < TILE_COUNT_X; j++)
		{
			IMG_MGR->FindImage(eImageTag::Tile)->Render(hdc,
				mBattleTile[i * TILE_COUNT_X + j].TileShape.left + TILE_SIZE / 2,
				mBattleTile[i * TILE_COUNT_X + j].TileShape.top + TILE_SIZE / 2,
				mBattleTile[i * TILE_COUNT_X + j].TilePos.x,
				mBattleTile[i * TILE_COUNT_X + j].TilePos.y);
		}
	}

}

void BattleScene::Release()
{
	/*mMapEditorInfo->Release();*/
}

void BattleScene::LoadMap(int loadIndex)
{

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
	if (ReadFile(hFile, mBattleTile, mapSaveTileInfo,
		&readByte, NULL) == false)
	{
		MessageBox(g_hWnd, "맵 데이터 로드에 실패했습니다.", "에러", MB_OK);
	}

	CloseHandle(hFile);
}

void BattleScene::ChangeBlock()
{
	for (int i = 0; i < TILE_COUNT_Y; i++)
	{
		for (int j = 0; j < TILE_COUNT_X; j++)
		{
			if (mBattleTile[i * TILE_COUNT_X + j].TileState == 0x0000) {}
			else if (mBattleTile[i * TILE_COUNT_X + j].TileState == 0x1111) { mBattleTile[i * TILE_COUNT_X + j].Terrain = eTerrain::None; mBattleTile[i * TILE_COUNT_X + j].TilePos.x = 4; mBattleTile[i * TILE_COUNT_X + j].TilePos.y = 4; }

			else if (mBattleTile[i * TILE_COUNT_X + j].TileState == 0x1110) { mBattleTile[i * TILE_COUNT_X + j].TilePos.x = 2; mBattleTile[i * TILE_COUNT_X + j].TilePos.y = 4; }
			else if (mBattleTile[i * TILE_COUNT_X + j].TileState == 0x0111) { mBattleTile[i * TILE_COUNT_X + j].TilePos.x = 3; mBattleTile[i * TILE_COUNT_X + j].TilePos.y = 4; }
			else if (mBattleTile[i * TILE_COUNT_X + j].TileState == 0x1011) { mBattleTile[i * TILE_COUNT_X + j].TilePos.x = 3; mBattleTile[i * TILE_COUNT_X + j].TilePos.y = 5; }
			else if (mBattleTile[i * TILE_COUNT_X + j].TileState == 0x1101) { mBattleTile[i * TILE_COUNT_X + j].TilePos.x = 2; mBattleTile[i * TILE_COUNT_X + j].TilePos.y = 5; }

			else if (mBattleTile[i * TILE_COUNT_X + j].TileState == 0x1100) { mBattleTile[i * TILE_COUNT_X + j].TilePos.x = 0; mBattleTile[i * TILE_COUNT_X + j].TilePos.y = 4; }
			else if (mBattleTile[i * TILE_COUNT_X + j].TileState == 0x0110) { mBattleTile[i * TILE_COUNT_X + j].TilePos.x = 1; mBattleTile[i * TILE_COUNT_X + j].TilePos.y = 5; }
			else if (mBattleTile[i * TILE_COUNT_X + j].TileState == 0x0011) { mBattleTile[i * TILE_COUNT_X + j].TilePos.x = 0; mBattleTile[i * TILE_COUNT_X + j].TilePos.y = 5; }
			else if (mBattleTile[i * TILE_COUNT_X + j].TileState == 0x1001) { mBattleTile[i * TILE_COUNT_X + j].TilePos.x = 1; mBattleTile[i * TILE_COUNT_X + j].TilePos.y = 4; }
			
			mbChangeBlock = false;
		}
	}
}

