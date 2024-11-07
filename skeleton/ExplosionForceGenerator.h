#pragma once
#include <cmath>
#include "ForceGenerator.h"
class ExplosionForceGenerator : public ForceGenerator
{
public:
	ExplosionForceGenerator(const physx::PxVec3& centro, float radio, float k, float ct);
	physx::PxVec3 calculateForce(Particle* p) override;
	virtual ~ExplosionForceGenerator() {};
	void caldulateDistance(Particle* p);
	virtual void update(double t, ParticleSystem* prSys);
private:
	physx::PxVec3 centro;
	float radio, k, ct, d;
};