#include "Particle.h"
#include "Image.h"
#include "Tank.h"

HRESULT Particle::Init(Image* image, int frameX, float durationTime, float repeatTime)
{
    mImage = image;
    mFrameX = frameX;
    mCurFrameX = 0;
    mDurationTime = durationTime;
    mOneFrameDurationTime = repeatTime / (float)(frameX + frameX - 1);
    mElapsedFrameTime = 0.0f;
    mElapsedTotalTime = 0.0f;
    return S_OK;
}

void Particle::Release()
{
}

void Particle::Update()
{
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
        if (mTank) { mTank->OnParticleEnded(this); mTank = nullptr; }
    }

    if (mTank) { mPos = mTank->GetPosition(); }
}

void Particle::Render(HDC hdc)
{
    mImage->Render(hdc, mPos.x, mPos.y, mCurFrameX, 0);
}
