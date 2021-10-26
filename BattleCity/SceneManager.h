#pragma once
#include "Singleton.h"

enum class eSceneTag
{
	None,
	TestScene,
	PhysicsScene,
	TankScene,
	MapToolScene,
	BattleScene,
};


class GameEntity;
class MapEditorScene;
class SceneManager : public Singleton<SceneManager>
{
private:
	map<eSceneTag, GameEntity*>	mapScenes;
	map<eSceneTag, GameEntity*>	mapLoadingScenes;
	map<string, int> mapSceneData;

public:
	static GameEntity* currScene;		// ���� ��� ���� ��
	static GameEntity* readyScene;		// �غ� ���� ��
	static GameEntity* loadingScene;	// �ε� ��



	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void AddScene(eSceneTag tag, GameEntity* scene);
	void AddLoadingScene(eSceneTag tag, GameEntity* scene);

	void SetSceneData(string key, int val);
	int GetSceneData(string key);

	HRESULT ChangeScene(eSceneTag tag);
	HRESULT ChangeScene(eSceneTag tag, eSceneTag loadingSceneName);
};

