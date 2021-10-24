#pragma once
#include "GameEntity.h"

class Tank;
class Image;
class Particle : public GameEntity
{
protected:
	Image* mImage = nullptr;
	Tank* mTank = nullptr;

	float mDurationTime = 0.0f;
	float mOneFrameDurationTime = 0.0f;
	float mElapsedFrameTime = 0.0f;
	float mElapsedTotalTime = 0.0f;

	int mFrameX = 0;
	int mCurFrameX = 0;
	int mDir = 1;

	POINTFLOAT mPos = { 0, 0 };

	bool mbIsEnd = false;
public:
	HRESULT Init(Image* image, int frameX, float durationTime, float repeatTime);
	void Release();
	void Update();
	void Render(HDC hdc);

	void SetPosition(POINTFLOAT pos) { mPos = pos; }
	void SetTank(Tank* tank) { mTank = tank; }

	bool IsEnd() { return mbIsEnd; }
	void SetIsEnd(bool set) { mbIsEnd = set; }
};

