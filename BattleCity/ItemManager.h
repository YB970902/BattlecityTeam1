#pragma once
#include "GameEntity.h"
#include "Observer.h"

struct ItemInfo
{
	ItemInfo() {}
	ItemInfo(eImageTag imageTag) :ImageTag(imageTag) {}
	eImageTag ImageTag = eImageTag::None;
};

class Item;
class Physcis;
class TileManager;
class ItemManager : public GameEntity, public Observer
{
private:
	Physcis* mPhysics = nullptr;
	TileManager* mTileManager = nullptr;

	vector<Item*> mVecItem;
	map<eItemTag, ItemInfo> mMapItemInfo;

public:
	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	void CreateItem(eItemTag itemTag);

	void SetPhysics(Physcis* physics) { mPhysics = physics; }
	void SetTileManager(TileManager* tileManager) { mTileManager = tileManager; };

	virtual ~ItemManager() {}

	virtual void OnNotify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag) override;
};