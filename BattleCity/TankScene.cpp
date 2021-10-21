#include "Config.h"
#include "TankScene.h"
#include "Tank.h"
#include "Image.h"
#include "TankController.h"

HRESULT TankScene::Init()
{
	mBackground = IMG_MGR->FindImage(eImageTag::BackGround);;

	mPlayer1Tank = new Tank();
	mPlayer1Tank->Init(eTankType::Player, PLAYER_TANK_INFO, eTankColor::Yellow, WIN_SIZE_X * 0.5f - 100, WIN_SIZE_Y * 0.5f);
	mPlayer2Tank = new Tank();
	mPlayer2Tank->Init(eTankType::Player, PLAYER_TANK_INFO, eTankColor::Green, WIN_SIZE_X * 0.5f + 100, WIN_SIZE_Y * 0.5f);
	mEnemyTank = new Tank();
	mEnemyTank->Init(eTankType::Enemey1, ENEMY1_TANK_INFO, eTankColor::White, WIN_SIZE_X * 0.5f, WIN_SIZE_Y * 0.5f);

	mPlayerController = new TankController();
	mPlayerController->Init(mPlayer1Tank, 'A', 'D', 'W', 'S');

	return S_OK;
}

void TankScene::Release()
{
}

void TankScene::Update()
{
	mPlayerController->Update();
	if (KEY_MGR->IsOnceKeyDown(VK_SPACE)) { mPlayer1Tank->AddStar(); }
}

void TankScene::Render(HDC hdc)
{
	mBackground->Render(hdc);

	mPlayer1Tank->Render(hdc);
	mPlayer2Tank->Render(hdc);
	mEnemyTank->Render(hdc);
}
