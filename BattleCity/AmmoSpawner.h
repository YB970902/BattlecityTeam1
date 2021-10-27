#pragma once
#include "GameEntity.h"

class Ammo;
class Physcis;
class GameManager;
class AmmoSpawner : public GameEntity
{
protected:
	GameManager* mGameManager = nullptr;
	vector<Ammo*> mVecAmmo;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	Ammo* Fire(eDir dir, eCollisionTag tag, float speed, POINTFLOAT pos);

	void SetGameManager(GameManager* gameManager) { mGameManager = gameManager; }
};