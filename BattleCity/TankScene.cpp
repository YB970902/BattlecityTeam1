#include "Config.h"
#include "TankScene.h"
#include "Tank.h"
#include "Image.h"
#include "AmmoSpawner.h"
#include "TankController.h"
#include "TankSpawner.h"
#include "AITankSpawner.h"
#include "Physcis.h"

HRESULT TankScene::Init()
{
	mBackground = IMG_MGR->FindImage(eImageTag::BackGround);;

	mPhysics = new Physcis();
	mPhysics->Init();

	for (int x = 0; x < 74; ++x)
	{
		mPhysics->CreateCollider(POINTFLOAT{ 16 + (float)x * 16, 16 }, 16, nullptr, eCollisionTag::Block);
		mPhysics->CreateCollider(POINTFLOAT{ 16 + (float)x * 16, WIN_SIZE_Y - 16 }, 16, nullptr, eCollisionTag::Block);
	}

	for (int x = 0; x < 10; ++x)
	{
		for (int y = 0; y < 10; ++y)
		{
			mPhysics->CreateCollider(POINTFLOAT{ WIN_SIZE_X * 0.5f - 80 + (float)x * 16, WIN_SIZE_Y * 0.5f - 80 + 16 * y }, 16, nullptr, eCollisionTag::Water);
		}
	}

	for (int y = 0; y < 47; ++y)
	{
		mPhysics->CreateCollider(POINTFLOAT{ 16, 32 + (float)y * 16 }, 16, nullptr, eCollisionTag::Block);
		mPhysics->CreateCollider(POINTFLOAT{ WIN_SIZE_X - 16, 32 + (float)y * 16 }, 16, nullptr, eCollisionTag::Block);
	}

	mAmmoSpawner = new AmmoSpawner();
	mAmmoSpawner->Init();
	mAmmoSpawner->SetPhysics(mPhysics);

	mFirstPlayerController = new TankController();
	mFirstPlayerController->Init(FIRST_PLAYER_KEY);
	mFirstPlayerController->SetAmmoSpawner(mAmmoSpawner);

	mFirstPlayerTankSpawner = new TankSpawner();
	mFirstPlayerTankSpawner->Init(mPhysics, SPAWN_INFO(eCollisionTag::FirstPlayerTank, eTankType::Player, eTankColor::Yellow, PLAYER_TANK_INFO), 5, 0.0f, POINTFLOAT{ WIN_SIZE_X * 0.5f - 100.0f, WIN_SIZE_Y * 0.9f });
	mFirstPlayerTankSpawner->SetController(mFirstPlayerController);

	mSecondPlayerController = new TankController();
	mSecondPlayerController->Init(SECOND_PLAYER_KEY);
	mSecondPlayerController->SetAmmoSpawner(mAmmoSpawner);

	mSecondPlayerTankSpawner = new TankSpawner();
	mSecondPlayerTankSpawner->Init(mPhysics, SPAWN_INFO(eCollisionTag::SecondPlayerTank, eTankType::Player, eTankColor::Green, PLAYER_TANK_INFO), 5, 0.0f, POINTFLOAT{ WIN_SIZE_X * 0.5f + 100.0f, WIN_SIZE_Y * 0.9f });
	mSecondPlayerTankSpawner->SetController(mSecondPlayerController);

	mAISpawner = new AITankSpawner();
	mAISpawner->Init(mPhysics, 0.5f, 8);
	mAISpawner->SetAmmoSpawner(mAmmoSpawner);

	POINTFLOAT* arrPoint = new POINTFLOAT[3];
	arrPoint[0].x = 300.0f;
	arrPoint[0].y = 200.0f;
	arrPoint[1].x = WIN_SIZE_X * 0.5f;
	arrPoint[1].y = 200.0f;
	arrPoint[2].x = WIN_SIZE_X - 300.0f;
	arrPoint[2].y = 200.0f;

	mAISpawner->SetSpawnPosition(arrPoint, 3);
	mAISpawner->AddTankSpawnInfo(TankSpawnInfo(eCollisionTag::EnemyTank, eTankType::NormalEnemy, eTankColor::White, NORMAL_TANK_INFO));
	mAISpawner->AddTankSpawnInfo(TankSpawnInfo(eCollisionTag::EnemyTank, eTankType::NormalEnemy, eTankColor::White, NORMAL_TANK_INFO));
	mAISpawner->AddTankSpawnInfo(TankSpawnInfo(eCollisionTag::EnemyTank, eTankType::QuickEnemy, eTankColor::White, QUICK_TANK_INFO));
	mAISpawner->AddTankSpawnInfo(TankSpawnInfo(eCollisionTag::EnemyTank, eTankType::QuickEnemy, eTankColor::White, QUICK_TANK_INFO));
	mAISpawner->AddTankSpawnInfo(TankSpawnInfo(eCollisionTag::EnemyTank, eTankType::RapidFireEnemy, eTankColor::White, RAPID_FIRE_TANK_INFO));
	mAISpawner->AddTankSpawnInfo(TankSpawnInfo(eCollisionTag::EnemyTank, eTankType::RapidFireEnemy, eTankColor::White, RAPID_FIRE_TANK_INFO));
	mAISpawner->AddTankSpawnInfo(TankSpawnInfo(eCollisionTag::EnemyTank, eTankType::DefenceEnemy, eTankColor::White, DEFENCE_TANK_INFO));
	mAISpawner->AddTankSpawnInfo(TankSpawnInfo(eCollisionTag::EnemyTank, eTankType::DefenceEnemy, eTankColor::White, DEFENCE_TANK_INFO));

	return S_OK;
}

void TankScene::Release()
{
	SAFE_RELEASE(mAmmoSpawner);
	SAFE_RELEASE(mFirstPlayerTankSpawner);
	SAFE_RELEASE(mSecondPlayerTankSpawner);
	SAFE_RELEASE(mAISpawner);
	SAFE_RELEASE(mPhysics);
}

void TankScene::Update()
{
	if (KEY_MGR->IsOnceKeyDown(VK_TAB)) { mbIsDebugMode = !mbIsDebugMode; }

	SAFE_UPDATE(mAmmoSpawner);
	SAFE_UPDATE(mFirstPlayerTankSpawner);
	SAFE_UPDATE(mSecondPlayerTankSpawner);
	SAFE_UPDATE(mAISpawner);
}

void TankScene::Render(HDC hdc)
{
	mBackground->Render(hdc);

	SAFE_RENDER(mFirstPlayerTankSpawner);
	SAFE_RENDER(mSecondPlayerTankSpawner);
	SAFE_RENDER(mAmmoSpawner);
	SAFE_RENDER(mAISpawner);
	if (mbIsDebugMode) { SAFE_RENDER(mPhysics); }
}
