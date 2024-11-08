#pragma once
#include "ForceGenerator.h"
class WindForceGenerator : public ForceGenerator
{
protected:
	Vector3 windVelocity;
	float k1, k2;
	Vector3 center;
	float radius;
public:
	WindForceGenerator(const Vector3& windVelocity, float k1, float k2, Vector3 center, float radius);
	virtual Vector3 newForce(Particle* particle);
	virtual ~WindForceGenerator() {};
	virtual void calculateVelocity(Particle* particle) {};
};