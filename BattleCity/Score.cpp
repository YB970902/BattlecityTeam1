#include "Score.h"
#include "Image.h"

HRESULT Score::Init()
{
    mScoreImg = IMG_MGR->FindImage(eImageTag::WhiteNumber);
    mBackGround = IMG_MGR->FindImage(eImageTag::BlackBG);
    mTotalScore = 0;
    mTempScore = mTotalScore;
    count=0;

    mScorePos.x = 200;
    mScorePos.y = 200;

    return S_OK;
}

HRESULT Score::Init(eImageTag tag, float posX, float posY, bool sort, float scale)
{
    switch (tag)
    {
        case eImageTag::WhiteNumber:
            mScoreImg = IMG_MGR->FindImage(eImageTag::WhiteNumber);
            break;
        case eImageTag::OrangeNumber:
            mScoreImg = IMG_MGR->FindImage(eImageTag::OrangeNumber);
            break;
    }
    mBackGround = IMG_MGR->FindImage(eImageTag::BlackBG);
    mTotalScore = 0;
    mTempScore = mTotalScore;
    count = 0;

    mScorePos.x = posX;
    mScorePos.y = posY;

    mSort = sort;

    mScale = scale;

    return S_OK;
}
//pos 지정
//좌우 정렬
//Tag에 따른 색 지정
//addscore

void Score::Update()
{
    //if (KEY_MGR->IsOnceKeyDown(VK_SPACE))
    //{
    //    mTotalScore += 100;
    //    mTempScore = mTotalScore;
    //    count = 0;
    //}
    if (mTotalScore == 0)
    {
        count = 1;
        mScore[count] = 0;
    }
    else
    {
        while (mTempScore > 0)
        {
            mScore[count] = mTempScore % 10;
            mTempScore /= 10;
            ++count;
        }
    }

   
}

void Score::Render(HDC hdc)
{
    //mBackGround->Render(hdc);

    switch (mSort)
    {
    case RIGHT_SORT:
        for (int i = 0; i < count ; ++i)
        {
            mScoreImg->Render(hdc, mScorePos.x - (i * (24 * mScale)), mScorePos.y, mScore[i] % 5, mScore[i] / 5, mScale);
        }
        break;
    case LEFT_SORT:
        for (int i = count - 1; i >= 0; --i)
        {
            mScoreImg->Render(hdc, mScorePos.x + ((count - i) * (24 * mScale)), mScorePos.y, mScore[i] % 5, mScore[i] / 5, mScale);
        }
        break;
    }
    

    
    
}

void Score::Release()
{

}

void Score::SetScore(int score)
{
    this->mTotalScore = score;
    this->mTempScore = mTotalScore;
    this->count = 0;
}

void Score::AddScore(int score)
{
    this->mTotalScore += score;
    this->mTempScore = mTotalScore;
    this->count = 0;
}
