#include "Config.h"
#include "ImageManager.h"
#include "Image.h"

void ImageManager::Init()
{
	IMG_MGR->AddImage(eImageTag::BackGround, "Image/background.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	IMG_MGR->AddImage(eImageTag::Tile, "Image/SampleTile1.bmp", 160, 96, 10, 6, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::TileBackground, "Image/background.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	IMG_MGR->AddImage(eImageTag::TileBackgroundInner, "Image/background2.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	IMG_MGR->AddImage(eImageTag::TileButton, "Image/button02.bmp", 60, 270, 2, 9, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::EnemyOrderBox, "Image/Enemy/Enemy.bmp", (32 * 8), (32 * 6), 8, 6, true, RGB(255, 0, 255));

	IMG_MGR->AddImage(eImageTag::Tank, "Image/tank.bmp", 512, 512, 16, 16, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::AmmoUp, "Image/Bullet/Missile_Up.bmp", 6, 8, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::AmmoDown, "Image/Bullet/Missile_Down.bmp", 6, 8, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::AmmoLeft, "Image/Bullet/Missile_Left.bmp", 8, 6, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::AmmoRight, "Image/Bullet/Missile_Right.bmp", 8, 6, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::BattleSceneGrayBG, "Image/background.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	IMG_MGR->AddImage(eImageTag::BattleSceneBlackBG, "Image/background2.bmp", 16 * 26, 16 * 26);

	IMG_MGR->AddImage(eImageTag::EffectSmallBoom, "Image/Effect/Boom_Effect.bmp", 96, 32, 3, 1, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::EffectBigBoom, "Image/Effect/Big_Boom_Effect.bmp", 128, 64, 2, 1, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::EffectShield, "Image/Effect/Shield.bmp", 64, 32, 2, 1, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::EffectSpawn, "Image/Effect/Spawn_Effect.bmp", 128, 32, 4, 1, true, RGB(255, 0, 255));
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
