#include "Config.h"
#include "ParticleManager.h"
#include "Particle.h"
#include "Tank.h"

HRESULT ParticleManager::Init()
{
	AddParticle(eParticleTag::SmallBoom, eImageTag::EffectSmallBoom, 4, 0.25, 0.175f);
	AddParticle(eParticleTag::BigBoom, eImageTag::EffectBigBoom, 4, 0.5, 0.25f, 0.25f);
	AddParticle(eParticleTag::Spawn, eImageTag::EffectSpawn, 4, TANK_SPAWNING_TIME, 0.25f);
	AddParticle(eParticleTag::Shield, eImageTag::EffectShield, 4, INVENCIBLE_ITEM_DURATION_TIME, 0.1f);
	return S_OK;
}

void ParticleManager::Release()
{
	Particle* particle;
	for (vector<Particle*>::iterator it = mVecParticle.begin(); it != mVecParticle.end();)
	{
		particle = (*it);
		it = mVecParticle.erase(it);
		SAFE_RELEASE(particle);
	}
}

void ParticleManager::Update()
{
	Particle* particle;
	for (vector<Particle*>::iterator it = mVecParticle.begin(); it != mVecParticle.end();)
	{
		(*it)->Update();
		if ((*it)->IsEnd())
		{
			particle = (*it);
			it = mVecParticle.erase(it);
			SAFE_RELEASE(particle);
		}
		else { ++it; }
	}
}

void ParticleManager::Render(HDC hdc)
{
	for (int i = 0; i < mVecParticle.size(); ++i)
	{
		mVecParticle[i]->Render(hdc);
	}
}

void ParticleManager::AddParticle(eParticleTag particleTag, eImageTag imageTag, int frameX, float durationTime, float repeatTime, float delayTime)
{
	if (mMapParticleInfo.find(particleTag) != mMapParticleInfo.end()) { return; }
	mMapParticleInfo[particleTag] = PARTICLE_INFO{ imageTag, frameX, durationTime, repeatTime, delayTime };
}

void ParticleManager::CreateParticle(eParticleTag tag, POINTFLOAT pos)
{
	Particle* newParticle = new Particle;
	newParticle->Init(IMG_MGR->FindImage(mMapParticleInfo[tag].ImageTag), mMapParticleInfo[tag].FrameX, mMapParticleInfo[tag].DurationTime, mMapParticleInfo[tag].RepeatTime, mMapParticleInfo[tag].DelayTime);
	newParticle->SetPosition(pos);
	mVecParticle.push_back(newParticle);
}

void ParticleManager::CreateParticle(eParticleTag tag, Tank* tank)
{
	Particle* newParticle = new Particle;
	newParticle->Init(IMG_MGR->FindImage(mMapParticleInfo[tag].ImageTag), mMapParticleInfo[tag].FrameX, mMapParticleInfo[tag].DurationTime, mMapParticleInfo[tag].RepeatTime, mMapParticleInfo[tag].DelayTime);
	newParticle->SetTank(tank);
	mVecParticle.push_back(newParticle);
}
