#pragma once
#include "GameEntity.h"

class Ammo;
class Physcis;
class AmmoSpawner : public GameEntity
{
protected:
	Physcis* mPhysics;
	vector<Ammo*> mVecAmmo;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	Ammo* Fire(eDir dir, eCollisionTag tag, float speed, POINTFLOAT pos);

	void SetPhysics(Physcis* physics) { mPhysics = physics; }
};