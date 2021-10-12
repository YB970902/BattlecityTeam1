#pragma once
#include <bitset>
#include "Singleton.h"

using namespace std;

#define KEY_MAX_COUNT	256

class KeyManager : public Singleton<KeyManager>
{
private:
	bitset<KEY_MAX_COUNT>	keyUp;
	bitset<KEY_MAX_COUNT>	keyDown;

public:
	void Init();
	void Release();
	void Update();

	bool IsOnceKeyDown(int key);
	bool IsOnceKeyUp(int key);	
	bool IsStayKeyDown(int key);
};

