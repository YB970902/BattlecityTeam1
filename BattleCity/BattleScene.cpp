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
#include "ItemManager.h"

HRESULT BattleScene::Init()
{
	SCENE_MGR->SetSceneData("FirstPlayerNormalEnemyTank", 0);
	SCENE_MGR->SetSceneData("FirstPlayerQuickEnemyTank", 0);
	SCENE_MGR->SetSceneData("FirstPlayerRapidFireEnemyTank", 0);
	SCENE_MGR->SetSceneData("FirstPlayerDefenceEnemyTank", 0);

	SCENE_MGR->SetSceneData("SecondPlayerNormalEnemyTank", 0);
	SCENE_MGR->SetSceneData("SecondPlayerQuickEnemyTank", 0);
	SCENE_MGR->SetSceneData("SecondPlayerRapidFireEnemyTank", 0);
	SCENE_MGR->SetSceneData("SecondPlayerDefenceEnemyTank", 0);

	// 1 : 1인모드 0 : 2인모드
	mbIsSoloMode = SCENE_MGR->GetSceneData("IsSoloMode");

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
	mTileManager->LoadMap(SCENE_MGR->GetSceneData("Stage"));
	mTileManager->LoadEnemyOrder(SCENE_MGR->GetSceneData("Stage"));
	mTileManager->CreateEdgeBlock();

	mFirstPlayerController = new TankController();
	mFirstPlayerController->Init(FIRST_PLAYER_KEY);
	mFirstPlayerController->SetAmmoSpawner(mAmmoSpawner);
	mFirstPlayerSpawner = new TankSpawner();
	mFirstPlayerSpawner->Init(mPhysics, SPAWN_INFO(eCollisionTag::FirstPlayerTank, eTankType::Player, eTankColor::Yellow, PLAYER_TANK_INFO), 1, mTileManager->GetFirstPlayerSpawnPosition());
	mFirstPlayerSpawner->SetController(mFirstPlayerController);

	mSecondPlayerController = new TankController();
	mSecondPlayerController->Init(SECOND_PLAYER_KEY);
	mSecondPlayerController->SetAmmoSpawner(mAmmoSpawner);
	mSecondPlayerSpawner = new TankSpawner();
	mSecondPlayerSpawner->Init(mPhysics, SPAWN_INFO(eCollisionTag::SecondPlayerTank, eTankType::Player, eTankColor::Green, PLAYER_TANK_INFO), 1, mTileManager->GetSecondPlayerSpawnPosition());
	mSecondPlayerSpawner->SetController(mSecondPlayerController);

	mAISpawner = new AITankSpawner();
	mAISpawner->Init(mPhysics, 5);
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

	mItemManager = new ItemManager();
	mItemManager->Init();
	mItemManager->SetPhysics(mPhysics);
	mItemManager->SetTileManager(mTileManager);
	mItemManager->SetFirstPlayerController(mFirstPlayerController);
	mItemManager->SetSecondPlayerController(mSecondPlayerController);
	mItemManager->SetAISpawner(mAISpawner);
	mAISpawner->SetItemManager(mItemManager);
	mItemManager->SetFirstPlayerSpawner(mFirstPlayerSpawner);
	mItemManager->SetSecondPlayerSpawner(mSecondPlayerSpawner);

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
	SAFE_UPDATE(mFirstPlayerSpawner);
	SAFE_UPDATE(mSecondPlayerSpawner);
	if (mbIsSoloMode)
	{
		if (mFirstPlayerSpawner && !mbIsFirstPlayerDead && mFirstPlayerSpawner->IsSpawnEnd())
		{
			mbIsFirstPlayerDead = true;
			mbIsGameEnd = true;
			UI_MGR->AddMovingUI(eImageTag::UIGameOver, POINTFLOAT{ WIN_SIZE_X * 0.5f, WIN_START_POS_Y }, POINTFLOAT{ WIN_SIZE_X * 0.5f, WIN_SIZE_Y * 0.5f }, 2.0f, 1.0f);
		}
	}
	else
	{
		if (mFirstPlayerSpawner && !mbIsFirstPlayerDead && mFirstPlayerSpawner->IsSpawnEnd())
		{
			mbIsFirstPlayerDead = true;
			if (mbIsSecondPlayerDead)
			{
				mbIsGameEnd = true;
				UI_MGR->AddMovingUI(eImageTag::UIGameOver, POINTFLOAT{ WIN_SIZE_X * 0.5f, WIN_START_POS_Y }, POINTFLOAT{ WIN_SIZE_X * 0.5f, WIN_SIZE_Y * 0.5f }, 2.0f, 1.0f);
			}
			else
			{
				UI_MGR->AddMovingUI(eImageTag::UIGameOver, POINTFLOAT{ WIN_START_POS_X, mFirstPlayerSpawner->GetSpawnPosition().y },
					POINTFLOAT{ mFirstPlayerSpawner->GetSpawnPosition().x, mFirstPlayerSpawner->GetSpawnPosition().y }, 2.0f, 1.0f);
			}
		}
		if (mSecondPlayerSpawner && !mbIsSecondPlayerDead && mSecondPlayerSpawner->IsSpawnEnd())
		{
			mbIsSecondPlayerDead = true;
			if (mbIsFirstPlayerDead)
			{
				mbIsGameEnd = true;
				UI_MGR->AddMovingUI(eImageTag::UIGameOver, POINTFLOAT{ WIN_SIZE_X * 0.5f, WIN_START_POS_Y }, POINTFLOAT{ WIN_SIZE_X * 0.5f, WIN_SIZE_Y * 0.5f }, 2.0f, 1.0f);
			}
			else
			{
				UI_MGR->AddMovingUI(eImageTag::UIGameOver, POINTFLOAT{ WIN_SIZE_X - WIN_START_POS_X, mSecondPlayerSpawner->GetSpawnPosition().y },
					POINTFLOAT{ mSecondPlayerSpawner->GetSpawnPosition().x, mSecondPlayerSpawner->GetSpawnPosition().y }, 2.0f, 1.0f);
			}
		}
	}
	SAFE_UPDATE(mItemManager);
	if (KEY_MGR->IsOnceKeyDown('O'))
	{
		mTileManager->ProtectNexus();
	}
	if (KEY_MGR->IsOnceKeyDown('E'))
	{
		mItemManager->CreateItem((eItemTag)RANDOM(1, 6));
	}
}

void BattleScene::Render(HDC hdc)
{
	mBackgroundGray->Render(hdc);
	mBackgroundBlack->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 2);
	SAFE_RENDER(mAISpawner);
	SAFE_RENDER(mAmmoSpawner);

	SAFE_RENDER(mFirstPlayerSpawner);
	SAFE_RENDER(mSecondPlayerSpawner);

	SAFE_RENDER(mTileManager);
	SAFE_RENDER(mItemManager);

	if (mbIsDebugMode) { SAFE_RENDER(mPhysics); }
}

void BattleScene::Release()
{
	SAFE_RELEASE(mAmmoSpawner);
	SAFE_RELEASE(mFirstPlayerSpawner);
	SAFE_RELEASE(mSecondPlayerSpawner);
	SAFE_RELEASE(mAISpawner);
	SAFE_RELEASE(mPhysics);
	SAFE_RELEASE(mTileManager);
	SAFE_RELEASE(mItemManager);
}