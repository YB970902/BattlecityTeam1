#include "Config.h"
#include "Physcis.h"
#include "Collider.h"

HRESULT Physcis::Init()
{
	return S_OK;
}

Collider* Physcis::CreateCollider(POINTFLOAT pos, float bodySize, CollisionChecker* obj, eCollisionTag tag)
{
	Collider* newCol = new Collider();
	newCol->Init(pos, bodySize, this, obj, tag);

	POINT prevPoint = newCol->GetPointGrid()[0];
	if (prevPoint.x == -1 || prevPoint.y == -1) { return nullptr; } // 생성실패!

	mGridMap[prevPoint.x][prevPoint.y].push_back(newCol);

	for (int i = 1; i < 4; i++)
	{
		if (newCol->GetPointGrid()[i].x == -1 || newCol->GetPointGrid()[i].y == -1)
		{
			break;
		}

		if (newCol->GetPointGrid()[i].x != prevPoint.x || newCol->GetPointGrid()[i].y != prevPoint.y)
		{
			prevPoint = newCol->GetPointGrid()[i];
			mGridMap[prevPoint.x][prevPoint.y].push_back(newCol);
		}
	}
	return newCol;
}

void Physcis::DestroyCollider(Collider* col)
{
	for (int i = 0; i < 4; i++)
	{
		vector<Collider*>::iterator it = find(mGridMap[col->GetPointGrid()[i].x][col->GetPointGrid()[i].y].begin(),
			mGridMap[col->GetPointGrid()[i].x][col->GetPointGrid()[i].y].end(),
			col);
		if (it != mGridMap[col->GetPointGrid()[i].x][col->GetPointGrid()[i].y].end())
		{
			mGridMap[col->GetPointGrid()[i].x][col->GetPointGrid()[i].y].erase(it);
		}
	}
	SAFE_DELETE(col);
}

