#include "Config.h"
#include "KeyManager.h"

void KeyManager::Init()
{
	for (int i = 0; i < KEY_MAX_COUNT; i++)
	{
		keyUp[i] = true;
		keyDown[i] = false;
	}
}

void KeyManager::Release()
{
}

void KeyManager::Update()
{
	for (int i = 0; i < KEY_MAX_COUNT; i++)
	{
		if (keyUp[i] && keyDown[i])
		{
			keyDown[i] = false;
		}
		else if (!keyUp[i] && !keyDown[i])
		{
			keyDown[i] = true;
		}
		else if (!keyUp[i] && keyDown[i] && !(GetAsyncKeyState(i) & 0x8000))
		{
			keyDown[i] = false;
			keyUp[i] = true;
		}
	}
}

bool KeyManager::IsOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (keyUp[key] == true)
		{
			keyUp[key] = false;
			return true;
		}
		else if (!keyDown[key] && !keyUp[key]) { return true; }
	}
	return false;
}

bool KeyManager::IsOnceKeyUp(int key)
{
	if ((GetAsyncKeyState(key) & 0x8000) == false)
	{
		if (keyUp[key] == false)
		{
			keyUp[key] = true;
			return true;
		}
		else if (keyUp[key] && keyDown[key]) { return true; }
	}
	return false;
}

bool KeyManager::IsStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8001) { return true; }

	return false;
}
