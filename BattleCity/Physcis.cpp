#include "Physcis.h"
#include "Collider.h"

Collider* Physcis::CreateCollider(POINTFLOAT pos, float bodySize)
{
	Collider* newCol = new Collider();
	newCol->Init(pos, bodySize);
	mVecCollider.push_back(newCol);
	return newCol;
}

void Physcis::CheckCollider(Collider* col1)
{
	//for (int i = 0; i < mVecCollider.size(); i++)
	//{
	//	for (int j = 0; j < mVecCollider.size(); j++)
	//	{
	//		if (mVecCollider[i] == mVecCollider[j]) { continue; }

	//		PreventOverlapped(mVecCollider[i], mVecCollider[j]);
	//	}
	//}


	for (int i = 0; i < mVecCollider.size(); i++)
	{
		if (mVecCollider[i] == col1) { continue; }

		PreventOverlapped(col1, mVecCollider[i]);
	}
}

bool Physcis::IsCollided(Collider* col1, Collider* col2)
{
	if (col1->GetPlayerBody().left > col2->GetPlayerBody().right) { return false; }
	if (col1->GetPlayerBody().right < col2->GetPlayerBody().left) { return false; }
	if (col1->GetPlayerBody().top > col2->GetPlayerBody().bottom) { return false; }
	if (col1->GetPlayerBody().bottom < col2->GetPlayerBody().top) { return false; }

	return true;
}

void Physcis::PreventOverlapped(Collider* col1, Collider* col2)
{
	if (IsCollided(col1, col2))
	{
		if (col1->GetPlayerPos().x - col2->GetPlayerPos().x < 0)
		{
			mDistance.x = col1->GetPlayerBody().right - col2->GetPlayerBody().left;
		}
		else
		{
			mDistance.x = col2->GetPlayerBody().right - col1->GetPlayerBody().left;
		}

		if (col1->GetPlayerPos().y - col2->GetPlayerPos().y < 0)
		{
			mDistance.y = col1->GetPlayerBody().bottom - col2->GetPlayerBody().top;
		}
		else
		{
			mDistance.y = col2->GetPlayerBody().bottom - col1->GetPlayerBody().top;
		}

		//mPos = col1->GetPlayerPos();
		if (mDistance.x < mDistance.y)
		{
			col1->GetPlayerPos().x - col2->GetPlayerPos().x > 0 ?
				//col1->GetPlayerPos().x += mDistance.x : col1->GetPlayerPos().x -= mDistance.x;
				col1->SetPlayerPos({ mDistance.x, 0 }) : col1->SetPlayerPos({ -mDistance.x, 0 });

			//mPos.x += mDistance.x : mPos.x -= mDistance.x;
		}
		else
		{
			col1->GetPlayerPos().y - col2->GetPlayerPos().y > 0 ? 
				//col1->GetPlayerPos().y += mDistance.y : col1->GetPlayerPos().y -= mDistance.y;

				col1->SetPlayerPos({ 0, mDistance.y }) : col1->SetPlayerPos({ 0, -mDistance.y });

				//mPos.y += mDistance.y : mPos.y -= mDistance.y;
		}

		//col1->Update();
		//col1->SetPlayerPos(mPos);
	}
}

void Physcis::Render(HDC hdc)
{
	for (int i = 0; i < mVecCollider.size(); i++)
	{
		mVecCollider[i]->Render(hdc);
	}
}
