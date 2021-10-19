#include "Config.h"
#include "Image.h"

HRESULT Image::Init(int width, int height)
{
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->width = width;
	imageInfo->height = height;
	imageInfo->loadType = ImageLoadType::Empty;
	imageInfo->hBitmap = CreateCompatibleBitmap(hdc, width, height);
	imageInfo->hMemDc = CreateCompatibleDC(hdc);

	imageInfo->hOldBit =
		(HBITMAP)SelectObject(imageInfo->hMemDc, imageInfo->hBitmap);

	ReleaseDC(g_hWnd, hdc);

	if (imageInfo->hBitmap == NULL)
	{
		Release();
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Image::Init(const char* fileName, int width, int height,
	bool isTrans, COLORREF transColor)
{
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->width = width;
	imageInfo->height = height;
	imageInfo->loadType = ImageLoadType::File;
	imageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height,
		LR_LOADFROMFILE);

	imageInfo->hMemDc = CreateCompatibleDC(hdc);
	imageInfo->hOldBit =
		(HBITMAP)SelectObject(imageInfo->hMemDc, imageInfo->hBitmap);

	ReleaseDC(g_hWnd, hdc);

	this->isTransparent = isTrans;
	this->transColor = transColor;

	if (imageInfo->hBitmap == NULL)
	{
		Release();
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Image::Init(const char* fileName, int width, int height, int maxFrameX, int maxFrameY,
	bool isTrans, COLORREF transColor)
{
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->width = width;
	imageInfo->height = height;
	imageInfo->loadType = ImageLoadType::File;
	imageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height,
		LR_LOADFROMFILE);
	imageInfo->hMemDc = CreateCompatibleDC(hdc);
	imageInfo->hOldBit =
		(HBITMAP)SelectObject(imageInfo->hMemDc, imageInfo->hBitmap);

	ReleaseDC(g_hWnd, hdc);

	this->isTransparent = isTrans;
	this->transColor = transColor;

	imageInfo->maxFrameX = maxFrameX;
	imageInfo->maxFrameY = maxFrameY;
	imageInfo->currFrameX = 0;
	imageInfo->currFrameY = 0;
	imageInfo->frameWidth = width / maxFrameX;
	imageInfo->frameHeight = height / maxFrameY;

	if (imageInfo->hBitmap == NULL)
	{
		Release();
		return E_FAIL;
	}

	return S_OK;
}

void Image::Release()
{
	if (imageInfo)
	{
		SelectObject(imageInfo->hMemDc, imageInfo->hOldBit);
		DeleteObject(imageInfo->hBitmap);
		DeleteDC(imageInfo->hMemDc);

		delete imageInfo;
		imageInfo = nullptr;
	}
}

void Image::Render(HDC hdc)
{
	BitBlt(hdc,				// 복사 목적지 DC
		0,					// 복사될 비트맵의 시작 위치 x
		0,					// 복사될 비트맵의 시작 위치 y
		imageInfo->width,	// 원본 복사할 가로 크기
		imageInfo->height,	// 원본 복사할 세로 크기
		imageInfo->hMemDc,	// 원본 DC
		0,					// 원본 비트맵 복사 시작 위치 x
		0,					// 원본 비트맵 복사 시작 위치 y
		SRCCOPY);			// 복사 옵션
}

void Image::Render(HDC hdc, int destX, int destY)
{
	if (isTransparent)
	{
		GdiTransparentBlt(
			hdc,
			destX - (imageInfo->width / 2),
			destY - (imageInfo->height / 2),
			imageInfo->width, imageInfo->height,

			imageInfo->hMemDc,
			0, 0,
			imageInfo->width, imageInfo->height,
			transColor
		);
	}
	else
	{
		BitBlt(hdc,
			destX - (imageInfo->width / 2),
			destY - (imageInfo->height / 2),
			imageInfo->width,
			imageInfo->height,
			imageInfo->hMemDc,
			0,
			0,
			SRCCOPY);
	}

}

void Image::Render(HDC hdc, int destX, int destY, int frameX, int frameY, float scale/* = 1.0f*/)
{
	if (isTransparent)
	{
		GdiTransparentBlt(
			hdc,
			destX - (imageInfo->frameWidth / 2),
			destY - (imageInfo->frameHeight / 2),
			imageInfo->frameWidth * scale,
			imageInfo->frameHeight * scale,	// 전체 프레임 수

			imageInfo->hMemDc,
			imageInfo->frameWidth * frameX,
			imageInfo->frameHeight * frameY ,
			imageInfo->frameWidth, imageInfo->frameHeight,
			transColor
		);
	}
	else
	{
		BitBlt(hdc,
			destX - (imageInfo->frameWidth / 2),
			destY - (imageInfo->frameHeight / 2),
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			imageInfo->hMemDc,
			imageInfo->frameWidth * frameX,
			imageInfo->frameHeight * frameY,
			SRCCOPY);
	}
}

void Image::Render(HDC hdc, int destX, int destY, int frameX, int frameY, float divide, float scale/* = 1.0f*/)
{
	if (isTransparent)
	{
		GdiTransparentBlt(
			hdc,
			destX - (imageInfo->frameWidth / 2),
			destY - (imageInfo->frameHeight / 2),
			imageInfo->frameWidth * scale,
			imageInfo->frameHeight * scale,	// 전체 프레임 수

			imageInfo->hMemDc,
			imageInfo->frameWidth * frameX,
			imageInfo->frameHeight * frameY,
			imageInfo->frameWidth, imageInfo->frameHeight,
			transColor
		);
	}
	else
	{
		BitBlt(hdc,
			destX - (imageInfo->frameWidth / 2),
			destY - (imageInfo->frameHeight / 2),
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			imageInfo->hMemDc,
			imageInfo->frameWidth * frameX,
			imageInfo->frameHeight * frameY,
			SRCCOPY);
	}
}
