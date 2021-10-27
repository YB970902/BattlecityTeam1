#include "TitleScene.h"
#include "Image.h"

HRESULT TitleScene::Init()
{
	mBlackBG = IMG_MGR->FindImage(eImageTag::BlackBG);
	mTileImage = IMG_MGR->FindImage(eImageTag::TitleScene);
	mTitleCursor = IMG_MGR->FindImage(eImageTag::TitleSceneCursor);

	SCENE_MGR->SetSceneData("FirstPlayerLife", 3);
	SCENE_MGR->SetSceneData("SecondPlayerLife", 3);
	SCENE_MGR->SetSceneData("Stage", 0);

	mTitleCursor->SetCurrFrameX(6);

	mbIsTitleAtCenter = false;
	mTitleRenderStartPos = 400.0f;

	for (int i = 0; i < 2; i++)
	{
		mSelectAreaPos[i] = { WIN_SIZE_X / 2 - WIN_SIZE_X / 5, WIN_SIZE_Y / 2 + 10 + (i * 55) };
	}
	mbSelectArea = false;
	mbCursorFrameToggle = false;
	mCursorToggleTime = 0.0f;

	return S_OK;
}

void TitleScene::Update()
{
	if (!mbIsTitleAtCenter)
	{
		mTitleRenderStartPos -= DELTA_TIME * 200;
		if (mTitleRenderStartPos <= 0)
		{
			mTitleRenderStartPos = 0;
			mbIsTitleAtCenter = true;
		}
	}
	else
	{
		mCursorToggleTime += DELTA_TIME;
		if (mCursorToggleTime > 0.07f)
		{
			mCursorToggleTime -= 0.07f;
			mbCursorFrameToggle = !mbCursorFrameToggle;
		}
		mTitleCursor->SetCurrFrameX(7 - mbCursorFrameToggle);
		if (KEY_MGR->IsOnceKeyDown(VK_DOWN))
		{
			mbSelectArea = true;
		}
		else if(KEY_MGR->IsOnceKeyDown(VK_UP))
		{
			mbSelectArea = false;
		}
		if (KEY_MGR->IsOnceKeyDown(VK_SPACE))
		{
			SCENE_MGR->SetSceneData("IsSoloMode", (int)mbSelectArea);
			SCENE_MGR->ChangeScene(eSceneTag::SlateScene);
		}
	}
}

void TitleScene::Render(HDC hdc)
{
	mBlackBG->Render(hdc);
	mTileImage->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 2 + (mTitleRenderStartPos));
	if (mbIsTitleAtCenter) { mTitleCursor->Render(hdc, mSelectAreaPos[0].x, mSelectAreaPos[mbSelectArea].y, mTitleCursor->GetCurrFrameX(), 1); }
}

void TitleScene::Release()
{

}