void Physcis::CheckCollider(Collider* col, POINTFLOAT dir, POINTFLOAT oldPos)
{
	POINTFLOAT addedForce = { 0,0 };
	POINTFLOAT oldOverlapped = { 0,0 };

	POINT arrOldVertex[4];
	for (int i = 0; i < 4; i++)
	{
		arrOldVertex[i] = col->GetPointGrid()[i];
	}
	//cout << "--------------------------" << endl;
	//for (int i = 0; i < 4; i++)
	//{
	//	cout << i << "count X : " << arrOldVertex[i].x << "  Y : " << arrOldVertex[i].y << endl;
	//}			// 소속그리드 확인구문

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < mGridMap[mCheckGrid[i].x][mCheckGrid[i].y].size(); j++)
		{
			if (mGridMap[mCheckGrid[i].x][mCheckGrid[i].y][j] == col) { continue; }
			PreventOverlapped(col, mGridMap[mCheckGrid[i].x][mCheckGrid[i].y][j], addedForce, dir, oldOverlapped);
		}
	}
	if ((dir.x > 0 && addedForce.x < 0) || (dir.x < 0 && addedForce.x > 0))
	{
		addedForce.y = 0;
	}
	else if ((dir.y > 0 && addedForce.y < 0) || (dir.y < 0 && addedForce.y > 0))
	{
		addedForce.x = 0;
	}

	col->AddPlayerPos(addedForce);

	if (IsCollided(col))
	{
		if ((dir.x > 0 && oldOverlapped.x < 0) || (dir.x < 0 && oldOverlapped.x > 0))
		{
			oldPos.x = col->GetPlayerPos().x + oldOverlapped.x;
		}
		else if ((dir.y > 0 && oldOverlapped.y < 0) || (dir.y < 0 && oldOverlapped.y > 0))
		{
			oldPos.y = col->GetPlayerPos().y + oldOverlapped.y;
		}
		col->SetPlayerPos({ oldPos.x,oldPos.y });
	}

	col->UpdateBodySize();
	col->UpdatePointGrid();

	POINT arrDeleteVertex[4] = { {-1, -1}, {-1, -1} , {-1, -1} , {-1, -1} };
	POINT arrNewfaceVertex[4] = { {-1, -1}, {-1, -1} , {-1, -1} , {-1, -1} };
	int curDeleteVertexIdx = 0;
	int curNewfaceVeretxIdx = 0;
	bool isExist = false;
	bool isNewface = true;
	for (int i = 0; i < 4; i++)
	{
		isExist = false;
		isNewface = true;
		for (int j = 0; j < 4; j++)
		{
			if (arrOldVertex[i].x == col->GetPointGrid()[j].x && arrOldVertex[i].y == col->GetPointGrid()[j].y)
			{
				isExist = true;
			}

			if (arrOldVertex[j].x == col->GetPointGrid()[i].x && arrOldVertex[j].y == col->GetPointGrid()[i].y)
			{
				isNewface = false;
			}
		}
		if (!isExist)
		{
			arrDeleteVertex[curDeleteVertexIdx++] = arrOldVertex[i];
		}
		if (isNewface)
		{
			arrNewfaceVertex[curNewfaceVeretxIdx++] = col->GetPointGrid()[i];
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (arrDeleteVertex[i].x == -1 || arrDeleteVertex[i].y == -1) { continue; }
		vector<Collider*>::iterator it = find(mGridMap[arrDeleteVertex[i].x][arrDeleteVertex[i].y].begin(),
			mGridMap[arrDeleteVertex[i].x][arrDeleteVertex[i].y].end(),
			col);
		if (it != mGridMap[arrDeleteVertex[i].x][arrDeleteVertex[i].y].end())
		{
			mGridMap[arrDeleteVertex[i].x][arrDeleteVertex[i].y].erase(it);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (arrNewfaceVertex[i].x == -1 || arrNewfaceVertex[i].y == -1) { continue; }
		vector<Collider*>::iterator it = find(mGridMap[arrNewfaceVertex[i].x][arrNewfaceVertex[i].y].begin(),
			mGridMap[arrNewfaceVertex[i].x][arrNewfaceVertex[i].y].end(),
			col);
		if (it == mGridMap[arrNewfaceVertex[i].x][arrNewfaceVertex[i].y].end())
		{
			mGridMap[arrNewfaceVertex[i].x][arrNewfaceVertex[i].y].push_back(col);
		}
	}

	mCheckGrid[0] = col->GetPointGrid()[0];
	mCheckGrid[1] = col->GetPointGrid()[1];
	mCheckGrid[2] = col->GetPointGrid()[2];
	mCheckGrid[3] = col->GetPointGrid()[3];
}

bool Physcis::IsCollided(Collider* col1, Collider* col2)
{
	if ((int)col1->GetPlayerBody().left >= (int)col2->GetPlayerBody().right) { return false; }
	if ((int)col1->GetPlayerBody().right <= (int)col2->GetPlayerBody().left) { return false; }
	if ((int)col1->GetPlayerBody().top >= (int)col2->GetPlayerBody().bottom) { return false; }
	if ((int)col1->GetPlayerBody().bottom <= (int)col2->GetPlayerBody().top) { return false; }

	return true;
}

bool Physcis::IsCollided(Collider* col)
{
	if (((((int)col->GetTag()) & 4) == 4))		//탱크일경우에만
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < mGridMap[mCheckGrid[i].x][mCheckGrid[i].y].size(); j++)
			{
				if (mGridMap[mCheckGrid[i].x][mCheckGrid[i].y][j] == col) { continue; }
				if (IsCollided(col, mGridMap[mCheckGrid[i].x][mCheckGrid[i].y][j])) { return true; }
			}
		}
	}
	return false;
}

