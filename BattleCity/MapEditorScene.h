#pragma once
#include "GameEntity.h"

class MapEditor;
class MapEditorScene : public GameEntity
{
private:
	MapEditor* mapEditor;
public:
	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;
};

