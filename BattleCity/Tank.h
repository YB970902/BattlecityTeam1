#pragma once
#include "GameObject.h"

enum class eTankType
{
	Player = 0,
	Enemey1 = 4,
	Enemey2 = 5,
	Enemey3 = 6,
	Enemey4 = 7,
};

enum class eTankColor
{
	Yellow,
	White,
	Green,
	Red,
};

typedef struct TankInfo
{
	float MoveSpeed;
	float AttackSpeed;
	int Health;
} TANK_INFO;

const TANK_INFO PLAYER_TANK_INFO{ 100.0f, 0.0f, 1 };
const TANK_INFO ENEMY1_TANK_INFO{ 100.0f, 0.0f, 1 };
const TANK_INFO ENEMY2_TANK_INFO{ 100.0f, 0.0f, 1 };
const TANK_INFO ENEMY3_TANK_INFO{ 100.0f, 0.0f, 1 };
const TANK_INFO ENEMY4_TANK_INFO{ 100.0f, 0.0f, 1 };

class Tank : public GameObject
{
protected:
	const POINT COLOR_START_FRAME[4] = { {0, 0}, {8, 0}, {0, 8}, {8, 8} };
	const int MAX_STAR_COUNT = 3;
	const POINT DIR_VALUE[4] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0} };
	const float MAX_ANIM_TIME = 0.1f;

	eCollisionTag mColTag;
	eTankType mType;
	eTankColor mColor;
	TANK_INFO mInfo;

	int mStarCount;

	float mElapsedAnimTime = 0.0f;
	int mCurAnim = 0;

public:
	HRESULT Init(eCollisionTag colTag, eTankType type, TANK_INFO info, eTankColor color, float x, float y);
	void Release();
	void Update();
	void Render(HDC hdc);

	void Move(eDir dir);

	inline void AddStar() { if (mType == eTankType::Player && mStarCount < MAX_STAR_COUNT) { mStarCount++; } }

	inline POINTFLOAT GetPosition() { return mPos; }
	inline void SetPosition(POINTFLOAT pos) { mPos = pos; }

	virtual void OnCollided(eCollisionDir dir, eCollisionTag tag) override;
};

