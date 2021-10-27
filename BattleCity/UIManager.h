#pragma once
#include "Singleton.h"

class FixedUI;
class UIManager : public Singleton<UIManager>
{
protected:
	vector<FixedUI*> mVecUI;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void AddFixedUI(eImageTag imageTag, POINTFLOAT pos);
	void AddStayUI(eImageTag imageTag, POINTFLOAT pos, float durationTime);
	void AddMovingUI(eImageTag imageTag, POINTFLOAT startPos, POINTFLOAT destPos, float durationTime, float waitingTime);
};