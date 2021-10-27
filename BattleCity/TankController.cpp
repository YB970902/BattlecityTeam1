#include "Config.h"
#include "TankController.h"
#include "Tank.h"
#include "Ammo.h"
#include "GameManager.h"

HRESULT TankController::Init(int leftKey, int rightKey, int upKey, int downKey, int fireKey)
{
    mLeftKey = leftKey;
    mRightKey = rightKey;
    mUpKey = upKey;
    mDownKey = downKey;
    mFireKey = fireKey;

    return S_OK;
}

HRESULT TankController::Init(const int arrKey[5])
{
    return Init(arrKey[0], arrKey[1], arrKey[2], arrKey[3], arrKey[4]);
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
        if (mTank->GetStartCount() == 3)
        {
            mGameManager->Fire(mTank->GetDirection(),
                mTank->GetCollisionTag() == eCollisionTag::FirstPlayerTank ? eCollisionTag::FirstPlayerSpecialAmmo : eCollisionTag::SecondPlayerSpecialAmmo,
                mTank->GetInfo().AmmoSpeed, mTank->GetBarrelPosition())->AddObserver(mTank);
        }
        else
        {
            mGameManager->Fire(mTank->GetDirection(),
                mTank->GetCollisionTag() == eCollisionTag::FirstPlayerTank ? eCollisionTag::FirstPlayerAmmo : eCollisionTag::SecondPlayerAmmo,
                mTank->GetInfo().AmmoSpeed, mTank->GetBarrelPosition())->AddObserver(mTank);
        }
    }
}

void TankController::Render(HDC hdc)
{
}

void TankController::AddStar()
{
    if (mTank) { mTank->AddStar(); }
}

void TankController::TurnToInvencible()
{
    if (mTank) { mTank->ChangeToInvencible(); }
}
