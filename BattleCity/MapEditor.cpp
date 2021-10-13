#include "MapEditor.h"
#include "Config.h"
#include "CommonFunction.h"
#include "Image.h"

HRESULT MapEditor::Init()
{
	mTileImage = IMG_MGR->AddImage(eImageTag::Tile, "Image/SamlpTile1.bmp", 176, 176, 11, 11, true, RGB(255, 0, 255));

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
			mTileInfo[i * TILE_COUNT_X + j].TilePos.x = 3;
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
				}
			}
		}
	}
}

void MapEditor::Render(HDC hdc)
{
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
			mTileImage->Render(hdc, WIN_SIZE_X / 2 + (TILE_SIZE * j), WIN_SIZE_Y / 2 + (TILE_SIZE * i), mClickedStartIndex.x + j, mClickedStartIndex.y + i);
		}
	}
}

void MapEditor::Release()
{

}
