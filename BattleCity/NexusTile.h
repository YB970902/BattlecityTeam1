#pragma once
#include "Tile.h"

class Subject;
class NexusTile : public Tile
{
protected:
	Subject* mSubject = nullptr;

public:
	virtual HRESULT Init(TagTile info, Collider* collider, POINT pos) override;
	virtual void Release();

	Subject* GetSubject() { return mSubject; }

	virtual void OnCollided(eCollisionDir dir, int tag) override;
};