#include "MapEditor.h"
#include "CommonFunction.h"
#include "Image.h"

HRESULT MapEditor::Init()
{
	mTileImage = IMG_MGR->FindImage(eImageTag::Tile);
	mBackground = IMG_MGR->FindImage(eImageTag::TileBackground);
	mBackgroundInner = IMG_MGR->FindImage(eImageTag::TileBackgroundInner);
	mButtonImage = IMG_MGR->FindImage(eImageTag::TileButton);
	mEnemyOrderBoxImage = IMG_MGR->FindImage(eImageTag::EnemyOrderBox);

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
			mTileInfo[i * TILE_COUNT_X + j].TilePos.x = 4;
			mTileInfo[i * TILE_COUNT_X + j].TilePos.y = 4;
			mTileInfo[i * TILE_COUNT_X + j].TileState = 0;
			mTileInfo[i * TILE_COUNT_X + j].Terrain = eTerrain::None;
			mTileInfo[i * TILE_COUNT_X + j].NexusAroundTile = false;
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
				if (j == 0 || j == 1 || j == 2 || j == 3) { mTileInfoSample[i * SAMPLETILE_COUNT_X + j].Terrain = eTerrain::Wall; }
				else if (j == 8 || j == 9) { mTileInfoSample[i * SAMPLETILE_COUNT_X + j].Terrain = eTerrain::FlagPlayer; }
				else{ mTileInfoSample[i * SAMPLETILE_COUNT_X + j].Terrain = eTerrain::None; }
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
	
	for (int i = 0; i < ENEMY_MAX_COUNT; i++)
	{
		mEnemyInfo.mEnemyOrderType[i] = { eTankType::None};
	}

	mEasySaveIndex = 0;
	return S_OK;

}

