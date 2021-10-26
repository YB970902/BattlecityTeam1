#include "MovingUI.h"
#include "Image.h"

HRESULT MovingUI::Init(Image* image, POINTFLOAT pos, float durationTime)
{
	mImage = image;
	mPos = pos;
	mDurationTime = durationTime;
	mbIsNotMoving = true;
	return S_OK;
}

HRESULT MovingUI::Init(Image* image, POINTFLOAT startPos, POINTFLOAT destPos, float durationTime)
{
	mImage = image;
	mStartPos = startPos;
	mDestPos = destPos;
	mDurationTime = durationTime;
	return S_OK;
}

HRESULT MovingUI::Init(Image* image, POINTFLOAT startPos, POINTFLOAT destPos, float durationTime, float waitingTime)
{
	mImage = image;
	mStartPos = startPos;
	mDestPos = destPos;
	mDurationTime = durationTime;
	mbIsWaitUI = true;
	mbIsWaiting = false;
	mDurationWaitTime = waitingTime;

	return S_OK;
}

void MovingUI::Release()
{
}

void MovingUI::Update()
{
	if (mbIsWaiting)
	{
		mElapsedWaitTime += DELTA_TIME;
		if (mElapsedWaitTime >= mDurationWaitTime) { mbIsDead = true; }
		return;
	}
	mElapsedTime += DELTA_TIME;
	if (mElapsedTime >= mDurationTime)
	{
		if (mbIsWaitUI) { mbIsWaiting = true; }
		else { mbIsDead = true; }
		return;
	}
	if (mbIsNotMoving) { return; }
	float weight = mElapsedTime / mDurationTime;
	mPos.x = mStartPos.x + (mDestPos.x - mStartPos.x) * weight;
	mPos.y = mStartPos.y + (mDestPos.y - mStartPos.y) * weight;
}

void MovingUI::Render(HDC hdc)
{
	if (mbIsDead) { return; }
	mImage->Render(hdc, mPos.x, mPos.y);
}
