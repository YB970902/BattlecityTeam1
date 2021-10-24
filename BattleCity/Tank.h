#pragma once
#include "GameObject.h"

enum class eTankType
{
	Player = 0,
	NormalEnemy = 4,
	QuickEnemy = 5,
	RapidFireEnemy = 6,
	DefenceEnemy = 7,
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
	int MaxAmmoCount;
} TANK_INFO;

const TANK_INFO PLAYER_TANK_INFO{ 100.0f, 0.0f, 1, 1 };
const TANK_INFO NORMAL_TANK_INFO{ 100.0f, 1.0f, 1, 1 };
const TANK_INFO QUICK_TANK_INFO{ 100.0f, 1.0f, 1, 1 };
const TANK_INFO RAPID_FIRE_TANK_INFO{ 100.0f, 0.5f, 1, 2 };
const TANK_INFO DEFENCE_TANK_INFO{ 100.0f, 1.0f, 1, 1 };

const float TANK_BODY_SIZE = 32.0f;

class Ammo;
class Tank : public GameObject
{
protected:
	const POINT COLOR_START_FRAME[4] = { {0, 0}, {8, 0}, {0, 8}, {8, 8} };
	const int MAX_STAR_COUNT = 3;
	const float MAX_ANIM_TIME = 0.1f;

	eTankType mType;
	eTankColor mColor;
	TANK_INFO mInfo;

	int mStarCount = 0;

	float mElapsedAnimTime = 0.0f;
	int mCurAnim = 0;

	vector<Ammo*> mVecAmmo;
	float mElapsedFireTime = 0.0f;
	bool mbIsCanFire = false;
public:
	virtual HRESULT Init(eCollisionTag colTag, eTankType type, TANK_INFO info, eTankColor color, POINTFLOAT pos, Collider* collider);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void Move(eDir dir);
	void MoveForward();

	inline TANK_INFO GetInfo() { return mInfo; }
	inline void AddStar() { if (mType == eTankType::Player && mStarCount < MAX_STAR_COUNT) { mStarCount++; } }
	inline bool IsCanFire() { return mbIsCanFire && mVecAmmo.size() < mInfo.MaxAmmoCount; }
	inline void SetIsCanFire(bool set) { mbIsCanFire = set; }
	inline POINTFLOAT GetBarrelPosition() { return POINTFLOAT{ mPos.x + DIR_VALUE[(int)mDir].x * TANK_BODY_SIZE, mPos.y + DIR_VALUE[(int)mDir].y * TANK_BODY_SIZE }; }

	virtual void OnCollided(eCollisionDir dir, int tag) override;

	void AddAmmo(Ammo* ammo);
	void OnAmmoCollided(Ammo* ammo);
};

