#include "ScoreScene.h"
#include "Score.h"
#include "Image.h"

HRESULT ScoreScene::Init()
{
    mElapedCount = 0;
    mCulCount = 0;

    mFirstTankTypeCount[0] = "FirstPlayerNormalEnemyTank";
    mFirstTankTypeCount[1] = "FirstPlayerQuickEnemyTank";
    mFirstTankTypeCount[2] = "FirstPlayerRapidFireEnemyTank";
    mFirstTankTypeCount[3] = "FirstPlayerDefenceEnemyTank";

    mSecondTankTypeCount[0] = "SecondPlayerNormalEnemyTank";
    mSecondTankTypeCount[1] = "SecondPlayerQuickEnemyTank";
    mSecondTankTypeCount[2] = "SecondPlayerRapidFireEnemyTank";
    mSecondTankTypeCount[3] = "SecondPlayerDefenceEnemyTank";

    mScoreSceneBG = IMG_MGR->FindImage(eImageTag::BlackBG);
    mPlayerText = IMG_MGR->FindImage(eImageTag::PlayerText);
    mPlayer2Text = IMG_MGR->FindImage(eImageTag::PlayerText2);
    mStageText = IMG_MGR->FindImage(eImageTag::StageText);
    mTotalText = IMG_MGR->FindImage(eImageTag::TotalText);
    mBonusText = IMG_MGR->FindImage(eImageTag::BonusText);
    mPTSTextBo = IMG_MGR->FindImage(eImageTag::PointsText);

    //SCENE_MGR->SetSceneData("Stage", 2);//지워도 되는 부분
    mStage = new Score;
    mStage->Init(eImageTag::OrangeNumber, 680, 145, 0, 1);
    mStage->SetScore(SCENE_MGR->GetSceneData("Stage"));

    mTotalCnt = new Score;
    mTotalCnt->Init(eImageTag::WhiteNumber, 525, 640);
    mTotalCnt2 = new Score;
    mTotalCnt2->Init(eImageTag::WhiteNumber, 735, 640);

    mTotalPTS = new Score;
    mTotalPTS->Init(eImageTag::OrangeNumber, 390, 240);

    mTotalPTS2 = new Score;
    mTotalPTS2->Init(eImageTag::OrangeNumber, 1010, 240);

    mBonus = new Score;
    mBonus->Init(eImageTag::WhiteNumber, 330, 700);
    mBonus->SetScore(1000);

    mBonus2 = new Score;
    mBonus2->Init(eImageTag::WhiteNumber, 910, 700);
    mBonus2->SetScore(1000);

    SCENE_MGR->SetSceneData("IsSoloMode", 1);
    cout << SCENE_MGR->GetSceneData("IsSoloMode") << endl;


    for (int i = 0; i < 4; ++i)
    {
        mTankImg[i] = IMG_MGR->FindImage(eImageTag::EnemyOrderBox);
        mTankCnt[i] = new Score;
        mTankCnt[i]->Init(eImageTag::WhiteNumber, 525, 318 + (90 * i));
        
        mTankPTS[i] = new Score;
        mTankPTS[i]->Init(eImageTag::WhiteNumber, 285, 318 + (90 * i));

        mArrowLeft[i] = IMG_MGR->FindImage(eImageTag::LeftArrow);
        mPTSText[i] = IMG_MGR->FindImage(eImageTag::PointsText);

        mTotalCnt->AddScore(mTankCnt[i]->GetScore());
        mTotalPTS->AddScore(mTankPTS[i]->GetScore());

        mArrowRight[i] = IMG_MGR->FindImage(eImageTag::RightArrow);

        mTankPTS2[i] = new Score;
        mTankPTS2[i]->Init(eImageTag::WhiteNumber, 915, 318 + (90 * i));

        mTankCnt2[i] = new Score;
        mTankCnt2[i]->Init(eImageTag::WhiteNumber, 735, 318 + (90 * i));
        
    }

    //없애도 되는 부분(Test Count)
    SCENE_MGR->SetSceneData(mFirstTankTypeCount[0], 10);
    SCENE_MGR->SetSceneData(mSecondTankTypeCount[0], 13);

    SCENE_MGR->SetSceneData(mFirstTankTypeCount[1], 5);
    SCENE_MGR->SetSceneData(mSecondTankTypeCount[1], 2);

    SCENE_MGR->SetSceneData(mFirstTankTypeCount[2], 21);
    SCENE_MGR->SetSceneData(mSecondTankTypeCount[2], 13);

    SCENE_MGR->SetSceneData(mFirstTankTypeCount[3], 6);
    SCENE_MGR->SetSceneData(mSecondTankTypeCount[3], 8);

    SCENE_MGR->SetSceneData("IsGameOver", 0);

    SCENE_MGR->SetSceneData("FirstPlayerGrenadeItem",55);
    SCENE_MGR->SetSceneData("SecondPlayerGrenadeItem", 55);
    //여기까지

    if (!(SCENE_MGR->GetSceneData("Stage") == 1))
    {
        mTotalPTS->AddScore(SCENE_MGR->GetSceneData("FirstPlayerTotalPts"));
        mTotalPTS2->AddScore(SCENE_MGR->GetSceneData("SecondPlayerTotalPts"));
    }
    for (int i = 0; i < 4; ++i)
    {
        mTotalPTS->AddScore(SCENE_MGR->GetSceneData(mFirstTankTypeCount[i]) * (100 * (i + 1)));
        mTotalPTS2->AddScore(SCENE_MGR->GetSceneData(mSecondTankTypeCount[i]) * (100 * (i + 1)));
    }
    mTotalPTS->AddScore(SCENE_MGR->GetSceneData("FirstPlayerGrenadeItem") * 500);
    mTotalPTS2->AddScore(SCENE_MGR->GetSceneData("SecondPlayerGrenadeItem") * 500);

    if (!(SCENE_MGR->GetSceneData("IsGameOver")))      //게임오버 판단 후 보너스점수
    {
        if (SCENE_MGR->GetSceneData("IsSoloMode") != 0 && (mTotalPTS->GetScore() > mTotalPTS2->GetScore()))
        {
            mTotalPTS->AddScore(1000);
        }
        else if (SCENE_MGR->GetSceneData("IsSoloMode") != 0 && (mTotalPTS->GetScore() < mTotalPTS2->GetScore()))
        {
            mTotalPTS2->AddScore(1000);
        }
    }




    return S_OK;
}

