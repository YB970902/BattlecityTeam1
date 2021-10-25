#pragma once

class Observer;
class Subject
{
private:
	vector<Observer*> mVecObserver;
public:
	virtual ~Subject() = default;

	void AddObserver(Observer* obs);
	void RemoveObserver(Observer* obs);

	int GetObserverCount() { return mVecObserver.size(); }

	void Notify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag);
};

