#include "Config.h"
#include "BattleScene.h"
#include "Image.h"
#include "Physcis.h"
#include "Collider.h"
#include "CollisionChecker.h"
#include "TileManager.h"
#include "Tile.h"
#include "TankController.h"
#include "TankSpawner.h"

HRESULT BattleScene::Init()
{
	mBackgroundGray = IMG_MGR->FindImage(eImageTag::BattleSceneGrayBG);
	mBackgroundBlack = IMG_MGR->FindImage(eImageTag::BattleSceneBlackBG);
	mStartPos = { (mBackgroundGray->GetWidth() - mBackgroundBlack->GetWidth()) / 2 ,(mBackgroundGray->GetHeight() - mBackgroundBlack->GetHeight()) / 2 };

	mPhysics = new Physcis;
	mTileManager = new TileManager();
	mTileManager->Init(mStartPos, POINT{ mBackgroundBlack->GetWidth(), mBackgroundBlack->GetHeight() });
	mTileManager->SetPhysics(mPhysics);
	mTileManager->LoadMap(0);

	mFirstPlayerController = new TankController();
	mFirstPlayerController->Init(FIRST_PLAYER_KEY);
	mFirstPlayerSpawner = new TankSpawner();
	//mFirstPlayerSpawner->Init(mPhysics, )

	// 2P¸¦ °ñ¶úÀ» °æ¿ì
	mSecondPlayerController = new TankController();
	mSecondPlayerController->Init(FIRST_PLAYER_KEY);

	return S_OK;
}

void BattleScene::Update()
{
	SAFE_UPDATE(mTileManager);
	if (KEY_MGR->IsOnceKeyDown('O'))
	{
		mTileManager->ProtectNexus();
	}
}

void BattleScene::Render(HDC hdc)
{
	mBackgroundGray->Render(hdc);
	mBackgroundBlack->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 2);
	SAFE_RENDER(mTileManager);
}

void BattleScene::Release()
{
	SAFE_RELEASE(mPhysics);
	SAFE_RELEASE(mTileManager);
}