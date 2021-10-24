#pragma once
#include "Config.h"

class CollisionChecker
{
public:
    virtual void OnCollided(eCollisionDir dir, eCollisionTag tag) = 0;

    virtual ~CollisionChecker() = default;
};