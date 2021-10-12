#pragma once
#include "Singleton.h"

enum class eSceneTag
{
	None,
	TestScene,
	PhysicsScene,
	TankScene,
	MapToolScene,
};

class GameEntity;
class SceneManager : public Singleton<SceneManager>
{
private:
	map<eSceneTag, GameEntity*>	mapScenes;
	map<eSceneTag, GameEntity*>	mapLoadingScenes;

public:
	static GameEntity* currScene;		// 현재 출력 중인 씬
	static GameEntity* readyScene;		// 준비 중인 씬
	static GameEntity* loadingScene;	// 로딩 씬

	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void AddScene(eSceneTag tag, GameEntity* scene);
	void AddLoadingScene(eSceneTag tag, GameEntity* scene);

	HRESULT ChangeScene(eSceneTag tag);
	HRESULT ChangeScene(eSceneTag tag, eSceneTag loadingSceneName);
};

