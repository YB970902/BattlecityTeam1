#include "MapEditor.h"
#include "Config.h"
#include "CommonFunction.h"
#include "Image.h"

HRESULT MapEditor::Init()
{
	mTileImage = IMG_MGR->AddImage(eImageTag::Tile, "Image/SampleTile1.bmp", 160, 96, 10, 6, true, RGB(255, 0, 255));
	mBackground = IMG_MGR->AddImage(eImageTag::TileBackground, "Image/background.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	mBackgroundInner = IMG_MGR->AddImage(eImageTag::TileBackgroundInner, "Image/background2.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	mButtonImage = IMG_MGR->AddImage(eImageTag::TileButton, "Image/button02.bmp", 60, 270, 2, 9, true, RGB(255, 0, 255));
	mEnemyOrderBoxImage = IMG_MGR->AddImage(eImageTag::EnemyOrderBox, "Image/Enemy/Enemy.bmp", (ENEMY_TILE_SIZE*8), (ENEMY_TILE_SIZE*6), 8, 6, true, RGB(255, 0, 255));

	if (mTileImage == nullptr)
	{
		cout << "로드 실패!" << endl;
		return E_FAIL;
	}

	for (int i = 0; i < TILE_COUNT_Y; i++)
	{
		for (int j = 0; j < TILE_COUNT_X; j++)
		{
			SetRect(&(mTileInfo[i * TILE_COUNT_X + j].TileShape), j * TILE_SIZE, i * TILE_SIZE, j * TILE_SIZE + TILE_SIZE, i * TILE_SIZE + TILE_SIZE);
			mTileInfo[i * TILE_COUNT_X + j].TilePos.x = 1;
			mTileInfo[i * TILE_COUNT_X + j].TilePos.y = 0;
			mTileInfo[i * TILE_COUNT_X + j].Terrain = eTerrain::None;
		}
	}

	for (int i = 0; i < SAMPLETILE_COUNT_Y; i++)
	{
		for (int j = 0; j < SAMPLETILE_COUNT_X; j++)
		{
			SetRect(&(mTileInfoSample[i * SAMPLETILE_COUNT_X + j].TileShape),
				WIN_SIZE_X - mTileImage->GetWidth() + j * TILE_SIZE,
				i * TILE_SIZE,
				WIN_SIZE_X - mTileImage->GetWidth() + j * TILE_SIZE + TILE_SIZE,
				i * TILE_SIZE + TILE_SIZE);

			mTileInfoSample[i * SAMPLETILE_COUNT_X + j].TilePos.x = j;
			mTileInfoSample[i * SAMPLETILE_COUNT_X + j].TilePos.y = i;

			if (i == 0 || i == 1)
			{
				if (j == 0 || j == 1) { mTileInfoSample[i * SAMPLETILE_COUNT_X + j].Terrain = eTerrain::Wall; }
				else if (j == 8 || j == 9) { mTileInfoSample[i * SAMPLETILE_COUNT_X + j].Terrain = eTerrain::Grass; }
				else { mTileInfoSample[i * SAMPLETILE_COUNT_X + j].Terrain = eTerrain::Water; }
			}
			else if (i == 2 || i == 3)
			{
				if (j == 0 || j == 1) { mTileInfoSample[i * SAMPLETILE_COUNT_X + j].Terrain = eTerrain::UnbreakableWall; }
				else if (j == 2 || j == 3) { mTileInfoSample[i * SAMPLETILE_COUNT_X + j].Terrain = eTerrain::Iron; }
				else if (j == 4 || j == 5) { mTileInfoSample[i * SAMPLETILE_COUNT_X + j].Terrain = eTerrain::Nexus; }
				else if (j == 6 || j == 7) { mTileInfoSample[i * SAMPLETILE_COUNT_X + j].Terrain = eTerrain::FlagNormal; }
				else if (j == 8 || j == 9) { mTileInfoSample[i * SAMPLETILE_COUNT_X + j].Terrain = eTerrain::FlagEnemy; }
			}
			else if (i == 4 || i == 5)
			{
				if (j == 8 || j == 9) { mTileInfoSample[i * SAMPLETILE_COUNT_X + j].Terrain = eTerrain::FlagPlayer; }
			}
			else
			{
				mTileInfoSample[i * SAMPLETILE_COUNT_X + j].Terrain = eTerrain::None;
			}
		}
	}
	mSampleAreaShape.left = WIN_SIZE_X - mTileImage->GetWidth();
	mSampleAreaShape.right = WIN_SIZE_X;
	mSampleAreaShape.top = 0;
	mSampleAreaShape.bottom = mTileImage->GetHeight();

	mDrawingArea.left = 0;
	mDrawingArea.top = 0;
	mDrawingArea.right = TILE_SIZE * TILE_COUNT_X;
	mDrawingArea.bottom = TILE_SIZE * TILE_COUNT_Y;

	for (int i = 0; i < BUTTON_ENEMY_MAX_COUNT; i++)
	{
		mButtonInfo[i].ButtonPos.x = (mDrawingArea.right + BUTTON_SIZE) + (i * BUTTON_SIZE);
		mButtonInfo[i].ButtonPos.y = BUTTON_SIZE;
		SetRect(&(mButtonInfo[i].ButtonShape),
			mButtonInfo[i].ButtonPos.x - BUTTON_SIZE / 2,
			mButtonInfo[i].ButtonPos.y - BUTTON_SIZE / 2,
			mButtonInfo[i].ButtonPos.x + BUTTON_SIZE / 2,
			mButtonInfo[i].ButtonPos.y + BUTTON_SIZE / 2);
		mButtonInfo[i].bButtonOn = false;
	}
	
	mEnemyOrderInfo.EnemyOrderPos = { TILE_SIZE , mDrawingArea.bottom + ENEMY_TILE_SIZE };
	mEnemyOrderInfo.EnemyOrderTypeImage = 0;

	return S_OK;

}

void MapEditor::Update()
{

	if (KEY_MGR->IsOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&(mSampleAreaShape), g_ptMouse))
		{
			mbIsClicked = true;
			mClickedStartIndex.x = (g_ptMouse.x - mSampleAreaShape.left) / TILE_SIZE;
			mClickedStartIndex.y = (g_ptMouse.y - mSampleAreaShape.top) / TILE_SIZE;
			mClickedArea.x = 1;
			mClickedArea.y = 1;
		}
	}
	if (KEY_MGR->IsOnceKeyUp(VK_LBUTTON))
	{
		if (PtInRect(&(mSampleAreaShape), g_ptMouse) && mbIsClicked)
		{
			POINT clickEndIndex = POINT{ (g_ptMouse.x - mSampleAreaShape.left) / TILE_SIZE,	(g_ptMouse.y - mSampleAreaShape.top) / TILE_SIZE };

			if (clickEndIndex.x < mClickedStartIndex.x)
			{
				long tempChangeIndex = clickEndIndex.x;
				clickEndIndex.x = mClickedStartIndex.x;
				mClickedStartIndex.x = tempChangeIndex;
			}
			if (clickEndIndex.y < mClickedStartIndex.y)
			{
				long tempChangeIndex = clickEndIndex.y;
				clickEndIndex.y = mClickedStartIndex.y;
				mClickedStartIndex.y = tempChangeIndex;
			}
			mClickedArea.x = (clickEndIndex.x - mClickedStartIndex.x) + 1;
			mClickedArea.y = (clickEndIndex.y - mClickedStartIndex.y) + 1;

		}
		mbIsClicked = false;
	}

	if (KEY_MGR->IsStayKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&mDrawingArea, g_ptMouse))
		{
			POINT clickedPos = { g_ptMouse.x / TILE_SIZE, g_ptMouse.y / TILE_SIZE };
			for (int x = 0; x < mClickedArea.x; x++)
			{
				for (int y = 0; y < mClickedArea.y; y++)
				{
					if ((clickedPos.x + x + 1) > (mDrawingArea.right / TILE_SIZE)) break;
					mTileInfo[(clickedPos.y + y) * TILE_COUNT_X + (clickedPos.x + x)].TilePos.x = mClickedStartIndex.x + x;
					mTileInfo[(clickedPos.y + y) * TILE_COUNT_X + (clickedPos.x + x)].TilePos.y = mClickedStartIndex.y + y;
					mTileInfo[(clickedPos.y + y) * TILE_COUNT_X + (clickedPos.x + x)].Terrain =
						mTileInfoSample[(mClickedStartIndex.y + y) * SAMPLETILE_COUNT_X + (mClickedStartIndex.x + x)].Terrain;
				}
			}
		}
	}

	for (int i = 0; i < BUTTON_ENEMY_MAX_COUNT; i++)
	{
		if (PtInRect(&(mButtonInfo[i].ButtonShape), g_ptMouse))
		{
			if (KEY_MGR->IsOnceKeyDown(VK_LBUTTON))
			{
				mButtonInfo[i].bButtonOn = true;
				switch (i)
				{
				case 0:
					if (mEnemyOrderInfo.EnemyCount < ENEMY_MAX_COUNT)ENEMY_INFO_BOX[mEnemyOrderInfo.EnemyCount++] = ENEMY_ORDERBOX_ENEMY1;
					break;
				case 1:
					if (mEnemyOrderInfo.EnemyCount < ENEMY_MAX_COUNT)ENEMY_INFO_BOX[mEnemyOrderInfo.EnemyCount++] = ENEMY_ORDERBOX_ENEMY2;
					break;
				case 2:
					if (mEnemyOrderInfo.EnemyCount < ENEMY_MAX_COUNT)ENEMY_INFO_BOX[mEnemyOrderInfo.EnemyCount++] = ENEMY_ORDERBOX_ENEMY3;
					break;
				case 3:
					if (mEnemyOrderInfo.EnemyCount < ENEMY_MAX_COUNT)ENEMY_INFO_BOX[mEnemyOrderInfo.EnemyCount++] = ENEMY_ORDERBOX_ENEMY4;
					break;
				case 4:
					if (mEnemyOrderInfo.EnemyCount > 0) { ENEMY_INFO_BOX[mEnemyOrderInfo.EnemyCount - 1] = NULL; mEnemyOrderInfo.EnemyCount--; }
					break;
				case 5:
					SaveMap();
					break;
				case 6:
					LoadMap();
					break;
				case 7:
					SaveEnemyOrder();
					break;
				case 8:
					LoadEnemyOrder();
					break;
				}
			}
			if (KEY_MGR->IsOnceKeyUp(VK_LBUTTON))
			{
				mButtonInfo[i].bButtonOn = false;
			}
		}
		else
		{
			mButtonInfo[i].bButtonOn = false;
		}
	}
	
	// 디버깅 용
	if(KEY_MGR->IsOnceKeyDown(VK_SPACE))
	{
		for (int i = 0; i < mEnemyOrderInfo.EnemyCount; i++)
			cout << mEnemyOrderInfo.EnemyOrderBox[mEnemyOrderInfo.EnemyCount] << endl;
	}
}

