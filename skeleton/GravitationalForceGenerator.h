#pragma once
#include "ForceGenerator.h"
class GravitationalForceGenerator : public ForceGenerator
{
public:
	GravitationalForceGenerator(const physx::PxVec3& gravity);
	physx::PxVec3 calculateForce(Particle* p) override;
	virtual ~GravitationalForceGenerator() {};
private:
	physx::PxVec3 gravity;
};