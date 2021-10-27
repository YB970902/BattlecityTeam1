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
class GameManager;
class ItemManager : public GameEntity, public Observer
{
private:
	GameManager* mGameManager = nullptr;

	vector<Item*> mVecItem;
	map<eItemTag, ItemInfo> mMapItemInfo;

public:
	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	void CreateItem(eItemTag itemTag);

	void SetGameManager(GameManager* gameManager) { mGameManager = gameManager; }

	virtual ~ItemManager() {}

	virtual void OnNotify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag) override;
};