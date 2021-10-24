#include "Config.h"
#include "TankController.h"
#include "Tank.h"
#include "AmmoSpawner.h"

HRESULT TankController::Init(int leftKey, int rightKey, int upKey, int downKey, int fireKey)
{
    mLeftKey = leftKey;
    mRightKey = rightKey;
    mUpKey = upKey;
    mDownKey = downKey;
    mFireKey = fireKey;

    return S_OK;
}

void TankController::Release()
{
}

void TankController::Update()
{
    SAFE_UPDATE(mTank);
    if (KEY_MGR->IsStayKeyDown(mLeftKey)) { mTank->Move(eDir::Left); }
    else if (KEY_MGR->IsStayKeyDown(mRightKey)) { mTank->Move(eDir::Right); }
    else if (KEY_MGR->IsStayKeyDown(mUpKey)) { mTank->Move(eDir::Up); }
    else if (KEY_MGR->IsStayKeyDown(mDownKey)) { mTank->Move(eDir::Down); }
    if (KEY_MGR->IsOnceKeyDown(mFireKey) && mTank->IsCanFire())
    {
        mTank->AddAmmo(mAmmoSpawner->Fire(mTank->GetDirection(),
            mTank->GetCollisionTag() == eCollisionTag::EnemyTank ? eCollisionTag::EnemyAmmo : eCollisionTag::PlayerAmmo,
            mTank->GetInfo().MoveSpeed));
    }
}

void TankController::Render(HDC hdc)
{
}
