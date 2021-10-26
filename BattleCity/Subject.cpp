#include "Config.h"
#include "Subject.h"
#include "Observer.h"
#include "GameEntity.h"

void Subject::AddObserver(Observer* obs)
{
	vector<Observer*>::iterator it = find(mVecObserver.begin(), mVecObserver.end(), obs);
	if (it == mVecObserver.end()) { mVecObserver.push_back(obs); }
}

void Subject::RemoveObserver(Observer* obs)
{
	vector<Observer*>::iterator it = find(mVecObserver.begin(), mVecObserver.end(), obs);
	if (it != mVecObserver.end()) { mVecObserver.erase(it); }
}

void Subject::Notify(GameEntity* obj, eSubjectTag subjectTag, eEventTag eventTag)
{
	for (int i = 0; i < mVecObserver.size(); ++i)
	{
		mVecObserver[i]->OnNotify(obj, subjectTag, eventTag);
	}
}
