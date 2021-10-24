#include "Config.h"
#include "GameObject.h"
#include "AmmoSpawner.h"
#include "Ammo.h"

HRESULT AmmoSpawner::Init()
{
    mVecAmmo.resize(100);
    for (int i = 0; i < mVecAmmo.size(); ++i)
    {
        mVecAmmo[i] = new Ammo();
        mVecAmmo[i]->SetIsFire(false);
    }
    return S_OK;
}

void AmmoSpawner::Release()
{
    Ammo* ammo;
    for (vector<Ammo*>::iterator it = mVecAmmo.begin(); it != mVecAmmo.end();)
    {
        ammo = (*it);
        it = mVecAmmo.erase(it);
        SAFE_RELEASE(ammo);
    }
}

void AmmoSpawner::Update()
{
    for (int i = 0; i < mVecAmmo.size(); ++i)
    {
        mVecAmmo[i]->Update();
    }
}

void AmmoSpawner::Render(HDC hdc)
{
    for (int i = 0; i < mVecAmmo.size(); ++i)
    {
        mVecAmmo[i]->Render(hdc);
    }
}

Ammo* AmmoSpawner::Fire(eDir dir, eCollisionTag tag, float speed)
{
    for (int i = 0; i < mVecAmmo.size(); ++i)
    {
        if (mVecAmmo[i]->IsFire()) continue;
        mVecAmmo[i]->Init(dir, tag, speed);
        return mVecAmmo[i];
    }

    Ammo* newAmmo = new Ammo;
    newAmmo->Init(dir, tag, speed);
    mVecAmmo.push_back(newAmmo);
    return newAmmo;
}