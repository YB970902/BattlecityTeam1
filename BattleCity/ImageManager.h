#pragma once
#include "Singleton.h"

enum class eImageTag
{
	None,
	BackGround,
	Tile,
	GrayBG,
	BlackBG,
	TileButton,
	EnemyOrderBox,
	BattleSceneBlackBG,
	BattleSceneGrayBG,
	Tank,
	AmmoUp,
	AmmoDown,
	AmmoLeft,
	AmmoRight,
	EffectSmallBoom,
	EffectBigBoom,
	EffectShield,
	EffectSpawn,
	UIScore100,
	UIScore200,
	UIScore300,
	UIScore400,
	UIScore500,
	UIGameOver,
	ItemInvencible,
	ItemProtect,
	ItemStar,
	ItemDetroyAll,
	ItemPauseAll,
	ItemLife,
	TitleScene,
	TitleSceneCursor,
	SlateSceneStage,
	WhiteNumber,
	OrangeNumber,

	LeftArrow,
	RightArrow,
	PointsText,
	PlayerText,
	PlayerText2,
	StageText,
	TotalText,
	BonusText


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

