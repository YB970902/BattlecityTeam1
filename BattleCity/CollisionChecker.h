#pragma once
#include "Config.h"

class CollisionChecker
{
public:
    virtual void OnCollided(eCollisionDir dir, int tag) = 0;

    virtual ~CollisionChecker() = default;
};