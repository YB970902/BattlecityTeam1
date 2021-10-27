#include "Config.h"
#include "GameManager.h"
#include "Image.h"
#include "Collider.h"
#include "CollisionChecker.h"
#include "Physcis.h"
#include "Tile.h"
#include "TileManager.h"
#include "TankController.h"
#include "TankSpawner.h"
#include "AITankSpawner.h"
#include "AmmoSpawner.h"
#include "ItemManager.h"
#include "Subject.h"

HRESULT GameManager::Init()
{
	mbIsSoloMode = !(bool)SCENE_MGR->GetSceneData("IsSoloMode");
	mBackgroundGray = IMG_MGR->FindImage(eImageTag::BattleSceneGrayBG);
	mBackgroundBlack = IMG_MGR->FindImage(eImageTag::BattleSceneBlackBG);
	mStartPos = { (mBackgroundGray->GetWidth() - mBackgroundBlack->GetWidth()) / 2 ,(mBackgroundGray->GetHeight() - mBackgroundBlack->GetHeight()) / 2 };

	mPhysics = new Physcis;

	mAmmoSpawner = new AmmoSpawner();
	mAmmoSpawner->Init();
	mAmmoSpawner->SetGameManager(this);

	mTileManager = new TileManager();
	mTileManager->Init(mStartPos, POINT{ mBackgroundBlack->GetWidth(), mBackgroundBlack->GetHeight() });
	mTileManager->SetGameManager(this);
	mTileManager->LoadMap(SCENE_MGR->GetSceneData("Stage"));
	mTileManager->LoadEnemyOrder(SCENE_MGR->GetSceneData("Stage"));
	mTileManager->CreateEdgeBlock();

	mFirstPlayerController = new TankController();
	mFirstPlayerController->Init(FIRST_PLAYER_KEY);
	mFirstPlayerController->SetGameManager(this);
	mFirstPlayerSpawner = new TankSpawner();
	mFirstPlayerSpawner->Init(this, SPAWN_INFO(eCollisionTag::FirstPlayerTank, eTankType::Player, eTankColor::Yellow, PLAYER_TANK_INFO), 1, mTileManager->GetFirstPlayerSpawnPosition());
	mFirstPlayerSpawner->SetController(mFirstPlayerController);
	mFirstPlayerSpawner->GetSubject()->AddObserver(this);

	if (!mbIsSoloMode)
	{
		mSecondPlayerController = new TankController();
		mSecondPlayerController->Init(SECOND_PLAYER_KEY);
		mSecondPlayerController->SetGameManager(this);
		mSecondPlayerSpawner = new TankSpawner();
		mSecondPlayerSpawner->Init(this, SPAWN_INFO(eCollisionTag::SecondPlayerTank, eTankType::Player, eTankColor::Green, PLAYER_TANK_INFO), 1, mTileManager->GetSecondPlayerSpawnPosition());
		mSecondPlayerSpawner->SetController(mSecondPlayerController);
		mSecondPlayerSpawner->GetSubject()->AddObserver(this);
	}

	mAISpawner = new AITankSpawner();
	mAISpawner->Init(this, 5);
	int size;
	POINTFLOAT* enemySpawnPos = mTileManager->GetEnemySpawnPosition(size);
	mAISpawner->SetSpawnPosition(enemySpawnPos, size);
	pair<TankSpawnInfo*, int> enemyList = mTileManager->GetEnemyList();
	for (int i = 0; i < enemyList.second; ++i)
	{
		mAISpawner->AddTankSpawnInfo(enemyList.first[i]);
	}
	delete[] enemyList.first;

	mItemManager = new ItemManager();
	mItemManager->Init();
	mItemManager->SetGameManager(this);

	return S_OK;
}

void GameManager::Release()
{
	SAFE_RELEASE(mAmmoSpawner);
	SAFE_RELEASE(mFirstPlayerSpawner);
	SAFE_RELEASE(mSecondPlayerSpawner);
	SAFE_RELEASE(mAISpawner);
	SAFE_RELEASE(mPhysics);
	SAFE_RELEASE(mTileManager);
	SAFE_RELEASE(mItemManager);
}

void GameManager::Update()
{
	if (KEY_MGR->IsOnceKeyDown(VK_TAB))
	{
		mbIsDebugMode = !mbIsDebugMode;
	}
	if (mbIsDebugMode)
	{
		if (KEY_MGR->IsOnceKeyDown('O'))
		{
			mTileManager->ProtectNexus();
		}
		if (KEY_MGR->IsOnceKeyDown('E'))
		{
			mItemManager->CreateItem((eItemTag)RANDOM(1, 6));
		}
	}

	SAFE_UPDATE(mTileManager);
	SAFE_UPDATE(mAmmoSpawner);
	SAFE_UPDATE(mAISpawner);
	if (mFirstPlayerSpawner && !mbIsFirstPlayerDead) { mFirstPlayerSpawner->Update(); }
	if (mSecondPlayerSpawner && !mbIsSecondPlayerDead) { mSecondPlayerSpawner->Update(); }
	SAFE_UPDATE(mItemManager);
}

