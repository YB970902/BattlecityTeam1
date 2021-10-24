#include "Config.h"
#include "ImageManager.h"
#include "Image.h"

void ImageManager::Init()
{
	IMG_MGR->AddImage(eImageTag::BackGround, "Image/background.bmp", WIN_SIZE_X, WIN_SIZE_Y);

	IMG_MGR->AddImage(eImageTag::Tank, "Image/tank.bmp", 512, 512, 16, 16, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::AmmoUp, "Image/Bullet/Missile_Up.bmp", 6, 8, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::AmmoDown, "Image/Bullet/Missile_Down.bmp", 6, 8, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::AmmoLeft, "Image/Bullet/Missile_Left.bmp", 8, 6, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::AmmoRight, "Image/Bullet/Missile_Right.bmp", 8, 6, true, RGB(255, 0, 255));
}

void ImageManager::Release()
{
	map<eImageTag, Image*>::iterator it;
	for (it = mapImages.begin(); it != mapImages.end();)
	{
		SAFE_RELEASE(it->second);
		it = mapImages.erase(it);
	}
	mapImages.clear();
}

Image* ImageManager::AddImage(eImageTag tag, const char* fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	// 이미 등록한 이미지는 nullptr을 반환
	if (mapImages.find(tag) != mapImages.end()) { return nullptr; }

	Image* img = new Image;
	if (FAILED(img->Init(fileName, width, height, isTrans, transColor)))
	{
		SAFE_RELEASE(img);
		return nullptr;
	}

	mapImages.insert(make_pair(tag, img));

	return img;
}

Image* ImageManager::AddImage(eImageTag tag, const char* fileName, int width, int height,
	int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
	// 이미 등록한 이미지는 nullptr을 반환
	if (mapImages.find(tag) != mapImages.end()) { return nullptr; }

	Image* img = new Image;
	if (FAILED(img->Init(fileName, width, height, maxFrameX, 
		maxFrameY, isTrans, transColor)))
	{
		SAFE_RELEASE(img);
		return nullptr;
	}

	mapImages.insert(make_pair(tag, img));

	return img;
}

Image* ImageManager::FindImage(eImageTag tag)
{
	map<eImageTag, Image*>::iterator it = mapImages.find(tag);
	if (it == mapImages.end())
	{
		return nullptr;
	}

	return it->second;
}

void ImageManager::DeleteImage(eImageTag tag)
{
	map<eImageTag, Image*>::iterator it = mapImages.find(tag);
	if (it == mapImages.end())
	{
		return;
	}

	SAFE_RELEASE(it->second);
	mapImages.erase(it);
}
