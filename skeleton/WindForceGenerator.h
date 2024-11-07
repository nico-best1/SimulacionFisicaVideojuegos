#pragma once
#include "ForceGenerator.h"
class WindForceGenerator : public ForceGenerator
{
public:
	WindForceGenerator(const physx::PxVec3& velocidad, float k1, float k2, physx::PxVec3 centro, float radio);
	virtual physx::PxVec3 calculateForce(Particle* p);
	virtual ~WindForceGenerator() {};
	virtual void calculateVel(Particle* p) {};
protected:
	physx::PxVec3 velocidad;
	float k1, k2;
	physx::PxVec3 centro;
	float radio;
};