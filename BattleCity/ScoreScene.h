#pragma once
#include "GameEntity.h"


class Score;
class Image;
class ScoreScene : public GameEntity
{
private:
	Image* mScoreSceneBG;
	Image* mTankImg[4];	
	Image* mArrowLeft[4];	
	Image* mArrowRight[4];
	Image* mPTSText[4];
	Image* mPTSTextBo;
	Image* mPlayerText;
	Image* mPlayer2Text;
	Image* mStageText;
	Image* mTotalText;
	Image* mBonusText;

	Image* mPTSText2[4];
	Image* mPTSTextBo2;
	Image* mPlayerText2;

	Score* mTankPTS[4];
	Score* mTankCnt[4];
	Score* mTotalCnt;
	Score* mTotalPTS;
	Score* mStage;
	Score* mBonus;
	Score* mBonus2;

	Score* mTankPTS2[4];
	Score* mTankCnt2[4];
	Score* mTotalCnt2;
	Score* mTotalPTS2;
	float mElapedCount;
	int mCulCount;

	string mFirstTankTypeCount[4];
	string mSecondTankTypeCount[4];

public:
	virtual ~ScoreScene() = default;

	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;

	

};

