#include "Config.h"
#include "ImageManager.h"
#include "Image.h"

void ImageManager::Init()
{
	AddImage(eImageTag::BackGround, "Image/background.bmp", WIN_SIZE_X, WIN_SIZE_Y);

	AddImage(eImageTag::Tank, "Image/tank.bmp", 512, 512, 16, 16, true, RGB(255, 0, 255));
	AddImage(eImageTag::AmmoUp, "Image/Bullet/Missile_Up.bmp", 6, 8, true, RGB(255, 0, 255));
	AddImage(eImageTag::AmmoDown, "Image/Bullet/Missile_Down.bmp", 6, 8, true, RGB(255, 0, 255));
	AddImage(eImageTag::AmmoLeft, "Image/Bullet/Missile_Left.bmp", 8, 6, true, RGB(255, 0, 255));
	AddImage(eImageTag::AmmoRight, "Image/Bullet/Missile_Right.bmp", 8, 6, true, RGB(255, 0, 255));
	AddImage(eImageTag::BattleSceneGrayBG, "Image/background.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	AddImage(eImageTag::BattleSceneBlackBG, "Image/background2.bmp", 16 * 26, 16 * 26);

	AddImage(eImageTag::EffectSmallBoom, "Image/Effect/Boom_Effect.bmp", 96, 32, 3, 1, true, RGB(255, 0, 255));
	AddImage(eImageTag::EffectBigBoom, "Image/Effect/Big_Boom_Effect.bmp", 128, 64, 2, 1, true, RGB(255, 0, 255));
	AddImage(eImageTag::EffectShield, "Image/Effect/Shield.bmp", 64, 32, 2, 1, true, RGB(255, 0, 255));
	AddImage(eImageTag::EffectSpawn, "Image/Effect/Spawn_Effect.bmp", 128, 32, 4, 1, true, RGB(255, 0, 255));
	//MapEditor
	IMG_MGR->AddImage(eImageTag::Tile, "Image/SampleTile1.bmp", 160, 96, 10, 6, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::GrayBG, "Image/background.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	IMG_MGR->AddImage(eImageTag::BlackBG, "Image/background2.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	IMG_MGR->AddImage(eImageTag::TileButton, "Image/button02.bmp", 60, 270, 2, 9, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::EnemyOrderBox, "Image/Enemy/Enemy.bmp", (32 * 8), (32 * 6), 8, 6, true, RGB(255, 0, 255));

	AddImage(eImageTag::UIScore100, "Image/Text/Score100.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage(eImageTag::UIScore200, "Image/Text/Score200.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage(eImageTag::UIScore300, "Image/Text/Score300.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage(eImageTag::UIScore400, "Image/Text/Score400.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage(eImageTag::UIScore500, "Image/Text/Score500.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage(eImageTag::UIGameOver, "Image/Text/Game_Over.bmp", 64, 30, true, RGB(255, 0, 255));

	AddImage(eImageTag::ItemInvencible, "Image/Item/Item1.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage(eImageTag::ItemProtect, "Image/Item/Item3.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage(eImageTag::ItemStar, "Image/Item/Item4.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage(eImageTag::ItemDetroyAll, "Image/Item/Item5.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage(eImageTag::ItemPauseAll, "Image/Item/Item2.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage(eImageTag::ItemLife, "Image/Item/Item6.bmp", 32, 32, true, RGB(255, 0, 255));
	//BattleScene
	IMG_MGR->AddImage(eImageTag::BattleSceneBlackBG, "Image/background2.bmp", 16 * 26, 16 * 26);

	//TitleScene
	IMG_MGR->AddImage(eImageTag::TitleScene, "Image/Title.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	IMG_MGR->AddImage(eImageTag::TitleSceneCursor, "Image/Player/Player.bmp", 512, 256, 8, 4, true, RGB(255,0,255));

	//SlateScene
	IMG_MGR->AddImage(eImageTag::SlateSceneStage, "Image/Text/Stage_W.bmp", 120, 24, 1, 1, true, RGB(255, 0, 255));
	IMG_MGR->AddImage(eImageTag::WhiteNumber, "Image/Text/Number_w.bmp", 120, 48, 5, 2, true, RGB(255, 0, 255));

	//Score
	IMG_MGR->AddImage(eImageTag::OrangeNumber, "Image/Text/ScoreNumber.bmp", 120, 48, 5, 2, true, RGB(255, 0, 255));
	AddImage(eImageTag::LeftArrow, "Image/Icon/Arrow_left.bmp", 23, 20, 1, 1, true, RGB(255, 0, 255));
	AddImage(eImageTag::RightArrow, "Image/Icon/Arrow_right.bmp", 23, 20, 1, 1, true, RGB(255, 0, 255));

	AddImage(eImageTag::PointsText, "Image/Text/PTS.bmp", 90, 40, 1, 1, true, RGB(255, 0, 255));
	AddImage(eImageTag::PlayerText, "Image/Text/Player1.bmp", 230, 40, 1, 1, true, RGB(255, 0, 255));
	AddImage(eImageTag::PlayerText2, "Image/Text/Player2.bmp", 212, 36, 1, 1, true, RGB(0, 0, 0));

	AddImage(eImageTag::StageText, "Image/Text/Stage_w.bmp", 111, 21, 1, 1, true, RGB(255, 0, 255));
	AddImage(eImageTag::TotalText, "Image/Text/TotalScore.bmp", 360, 40, 1, 1, true, RGB(255, 0, 255));
	//AddImage(eImageTag::BonusText, "Image/Text/Player1.bmp", 23, 20, 1, 1, true, RGB(255, 0, 255));
	AddImage(eImageTag::BonusText, "Image/Text/BONUS.bmp", 99, 17, 1, 1, true, RGB(255, 0, 255));





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
	// ???? ?????? ???????? nullptr?? ????
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
	// ???? ?????? ???????? nullptr?? ????
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