void Physcis::PreventOverlapped(Collider* col1, Collider* col2, POINTFLOAT& addedForce, POINTFLOAT dir, POINTFLOAT& oldOverlapped)
{
	
	// 아래의 경우 서로 밀어내면 안되기때문
	if ((((int)col1->GetTag() | (int)col2->GetTag()) & 34) == 34) { return; } // 물 (32) + 아모 (2) 여부 확인
	if ((((int)col1->GetTag() & (int)col2->GetTag()) & 2) == 2)
	{
		if (((int)col1->GetTag() & 1) == ((int)col2->GetTag() & 1))
		{
			return;
		}
	}
	if (IsCollided(col1, col2))
	{
		float overlappedX = 0, overlappedY = 0;

		// col1이 왼쪽에 있는경우
		if (col1->GetPlayerPos().x < col2->GetPlayerPos().x)
		{
			overlappedX = col2->GetPlayerBody().left - col1->GetPlayerBody().right;
		}
		else
		{
			overlappedX = col2->GetPlayerBody().right - col1->GetPlayerBody().left;
		}

		// col1이 위에 있는경우
		if (col1->GetPlayerPos().y < col2->GetPlayerPos().y)
		{
			overlappedY = col2->GetPlayerBody().top - col1->GetPlayerBody().bottom;
		}
		else
		{
			overlappedY = col2->GetPlayerBody().bottom - col1->GetPlayerBody().top;
		}
		oldOverlapped = { overlappedX,overlappedY };

		if (fabs(overlappedX) < fabs(overlappedY))
		{
			// 좌우이동
			if (overlappedX < 0)
			{
				// 쪽에서 박음
				col1->OnCollided(eCollisionDir::Right, col2->GetTag());
			}
			else
			{
				// 오른쪽에서 박음
				col1->OnCollided(eCollisionDir::Left, col2->GetTag());
			}
		}
		else
		{
			// 상하이동
			if (overlappedY < 0)
			{
				// 위쪽에서 박음
				col1->OnCollided(eCollisionDir::Bottom, col2->GetTag());
			}
			else
			{
				// 아래쪽에서 박음
				col1->OnCollided(eCollisionDir::Top, col2->GetTag());
			}
		}
		// 겹친 영역이 세로로 길쭉한 형태이고 보정하기에 딱 좋은 상태
		//바디 사이즈의 3분의 1 보다 작게 겹친경우 보는 방향에 따라 Y위치 보정을 해 준다.
		if (fabs(overlappedX) < fabs(overlappedY) &&
			fabs(overlappedY) < (col2->GetPlayerBodySize() * 0.5f) &&
			((overlappedX > 0 && dir.x < 0) || (overlappedX < 0 && dir.x > 0)))
		{
			if (addedForce.y < fabs(overlappedY))
			{
				addedForce.y = overlappedY;
			}
		}
		// 겹친 영역이 가로로 길쭉한 형태이고 보정하기에 딱 좋은 상태
		// 바디 사이즈의 3분의 1보자 작게 겹친경우 보는 방향에 따라 X위치 보정을 해 준다.
		else if (fabs(overlappedX) > fabs(overlappedY) &&
			fabs(overlappedX) < (col2->GetPlayerBodySize() * 0.5f) &&
			((overlappedY < 0 && dir.y > 0) || (overlappedY > 0 && dir.y < 0)))
		{
			if (addedForce.x < fabs(overlappedX))
			{
				addedForce.x = overlappedX;
			}
		}
		// 좌우이동
		else if (fabs(overlappedX) < fabs(overlappedY) && addedForce.x < fabs(overlappedX))
		{
			addedForce.x = overlappedX;
		}
		// 상하이동
		else if (fabs(overlappedX) > fabs(overlappedY) && addedForce.y < fabs(overlappedY))
		{
			addedForce.y = overlappedY;
		}
	}
}

void Physcis::Render(HDC hdc)
{
	// rendering near collider
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < mGridMap[mCheckGrid[i].x][mCheckGrid[i].y].size(); j++)
	//	{
	//		mGridMap[mCheckGrid[i].x][mCheckGrid[i].y][j]->Render(hdc);
	//	}
	//}

	// rendering all
	for (map<int, map<int, vector<Collider*>>>::iterator itX = mGridMap.begin(); itX != mGridMap.end(); itX++)
	{
		for (map<int, vector<Collider*>>::iterator itY = (*itX).second.begin(); itY != (*itX).second.end(); itY++)
		{
			for (vector<Collider*>::iterator vecIter = (*itY).second.begin(); vecIter != (*itY).second.end(); vecIter++)
			{
				(*vecIter)->Render(hdc);
			}
		}
	}
}

void Physcis::Release()
{
	for (map<int, map<int, vector<Collider*>>>::iterator itX = mGridMap.begin(); itX != mGridMap.end(); itX++)
	{
		for (map<int, vector<Collider*>>::iterator itY = (*itX).second.begin(); itY != (*itX).second.end(); itY++)
		{
			for (vector<Collider*>::iterator vecIter = (*itY).second.begin(); vecIter != (*itY).second.end();)
			{
				Collider* temp = (*vecIter);
				vecIter++;
				SAFE_DELETE(temp);
			}
		}
	}
}