void MapEditor::Update()
{
	if (KEY_MGR->IsStayKeyDown('Q'))
	{
		if (KEY_MGR->IsOnceKeyDown('0'))
		{
			mEasySaveIndex = '0';
			cout << mEasySaveIndex << endl;
		}
		if (KEY_MGR->IsOnceKeyDown('1'))
		{
			mEasySaveIndex = '1';
			cout << mEasySaveIndex << endl;
		}
		if (KEY_MGR->IsOnceKeyDown('2'))
		{
			mEasySaveIndex = '2';
			cout << mEasySaveIndex << endl;
		}
		if (KEY_MGR->IsOnceKeyDown('3'))
		{
			mEasySaveIndex = '3';
			cout << mEasySaveIndex << endl;
		}

		for (int i = 0; i < TILE_COUNT_Y; i++)
		{
			for (int j = 0; j < TILE_COUNT_X; j++)
			{
				if (PtInRect(&(mTileInfo[i * TILE_COUNT_X + j].TileShape), g_ptMouse))
				{
					if (KEY_MGR->IsOnceKeyDown(VK_LBUTTON))
					{
						cout << (int)mTileInfo[i * TILE_COUNT_X + j].Terrain << endl;

					}
				}
			}
		}
	}

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

					if (mTileInfo[(clickedPos.y + y) * TILE_COUNT_X + (clickedPos.x + x)].Terrain == eTerrain::Nexus)
					{
						if (mTileInfo[(clickedPos.y + y) * TILE_COUNT_X + (clickedPos.x + x)].TilePos.x == NEXUS_AROUNDTILE_LEFT)
						{
							if (mTileInfo[(clickedPos.y + y) * TILE_COUNT_X + (clickedPos.x + x)].TilePos.y == NEXUS_AROUNDTILE_TOP)
							{
								SetNexusAroundTile(x, y, clickedPos.x, clickedPos.y, NEXUS_AROUNDTILE_LEFT, NEXUS_AROUNDTILE_TOP);
							}
							else if (mTileInfo[(clickedPos.y + y) * TILE_COUNT_X + (clickedPos.x + x)].TilePos.y == NEXUS_AROUNDTILE_BOTTOM)
							{
								SetNexusAroundTile(x, y, clickedPos.x, clickedPos.y, NEXUS_AROUNDTILE_LEFT, NEXUS_AROUNDTILE_BOTTOM);
							}
						}
						else if (mTileInfo[(clickedPos.y + y) * TILE_COUNT_X + (clickedPos.x + x)].TilePos.x == NEXUS_AROUNDTILE_RIGHT)
						{
							if (mTileInfo[(clickedPos.y + y) * TILE_COUNT_X + (clickedPos.x + x)].TilePos.y == NEXUS_AROUNDTILE_TOP)
							{
								SetNexusAroundTile(x, y, clickedPos.x, clickedPos.y, NEXUS_AROUNDTILE_RIGHT, NEXUS_AROUNDTILE_TOP);
							}
							else if (mTileInfo[(clickedPos.y + y) * TILE_COUNT_X + (clickedPos.x + x)].TilePos.y == NEXUS_AROUNDTILE_BOTTOM)
							{
								SetNexusAroundTile(x, y, clickedPos.x, clickedPos.y, NEXUS_AROUNDTILE_RIGHT, NEXUS_AROUNDTILE_BOTTOM);
							}
						}
					}
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
					if (mEnemyInfo.mEnemyOrderCount >= ENEMY_MAX_COUNT) break;
					mEnemyInfo.mEnemyOrderType[mEnemyInfo.mEnemyOrderCount] = eTankType::NormalTank;
					mEnemyInfo.mEnemyOrderCount++;
					break;
				case 1:
					if (mEnemyInfo.mEnemyOrderCount >= ENEMY_MAX_COUNT) break;
					mEnemyInfo.mEnemyOrderType[mEnemyInfo.mEnemyOrderCount] = eTankType::FastSpeedTank;
					mEnemyInfo.mEnemyOrderCount++;
					break;
				case 2:
					if (mEnemyInfo.mEnemyOrderCount >= ENEMY_MAX_COUNT) break;
					mEnemyInfo.mEnemyOrderType[mEnemyInfo.mEnemyOrderCount] = eTankType::FastAmmoTank;
					mEnemyInfo.mEnemyOrderCount++;
					break;
				case 3:
					if (mEnemyInfo.mEnemyOrderCount >= ENEMY_MAX_COUNT) break;
					mEnemyInfo.mEnemyOrderType[mEnemyInfo.mEnemyOrderCount] = eTankType::TankerTank;
					mEnemyInfo.mEnemyOrderCount++;
					break;
				case 4:
					if (mEnemyInfo.mEnemyOrderCount <= 0) break;
					mEnemyInfo.mEnemyOrderType[mEnemyInfo.mEnemyOrderCount-1] = eTankType::None;
					mEnemyInfo.mEnemyOrderCount--;
					break;
				case 5:
					switch (mEasySaveIndex)
					{
					case '0':
						SaveMap(0);
						break;
					case '1':
						SaveMap(1);
						break;
					case '2':
						SaveMap(2);
						break;
					case '3':
						SaveMap(3);
						break;
					}
					break;
				case 6:
					switch (mEasySaveIndex)
					{
					case '0':
						LoadMap(0);
						break;
					case '1':
						LoadMap(1);
						break;
					case '2':
						LoadMap(2);
						break;
					case '3':
						LoadMap(3);
						break;
					}
					break;
				case 7:
					switch (mEasySaveIndex)
					{
					case '0':
						SaveEnemyOrder(0);
						break;
					case '1':
						SaveEnemyOrder(1);
						break;
					case '2':
						SaveEnemyOrder(2);
						break;
					case '3':
						SaveEnemyOrder(3);
						break;
					}
					break;
				case 8:
					switch (mEasySaveIndex)
					{
					case '0':
						LoadEnemyOrder(0);
						break;
					case '1':
						LoadEnemyOrder(1);
						break;
					case '2':
						LoadEnemyOrder(2);
						break;
					case '3':
						LoadEnemyOrder(3);
						break;
					}
					break;
				}
				for (int i = 0; i < ENEMY_MAX_COUNT; i++)
				{
					cout << (int)mEnemyInfo.mEnemyOrderType[i] << endl;
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

	for (int i = 0; i < ENEMY_MAX_COUNT; i++)
	{
		switch (mEnemyInfo.mEnemyOrderType[i])
		{
		case eTankType::NormalTank :
			mEnemyOrderBoxImage->Render(hdc, 30 * ((i % 10) + 1), (mDrawingArea.bottom + 30 * (i / 10)) + 50, 0, 0);
			break;
		case eTankType::FastSpeedTank:
			mEnemyOrderBoxImage->Render(hdc, 30 * ((i % 10) + 1), (mDrawingArea.bottom + 30 * (i / 10)) + 50, 0, 1);
			break;
		case eTankType::FastAmmoTank:
			mEnemyOrderBoxImage->Render(hdc, 30 * ((i % 10) + 1), (mDrawingArea.bottom + 30 * (i / 10)) + 50, 0, 2);
			break;
		case eTankType::TankerTank:
			mEnemyOrderBoxImage->Render(hdc, 30 * ((i % 10) + 1), (mDrawingArea.bottom + 30 * (i / 10)) + 50, 0, 3);
			break;
		default:
			break;
		}
	}
}

void MapEditor::Release()
{

}

