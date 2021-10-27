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
#include "GameManager.h"

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

	mGameManager = new GameManager();
	mGameManager->Init();

	return S_OK;
}

void BattleScene::Update()
{
	SAFE_UPDATE(mGameManager);
}

void BattleScene::Render(HDC hdc)
{
	SAFE_RENDER(mGameManager);
}

void BattleScene::Release()
{
	SAFE_RELEASE(mGameManager);
}