void ScoreScene::Update()
{
    mElapedCount += DELTA_TIME;
    mStage->Update();
    for (int i = 0; i < 4; ++i)
    {
        mTankCnt[i]->Update();
        mTankPTS[i]->Update();
        mTankCnt2[i]->Update();
        mTankPTS2[i]->Update();
    }
    mTotalCnt->Update();
    mTotalPTS->Update();
    mTotalCnt2->Update();
    mTotalPTS2->Update();
    mBonus->Update();
    mBonus2->Update();


    if (mElapedCount >= 0.1f && (mCulCount < 4))
    {
        
        if (mTankCnt[mCulCount]->GetScore() < SCENE_MGR->GetSceneData(mFirstTankTypeCount[mCulCount]))
        {
            mTankCnt[mCulCount]->SetScore(mTankCnt[mCulCount]->GetScore() + 1);
            mTankPTS[mCulCount]->SetScore(mTankCnt[mCulCount]->GetScore() * 100*(mCulCount + 1));
        }

        if (mTankCnt2[mCulCount]->GetScore() < SCENE_MGR->GetSceneData(mSecondTankTypeCount[mCulCount]))
        {
            mTankCnt2[mCulCount]->SetScore(mTankCnt2[mCulCount]->GetScore() + 1);
            mTankPTS2[mCulCount]->SetScore(mTankCnt2[mCulCount]->GetScore() * 100 * (mCulCount + 1));
        }

        mElapedCount = 0;
        if (mTankCnt[mCulCount]->GetScore() == SCENE_MGR->GetSceneData(mFirstTankTypeCount[mCulCount]) &&
            mTankCnt2[mCulCount]->GetScore() == SCENE_MGR->GetSceneData(mSecondTankTypeCount[mCulCount]) && 
            (mCulCount < 4))
        {
            ++mCulCount;
        }
    }
    else if (mCulCount == 4)
    {
        for (int i = 0; i < 4; ++i)
        {
            mTotalCnt->AddScore(mTankCnt[i]->GetScore());
            mTotalCnt2->AddScore(mTankCnt2[i]->GetScore());
        }
        mElapedCount = 0;
        ++mCulCount;
    }

    if (mElapedCount >= 1.5f && mCulCount == 5)
    {
        if (SCENE_MGR->GetSceneData("IsGameOver"))
        {
            for (int i = 0; i < 4; ++i)
            {
                SCENE_MGR->SetSceneData(mFirstTankTypeCount[i], 0);
                SCENE_MGR->SetSceneData(mSecondTankTypeCount[i], 0);
            }
                SCENE_MGR->SetSceneData("FirstPlayerTotalPts", 0);
                SCENE_MGR->SetSceneData("SecondPlayerTotalPts", 0);
                SCENE_MGR->SetSceneData("Stage", 0);
            SCENE_MGR->ChangeScene(eSceneTag::TitleScene);
        }
        else
        {
            //넘겨줄 정보 입력
            for (int i = 0; i < 4; ++i)
            {
                SCENE_MGR->SetSceneData(mFirstTankTypeCount[i],0);
                SCENE_MGR->SetSceneData(mSecondTankTypeCount[i],0);
            }
                SCENE_MGR->SetSceneData("FirstPlayerTotalPts", mTotalPTS->GetScore());
                SCENE_MGR->SetSceneData("SecondPlayerTotalPts", mTotalPTS2->GetScore());
                SCENE_MGR->SetSceneData("Stage", SCENE_MGR->GetSceneData("Stage") + 1);
            SCENE_MGR->ChangeScene(eSceneTag::BattleScene);
        }
    }
}

