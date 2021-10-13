#pragma once
#include "GameEntity.h"

class Collider : public GameEntity
{
private:
	POINTFLOAT mPlayerPos;
	RECT mPlayerBody;
	float mPlayerBodySize;

	int mX, mY;

public:
	HRESULT Init(POINTFLOAT pos, float bodySize);
	void UpdateBodySize();


	void Render(HDC hdc) override;

	inline POINTFLOAT GetPlayerPos() { return this->mPlayerPos; }
	inline float GetPlayerBodySize() { return this->mPlayerBodySize; }
	inline RECT GetPlayerBody() { return this->mPlayerBody; }
	void SetPlayerPos(POINTFLOAT movePos);

};