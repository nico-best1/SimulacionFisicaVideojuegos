#pragma once
#include <cmath>
#include "ForceGenerator.h"
class ExplosionForceGenerator : public ForceGenerator
{
private:
	Vector3 explosionCenter;
	float explosionRadius, forceConstant, decayTime, distance;
public:
	ExplosionForceGenerator(const Vector3& explosionCenter, float explosionRadius, float forceConstant, float decayTime);
	virtual ~ExplosionForceGenerator() {};
	void calculateDistance(Particle* particle);
	Vector3 calculateForce(Particle* particle) override;
	virtual void update(double deltaTime, ParticleSystem* particleSystem);
};