#include "Config.h"
#include "PhyscisScene.h"
#include "Collider.h"
#include "Physcis.h"
#include "Image.h"
#include "CollisionChecker.h"

HRESULT PhyscisScene::Init()
{
	IMG_MGR->AddImage(eImageTag::BackGround, "Image/background.bmp", 1024, 768);
	mBackGround = IMG_MGR->FindImage(eImageTag::BackGround);

	mPhyscis = new Physcis;

	mPhyscis->CreateCollider({ 100,100 }, 10, nullptr, eCollisionTag::Block);
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			mPhyscis->CreateCollider({ float(i * 32)+8,float(j*32)+8 }, 16, nullptr, eCollisionTag::Block);
		}
	}
	mPhyscis->CreateCollider({ (float)32*9,(float)32*10 }, 16, nullptr, eCollisionTag::Water);
	mPhyscis->CreateCollider({ (float)32*9,(float)32*14 }, 16, nullptr, eCollisionTag::Block);
	mPhyscis->CreateCollider({ (float)32*9,(float)32*18 }, 16, nullptr, eCollisionTag::EnemyAmmo);
	mPhyscis->CreateCollider({ (float)32 * 9,(float)32 * 24 }, 16, nullptr, eCollisionTag::PlayerAmmo);

	mCurCollider = mPhyscis->CreateCollider({ WIN_SIZE_X / 2,WIN_SIZE_Y / 2 }, 16, nullptr, eCollisionTag::PlayerTank);

	return S_OK;
}

void PhyscisScene::Release()
{
	SAFE_RELEASE(mPhyscis);
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
		mCurCollider = mPhyscis->CreateCollider({ WIN_SIZE_X/2,WIN_SIZE_Y/2 }, 16, nullptr, eCollisionTag::PlayerAmmo);
	}
	if (KEY_MGR->IsOnceKeyDown(VK_ESCAPE))
	{
		//mPhyscis->DestoryCollider(mCurCollider);
	}
}
