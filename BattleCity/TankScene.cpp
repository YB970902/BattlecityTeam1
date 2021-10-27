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

	mFirstPlayerController = new TankController();
	mFirstPlayerController->Init(FIRST_PLAYER_KEY);

	mFirstPlayerTankSpawner = new TankSpawner();
	mFirstPlayerTankSpawner->SetController(mFirstPlayerController);

	mSecondPlayerController = new TankController();
	mSecondPlayerController->Init(SECOND_PLAYER_KEY);

	mSecondPlayerTankSpawner = new TankSpawner();
	mSecondPlayerTankSpawner->SetController(mSecondPlayerController);

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
