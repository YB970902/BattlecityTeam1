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
#define SAFE_UPDATE(p) { if (p) { p->Update(); } }
#define SAFE_RENDER(p) { if (p) { p->Render(hdc); } }

#include "TimerManager.h"
#include "ImageManager.h"
#include "KeyManager.h"
#include "SceneManager.h"
#include "ParticleManager.h"

#define TIMER_MGR TimerManager::GetSingleton()
#define KEY_MGR KeyManager::GetSingleton()
#define SCENE_MGR SceneManager::GetSingleton()
#define IMG_MGR ImageManager::GetSingleton()
#define PART_MGR ParticleManager::GetSingleton()

#define DELTA_TIME TIMER_MGR->GetDeltaTime()


#define TILE_COUNT_X 26
#define TILE_COUNT_Y 26

#define RANDOM(min, max) (rand() % ((max) - (min) + 1) + (min))

const float ITEM_BODY_SIZE = 32.0f;
const float TANK_SPAWNING_TIME = 0.5f;
const float PROTECT_ITEM_DURATION_TIME = 20.0f;
const float INVENCIBLE_ITEM_DURATION_TIME = 10.0f;

enum class eEventTag
{
    Released,
    Added,
    Collided,
};

enum class eSubjectTag
{
    Ammo,
    Tank,
    Particle,
    Item,
};

enum class eItemTag
{
    None,
    TankInvencible,
    ProtectedWall,
    TankStar,
    DestroyAllEnemy,
    PauseAllEnemy,
    AddPlayerLife,
};

enum class eDir
{
    Up = 0,
    Left = 1,
    Down = 2,
    Right = 3,
};

enum class eCollisionDir
{
    Top,
    Left,
    Bottom,
    Right,
};

// (0: 1P, 1: 2P)(통과물체여부)(아이템여부)(넥서스여부)(물여부)(특수여부)(블럭여부)(탱크여부)(아모여부)(1:플레이어, 2:적)
enum class eCollisionTag
{
    FirstPlayerTank        =0b0000000101,  //5
    SecondPlayerTank       =0b1000000101,  //517
    FirstPlayerAmmo        =0b0000000011,  //3
    SecondPlayerAmmo       =0b1000000011,  //515
    PlayerSpecialAmmo      =0b0000010011,  //19
    EnemyTank              =0b0000000100,  //4
    EnemyAmmo              =0b0000000010,  //2
    EnemySpecialAmmo       =0b0000010010,  //18
    Water                  =0b0000100000,  //32
    Block                  =0b0000001000,  //8
    SpecialBlock           =0b0000011000,  //24
    NexusBlock             =0b0001000000,  //64
    Item                   =0b0010000000,  //128
    PassedEnemyTank        =0b0100000000,  //256
};

#define IS_PLAYER_TANK(bit) ((bool)((bit & 0b101) == 0b101))

enum class eTerrain { None, Wall, Water, Grass, UnbreakableWall, Iron, Nexus, FlagNormal, FlagEnemy, FlagFirstPlayer, NexusAroundTile, FlagSecondPlayer };

struct TagTile
{
    RECT		TileShape;
    POINT		TilePos;
    eTerrain	Terrain;
    int			TileState;
    int			CollisionCount;
    bool        NexusAroundTile;

    TagTile()
    {
        TileShape = {};
        TilePos = {};
        Terrain = eTerrain::None;
        TileState = 0;
        CollisionCount = 0;
        NexusAroundTile = false;
    };
};

enum class eTankType
{
    None = 1,
    Player = 0,
    NormalEnemy = 4,
    QuickEnemy = 5,
    RapidFireEnemy = 6,
    DefenceEnemy = 7,
};

enum class eTankColor
{
    Yellow,
    White,
    Green,
    Red,
};

typedef struct TankInfo
{
    float MoveSpeed;
    float AttackSpeed;
    int Health;
    int MaxAmmoCount;
    float AmmoSpeed;
} TANK_INFO;

const TANK_INFO PLAYER_TANK_INFO{ 100.0f, 0.0f, 1, 1, 300.0f };
const TANK_INFO NORMAL_TANK_INFO{ 100.0f, 1.0f, 1, 1, 300.0f };
const TANK_INFO QUICK_TANK_INFO{ 200.0f, 1.0f, 1, 1, 300.0f };
const TANK_INFO RAPID_FIRE_TANK_INFO{ 100.0f, 0.5f, 1, 2, 300.0f };
const TANK_INFO DEFENCE_TANK_INFO{ 100.0f, 1.0f, 1, 1, 300.0f };

typedef struct TankSpawnInfo
{
    TankSpawnInfo() {}
    TankSpawnInfo(eCollisionTag collisionTag, eTankType type, eTankColor color, TANK_INFO tankInfo) :
        CollisionTag(collisionTag), Type(type), Color(color), TankInfo(tankInfo) { }

    eCollisionTag CollisionTag = eCollisionTag::Block;
    eTankType Type = eTankType::Player;
    eTankColor Color = eTankColor::Yellow;
    TANK_INFO TankInfo = NORMAL_TANK_INFO;
} SPAWN_INFO;

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;