void MapEditor::SaveMap(int saveIndex)
{
	string saveFileName = "Save/saveMapData_" + (to_string(saveIndex)) + ".map";


	HANDLE hFile = CreateFile(saveFileName.c_str(),
		GENERIC_WRITE,
		NULL, NULL, 
		CREATE_ALWAYS,     
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	DWORD mapSaveTileInfo = sizeof(TagTile) * TILE_COUNT_X * TILE_COUNT_Y;

	DWORD writtenByte;
	if (WriteFile(hFile, 
		mTileInfo,       
		mapSaveTileInfo,  
		&writtenByte,   
		NULL) == false)  
	{
		MessageBox(g_hWnd, "맵 데이터 저장에 실패했습니다.", "에러", MB_OK);
	}

	CloseHandle(hFile);
}



void MapEditor::LoadMap(int loadIndex)
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
		GENERIC_WRITE,         
		0, NULL,              
		CREATE_ALWAYS,         
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	// 쓰기
	DWORD writtenByte;
	if (WriteFile(hFile,   
		&mEnemyInfo,       
		sizeof(mEnemyInfo),
		&writtenByte, 
		NULL) == false)    
	{
		MessageBox(g_hWnd, "맵 데이터 저장에 실패했습니다.", "에러", MB_OK);
	}
	CloseHandle(hFile);

}

void MapEditor::LoadEnemyOrder(int loadIndex)
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
	if (ReadFile(hFile, &mEnemyInfo, sizeof(mEnemyInfo),
		&readByte, NULL) == false)
	{
		MessageBox(g_hWnd, "맵 데이터 로드에 실패했습니다.", "에러", MB_OK);
	}

	CloseHandle(hFile);
}

