#pragma once
#include "GameObject.h"
#include "Observer.h"

const float TANK_BODY_SIZE = 32.0f;

class Ammo;
class Subject;
class Tank : public GameObject, public Observer
{
protected:
	const POINT COLOR_START_FRAME[4] = { {0, 0}, {8, 0}, {0, 8}, {8, 8} };
	const int MAX_STAR_COUNT = 3;
	const float MAX_ANIM_TIME = 0.1f;
	const float MAX_SPARKLE_TIME = 0.175;
	const float MAX_STUN_TIME = 5.0f;

	eTankType mType;
	eTankColor mColor;
	TANK_INFO mInfo;

	int mStarCount = 0;

	float mElapsedAnimTime = 0.0f;
	int mCurAnim = 0;

	Subject* mSubject = nullptr;

	vector<Ammo*> mVecAmmo;
	float mElapsedFireTime = 0.0f;
	bool mbIsCanFire = false;

	bool mbIsInvincible = false;
	float mElapsedInvencibleTime = 0.0f;

	bool mbIsStun = false;
	float mElapsedStunTime = 0.0f;
	bool mbIsSparkle = false;
	float mElapsedSparkleTime = 0.0f;
public:
	virtual ~Tank() { }
	virtual HRESULT Init(eCollisionTag colTag, eTankType type, TANK_INFO info, eTankColor color, POINTFLOAT pos, Collider* collider);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void Move(eDir dir);
	void MoveForward();

	inline void SetIsInvencible(bool set) { mbIsInvincible = set; if (set) { TurnOnInvencible(); } }
	inline TANK_INFO GetInfo() { return mInfo; }
	inline void AddStar() { if (mType == eTankType::Player && mStarCount < MAX_STAR_COUNT) { mStarCount++; } }
	inline bool IsCanFire() { return mbIsCanFire && mVecAmmo.size() < mInfo.MaxAmmoCount && !mbIsStun; }
	inline void SetIsCanFire(bool set) { mbIsCanFire = set; }
	inline POINTFLOAT GetBarrelPosition() { return POINTFLOAT{ mPos.x + DIR_VALUE[(int)mDir].x, mPos.y + DIR_VALUE[(int)mDir].y }; }

	virtual void OnCollided(eCollisionDir dir, int tag) override;

	void AddAmmo(Ammo* ammo);
	void OnAmmoCollided(Ammo* ammo);

	void TurnOnInvencible();
protected:
	void TurnOnStun();

	virtual void OnNotify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag) override;
};

