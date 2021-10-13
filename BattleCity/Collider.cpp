#include "Config.h"
#include "Collider.h"

HRESULT Collider::Init(POINTFLOAT pos, float bodySize)
{
	mPlayerBodySize = bodySize;

	mPlayerPos = pos;
	UpdateBodySize();

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

void Collider::SetPlayerPos(POINTFLOAT movePos)
{
	this->mPlayerPos.x += movePos.x;
	this->mPlayerPos.y += movePos.y;
	UpdateBodySize();
}
