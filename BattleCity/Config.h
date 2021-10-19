#pragma once
#include <Windows.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <map>
#include <iostream>
#include <string>

using namespace std;

#pragma comment(lib, "winmm.lib")

#define WIN_START_POS_X	400
#define WIN_START_POS_Y	100
#define WIN_SIZE_X	1200
#define WIN_SIZE_Y	800

#define PI 3.14159265357989
#define PI2 (3.14159265357989 * 2)

#define DEGREE_TO_RADIAN(x)		(x * PI / 180.0f)

#define SAFE_RELEASE(p)	{ if (p) { p->Release(); delete p; p = nullptr; } }
#define SAFE_DELETE(p)	{ if (p) { delete p; p = nullptr; } }

#include "TimerManager.h"
#include "ImageManager.h"
#include "KeyManager.h"
#include "SceneManager.h"

#define TIMER_MGR TimerManager::GetSingleton()
#define KEY_MGR KeyManager::GetSingleton()
#define SCENE_MGR SceneManager::GetSingleton()
#define IMG_MGR ImageManager::GetSingleton()

#define DELTA_TIME TIME_MGR->GetDeltaTime()

#define RANDOM(min, max) (rand() % ((max) - (min) + 1) + (min))

#define TILE_COUNT_X 26
#define TILE_COUNT_Y 26

enum class eTerrain { None, Wall, Water, Grass, UnbreakableWall, Iron, Nexus, FlagNormal, FlagEnemy, FlagPlayer };

struct TagTile
{
	RECT		TileShape;
	POINT		TilePos;
	eTerrain	Terrain;
	int			TileState;
	int			CollisionCount = 0;
};

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;