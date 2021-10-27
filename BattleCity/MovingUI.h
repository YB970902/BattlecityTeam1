#pragma once
#include "FixedUI.h"

class MovingUI : public FixedUI
{
protected:
	POINTFLOAT mStartPos = { 0, 0 };
	POINTFLOAT mDestPos = { 0, 0 };

	float mDurationTime = 0.0f;
	float mElapsedTime = 0.0f;

	bool mbIsNotMoving = false;
	bool mbIsWaitUI = false;
	bool mbIsWaiting = false;
	float mDurationWaitTime = 0.0f;
	float mElapsedWaitTime = 0.0f;
public:
	virtual HRESULT Init(Image* image, POINTFLOAT pos, float durationTime);
	virtual HRESULT Init(Image* image, POINTFLOAT startPos, POINTFLOAT destPos, float durationTime);
	virtual HRESULT Init(Image* image, POINTFLOAT startPos, POINTFLOAT destPos, float durationTime, float waitingTime);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual ~MovingUI() {}
};

