#include "Config.h"
#include "AmmoSpawner.h"
#include "Ammo.h"
#include "GameManager.h"

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
        if (mVecAmmo[i]->IsDead())
        {
            mGameManager->DestroyCollider(mVecAmmo[i]->GetCollider());
            mVecAmmo[i]->Release();
            mVecAmmo[i]->SetCollider(nullptr);
            mVecAmmo[i]->SetIsDead(false);
            mVecAmmo[i]->SetIsFire(false);
        }
    }
}

void AmmoSpawner::Render(HDC hdc)
{
    for (int i = 0; i < mVecAmmo.size(); ++i)
    {
        mVecAmmo[i]->Render(hdc);
    }
}

Ammo* AmmoSpawner::Fire(eDir dir, eCollisionTag tag, float speed, POINTFLOAT pos)
{
    for (int i = 0; i < mVecAmmo.size(); ++i)
    {
        if (mVecAmmo[i]->IsFire()) { continue; }
        mVecAmmo[i]->Init(dir, tag, speed);
        mVecAmmo[i]->SetPosition(pos);
        mVecAmmo[i]->SetCollider(mGameManager->CreateCollider(pos, AMMO_BODY_SIZE, mVecAmmo[i], tag));
        return mVecAmmo[i];
    }

    Ammo* newAmmo = new Ammo;
    newAmmo->Init(dir, tag, speed);
    newAmmo->SetPosition(pos);
    newAmmo->SetCollider(mGameManager->CreateCollider(pos, AMMO_BODY_SIZE, newAmmo, tag));
    mVecAmmo.push_back(newAmmo);
    return newAmmo;
}