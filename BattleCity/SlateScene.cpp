#include "SlateScene.h"
#include "Image.h"

HRESULT SlateScene::Init()
{
	mStage = IMG_MGR->FindImage(eImageTag::SlateSceneStage);
	mBackGroundBlack = IMG_MGR->FindImage(eImageTag::BlackBG);
	mNumber = IMG_MGR->FindImage(eImageTag::SlateSceneNumber);
	for (int i = 0; i < 2; i++)
	{
		mBackGroundGray[i] = IMG_MGR->FindImage(eImageTag::GrayBG);
	}

	mGrayBGStartPos = WIN_SIZE_Y;

    return S_OK;
}

void SlateScene::Update()
{
	mGrayBGStartPos -= (700 * DELTA_TIME);

	if (mGrayBGStartPos <= -WIN_SIZE_Y)
	{
		SCENE_MGR->ChangeScene(eSceneTag::TestScene);
	}

}

void SlateScene::Render(HDC hdc)
{
	mBackGroundBlack->Render(hdc);

	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			mBackGroundGray[i]->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 2 + mGrayBGStartPos);
		}
		else if (i == 1)
		{
			mBackGroundGray[i]->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 2 - mGrayBGStartPos);
		}
		
	}
	if (mGrayBGStartPos <= 700)
	{
		mStage->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 2);
		mNumber->Render(hdc, WIN_SIZE_X / 2 + 50, WIN_SIZE_Y / 2, 1, 0);
	}
	
}

void SlateScene::Release()
{

}
