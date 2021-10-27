#pragma once
#include "GameEntity.h"
#include "Observer.h"

class Image;
class Physcis;
class Collider;
class CollisionChecker;
class TileManager;
class AmmoSpawner;
class Ammo;
class TankController;
class TankSpawner;
class AITankSpawner;
class ItemManager;
class GameManager : public GameEntity, public Observer
{
protected:
	const float MAX_DURATION_CHANGE_SCENE_TIME = 5.0f;

	Image* mBackgroundGray = nullptr;
	Image* mBackgroundBlack = nullptr;

	Physcis* mPhysics = nullptr;
	TileManager* mTileManager = nullptr;
	AmmoSpawner* mAmmoSpawner = nullptr;
	
	TankController* mFirstPlayerController = nullptr;
	TankSpawner* mFirstPlayerSpawner = nullptr;

	TankController* mSecondPlayerController = nullptr;
	TankSpawner* mSecondPlayerSpawner = nullptr;

	AITankSpawner* mAISpawner = nullptr;

	ItemManager* mItemManager = nullptr;

	POINT mStartPos = {};

	bool mbIsDebugMode = false;

	bool mbIsSoloMode = true;
	bool mbIsFirstPlayerDead = false;
	bool mbIsSecondPlayerDead = false;
	bool mbIsGameEnd = false;
	float mElaspedChangeSceneTime = 0.0f;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	Collider* CreateCollider(POINTFLOAT pos, float bodySize, CollisionChecker* obj, eCollisionTag tag);
	void DestroyCollider(Collider* collider);
	Ammo* Fire(eDir dir, eCollisionTag tag, float speed, POINTFLOAT pos);
	void CreateItem(eItemTag itemTag);
	POINTFLOAT GetItemSpawnPosition();
	void FirstPlayerSetInvencible();
	void SecondPlayerSetInvencible();
	void FirstPlayerAddStar();
	void SecondPlayerAddStar();
	void FirstPlayerAddLife();
	void SecondPlayerAddLife();
	void ProtectNexus();
	void DestroyAllEnemy();
	void PauseAllEnemy();

	inline POINT GetStartPosition() { return mStartPos; }

	virtual void OnNotify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag) override;

protected:
	void SetGameOver();
	void GameOverLogoAtFirstPlayer();
	void GameOverLogoAtSecondPlayer();
};