void MapEditor::Render(HDC hdc)
{
	mBackground->Render(hdc);
	mBackgroundInner->Render(hdc);

	for (int i = 0; i < TILE_COUNT_Y; i++)
	{
		for (int j = 0; j < TILE_COUNT_X; j++)
		{
			mTileImage->Render(hdc,
				mTileInfo[i * TILE_COUNT_X + j].TileShape.left + TILE_SIZE / 2,
				mTileInfo[i * TILE_COUNT_X + j].TileShape.top + TILE_SIZE / 2,
				mTileInfo[i * TILE_COUNT_X + j].TilePos.x,
				mTileInfo[i * TILE_COUNT_X + j].TilePos.y);
		}
	}

	mTileImage->Render(hdc,
		WIN_SIZE_X - mTileImage->GetWidth() / 2, mTileImage->GetHeight() / 2);

	for (int i = 0; i < mClickedArea.y; i++)
	{
		for (int j = 0; j < mClickedArea.x; j++)
		{
			mTileImage->Render(hdc, WIN_SIZE_X - mTileImage->GetWidth() + (TILE_SIZE * j), WIN_SIZE_Y / 4 + (TILE_SIZE * i), mClickedStartIndex.x + j, mClickedStartIndex.y + i);
		}
	}

	for (int i = 0; i < BUTTON_ENEMY_MAX_COUNT; i++)
	{
		mButtonImage->Render(hdc,
			mButtonInfo[i].ButtonPos.x,
			mButtonInfo[i].ButtonPos.y,
			mButtonInfo[i].bButtonOn,
			0 + i);
	}

	for (int i = 0; i < mEnemyOrderInfo.EnemyCount; i++)
	{
		switch (mEnemyOrderInfo.EnemyOrderBox[i])
		{
		case ENEMY_ORDERBOX_ENEMY1:
			mEnemyOrderInfo.EnemyOrderTypeImage = 0;
			break;
		case ENEMY_ORDERBOX_ENEMY2:
			mEnemyOrderInfo.EnemyOrderTypeImage = 1;
			break;
		case ENEMY_ORDERBOX_ENEMY3:
			mEnemyOrderInfo.EnemyOrderTypeImage = 2;
			break;
		case ENEMY_ORDERBOX_ENEMY4:
			mEnemyOrderInfo.EnemyOrderTypeImage = 3;
			break;
		}

		mEnemyOrderBoxImage->Render(hdc,
			mEnemyOrderInfo.EnemyOrderPos.x + ((i % 10) * ENEMY_TILE_SIZE),
			mEnemyOrderInfo.EnemyOrderPos.y + ((i / 10) * ENEMY_TILE_SIZE),
			0,
			mEnemyOrderInfo.EnemyOrderTypeImage);
	}
}

