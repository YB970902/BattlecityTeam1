#pragma once
#include "GameObject.h"
#include "CollisionChecker.h"
#include "Observer.h"

const float AMMO_BODY_SIZE = 16.0f;

class Subject;
class Ammo : public GameObject, public CollisionChecker, public Observer
{
protected:
	Subject* mSubject = nullptr;

	float mSpeed = 0.0f;
	POINTFLOAT mDirPoint = {};

	bool mbIsFire = false;

public:
	virtual ~Ammo() { }
	virtual HRESULT Init(eDir dir, eCollisionTag tag, float speed);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual void OnCollided(eCollisionDir dir, int tag) override;

	void AddObserver(Observer* obs);
	inline bool IsFire() { return mbIsFire; }
	inline void SetIsFire(bool set) { mbIsFire = set; }
	inline void SetCollider(Collider* collider) { mCollider = collider; };

	virtual void OnNotify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag) override;

};