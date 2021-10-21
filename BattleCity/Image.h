#pragma once
#include <WinUser.h>

class Image
{
public:
	enum ImageLoadType
	{
		Resource,
		File,
		Empty,
		End
	};

	typedef struct tagImageInfo
	{
		HDC hMemDc;		
		HBITMAP hBitmap;
		HBITMAP hOldBit;
		int width;		
		int height;		
		BYTE loadType;	

		int maxFrameX;
		int maxFrameY;
		int frameWidth;
		int frameHeight;
		int currFrameX;
		int currFrameY;

		tagImageInfo()
		{
			hMemDc = NULL;
			hBitmap = NULL;
			hOldBit = NULL;
			width = 0;
			height = 0;
			loadType = ImageLoadType::Empty;

			maxFrameX = 0;
			maxFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
			currFrameX = 0;
			currFrameY = 0;
		};
	} IMAGE_INFO, *LPIMAGE_INFO;

private:
	LPIMAGE_INFO imageInfo;
	bool isTransparent;
	COLORREF transColor;

public:
	HRESULT Init(int width, int height);
	HRESULT Init(const char* fileName, int width, int height,
		bool isTrans = false, COLORREF transColor = NULL);
	HRESULT Init(const char* fileName, int width, int height,
		int maxFrameX, int maxFrameY,
		bool isTrans = false, COLORREF transColor = NULL);

	void Release();

	void Render(HDC hdc);
	void Render(HDC hdc, int destX, int destY);
	void Render(HDC hdc, int destX, int destY, float scale);
	void Render(HDC hdc, int destX, int destY, int frameX, int frameY, float scale = 1.0f);
	void Render(HDC hdc, int destX, int destY, int frameX, int frameY, float divide, float scale = 1.0f);

	HDC GetMemDC() { if (imageInfo) return imageInfo->hMemDc; return NULL; }

	LPIMAGE_INFO GetImageInfo() { return imageInfo; }

	int GetCurrFrameX() { return imageInfo->currFrameX; }
	void SetCurrFrameX(int frame) { imageInfo->currFrameX = frame; }
	int GetCurrFrameY() { return imageInfo->currFrameY; }
	void SetCurrFrameY(int frame) { imageInfo->currFrameY = frame; }

	int GetMaxFrameX() { return imageInfo->maxFrameX; }
	int GetMaxFrameY() { return imageInfo->maxFrameY; }

	int GetFrameWidth() { return imageInfo->frameWidth; }
	int GetFrameHeight() { return imageInfo->frameHeight; }

	int GetWidth() { return imageInfo->width; }
	int GetHeight() { return imageInfo->height; }
};

