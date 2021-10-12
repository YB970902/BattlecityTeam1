#pragma once
#include "Config.h"

class GameEntity
{
public:
	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};