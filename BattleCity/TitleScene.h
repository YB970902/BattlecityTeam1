#pragma once
#include "GameEntity.h"

#define CURSOR_SIZE 64

class Image;
class TitleScene : public GameEntity
{
private:
	Image* mBlackBG;
	Image* mTileImage;
	Image* mTitleCursor;

	float mTitleRenderStartPos = 0.0f;

	POINT mSelectAreaPos[2] = { };
	bool mbSelectArea = false;
	bool mbCursorFrameToggle = false;
	float mCursorToggleTime = 0.0f;



public:
	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;

	virtual ~TitleScene() {};
};

