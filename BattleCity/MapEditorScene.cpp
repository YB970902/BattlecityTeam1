#include "MapEditorScene.h"
#include "MapEditor.h"

HRESULT MapEditorScene::Init()
{
    mapEditor = new MapEditor;
    mapEditor->Init();
    return S_OK;
}

void MapEditorScene::Update()
{
    mapEditor->Update();
}

void MapEditorScene::Render(HDC hdc)
{
    mapEditor->Render(hdc);
}

void MapEditorScene::Release()
{
    SAFE_RELEASE(mapEditor);
}
