#pragma once
#include "GameEntity.h"

class Image;
class FixedUI : public GameEntity
{
protected:
	Image* mImage = nullptr;

	POINTFLOAT mPos = { 0, 0 };
	bool mbIsDead = false;
public:
	virtual HRESULT Init(Image* image, POINTFLOAT pos);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	POINTFLOAT GetPosition() { return mPos; }
	void SetPosition(POINTFLOAT pos) { mPos = pos; }
	bool IsDead() { return mbIsDead; }

	virtual ~FixedUI() {}
};