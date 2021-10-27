#include "NexusTile.h"
#include "Subject.h"

HRESULT NexusTile::Init(TagTile info, Collider* collider, POINT pos)
{
	mSubject = new Subject;
	return Tile::Init(info, collider, pos);
}

void NexusTile::Release()
{
	SAFE_DELETE(mSubject);
	Tile::Release();
}

void NexusTile::OnCollided(eCollisionDir dir, int tag)
{
	// 총알과 충돌하면
	if ((tag & 0b10) == 0b10) { mSubject->Notify(this, eSubjectTag::Nexus, eEventTag::Collided); }
}
