#include "FixedUI.h"
#include "Image.h"

HRESULT FixedUI::Init(Image* image, POINTFLOAT pos)
{
	mImage = image;
	mPos = pos;
	return S_OK;
}

void FixedUI::Release()
{
}

void FixedUI::Update()
{
}

void FixedUI::Render(HDC hdc)
{
}
