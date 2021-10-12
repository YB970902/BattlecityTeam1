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
			if (GetAsyncKeyState(i) & 0x8000) { keyUp[i] = false; }
			else { keyDown[i] = false; }
		}
	}
}

bool KeyManager::IsOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (keyDown[key] == false)
		{
			keyDown[key] = true;
			return true;
		}
		else if (keyDown[key] && keyUp[key]) { return true; }
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
