#include "Config.h"
#include "UIManager.h"
#include "FixedUI.h"
#include "MovingUI.h"

HRESULT UIManager::Init()
{
	return S_OK;
}

void UIManager::Release()
{
	FixedUI* ui;
	for (vector<FixedUI*>::iterator it = mVecUI.begin(); it != mVecUI.end();)
	{
		ui = (*it);
		it = mVecUI.erase(it);
		SAFE_RELEASE(ui);
	}
}

void UIManager::Update()
{
	for (vector<FixedUI*>::iterator it = mVecUI.begin(); it != mVecUI.end();)
	{
		(*it)->Update();
		if ((*it)->IsDead())
		{
			FixedUI* ui = (*it);
			it = mVecUI.erase(it);
			SAFE_RELEASE(ui);
		}
		else { ++it; }
	}
}

void UIManager::Render(HDC hdc)
{
	for (int i = 0; i < mVecUI.size(); ++i)
	{
		mVecUI[i]->Render(hdc);
	}
}

void UIManager::AddFixedUI(eImageTag imageTag, POINTFLOAT pos)
{
	FixedUI* newUI = new FixedUI;
	newUI->Init(IMG_MGR->FindImage(imageTag), pos);
	mVecUI.push_back(newUI);
}

void UIManager::AddStayUI(eImageTag imageTag, POINTFLOAT pos, float durationTime)
{
	MovingUI* newUI = new MovingUI;
	newUI->Init(IMG_MGR->FindImage(imageTag), pos, durationTime);
	mVecUI.push_back(newUI);
}

void UIManager::AddMovingUI(eImageTag imageTag, POINTFLOAT startPos, POINTFLOAT destPos, float durationTime, float waitingTime)
{
	MovingUI* newUI = new MovingUI;
	newUI->Init(IMG_MGR->FindImage(imageTag), startPos, destPos, durationTime, waitingTime);
	mVecUI.push_back(newUI);
}