void ScoreScene::Render(HDC hdc)
{
    mScoreSceneBG->Render(hdc);
    mStage->Render(hdc);
    mTotalCnt->Render(hdc);
    mTotalPTS->Render(hdc);
    mPlayerText->Render(hdc, 300, 195);
    mStageText->Render(hdc, 570, 145);
    mTotalText->Render(hdc, 537, 640);
    
    for (int i = 0; i < 4; ++i)
    {
        mTankImg[i]->Render(hdc, 600, 300 + (90 * i), 0, i, 2.0f);
        mTankCnt[i]->Render(hdc);
        mTankPTS[i]->Render(hdc);

        mArrowLeft[i]->Render(hdc, 550, 313 + (90 * i), 0, 0, 1.5f);
        mPTSText[i]->Render(hdc, 360, 318 + (90 * i));
    }
    
    if (SCENE_MGR->GetSceneData("IsSoloMode") != 0 && (mTotalPTS->GetScore() > mTotalPTS2->GetScore())&& mCulCount == 5)
    {
        mBonus->Render(hdc);
        mBonusText->Render(hdc, 300, 670, 0, 0, 1.5f);
        mPTSTextBo->Render(hdc, 400, 700);
    }
    

    if (SCENE_MGR->GetSceneData("IsSoloMode") != 0)
    {
        mTotalCnt2->Render(hdc);
        mTotalPTS2->Render(hdc);
        mPlayer2Text->Render(hdc, 920, 195);

        if (mTotalPTS->GetScore() < mTotalPTS2->GetScore())
        {
            mBonus2->Render(hdc);
            mBonusText->Render(hdc, 880, 670, 0, 0, 1.5f);
            mPTSTextBo->Render(hdc, 980, 700);
        }

        for (int i = 0; i < 4; ++i)
        {
            mTankCnt2[i]->Render(hdc);
            mTankPTS2[i]->Render(hdc);

            mArrowRight[i]->Render(hdc, 665, 318 + (90 * i), 0, 0, 1.5f);
            mPTSText[i]->Render(hdc, 980, 318 + (90 * i));
        }
    }
}

void ScoreScene::Release()
{
    SAFE_RELEASE(mStage);
    SAFE_RELEASE(mTotalCnt);
    SAFE_RELEASE(mTotalPTS);
    SAFE_RELEASE(mTotalCnt2);
    SAFE_RELEASE(mTotalPTS2);
    SAFE_RELEASE(mBonus);
    SAFE_RELEASE(mBonus2);

    for (int i = 0; i < 4; i++)
    {
        SAFE_RELEASE(mTankPTS[i]);
        SAFE_RELEASE(mTankCnt[i]);
        SAFE_RELEASE(mTankPTS2[i]);
        SAFE_RELEASE(mTankCnt2[i]);
    }
}
