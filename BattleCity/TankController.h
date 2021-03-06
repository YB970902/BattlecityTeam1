#pragma once
#include "GameEntity.h"

const int FIRST_PLAYER_KEY[5] = { 'A', 'D', 'W', 'S', VK_SPACE };
const int SECOND_PLAYER_KEY[5] = { VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_RETURN };

class Tank;
class GameManager;
class TankController : public GameEntity
{
private:
	GameManager* mGameManager = nullptr;
	Tank* mTank = nullptr;

	int mLeftKey = 0;
	int mRightKey = 0;
	int mUpKey = 0;
	int mDownKey = 0;
	int mFireKey = 0;
public:
	virtual ~TankController() {}
	HRESULT Init(int leftKey, int rightKey, int upKey, int downKey, int fireKey);
	HRESULT Init(const int arrKey[5]);
	void Release();
	void Update();
	void Render(HDC hdc);

	void AddStar();
	void TurnToInvencible();

	inline void SetTank(Tank* tank) { mTank = tank; }
	inline void SetGameManager(GameManager* gameManager) { mGameManager = gameManager; }
};

