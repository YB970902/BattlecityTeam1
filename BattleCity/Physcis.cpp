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
	if (prevPoint.x == -1 || prevPoint.y == -1) { return nullptr; } // ��������!

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
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < mGridMap[mCheckGrid[i].x][mCheckGrid[i].y].size(); j++)
		{
			if (mGridMap[mCheckGrid[i].x][mCheckGrid[i].y][j] == col) { continue; }
			collidedTag |= PreventOverlapped(col, mGridMap[mCheckGrid[i].x][mCheckGrid[i].y][j], addedForce, dir, oldOverlapped);
		}
	}

	if (collidedTag > 1)
	{
		if ((dir.x > 0 && addedForce.x < 0) || (dir.x < 0 && addedForce.x > 0))
		{
			addedForce.y = 0;
		}
		else if ((dir.y > 0 && addedForce.y < 0) || (dir.y < 0 && addedForce.y > 0))
		{
			addedForce.x = 0;
		}

		// ���⿡ ���� ������ ���� ����.
		if (IsCollided(col))
		{
			if ((dir.x > 0 && oldOverlapped.x < 0) || (dir.x < 0 && oldOverlapped.x > 0))
			{
				oldPos.x = col->GetPlayerPos().x + oldOverlapped.x;
				if (dir.x > 0) { col->OnCollided(eCollisionDir::Left, collidedTag); }
				else { col->OnCollided(eCollisionDir::Right, collidedTag); }
			}
			else if ((dir.y > 0 && oldOverlapped.y < 0) || (dir.y < 0 && oldOverlapped.y > 0))
			{
				oldPos.y = col->GetPlayerPos().y + oldOverlapped.y;
				if (dir.y > 0) { col->OnCollided(eCollisionDir::Top, collidedTag); }
				else { col->OnCollided(eCollisionDir::Bottom, collidedTag); }
			}
			// �̿ܿ� ������ ������ ����
			else
			{
				col->OnCollided(eCollisionDir::Top, collidedTag);
			}
			col->SetPlayerPos({ oldPos.x,oldPos.y });
		}
		//����� ������ ��
		else
		{
			if (addedForce.x != 0)
			{
				if (addedForce.y < 0) { col->OnCollided(eCollisionDir::Bottom, collidedTag); }
				if (addedForce.y > 0) { col->OnCollided(eCollisionDir::Top, collidedTag); }
			}
			else
			{
				if (addedForce.x < 0) { col->OnCollided(eCollisionDir::Right, collidedTag); }
				if (addedForce.x > 0) { col->OnCollided(eCollisionDir::Left, collidedTag); }
			}
			col->AddPlayerPos(addedForce);
		}
	}
	// �̵���Ű���� ������ �浹�ߴٰ� ���� ����
	else if (collidedTag == 1)
	{
		if (addedForce.x != 0)
		{
			if (addedForce.y < 0) { col->OnCollided(eCollisionDir::Bottom, collidedTag); }
			if (addedForce.y > 0) { col->OnCollided(eCollisionDir::Top, collidedTag); }
		}
		else
		{
			if (addedForce.x < 0) { col->OnCollided(eCollisionDir::Right, collidedTag); }
			if (addedForce.x > 0) { col->OnCollided(eCollisionDir::Left, collidedTag); }
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
	//if ((((int)col->GetTag()) & 4) == 4)		//��ũ�ϰ�쿡��
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
	// �浹 ����
	int col1Tag = (int)col1->GetTag();
	int col2Tag = (int)col2->GetTag();
	bool isNotMoved = false;
	if (((col1Tag | col2Tag) & 0b100010) == 0b100010) { return 0; } // �� (32) �� �Ƹ� (2) ���� Ȯ��
	if (((col1Tag & col2Tag) & 0b100000100) == 0b100000100) { return 0; } // �����ũ�� �Ϲ���ũ���� �浹����
	if (((col1Tag & col2Tag) & 0b10) == 0b10 && ((col1Tag & 1) ^ (col2Tag & 1)) == 0) { return 0; } // ���� ���� �Ƹ𳢸� �浹����
	if (((col1Tag & col2Tag) & 0b100) == 0b100 && ((col1Tag & 1) ^ (col2Tag & 1)) == 1) { return 0; } // �ٸ� ���� ��ũ���� �浹����
	if (((col1Tag | col2Tag) & 0b110) == 0b110 && ((col1Tag & 1) ^ (col2Tag & 1)) == 0) // ���� ���� �Ѿ˰� ��ũ �浹����, ���� �ٸ� �÷��̾��ΰ��� �浹�ϱ�
	{
		// ���� ���� �÷��̾��� �Ѿ˰� ��ũ�ΰ��� �浹����
		if (((col1Tag & 0b1000000000) ^ (col2Tag & 0b1000000000)) == 0) { return 0; }
		// ���� �ٸ� �÷��̾��� �Ѿ˰� ��ũ�� �浹�ϱ�
	}
	if (IsCollided(col1, col2))
	{
		float overlappedX = 0, overlappedY = 0;

		// col1�� ���ʿ� �ִ°��
		if (col1->GetPlayerPos().x < col2->GetPlayerPos().x)
		{
			overlappedX = col2->GetPlayerBody().left - col1->GetPlayerBody().right;
		}
		else
		{
			overlappedX = col2->GetPlayerBody().right - col1->GetPlayerBody().left;
		}

		// col1�� ���� �ִ°��
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
			// �¿��̵�
			if (overlappedX < 0)
			{
				// �ʿ��� ����
				//col1->OnCollided(eCollisionDir::Right, col2->GetTag());
				col2->OnCollided(eCollisionDir::Left, (int)col1->GetTag());
			}
			else
			{
				// �����ʿ��� ����
				//col1->OnCollided(eCollisionDir::Left, col2->GetTag());
				col2->OnCollided(eCollisionDir::Right, (int)col1->GetTag());
			}
		}
		else
		{
			// �����̵�
			if (overlappedY < 0)
			{
				// ���ʿ��� ����
				//col1->OnCollided(eCollisionDir::Bottom, col2->GetTag());
				col2->OnCollided(eCollisionDir::Top, (int)col1->GetTag());
			}
			else
			{
				// �Ʒ��ʿ��� ����
				//col1->OnCollided(eCollisionDir::Top, col2->GetTag());
				col2->OnCollided(eCollisionDir::Bottom, (int)col1->GetTag());
			}
		}
		// ��ģ ������ ���η� ������ �����̰� �����ϱ⿡ �� ���� ����
		//�ٵ� �������� 3���� 1 ���� �۰� ��ģ��� ���� ���⿡ ���� Y��ġ ������ �� �ش�.
		if (fabs(overlappedX) < fabs(overlappedY) &&
			fabs(overlappedY) < (col2->GetPlayerBodySize() * 0.5f) &&
			((overlappedX > 0 && dir.x < 0) || (overlappedX < 0 && dir.x > 0)))
		{
			if (addedForce.y < fabs(overlappedY))
			{
				addedForce.y = overlappedY;
			}
		}
		// ��ģ ������ ���η� ������ �����̰� �����ϱ⿡ �� ���� ����
		// �ٵ� �������� 3���� 1���� �۰� ��ģ��� ���� ���⿡ ���� X��ġ ������ �� �ش�.
		else if (fabs(overlappedX) > fabs(overlappedY) &&
			fabs(overlappedX) < (col2->GetPlayerBodySize() * 0.5f) &&
			((overlappedY < 0 && dir.y > 0) || (overlappedY > 0 && dir.y < 0)))
		{
			if (addedForce.x < fabs(overlappedX))
			{
				addedForce.x = overlappedX;
			}
		}
		// �¿��̵�
		else if (fabs(overlappedX) < fabs(overlappedY) && addedForce.x < fabs(overlappedX))
		{
			addedForce.x = overlappedX;
		}
		// �����̵�
		else if (fabs(overlappedX) > fabs(overlappedY) && addedForce.y < fabs(overlappedY))
		{
			addedForce.y = overlappedY;
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
