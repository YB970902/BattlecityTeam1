#pragma once
#include "GameEntity.h"
#include "Observer.h"

class Tank;
class Image;
class Subject;
class Particle : public GameEntity, public Observer
{
protected:
	Image* mImage = nullptr;
	Tank* mTank = nullptr;

	Subject* mSubject = nullptr;

	float mDurationTime = 0.0f;
	float mOneFrameDurationTime = 0.0f;
	float mElapsedFrameTime = 0.0f;
	float mElapsedTotalTime = 0.0f;
	float mDurationDelayTime = 0.0f;
	float mElapsedDelayTime = 0.0f;

	int mFrameX = 0;
	int mCurFrameX = 0;
	int mDir = 1;

	POINTFLOAT mPos = { 0, 0 };

	bool mbIsEnd = false;
	bool mbIsWait = false;
public:
	virtual ~Particle() {}
	HRESULT Init(Image* image, int frameX, float durationTime, float repeatTime, float delayTime);
	void Release();
	void Update();
	void Render(HDC hdc);

	void SetPosition(POINTFLOAT pos) { mPos = pos; }
	void SetTank(Tank* tank);

	bool IsEnd() { return mbIsEnd; }
	void SetIsEnd(bool set) { mbIsEnd = set; }

	virtual void OnNotify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag) override;
};

