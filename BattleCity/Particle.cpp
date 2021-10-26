#include "Particle.h"
#include "Image.h"
#include "Tank.h"
#include "Subject.h"

HRESULT Particle::Init(Image* image, int frameX, float durationTime, float repeatTime, float delayTime)
{
    mImage = image;
    mSubject = new Subject();
    mFrameX = frameX;
    mCurFrameX = 0;
    mDurationTime = durationTime;
    mOneFrameDurationTime = repeatTime / (float)(frameX + frameX - 1);
    mElapsedFrameTime = 0.0f;
    mElapsedTotalTime = 0.0f;
    mDurationDelayTime = delayTime;
    mElapsedDelayTime = 0.0f;

    mbIsWait = true;
    return S_OK;
}

void Particle::Release()
{
    mSubject->Notify(this, eSubjectTag::Particle, eEventTag::Released);
    SAFE_DELETE(mSubject);
}

void Particle::Update()
{
    if (mbIsWait)
    {
        mElapsedDelayTime += DELTA_TIME;
        if (mElapsedDelayTime > mDurationDelayTime)
        {
            mbIsWait = false;
        }
        else { return; }
    }
    mElapsedFrameTime += DELTA_TIME;
    if (mElapsedFrameTime >= mOneFrameDurationTime)
    {
        mElapsedFrameTime -= mOneFrameDurationTime;
        mCurFrameX += mDir;
        if (mCurFrameX == mFrameX)
        {
            mCurFrameX -= 2;
            mDir = -1;
        }
        else if (mCurFrameX == -1)
        {
            mCurFrameX += 2;
            mDir = 1;
        }
    }
    mElapsedTotalTime += DELTA_TIME;
    if (mElapsedTotalTime >= mDurationTime)
    {
        mbIsEnd = true;
    }

    if (mTank) { mPos = mTank->GetPosition(); }
}

void Particle::Render(HDC hdc)
{
    if (mbIsWait) { return; }
    mImage->Render(hdc, mPos.x, mPos.y, mCurFrameX, 0);
}

void Particle::SetTank(Tank* tank)
{
    mTank = tank;
    if (tank)
    {
        mSubject->AddObserver(static_cast<Observer*>(tank));
        mSubject->Notify(this, eSubjectTag::Particle, eEventTag::Added);
    }
}

void Particle::OnNotify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag)
{
    cout << "신호를 받음 서브젝트 : " << (int)subjectTag << ", 이벤트 : " << (int)eventTag << endl;

    switch (subjectTag)
    {
    case eSubjectTag::Tank:
        switch (eventTag)
        {
        case eEventTag::Released:
            cout << "파티클이 탱크 제거" << endl;
            mSubject->RemoveObserver(dynamic_cast<Observer*>(obj));
            mbIsEnd = true;
            break;
        }
        break;
    }
}
