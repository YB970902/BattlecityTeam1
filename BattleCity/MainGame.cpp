#include "Config.h"
#include "MainGame.h"
#include "Image.h"

HRESULT MainGame::Init()
{
	KEY_MGR->Init();
	IMG_MGR->Init();
	TIMER_MGR->Init();
	SCENE_MGR->Init();

	srand((unsigned int) time(nullptr));

	hTimer = (HANDLE)SetTimer(g_hWnd, 0, 10, NULL);

	mousePosX = 0;
	mousePosY = 0;
	clickedMousePosX = 0; 
	clickedMousePosY = 0; 

	backBuffer = new Image();
	backBuffer->Init("Image/mapImage.bmp", WIN_SIZE_X, WIN_SIZE_Y);

	return S_OK;
}

void MainGame::Update()
{
	TIMER_MGR->Update();

	SCENE_MGR->Update();

	KEY_MGR->Update();

	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render(HDC hdc)
{
	HDC hBackBufferDC = backBuffer->GetMemDC();

	SCENE_MGR->Render(hBackBufferDC);

	TIMER_MGR->Render(hBackBufferDC);

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

	KillTimer(g_hWnd, 0);
}


LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
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