void MapEditor::Release()
{

}

void MapEditor::SaveMap(int saveIndex)
{
	string saveFileName = "Save/saveMapData_" + (to_string(saveIndex)) + ".map";
	/*saveFileName += ".map";*/

	HANDLE hFile = CreateFile(saveFileName.c_str(),
		GENERIC_WRITE,          // 읽기, 쓰기
		NULL, NULL,                // 공유, 보안 모드
		CREATE_ALWAYS,          // 파일 만들거나 읽을 때 옵션
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성(읽기 전용, 숨김 등등)
		NULL);

	// 쓰기
	DWORD mapSaveTileInfo = sizeof(TagTile) * TILE_COUNT_X * TILE_COUNT_Y;

	DWORD writtenByte;
	if (WriteFile(hFile,    // 파일 핸들
		mTileInfo,       // 메모리 시작주소
		mapSaveTileInfo,  // 메모리 크기
		&writtenByte,   // 실제 쓰여진 파일 용량
		NULL) == false)          // ???
	{
		MessageBox(g_hWnd, "맵 데이터 저장에 실패했습니다.", "에러", MB_OK);
	}

	CloseHandle(hFile);

	//(parsing)  .ini, .json, html
}



void MapEditor::LoadMap(int loadIndex)
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
	if (ReadFile(hFile, mTileInfo, mapSaveTileInfo,
		&readByte, NULL) == false)
	{
		MessageBox(g_hWnd, "맵 데이터 로드에 실패했습니다.", "에러", MB_OK);
	}

	CloseHandle(hFile);
}

