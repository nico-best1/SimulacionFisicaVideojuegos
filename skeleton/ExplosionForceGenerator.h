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
	Vector3 newForce(Particle* particle) override;
	Vector3 newForceSolid(SolidRigid* p) override { return Vector3(0, 0, 0); }
	virtual void update(double deltaTime, ParticleSystem* particleSystem);
	virtual void updateSolid(double t, SolidRigidSystem* srSys) {}
};