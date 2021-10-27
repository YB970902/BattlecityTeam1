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

	mScore1 = new Score;
	mScore1->Init(eImageTag::WhiteNumber, WIN_SIZE_X / 2 , WIN_SIZE_Y / 2 + 100, 1, 2);
	//				색지정						위치							좌우정렬 폰트크기(Size)
	// 입니다
	mScore2 = new Score;
	mScore2->Init(eImageTag::OrangeNumber, WIN_SIZE_X / 2, WIN_SIZE_Y / 2 + 200, 0, 1.5);

	mGrayBGStartPos = WIN_SIZE_Y;

    return S_OK;
}

void SlateScene::Update()
{
	if (KEY_MGR->IsStayKeyDown(VK_SPACE))
	{
		
		mScore1->AddScore(1);
		mScore2->AddScore(3200);
		//mScore->SetScorePos({ 300 , 300 });
		
	}

	mGrayBGStartPos -= (700 * DELTA_TIME);

	if (mGrayBGStartPos <= -WIN_SIZE_Y)
	{
		mScore1->Update();
		mScore2->Update();
		/*SCENE_MGR->ChangeScene(eSceneTag::TestScene);*/
		
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
	if (mGrayBGStartPos <= -WIN_SIZE_Y)
	{
		mScore1->Render(hdc);
		mScore2->Render(hdc);
	}
}

void SlateScene::Release()
{
	SAFE_RELEASE(mScore1);
	SAFE_RELEASE(mScore2);
}
