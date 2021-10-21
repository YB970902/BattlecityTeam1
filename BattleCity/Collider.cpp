#include "Config.h"
#include "Collider.h"
#include "Physcis.h"
#include "CollisionChecker.h"

HRESULT Collider::Init(POINTFLOAT pos, float bodySize, Physcis* physcis, CollisionChecker* obj, eCollisionTag tag)
{
	mPhyscis = physcis;
	mPlayerBodySize = bodySize;

	mPlayerPos = pos;
	mTag = tag;
	pObj = obj;
	pFunc = &CollisionChecker::OnCollided;


	UpdateBodySize();
	UpdatePointGrid();

	return S_OK;
}

void Collider::UpdateBodySize()
{
	mPlayerBody.left = mPlayerPos.x - (mPlayerBodySize / 2);
	mPlayerBody.top = mPlayerPos.y - (mPlayerBodySize / 2);
	mPlayerBody.right = mPlayerBody.left + mPlayerBodySize;
	mPlayerBody.bottom = mPlayerBody.top + mPlayerBodySize;

}

void Collider::Render(HDC hdc)
{
	Rectangle(hdc, mPlayerBody.left, mPlayerBody.top, mPlayerBody.right, mPlayerBody.bottom);
}

void Collider::UpdatePointGrid()
{
	mPointGrid[0] = { mPlayerBody.left / 64, mPlayerBody.top / 64 };
	mPointGrid[1] = { mPlayerBody.right / 64, mPlayerBody.top / 64 };
	mPointGrid[2] = { mPlayerBody.left / 64, mPlayerBody.bottom / 64 };
	mPointGrid[3] = { mPlayerBody.right / 64, mPlayerBody.bottom / 64 };
}

void Collider::SetPlayerPos(POINTFLOAT movePos)
{
	this->mPlayerPos.x = movePos.x;
	this->mPlayerPos.y = movePos.y;
	UpdateBodySize();
}

void Collider::AddPlayerPos(POINTFLOAT addPos)
{
	this->mPlayerPos.x += addPos.x;
	this->mPlayerPos.y += addPos.y;
	UpdateBodySize();
}

void Collider::MoveTo(POINTFLOAT dir, float moveSpeed)
{
	POINTFLOAT oldPos = this->mPlayerPos;
	this->mPlayerPos.x += dir.x * moveSpeed;
	this->mPlayerPos.y += dir.y * moveSpeed;
	UpdateBodySize();

	mPhyscis->CheckCollider(this, dir, oldPos);
}

void Collider::OnCollided(eCollisionDir dir, eCollisionTag tag)
{
	if (pObj != nullptr) { (pObj->*pFunc)(dir, tag); }
}