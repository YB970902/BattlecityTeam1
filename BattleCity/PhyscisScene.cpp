#include "Config.h"
#include "PhyscisScene.h"
#include "Collider.h"
#include "Physcis.h"
#include "Image.h"
#include "CollisionChecker.h"

HRESULT PhyscisScene::Init()
{
	mBackGround = new Image;
	IMG_MGR->AddImage(eImageTag::BackGround, "Image/background.bmp", 1024, 768);
	mBackGround = IMG_MGR->FindImage(eImageTag::BackGround);

	mPhyscis = new Physcis;

	mCurCollider = mPhyscis->CreateCollider({ 150,100 }, 50, nullptr, eCollisionTag::Block);

	mPhyscis->CreateCollider({ 100,100 }, 50, nullptr, eCollisionTag::Block);
	mPhyscis->CreateCollider({ 200,100 }, 50, nullptr, eCollisionTag::Block);
	mPhyscis->CreateCollider({ 290,100 }, 50, nullptr, eCollisionTag::Block);

	return S_OK;
}

void PhyscisScene::Release()
{
}

void PhyscisScene::Render(HDC hdc)
{
	mBackGround->Render(hdc);
	mPhyscis->Render(hdc);
}

void PhyscisScene::Update()
{
	if (KEY_MGR->IsStayKeyDown('A')) { mCurCollider->MoveTo(DIR_LEFT, mMoveSpeed * DELTA_TIME); }
	else if (KEY_MGR->IsStayKeyDown('D')) { mCurCollider->MoveTo(DIR_RIGHT, mMoveSpeed * DELTA_TIME); }
	else if (KEY_MGR->IsStayKeyDown('W')) { mCurCollider->MoveTo(DIR_UP, mMoveSpeed * DELTA_TIME); }
	else if (KEY_MGR->IsStayKeyDown('S')) { mCurCollider->MoveTo(DIR_DOWN, mMoveSpeed * DELTA_TIME); }
	if (KEY_MGR->IsOnceKeyDown(VK_SPACE))
	{
		mCurCollider = mPhyscis->CreateCollider({ WIN_SIZE_X/2,WIN_SIZE_Y/2 }, 50, nullptr, eCollisionTag::Block);
	}
	if (KEY_MGR->IsOnceKeyDown(VK_ESCAPE))
	{
		//mPhyscis->DestoryCollider(mCurCollider);
	}
}
