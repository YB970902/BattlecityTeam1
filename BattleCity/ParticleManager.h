#pragma once
#include "Singleton.h"

enum class eParticleTag
{
	SmallBoom,
	BigBoom,
	Shield,
	Spawn,
};

typedef struct ParticleInfo
{
	eImageTag ImageTag = eImageTag::None;
	int FrameX = 0;
	float DurationTime = 0.0f;
	float RepeatTime = 0.0f;
	float DelayTime = 0.0f;
} PARTICLE_INFO;

class Tank;
class Particle;
class ParticleManager : public Singleton<ParticleManager>
{
protected:
	map<eParticleTag, PARTICLE_INFO> mMapParticleInfo;
	vector<Particle*> mVecParticle;
public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void AddParticle(eParticleTag particleTag, eImageTag imageTag, int frameX, float durationTime, float repeatTime, float delayTime = 0.0f);

	void CreateParticle(eParticleTag tag, POINTFLOAT pos);
	void CreateParticle(eParticleTag tag, Tank* tank);
};

