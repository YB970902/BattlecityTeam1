#pragma once
#include "GameEntity.h"


class Image;
class Collider;
class Physcis;

class MainGame : public GameEntity
{
private:
	HANDLE hTimer;

	Collider* collider1;
	Collider* collider2;
	Collider* collider3;

	Physcis* physcis;


	int mousePosX;
	int mousePosY;
	int clickedMousePosX;
	int clickedMousePosY;

	Image* backBuffer;

public:
	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};