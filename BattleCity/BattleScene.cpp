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
#include "AITankSpawner.h"
#include "AmmoSpawner.h"

HRESULT BattleScene::Init()
{
	mBackgroundGray = IMG_MGR->FindImage(eImageTag::BattleSceneGrayBG);
	mBackgroundBlack = IMG_MGR->FindImage(eImageTag::BattleSceneBlackBG);
	mStartPos = { (mBackgroundGray->GetWidth() - mBackgroundBlack->GetWidth()) / 2 ,(mBackgroundGray->GetHeight() - mBackgroundBlack->GetHeight()) / 2 };

	mPhysics = new Physcis;

	mAmmoSpawner = new AmmoSpawner();
	mAmmoSpawner->Init();
	mAmmoSpawner->SetPhysics(mPhysics);

	mTileManager = new TileManager();
	mTileManager->Init(mStartPos, POINT{ mBackgroundBlack->GetWidth(), mBackgroundBlack->GetHeight() });
	mTileManager->SetPhysics(mPhysics);
	mTileManager->LoadMap(0);
	mTileManager->LoadEnemyOrder(0);
	mTileManager->CreateEdgeBlock();

	//mFirstPlayerController = new TankController();
	//mFirstPlayerController->Init(FIRST_PLAYER_KEY);
	//mFirstPlayerSpawner = new TankSpawner();
	//mFirstPlayerSpawner->Init(mPhysics, )

	// 2P�� ����� ���
	//mSecondPlayerController = new TankController();
	//mSecondPlayerController->Init(FIRST_PLAYER_KEY);

	mAISpawner = new AITankSpawner();
	mAISpawner->Init(mPhysics, 3);
	mAISpawner->SetAmmoSpawner(mAmmoSpawner);
	int size;
	POINTFLOAT* enemySpawnPos = mTileManager->GetEnemySpawnPosition(size);
	mAISpawner->SetSpawnPosition(enemySpawnPos, size);
	vector<TankSpawnInfo>* vecInfo = mTileManager->GetEnemyList();
	for (int i = 0; i < vecInfo->size(); ++i)
	{
		mAISpawner->AddTankSpawnInfo((*vecInfo)[i]);
	}
	delete vecInfo;

	return S_OK;
}

void BattleScene::Update()
{
	if (KEY_MGR->IsOnceKeyDown(VK_TAB))
	{
		mbIsDebugMode = !mbIsDebugMode;
	}

	SAFE_UPDATE(mTileManager);
	SAFE_UPDATE(mAmmoSpawner);
	SAFE_UPDATE(mAISpawner);
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
	SAFE_RENDER(mAISpawner);
	SAFE_RENDER(mAmmoSpawner);

	if (mbIsDebugMode) { SAFE_RENDER(mPhysics); }
}

void BattleScene::Release()
{
	SAFE_RELEASE(mAmmoSpawner);
	SAFE_RELEASE(mAISpawner);
	SAFE_RELEASE(mPhysics);
	SAFE_RELEASE(mTileManager);
}