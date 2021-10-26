#pragma once
#include "GameEntity.h"

#define TILE_SIZE	16
#define BUTTON_SIZE 30
#define ENEMY_TILE_SIZE	32

#define SAMPLETILE_COUNT_X 10
#define SAMPLETILE_COUNT_Y 6

#define BUTTON_ENEMY_MAX_COUNT 9
#define ENEMY_MAX_COUNT 20

#define NEXUS_AROUNDTILE_LEFT 4
#define NEXUS_AROUNDTILE_RIGHT 5
#define NEXUS_AROUNDTILE_TOP 2
#define NEXUS_AROUNDTILE_BOTTOM 3

struct TagEnemyInfo
{
	eTankType mEnemyOrderType[20];
	int mEnemyOrderCount;

	TagEnemyInfo()
	{
		mEnemyOrderType[0] = {};
		mEnemyOrderCount = 0;
	};

};

class Image;
class MapEditor : public GameEntity
{
public:
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

		TagButton()
		{
			ButtonShape = {};
			ButtonPos = {};
			bButtonOn = false;
		}
	};


protected:
	Image* mBackground = nullptr;
	Image* mBackgroundInner = nullptr;
	Image* mButtonImage = nullptr;
	Image* mTileImage = nullptr;
	Image* mEnemyOrderBoxImage = nullptr;

	TagTile mTileInfo[TILE_COUNT_X * TILE_COUNT_Y] = {};
	TagTile mTileInfoSample[TILE_COUNT_X * TILE_COUNT_Y] = {};

	RECT mDrawingArea = {};
	POINT mClickedStartIndex = {};
	POINT mClickedArea = {};

	bool mbIsClicked = false;

	RECT mSampleAreaShape = {};

	char mEasySaveIndex = NULL;

	SampleAreaMouse sampleAreaMouse;
	eTerrain mClickedTileTerrain;
	TagButton mButtonInfo[BUTTON_ENEMY_MAX_COUNT];
	TagEnemyInfo mEnemyInfo;

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

	void SetNexusAroundTile(int x, int y, int clickedPosX, int clickedPosY, int NEXUS_AROUNDTILE_POS_X, int NEXUS_AROUNDTILE_POS_Y);
};