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

	mAmmoSpawner = new AmmoSpawner();
	mAmmoSpawner->Init();

	mPlayerController = new TankController();
	mPlayerController->Init('A', 'D', 'W', 'S', VK_SPACE);
	mPlayerController->SetAmmoSpawner(mAmmoSpawner);

	mPlayerTankSpawner = new TankSpawner();
	mPlayerTankSpawner->Init(mPhysics, SPAWN_INFO(eCollisionTag::PlayerTank, eTankType::Player, eTankColor::Yellow, NORMAL_TANK_INFO), 5, 0.0f, POINTFLOAT{ WIN_SIZE_X * 0.5f, WIN_SIZE_Y * 0.5f });
	mPlayerTankSpawner->SetController(mPlayerController);

	mAISpawner = new AITankSpawner();
	mAISpawner->Init(mPhysics, 0.5f, 3);
	mAISpawner->SetAmmoSpawner(mAmmoSpawner);

	POINTFLOAT* arrPoint = new POINTFLOAT[3];
	arrPoint[0].x = 300.0f;
	arrPoint[0].y = 200.0f;
	arrPoint[1].x = WIN_SIZE_X * 0.5f;
	arrPoint[1].y = 200.0f;
	arrPoint[2].x = WIN_SIZE_X - 300.0f;
	arrPoint[2].y = 200.0f;

	mAISpawner->SetSpawnPosition(arrPoint, 3);
	mAISpawner->AddTankSpawnInfo(TankSpawnInfo(eCollisionTag::EnemyTank, eTankType::Player, eTankColor::White, NORMAL_TANK_INFO));
	mAISpawner->AddTankSpawnInfo(TankSpawnInfo(eCollisionTag::EnemyTank, eTankType::Player, eTankColor::White, NORMAL_TANK_INFO));
	mAISpawner->AddTankSpawnInfo(TankSpawnInfo(eCollisionTag::EnemyTank, eTankType::Player, eTankColor::White, NORMAL_TANK_INFO));
	mAISpawner->AddTankSpawnInfo(TankSpawnInfo(eCollisionTag::EnemyTank, eTankType::Player, eTankColor::White, NORMAL_TANK_INFO));
	mAISpawner->AddTankSpawnInfo(TankSpawnInfo(eCollisionTag::EnemyTank, eTankType::Player, eTankColor::White, NORMAL_TANK_INFO));
	mAISpawner->AddTankSpawnInfo(TankSpawnInfo(eCollisionTag::EnemyTank, eTankType::Player, eTankColor::White, NORMAL_TANK_INFO));

	return S_OK;
}

void TankScene::Release()
{
	SAFE_RELEASE(mAmmoSpawner);
	SAFE_RELEASE(mPlayerTankSpawner);
	SAFE_RELEASE(mAISpawner);
	SAFE_RELEASE(mPhysics);
}

void TankScene::Update()
{
	SAFE_UPDATE(mAmmoSpawner);
	SAFE_UPDATE(mPlayerTankSpawner);
	SAFE_UPDATE(mAISpawner);
}

void TankScene::Render(HDC hdc)
{
	mBackground->Render(hdc);

	SAFE_RENDER(mAmmoSpawner);
	SAFE_RENDER(mPlayerTankSpawner);
	SAFE_RENDER(mAISpawner);
	SAFE_RENDER(mPhysics);
}
