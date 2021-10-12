#pragma once
#include "Config.h"
#include <math.h>

inline float GetDistance(const POINTFLOAT& pos1, const POINTFLOAT& pos2)
{
	return sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.y - pos2.y) * (pos1.y - pos2.y));
}

inline void SetWindowSize(int startX, int startY, int sizeX, int sizeY)
{
	RECT rc;
	rc.left = 0; rc.top = 0;
	rc.right = sizeX;	rc.bottom = sizeY;

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	MoveWindow(g_hWnd, startX, startY,
		rc.right - rc.left, rc.bottom - rc.top, true);
}