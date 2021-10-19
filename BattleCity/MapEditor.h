#pragma once
#include "GameEntity.h"
class Image;
class MapEditor : public GameEntity
{
public:
#define TILE_SIZE	16
#define BUTTON_SIZE 30
#define ENEMY_TILE_SIZE	32

#define SAMPLETILE_COUNT_X 10
#define SAMPLETILE_COUNT_Y 6

#define BUTTON_ENEMY_MAX_COUNT 9
#define ENEMY_MAX_COUNT 20

#define ENEMY_ORDERBOX_ENEMY1 0x0002
#define ENEMY_ORDERBOX_ENEMY2 0x0004
#define ENEMY_ORDERBOX_ENEMY3 0x0006
#define ENEMY_ORDERBOX_ENEMY4 0x0008

#define ENEMY_INFO_BOX mEnemyOrderInfo.EnemyOrderBox

struct SampleAreaMouse
{
	POINT Start;
	POINT End;

	SampleAreaMouse()
	{
		Start = { 0 , 0 };
		End = { 0 , 0 };
	}
};


struct TagButton
{
	RECT	ButtonShape;
	POINT	ButtonPos;
	bool    bButtonOn;
};
struct TagEnemyOrderInfo
{
	POINT EnemyOrderPos;
	int EnemyOrderTypeImage;
	int EnemyOrderBox[20];
	int EnemyCount = 0;
};

protected:
	Image* mBackground;
	Image* mBackgroundInner;
	Image* mButtonImage;
	Image* mTileImage;
	Image* mEnemyOrderBoxImage;

	TagTile mTileInfo[TILE_COUNT_X * TILE_COUNT_Y];
	TagTile mTileInfoSample[TILE_COUNT_X * TILE_COUNT_Y];

	RECT mDrawingArea;
	POINT mClickedStartIndex;
	POINT mClickedArea;

	bool mbIsClicked = false;

	RECT mSampleAreaShape;
	SampleAreaMouse sampleAreaMouse;

	eTerrain mClickedTileTerrain;

	TagButton mButtonInfo[BUTTON_ENEMY_MAX_COUNT];

	TagEnemyOrderInfo mEnemyOrderInfo;

	char mEasySaveIndex;

public:
	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;

	void SaveMap(int saveIndex = 0);
	void LoadMap(int loadIndex = 0);

	void SaveEnemyOrder(int saveIndex = 0);
	void LoadEnemyOrder(int loadIndex = 0);

	Image* GetTileImage() { return this->mTileImage; }
	void SetTileImage(Image* mTileImage) { this->mTileImage = mTileImage; };

	/*TagTile GetTileInfo() { return this->mTileInfo; }
	void SetTileInfo(TagTile mTileInfo, int length)
	{
		for(int j = 0; j < length; j++)
		{
			for (int i = 0; i < length; i++)
			{
				this->mTileInfo[i *] = mTileInfo;
			}
		}
		
		
	};*/

};