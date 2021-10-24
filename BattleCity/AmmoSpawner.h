#pragma once
#include "GameEntity.h"

class Ammo;
class AmmoSpawner : public GameEntity
{
protected:
	vector<Ammo*> mVecAmmo;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	Ammo* Fire(eDir dir, eCollisionTag tag, float speed);
};