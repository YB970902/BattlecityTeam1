#include "Config.h"
#include "MainGame.h"
#include "Image.h"
#include "MapEditorScene.h"
#include "BattleScene.h"
#include "CommonFunction.h"
#include "TitleScene.h"
#include "SlateScene.h"
#include "ScoreScene.h"
#include "PhyscisScene.h"
#include "TankScene.h"

HRESULT MainGame::Init()
{
	KEY_MGR->Init();
	IMG_MGR->Init();
	TIMER_MGR->Init();
	SCENE_MGR->Init();
	PART_MGR->Init();
	UI_MGR->Init();

	srand((unsigned int) time(nullptr));

	hTimer = (HANDLE)SetTimer(g_hWnd, 0, 10, NULL);

	mousePosX = 0;
	mousePosY = 0;
	clickedMousePosX = 0; 
	clickedMousePosY = 0; 

	backBuffer = new Image();
	backBuffer->Init("Image/mapImage.bmp", WIN_SIZE_X, WIN_SIZE_Y);

	SCENE_MGR->AddScene(eSceneTag::MapToolScene, new MapEditorScene);
	SCENE_MGR->AddScene(eSceneTag::PhysicsScene, new PhyscisScene);
	SCENE_MGR->AddScene(eSceneTag::BattleScene, new BattleScene);
	SCENE_MGR->AddScene(eSceneTag::TankScene, new TankScene);
	SCENE_MGR->AddScene(eSceneTag::TitleScene, new TitleScene);
	SCENE_MGR->AddScene(eSceneTag::SlateScene, new SlateScene);
	SCENE_MGR->AddScene(eSceneTag::ScoreScene, new ScoreScene);
	
	SCENE_MGR->ChangeScene(eSceneTag::TitleScene);

	return S_OK;
}

void MainGame::Update()
{
	TIMER_MGR->Update();
	
	SCENE_MGR->Update();

	PART_MGR->Update();

	UI_MGR->Update();
	
	KEY_MGR->Update();

	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render(HDC hdc)
{
	HDC hBackBufferDC = backBuffer->GetMemDC();
	
	SCENE_MGR->Render(hBackBufferDC);

	PART_MGR->Render(hBackBufferDC);

	UI_MGR->Render(hBackBufferDC);
	
	//TIMER_MGR->Render(hBackBufferDC);
	
	backBuffer->Render(hdc);

}

void MainGame::Release()
{
	SAFE_RELEASE(backBuffer);

	TIMER_MGR->Release();
	TIMER_MGR->ReleaseSingleton();
	
	IMG_MGR->Release();
	IMG_MGR->ReleaseSingleton();

	KEY_MGR->Release();
	KEY_MGR->ReleaseSingleton();

	SCENE_MGR->Release();
	SCENE_MGR->ReleaseSingleton();

	PART_MGR->Release();
	PART_MGR->ReleaseSingleton();

	UI_MGR->Release();
	UI_MGR->ReleaseSingleton();

	KillTimer(g_hWnd, 0);
}


LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_SIZE:
		SetWindowSize(WIN_START_POS_X, WIN_START_POS_Y, WIN_SIZE_X, WIN_SIZE_Y);
		break;
	case WM_LBUTTONDOWN:
		clickedMousePosX = LOWORD(lParam);
		clickedMousePosY = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
