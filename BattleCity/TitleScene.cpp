#include "TitleScene.h"
#include "Image.h"

HRESULT TitleScene::Init()
{
	mBlackBG = IMG_MGR->FindImage(eImageTag::BlackBG);
	mTileImage = IMG_MGR->FindImage(eImageTag::TitleScene);
	mTitleCursor = IMG_MGR->FindImage(eImageTag::TitleSceneCursor);

	mTitleCursor->SetCurrFrameX(6);

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
	if ((mCursorToggleTime = float(mCursorToggleTime + DELTA_TIME)) >= 0.07)
	{
		if (mbCursorFrameToggle == false) mbCursorFrameToggle = true;
		else mbCursorFrameToggle = false;

		mCursorToggleTime = 0.0f;
	}

	if (!(mTitleRenderStartPos == 0))
	{
		if (!(mTitleRenderStartPos <= 0)) mTitleRenderStartPos -= DELTA_TIME * 200;
		else mTitleRenderStartPos = 0.0f;
	}
	else if (mTitleRenderStartPos == 0)
	{
		if (KEY_MGR->IsOnceKeyDown(VK_DOWN)
			|| KEY_MGR->IsOnceKeyDown(VK_UP)){ mbSelectArea == true ? mbSelectArea = false : mbSelectArea = true; }
			mTitleCursor->SetCurrFrameX(7 - mbCursorFrameToggle);
	}
	if (KEY_MGR->IsOnceKeyDown(VK_SPACE))
	{
		if (mbSelectArea == false)
		{
			SCENE_MGR->SetSceneData("1P", (int)mbSelectArea);
		}
		else
		{
			SCENE_MGR->SetSceneData("2P", (int)mbSelectArea);
		}
		SCENE_MGR->ChangeScene(eSceneTag::SlateScene);
	}
}

void TitleScene::Render(HDC hdc)
{
	mBlackBG->Render(hdc);
	mTileImage->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 2 + (mTitleRenderStartPos));
	if (mTitleRenderStartPos == 0)
	{
		mTitleCursor->Render(hdc, mSelectAreaPos[0].x, mSelectAreaPos[mbSelectArea].y, mTitleCursor->GetCurrFrameX(), 1);
	}
}

void TitleScene::Release()
{

}