void MapEditor::SetNexusAroundTile(int x, int y, int clickedPosX, int clickedPosY, int NEXUS_AROUNDTILE_POS_X, int NEXUS_AROUNDTILE_POS_Y)
{
	int nexusAroundTilePos = 1;
	if (NEXUS_AROUNDTILE_POS_X == NEXUS_AROUNDTILE_LEFT) {
		if (NEXUS_AROUNDTILE_POS_Y == NEXUS_AROUNDTILE_TOP)
		{
			mTileInfo[(clickedPosY + y) * TILE_COUNT_X + ((clickedPosX - nexusAroundTilePos) + x)].TilePos = mTileInfoSample[0].TilePos;
			mTileInfo[((clickedPosY - nexusAroundTilePos) + y) * TILE_COUNT_X + (clickedPosX + x)].TilePos = mTileInfoSample[0].TilePos;
			mTileInfo[((clickedPosY - nexusAroundTilePos) + y) * TILE_COUNT_X + ((clickedPosX - nexusAroundTilePos) + x)].TilePos = mTileInfoSample[0].TilePos;

			mTileInfo[(clickedPosY + y) * TILE_COUNT_X + ((clickedPosX - nexusAroundTilePos) + x)].Terrain = eTerrain::Wall;
			mTileInfo[((clickedPosY - nexusAroundTilePos) + y) * TILE_COUNT_X + (clickedPosX + x)].Terrain = eTerrain::Wall;
			mTileInfo[((clickedPosY - nexusAroundTilePos) + y) * TILE_COUNT_X + ((clickedPosX - nexusAroundTilePos) + x)].Terrain = eTerrain::Wall;

			mTileInfo[(clickedPosY + y) * TILE_COUNT_X + ((clickedPosX - nexusAroundTilePos) + x)].NexusAroundTile = true;
			mTileInfo[((clickedPosY - nexusAroundTilePos) + y) * TILE_COUNT_X + (clickedPosX + x)].NexusAroundTile = true;
			mTileInfo[((clickedPosY - nexusAroundTilePos) + y) * TILE_COUNT_X + ((clickedPosX - nexusAroundTilePos) + x)].NexusAroundTile = true;
		}
		else if (NEXUS_AROUNDTILE_POS_Y == NEXUS_AROUNDTILE_BOTTOM)
		{
			mTileInfo[(clickedPosY + y) * TILE_COUNT_X + ((clickedPosX - nexusAroundTilePos) + x)].TilePos = mTileInfoSample[0].TilePos;
			//mTileInfo[((clickedPosY + nexusAroundTilePos) + y) * TILE_COUNT_X + (clickedPosX + x)].TilePos = mTileInfoSample[0].TilePos;
			//mTileInfo[((clickedPosY + nexusAroundTilePos) + y) * TILE_COUNT_X + ((clickedPosX - nexusAroundTilePos) + x)].TilePos = mTileInfoSample[0].TilePos;

			mTileInfo[(clickedPosY + y) * TILE_COUNT_X + ((clickedPosX - nexusAroundTilePos) + x)].Terrain = eTerrain::Wall;
			//mTileInfo[((clickedPosY + nexusAroundTilePos) + y) * TILE_COUNT_X + (clickedPosX + x)].Terrain = eTerrain::Wall;
			//mTileInfo[((clickedPosY + nexusAroundTilePos) + y) * TILE_COUNT_X + ((clickedPosX - nexusAroundTilePos) + x)].Terrain = eTerrain::Wall;

			mTileInfo[(clickedPosY + y) * TILE_COUNT_X + ((clickedPosX - nexusAroundTilePos) + x)].NexusAroundTile = true;
			//mTileInfo[((clickedPosY + nexusAroundTilePos) + y) * TILE_COUNT_X + (clickedPosX + x)].NexusAroundTile = true;
			//mTileInfo[((clickedPosY + nexusAroundTilePos) + y) * TILE_COUNT_X + ((clickedPosX - nexusAroundTilePos) + x)].NexusAroundTile = true;
		}
	}
	else if(NEXUS_AROUNDTILE_POS_X == NEXUS_AROUNDTILE_RIGHT)
	{
		if (NEXUS_AROUNDTILE_POS_Y == NEXUS_AROUNDTILE_TOP)
		{
			mTileInfo[(clickedPosY + y) * TILE_COUNT_X + ((clickedPosX + nexusAroundTilePos) + x)].TilePos = mTileInfoSample[0].TilePos;
			mTileInfo[((clickedPosY - nexusAroundTilePos) + y) * TILE_COUNT_X + (clickedPosX + x)].TilePos = mTileInfoSample[0].TilePos;
			mTileInfo[((clickedPosY - nexusAroundTilePos) + y) * TILE_COUNT_X + ((clickedPosX + nexusAroundTilePos) + x)].TilePos = mTileInfoSample[0].TilePos;

			mTileInfo[(clickedPosY + y) * TILE_COUNT_X + ((clickedPosX + nexusAroundTilePos) + x)].Terrain = eTerrain::Wall;
			mTileInfo[((clickedPosY - nexusAroundTilePos) + y) * TILE_COUNT_X + (clickedPosX + x)].Terrain = eTerrain::Wall;
			mTileInfo[((clickedPosY - nexusAroundTilePos) + y) * TILE_COUNT_X + ((clickedPosX + nexusAroundTilePos) + x)].Terrain = eTerrain::Wall;

			mTileInfo[(clickedPosY + y) * TILE_COUNT_X + ((clickedPosX + nexusAroundTilePos) + x)].NexusAroundTile = true;
			mTileInfo[((clickedPosY - nexusAroundTilePos) + y) * TILE_COUNT_X + (clickedPosX + x)].NexusAroundTile = true;
			mTileInfo[((clickedPosY - nexusAroundTilePos) + y) * TILE_COUNT_X + ((clickedPosX + nexusAroundTilePos) + x)].NexusAroundTile = true;
		}
		else if (NEXUS_AROUNDTILE_POS_Y == NEXUS_AROUNDTILE_BOTTOM)
		{
			mTileInfo[(clickedPosY + y) * TILE_COUNT_X + ((clickedPosX + nexusAroundTilePos) + x)].TilePos = mTileInfoSample[0].TilePos;
			//mTileInfo[((clickedPosY + nexusAroundTilePos) + y) * TILE_COUNT_X + (clickedPosX + x)].TilePos = mTileInfoSample[0].TilePos;
			//mTileInfo[((clickedPosY + nexusAroundTilePos) + y) * TILE_COUNT_X + ((clickedPosX + nexusAroundTilePos) + x)].TilePos = mTileInfoSample[0].TilePos;

			mTileInfo[(clickedPosY + y) * TILE_COUNT_X + ((clickedPosX + nexusAroundTilePos) + x)].Terrain = eTerrain::Wall;
			//mTileInfo[((clickedPosY + nexusAroundTilePos) + y) * TILE_COUNT_X + (clickedPosX + x)].Terrain = eTerrain::Wall;
			//mTileInfo[((clickedPosY + nexusAroundTilePos) + y) * TILE_COUNT_X + ((clickedPosX + nexusAroundTilePos) + x)].Terrain = eTerrain::Wall;

			mTileInfo[(clickedPosY + y) * TILE_COUNT_X + ((clickedPosX + nexusAroundTilePos) + x)].NexusAroundTile = true;
			//mTileInfo[((clickedPosY + nexusAroundTilePos) + y) * TILE_COUNT_X + (clickedPosX + x)].NexusAroundTile = true;
			//mTileInfo[((clickedPosY + nexusAroundTilePos) + y) * TILE_COUNT_X + ((clickedPosX + nexusAroundTilePos) + x)].NexusAroundTile = true;
		}
	}
	
}
