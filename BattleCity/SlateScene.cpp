#include "SlateScene.h"
#include "Image.h"
#include "Score.h"

HRESULT SlateScene::Init()
{
	mStage = IMG_MGR->FindImage(eImageTag::SlateSceneStage);
	mBackGroundBlack = IMG_MGR->FindImage(eImageTag::BlackBG);
	mNumber = IMG_MGR->FindImage(eImageTag::WhiteNumber);
	for (int i = 0; i < 2; i++)
	{
		mBackGroundGray[i] = IMG_MGR->FindImage(eImageTag::GrayBG);
	}
	mGrayBGStartPos = WIN_SIZE_Y;

	mStageNum = new Score;
	mStageNum->Init(eImageTag::WhiteNumber, WIN_SIZE_X / 2 + 50, WIN_SIZE_Y / 2);
	mStageNum->SetScore(SCENE_MGR->GetSceneData("Stage") + 1);

    return S_OK;
}

void SlateScene::Update()
{
	mGrayBGStartPos -= (700 * DELTA_TIME);
	mStageNum->Update();
	if (mGrayBGStartPos <= -WIN_SIZE_Y)
	{
		SCENE_MGR->ChangeScene(eSceneTag::BattleScene);
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
		mStageNum->Render(hdc);
	}
}

void SlateScene::Release()
{
	SAFE_RELEASE(mStageNum);
}
