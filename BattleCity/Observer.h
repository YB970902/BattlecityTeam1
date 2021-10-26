#pragma once

class GameEntity;
class Observer
{
public:
	virtual ~Observer() = default;
	virtual void OnNotify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag) = 0;
};

