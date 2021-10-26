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
	mVecCollider.push_back(newCol);

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
	if (col == NULL)
	{
		cout << "왜 널?" << endl;
		return;
	}
	vector<Collider*>::iterator it;
	for (int i = 0; i < 4; i++)
	{
		it = find(mGridMap[col->GetPointGrid()[i].x][col->GetPointGrid()[i].y].begin(),
			mGridMap[col->GetPointGrid()[i].x][col->GetPointGrid()[i].y].end(),
			col);
		if (it != mGridMap[col->GetPointGrid()[i].x][col->GetPointGrid()[i].y].end())
		{
			mGridMap[col->GetPointGrid()[i].x][col->GetPointGrid()[i].y].erase(it);
		}
	}

	it = find(mVecCollider.begin(), mVecCollider.end(), col);
	mVecCollider.erase(it);

	SAFE_DELETE(col);
}

void Physcis::CheckCollider(Collider* col, POINTFLOAT dir, POINTFLOAT oldPos)
{
	POINTFLOAT addedForce = { 0,0 };
	POINTFLOAT oldOverlapped = { 0,0 };

	mCheckGrid[0] = col->GetPointGrid()[0];
	mCheckGrid[1] = col->GetPointGrid()[1];
	mCheckGrid[2] = col->GetPointGrid()[2];
	mCheckGrid[3] = col->GetPointGrid()[3];

	POINT arrOldVertex[4];
	for (int i = 0; i < 4; i++)
	{
		arrOldVertex[i] = col->GetPointGrid()[i];
	}

	int collidedTag = 0;
	eCollisionTag tag = col->GetTag();
	map<Collider*, bool> mapDetected;
	int curCollidedTag = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < mGridMap[mCheckGrid[i].x][mCheckGrid[i].y].size(); j++)
		{
			if (mGridMap[mCheckGrid[i].x][mCheckGrid[i].y][j] == col) { continue; }
			// 이미 검사한 애
			if (mapDetected.find(mGridMap[mCheckGrid[i].x][mCheckGrid[i].y][j]) != mapDetected.end()) { continue; }
			curCollidedTag = PreventOverlapped(col, mGridMap[mCheckGrid[i].x][mCheckGrid[i].y][j], addedForce, dir, oldOverlapped);
			if (curCollidedTag > 0)
			{
				mapDetected[mGridMap[mCheckGrid[i].x][mCheckGrid[i].y][j]] = true;
				collidedTag |= curCollidedTag;
			}
		}
	}

	mapDetected.clear();

	if (collidedTag > 1)
	{
		// 내가 가려는 방향의 반대방향의 힘이 작용한다면 보정을 막는다.
		if ((dir.x > 0 && addedForce.x < 0) || (dir.x < 0 && addedForce.x > 0))
		{
			addedForce.y = 0;
		}
		else if ((dir.y > 0 && addedForce.y < 0) || (dir.y < 0 && addedForce.y > 0))
		{
			addedForce.x = 0;
		}

		// 움직이게 함
		col->AddPlayerPos(addedForce);

		// 이동후에 충돌검사를 하고 충돌을 했을경우 잘못 된것이므로 수정한다.
		// 수정방법은 상대 충돌체와 딱 붙도록 수정해준다.
		if (IsCollided(col))
		{
			// oldOverlapped는 충돌면적이다.
			
			// 충돌면적이 세로로 길쭉함
			if (fabs(oldOverlapped.x) < fabs(oldOverlapped.y))
			{
				if (oldOverlapped.x < 0) { col->OnCollided(eCollisionDir::Right, collidedTag); }
				else { col->OnCollided(eCollisionDir::Left, collidedTag); }
				oldPos.x += oldOverlapped.x;
			}
			// 충돌면적이 가로로 길쭉함
			else
			{
				if (oldOverlapped.y < 0) { col->OnCollided(eCollisionDir::Bottom, collidedTag); }
				else { col->OnCollided(eCollisionDir::Top, collidedTag); }
				oldPos.y += oldOverlapped.y;
			}
			col->SetPlayerPos({ oldPos.x,oldPos.y });
		}
		//여기는 보정이 됨
		else
		{
			if (addedForce.x != 0)
			{
				if (addedForce.y < 0) { col->OnCollided(eCollisionDir::Bottom, collidedTag); }
				else { col->OnCollided(eCollisionDir::Top, collidedTag); }
			}
			else
			{
				if (addedForce.x < 0) { col->OnCollided(eCollisionDir::Right, collidedTag); }
				else { col->OnCollided(eCollisionDir::Left, collidedTag); }
			}
		}
	}
	// 이동시키지는 않지만 충돌했다고 말은 해줌
	else if (collidedTag == 1)
	{
		if (addedForce.x != 0)
		{
			if (addedForce.y < 0) { col->OnCollided(eCollisionDir::Bottom, collidedTag); }
			else { col->OnCollided(eCollisionDir::Top, collidedTag); }
		}
		else
		{
			if (addedForce.x < 0) { col->OnCollided(eCollisionDir::Right, collidedTag); }
			else { col->OnCollided(eCollisionDir::Left, collidedTag); }
		}
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

int Physcis::PreventOverlapped(Collider* col1, Collider* col2, POINTFLOAT& addedForce, POINTFLOAT dir, POINTFLOAT& oldOverlapped)
{
	// 충돌 필터
	int col1Tag = (int)col1->GetTag();
	int col2Tag = (int)col2->GetTag();
	bool isNotMoved = false;
	if (((col1Tag | col2Tag) & 0b100010) == 0b100010) { return 0; } // 물 (32) 과 아모 (2) 여부 확인
	if (((col1Tag | col2Tag) & 0b0010000000) && ((col1Tag | col2Tag) & 0b0010000101) != 0b0010000101) { return 0; } // 아이템과 플레이어 탱크 이외엔 충돌 금지
	if (((col1Tag & col2Tag) & 0b100000100) == 0b100000100) { return 0; } // 통과탱크와 일반탱크끼리 충돌금지
	if (((col1Tag & col2Tag) & 0b10) == 0b10 && ((col1Tag & 1) ^ (col2Tag & 1)) == 0) { return 0; } // 같은 팀의 아모끼리 충돌금지
	if (((col1Tag & col2Tag) & 0b100) == 0b100 && ((col1Tag & 1) ^ (col2Tag & 1)) == 1) { return 0; } // 다른 팀의 탱크끼리 충돌금지
	if (((col1Tag | col2Tag) & 0b110) == 0b110 && ((col1Tag & 1) ^ (col2Tag & 1)) == 0) // 같은 팀의 총알과 탱크 충돌금지, 서로 다른 플레이어인경우는 충돌하기
	{
		// 서로 같은 플레이어의 총알과 탱크인경우는 충돌금지
		if (((col1Tag & 0b1000000000) ^ (col2Tag & 0b1000000000)) == 0) { return 0; }
		// 서로 다른 플레이어의 총알과 탱크는 충돌하기
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

		if (((col1Tag | col2Tag) & 0b1010) == 0b1010)
		{
			if (fabs(overlappedX) < fabs(overlappedY) && fabs(addedForce.x) < fabs(overlappedX))
			{
				addedForce.x = overlappedX;
			}
			// 상하이동
			else if (fabs(overlappedX) > fabs(overlappedY) && fabs(addedForce.y) < fabs(overlappedY))
			{
				addedForce.y = overlappedY;
			}
			if (dir.x > 0) { col2->OnCollided(eCollisionDir::Left, col1Tag); }
			else if (dir.x < 0) { col2->OnCollided(eCollisionDir::Right, col1Tag); }
			else if (dir.y > 0) { col2->OnCollided(eCollisionDir::Top, col1Tag); }
			else { col2->OnCollided(eCollisionDir::Bottom, col1Tag); }
			return col2Tag;
		}

		if (fabs(overlappedX) < fabs(overlappedY))
		{
			// 좌우이동
			if (overlappedX < 0)
			{
				// 쪽에서 박음
				col2->OnCollided(eCollisionDir::Left, (int)col1->GetTag());
			}
			else
			{
				// 오른쪽에서 박음
				col2->OnCollided(eCollisionDir::Right, (int)col1->GetTag());
			}
		}
		else
		{
			// 상하이동
			if (overlappedY < 0)
			{
				// 위쪽에서 박음
				col2->OnCollided(eCollisionDir::Top, (int)col1->GetTag());
			}
			else
			{
				// 아래쪽에서 박음
				col2->OnCollided(eCollisionDir::Bottom, (int)col1->GetTag());
			}
		}

		if (fabs(oldOverlapped.x) < fabs(overlappedX)) { oldOverlapped.x = overlappedX; }
		if (fabs(oldOverlapped.y) < fabs(overlappedY)) { oldOverlapped.y = overlappedY; }

		// 충돌면적이 세로로 길쭉함
		if (fabs(overlappedX) < fabs(overlappedY))
		{
			// 세로 길이가 상대 충돌체의 크기의 절반보다 적음(보정될만한 크기)
			// 겹친 가로면적이 0보다 적고(왼쪽으로 강제이동하려고 함) 왼쪽을 향해 움직이고 있거나
			// 겹친 가로면적이 0보다 크고(오른쪽으로 강제이동하려고 함) 왼쪽을 향해 움직이고 있음
			if (fabs(overlappedY) < (col2->GetPlayerBodySize() * 0.5f) &&
				((overlappedX > 0 && dir.x < 0) || (overlappedX < 0 && dir.x > 0)))
			{
				// 상대 충돌체가 나보다 위에 있으면 아래로 내려주고
				// 나보다 아래에 있으면 위로 올려줘야 함

				// 상대가 나보다 아래에 있음
				if (overlappedY < 0)
				{
					if (addedForce.y > overlappedY) { addedForce.y = overlappedY; }
				}
				// 상대가 나보다 위에 있음
				else
				{
					if (addedForce.y < overlappedY) { addedForce.y = overlappedY; }
				}
			}
			// 보정하지 않아도 될만함
			else
			{
				// 좌우이동
				if (fabs(overlappedX) < fabs(overlappedY) && fabs(addedForce.x) < fabs(overlappedX))
				{
					addedForce.x = overlappedX;
				}
			}
		}
		// 충돌면적이 가로로 길쭉함
		else
		{
			// 가로 길이가 상대 충돌체의 크기의 절반보다 적음(보정될만한 크기)
			// 겹친 세로면적이 0보다 적고(위쪽으로 강제이동하려고 함) 아래쪽을 향해 움직이고 있거나
			// 겹친 세로면적이 0보다 크고(아래쪽으로 강제이동하려고 함) 위쪽을 향해 움직이고 있음
			if (fabs(overlappedX) < (col2->GetPlayerBodySize() * 0.5f) &&
				((overlappedY < 0 && dir.y > 0) || (overlappedY > 0 && dir.y < 0)))
			{
				// 상대 충돌체가 나보다 왼쪽에 있으면 오른쪽으로 밀어주고
				// 나보다 오른쪽에 있으면 왼쪽으로 밀어줘야 함

				// 상대가 나보다 오른쪽에 있음
				if (overlappedX < 0)
				{
					if (addedForce.x > overlappedX) { addedForce.x = overlappedX; }
				}
				// 상대가 나보다 왼쪽에 있음
				else
				{
					if (addedForce.x < overlappedX) { addedForce.x = overlappedX; }
				}
			}
			else
			{
				// 상하이동
				if (fabs(overlappedX) > fabs(overlappedY) && fabs(addedForce.y) < fabs(overlappedY))
				{
					addedForce.y = overlappedY;
				}
			}
		}

		return (int)col2->GetTag();
	}
	else
	{
		return 0;
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
	for (vector<Collider*>::iterator it = mVecCollider.begin(); it != mVecCollider.end(); ++it)
	{
		(*it)->Render(hdc);
	}
}

void Physcis::Release()
{
	Collider* col;
	for (vector<Collider*>::iterator it = mVecCollider.begin(); it != mVecCollider.end();)
	{
		col = (*it);
		it = mVecCollider.erase(it);
		SAFE_DELETE(col);
	}
}
