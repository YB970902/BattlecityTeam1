#pragma once
#include "GameEntity.h"

class Image;
class SlateScene : public GameEntity
{
private:
	Image* mStage;
	Image* mNumber;
	Image* mBackGroundGray[2];
	Image* mBackGroundBlack;

	int mGrayBGStartPos = 0;

public:
	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;

	virtual ~SlateScene() {};

};

