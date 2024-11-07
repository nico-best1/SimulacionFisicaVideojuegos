#pragma once
#include "WindForceGenerator.h"
class TorbellinoForceGenerator : public WindForceGenerator
{
private:
	float intensity;
public:
	TorbellinoForceGenerator(const physx::PxVec3& velocidad, float k1, float k2, physx::PxVec3 centro, float radio, float intensity);
	~TorbellinoForceGenerator() {};
	void calculateVel(Particle* p) override;
};