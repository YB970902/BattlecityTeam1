#include "ItemManager.h"
#include "Item.h"
#include "Physcis.h"
#include "TileManager.h"
#include "Subject.h"
#include "TankController.h"
#include "AITankSpawner.h"
#include "TankSpawner.h"

HRESULT ItemManager::Init()
{
	mMapItemInfo[eItemTag::TankInvencible] = ItemInfo(eImageTag::ItemInvencible);
	mMapItemInfo[eItemTag::ProtectedWall] = ItemInfo(eImageTag::ItemProtect);
	mMapItemInfo[eItemTag::TankStar] = ItemInfo(eImageTag::ItemStar);
	mMapItemInfo[eItemTag::DestroyAllEnemy] = ItemInfo(eImageTag::ItemDetroyAll);
	mMapItemInfo[eItemTag::PauseAllEnemy] = ItemInfo(eImageTag::ItemPauseAll);
	mMapItemInfo[eItemTag::AddPlayerLife] = ItemInfo(eImageTag::ItemLife);

	return S_OK;
}

void ItemManager::Release()
{
	Item* item;
	for (vector<Item*>::iterator it = mVecItem.begin(); it != mVecItem.end();)
	{
		item = (*it);
		it = mVecItem.erase(it);
		SAFE_RELEASE(item);
	}
}

void ItemManager::Update()
{
	for (vector<Item*>::iterator it = mVecItem.begin(); it != mVecItem.end();)
	{
		(*it)->Update();
		if ((*it)->IsDead())
		{
			Item* item = (*it);
			it = mVecItem.erase(it);
			mPhysics->DestroyCollider(item->GetCollider());
			SAFE_RELEASE(item);
		}
		else { it++; }
	}
}

void ItemManager::Render(HDC hdc)
{
	for (int i = 0; i < mVecItem.size(); ++i)
	{
		mVecItem[i]->Render(hdc);
	}
}

void ItemManager::CreateItem(eItemTag itemTag)
{
	Item* newItem = new Item;
	POINTFLOAT pos = mTileManager->GetItemSpawnPosition();
	newItem->Init(mPhysics->CreateCollider(pos, ITEM_BODY_SIZE, newItem, eCollisionTag::Item),
		mMapItemInfo[itemTag].ImageTag, itemTag);
	newItem->GetSubject()->AddObserver(this);
	newItem->SetPosition(pos);
	mVecItem.push_back(newItem);
}

void ItemManager::OnNotify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag)
{
	if (subjectTag == eSubjectTag::Item && eventTag == eEventTag::Collided)
	{
		switch (dynamic_cast<Item*>(obj)->GetItemTag())
		{
		case eItemTag::TankInvencible:
			if (dynamic_cast<Item*>(obj)->GetCollidedTag() == eCollisionTag::FirstPlayerTank) { mFirstPlayerController->TurnToInvencible(); }
			else if (dynamic_cast<Item*>(obj)->GetCollidedTag() == eCollisionTag::SecondPlayerTank) { mSecondPlayerController->TurnToInvencible(); }
			break;
		case eItemTag::ProtectedWall:
			mTileManager->ProtectNexus();
			break;
		case eItemTag::TankStar:
			if (dynamic_cast<Item*>(obj)->GetCollidedTag() == eCollisionTag::FirstPlayerTank) { mFirstPlayerController->AddStar(); }
			else if (dynamic_cast<Item*>(obj)->GetCollidedTag() == eCollisionTag::SecondPlayerTank) { mSecondPlayerController->AddStar(); }
			break;
		case eItemTag::DestroyAllEnemy:
			mAISpawner->DestroyAll();
			break;
		case eItemTag::PauseAllEnemy:
			mAISpawner->PauseAll();
			break;
		case eItemTag::AddPlayerLife:
			if (dynamic_cast<Item*>(obj)->GetCollidedTag() == eCollisionTag::FirstPlayerTank) { mFirstPlayerSpawner->AddLife(); }
			else if (dynamic_cast<Item*>(obj)->GetCollidedTag() == eCollisionTag::SecondPlayerTank) { mSecondPlayerSpawner->AddLife(); }
			break;
		}
	}
}
