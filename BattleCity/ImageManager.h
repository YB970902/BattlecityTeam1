#pragma once
#include "Singleton.h"

enum class eImageTag
{
	None,
	BackGround,
	Tile,
	TileBackground,
	TileBackgroundInner,
	TileButton,
	EnemyOrderBox,
	BattleSceneGrayBG,
	BattleSceneBlackBG,
	Tank,
	AmmoUp,
	AmmoDown,
	AmmoLeft,
	AmmoRight,
	EffectSmallBoom,
	EffectBigBoom,
	EffectShield,
	EffectSpawn,
};

class Image;
class ImageManager : public Singleton<ImageManager>
{
private:
	map<eImageTag, Image*>	mapImages;

public:
	void Init();
	void Release();

	Image* AddImage(eImageTag tag, const char* fileName, int width, int height,
		bool isTrans = false, COLORREF transColor = NULL);
	
	Image* AddImage(eImageTag tag, const char* fileName, int width, int height,
		int maxFrameX, int maxFrameY,
		bool isTrans = false, COLORREF transColor = NULL);
	
	Image* FindImage(eImageTag tag);
	void DeleteImage(eImageTag tag);
};