void MapEditor::SaveEnemyOrder(int saveIndex)
{
	string saveFileName = "Save/saveEnemyOrderData_" + to_string(saveIndex);
	saveFileName += ".map";

	HANDLE hFile = CreateFile(saveFileName.c_str(),
		GENERIC_WRITE,          // 읽기, 쓰기
		0, NULL,                // 공유, 보안 모드
		CREATE_ALWAYS,          // 파일 만들거나 읽을 때 옵션
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성(읽기 전용, 숨김 등등)
		NULL);

	// 쓰기
	DWORD writtenByte;
	if (WriteFile(hFile,    // 파일 핸들
		&mEnemyOrderInfo,       // 메모리 시작주소
		sizeof(TagEnemyOrderInfo),  // 메모리 크기
		&writtenByte,   // 실제 쓰여진 파일 용량
		NULL) == false)          // ???
	{
		MessageBox(g_hWnd, "맵 데이터 저장에 실패했습니다.", "에러", MB_OK);
	}

	CloseHandle(hFile);

	//(parsing)  .ini, .json, html
}

void MapEditor::LoadEnemyOrder(int loadIndex)
{
	string loadFileName = "Save/saveEnemyOrderData_" + to_string(loadIndex);
	loadFileName += ".map";

	HANDLE hFile = CreateFile(loadFileName.c_str(),
		GENERIC_READ,           // 읽기, 쓰기
		0, NULL,                // 공유, 보안 모드
		OPEN_EXISTING,          // 파일 만들거나 읽을 때 옵션
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성(읽기 전용, 숨김 등등)
		NULL);

	// 읽기
	DWORD readByte;
	if (ReadFile(hFile, &mEnemyOrderInfo, sizeof(TagEnemyOrderInfo),
		&readByte, NULL) == false)
	{
		MessageBox(g_hWnd, "맵 데이터 로드에 실패했습니다.", "에러", MB_OK);
	}

	CloseHandle(hFile);
}
