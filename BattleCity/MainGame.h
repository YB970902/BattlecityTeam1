#pragma once
#include "GameEntity.h"

class MapEditor;
class Image;
class Collider;
class Physcis;
class TitleScene;

class MainGame : public GameEntity
{
private:
	HANDLE hTimer;

	int mousePosX;
	int mousePosY;
	int clickedMousePosX;
	int clickedMousePosY;

	Image* backBuffer;

	MapEditor* mapEditor;

public:
	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};