void GameManager::Render(HDC hdc)
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

Collider* GameManager::CreateCollider(POINTFLOAT pos, float bodySize, CollisionChecker* obj, eCollisionTag tag)
{
	return mPhysics->CreateCollider(pos, bodySize, obj, tag);
}

void GameManager::DestroyCollider(Collider* collider)
{
	mPhysics->DestroyCollider(collider);
}

Ammo* GameManager::Fire(eDir dir, eCollisionTag tag, float speed, POINTFLOAT pos)
{
	return mAmmoSpawner->Fire(dir, tag, speed, pos);
}

void GameManager::CreateItem(eItemTag itemTag)
{
	mItemManager->CreateItem(itemTag);
}

POINTFLOAT GameManager::GetItemSpawnPosition()
{
	return mTileManager->GetItemSpawnPosition();
}

void GameManager::FirstPlayerSetInvencible()
{
	mFirstPlayerController->TurnToInvencible();
}

void GameManager::SecondPlayerSetInvencible()
{
	mSecondPlayerController->TurnToInvencible();
}

void GameManager::FirstPlayerAddStar()
{
	mFirstPlayerController->AddStar();
}

void GameManager::SecondPlayerAddStar()
{
	mSecondPlayerController->AddStar();
}

void GameManager::FirstPlayerAddLife()
{
	mFirstPlayerSpawner->AddLife();
}

void GameManager::SecondPlayerAddLife()
{
	mSecondPlayerSpawner->AddLife();
}

void GameManager::ProtectNexus()
{
	mTileManager->ProtectNexus();
}

void GameManager::DestroyAllEnemy()
{
	mAISpawner->DestroyAll();
}

void GameManager::PauseAllEnemy()
{
	mAISpawner->PauseAll();
}

void GameManager::OnNotify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag)
{
	if (mbIsGameEnd) { return; }
	if (subjectTag == eSubjectTag::Player && eventTag == eEventTag::PlayerDown)
	{
		if (mbIsGameEnd) { return; }
		if (mbIsSoloMode)
		{
			mbIsFirstPlayerDead = true;
			mbIsGameEnd = true;
			SetGameOver();
		}
		else
		{
			if (!mbIsSecondPlayerDead && mFirstPlayerSpawner == obj)
			{
				mbIsFirstPlayerDead = true;
				GameOverLogoAtFirstPlayer();
			}
			else if (!mbIsFirstPlayerDead && mSecondPlayerSpawner == obj)
			{
				mbIsSecondPlayerDead = true;
				GameOverLogoAtSecondPlayer();
			}
			else
			{
				mbIsGameEnd = true;
				SetGameOver();
			}
		}
	}
	else if (subjectTag == eSubjectTag::Nexus && eventTag == eEventTag::Collided)
	{
		if (mbIsGameEnd) { return; }
		mbIsGameEnd = true;
		mbIsFirstPlayerDead = true;
		mbIsSecondPlayerDead = true;
		SetGameOver();
	}
}

void GameManager::SetGameOver()
{
	mTileManager->ChangeNexusImageToFlag();
	UI_MGR->AddMovingUI(eImageTag::UIGameOver, POINTFLOAT{ WIN_SIZE_X * 0.5f, WIN_START_POS_Y }, POINTFLOAT{ WIN_SIZE_X * 0.5f, WIN_SIZE_Y * 0.5f }, 2.0f, 1.0f);
}

void GameManager::GameOverLogoAtFirstPlayer()
{
	UI_MGR->AddMovingUI(eImageTag::UIGameOver, POINTFLOAT{ WIN_START_POS_X, mFirstPlayerSpawner->GetSpawnPosition().y },
		POINTFLOAT{ mFirstPlayerSpawner->GetSpawnPosition().x, mFirstPlayerSpawner->GetSpawnPosition().y }, 2.0f, 1.0f);
}

void GameManager::GameOverLogoAtSecondPlayer()
{
	UI_MGR->AddMovingUI(eImageTag::UIGameOver, POINTFLOAT{ WIN_SIZE_X - WIN_START_POS_X, mSecondPlayerSpawner->GetSpawnPosition().y },
		POINTFLOAT{ mSecondPlayerSpawner->GetSpawnPosition().x, mSecondPlayerSpawner->GetSpawnPosition().y }, 2.0f, 1.0f);
}
