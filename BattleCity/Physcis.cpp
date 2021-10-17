#include "Config.h"
#include "Physcis.h"
#include "Collider.h"

Collider* Physcis::CreateCollider(POINTFLOAT pos, float bodySize, CollisionChecker* obj, eCollisionTag tag)
{
	Collider* newCol = new Collider();
	newCol->Init(pos, bodySize, this, obj, tag);
	mVecCollider.push_back(newCol);
	return newCol;
}

void Physcis::DestoryCollider(Collider* col1)
{
	mVecCollider.erase(mVecCollider.begin());
}

void Physcis::CheckCollider(Collider* col, POINTFLOAT dir, POINTFLOAT oldPos)
{
	POINTFLOAT addedForce = { 0,0 };
	POINTFLOAT oldOverlapped = { 0,0 };


	for (int i = 0; i < mVecCollider.size(); i++)
	{
		if (mVecCollider[i] == col) { continue; }
		PreventOverlapped(col, mVecCollider[i], addedForce, dir, oldOverlapped);
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

		cout << "�̵� �� �浹����" << endl;

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
	for (int i = 0; i < mVecCollider.size(); i++)
	{
		if (mVecCollider[i] == col) { continue; }
		if (IsCollided(col, mVecCollider[i])) { return true; }
	}
	return false;
}

void Physcis::PreventOverlapped(Collider* col1, Collider* col2, POINTFLOAT& addedForce, POINTFLOAT dir, POINTFLOAT& oldOverlapped)
{
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
				col1->OnCollided(eCollisionDir::Right, col2->GetTag());
			}
			else
			{
				// �����ʿ��� ����
				col1->OnCollided(eCollisionDir::Left, col2->GetTag());
			}
		}
		else
		{
			// �����̵�
			if (overlappedY < 0)
			{
				// ���ʿ��� ����
				col1->OnCollided(eCollisionDir::Bottom, col2->GetTag());
			}
			else
			{
				// �Ʒ��ʿ��� ����
				col1->OnCollided(eCollisionDir::Top, col2->GetTag());
			}
		}
		// ��ģ ������ ���η� ������ �����̰� �����ϱ⿡ �� ���� ����
		//�ٵ� �������� 3���� 1 ���� �۰� ��ģ��� ���� ���⿡ ���� Y��ġ ������ �� �ش�.
		if (fabs(overlappedX) < fabs(overlappedY) && 
			fabs(overlappedY) < (col2->GetPlayerBodySize() * 0.33f) && 
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
			fabs(overlappedX) < (col2->GetPlayerBodySize() * 0.33f) &&
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

	}
}

void Physcis::Render(HDC hdc)
{
	for (int i = 0; i < mVecCollider.size(); i++)
	{
		mVecCollider[i]->Render(hdc);
	}
}
