#pragma once
#include "GameEntity.h"

#define LEFT_SORT 1
#define RIGHT_SORT 0

Image;
class Score : public GameEntity
{
private:
	Image* mScoreImg;
	Image* mBackGround;
	POINTFLOAT mScorePos;
	int mTempScore;
	int mTotalScore;
	int mScore[9];
	int count;
	bool mSort;
	float mScale;

	float mRenderTime;
	
public:
	HRESULT Init() override;
	HRESULT Init(eImageTag tag = eImageTag::WhiteNumber, float posX = 0, float posY = 0, bool sort = 0, float scale = 1.0f);
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;

	void SetScore(int score);
	inline void SetScorePos(POINTFLOAT pos) { this->mScorePos = pos; }
	inline int GetScore() { return this->mTotalScore; }
	void AddScore(int score);


	virtual ~Score() {};

};

