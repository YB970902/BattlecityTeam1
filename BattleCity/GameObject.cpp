#include "GameObject.h"
#include "Image.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

HRESULT GameObject::Init()
{
    return S_OK;
}

void GameObject::Release()
{
    
}

void GameObject::Update()
{
}

void GameObject::Render()
{
}

void GameObject::OnCollided(eCollisionDir dir, eCollisionTag tag)
{
}
