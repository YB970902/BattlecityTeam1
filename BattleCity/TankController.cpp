#include "Config.h"
#include "TankController.h"
#include "Tank.h"

HRESULT TankController::Init(Tank* tank, int leftKey, int rightKey, int upKey, int downKey)
{
    mTank = tank;
    mLeftKey = leftKey;
    mRightKey = rightKey;
    mUpKey = upKey;
    mDownKey = downKey;
    return S_OK;
}

void TankController::Release()
{
}

void TankController::Update()
{
    if (KEY_MGR->IsStayKeyDown(mLeftKey)) { mTank->Move(eDir::Left); }
    else if (KEY_MGR->IsStayKeyDown(mRightKey)) { mTank->Move(eDir::Right); }
    else if (KEY_MGR->IsStayKeyDown(mUpKey)) { mTank->Move(eDir::Up); }
    else if (KEY_MGR->IsStayKeyDown(mDownKey)) { mTank->Move(eDir::Down); }
}

void TankController::Render(HDC hdc)
{
}

void TankController::OnCollided(eCollisionDir dir, eCollisionTag tag)
{
    if (mTank) { mTank->OnCollided }
}
