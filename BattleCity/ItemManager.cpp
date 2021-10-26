#include "ItemManager.h"
#include "Item.h"
#include "Physcis.h"
#include "TileManager.h"
#include "Subject.h"

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
			// TankSpawner�� �˾ƾ� ��
			cout << "��ũ ����" << endl;
			break;
		case eItemTag::ProtectedWall:
			mTileManager->ProtectNexus();
			cout << "�ؼ��� ��ȣ" << endl;
			break;
		case eItemTag::TankStar:
			// TankSpawner�� �˾ƾ� ��
			cout << "��ũ �� ȹ��" << endl;
			break;
		case eItemTag::DestroyAllEnemy:
			// AISpawner�� �˾ƾ� ��
			cout << "��� �� �ı�" << endl;
			break;
		case eItemTag::PauseAllEnemy:
			// AISpawner�� �˾ƾ� ��
			cout << "��� �� �Ͻ�����" << endl;
			break;
		case eItemTag::AddPlayerLife:
			// TankSpawner�� �˾ƾ� ��
			cout << "�÷��̾� ��� �߰�" << endl;
			break;
		}
	}
}
