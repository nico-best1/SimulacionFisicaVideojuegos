#pragma once
#include "ForceGenerator.h"
class GravitationalForceGenerator : public ForceGenerator
{
private:
	Vector3 gravity;
public:
	GravitationalForceGenerator(const Vector3& gravity);
	virtual ~GravitationalForceGenerator() {};
	Vector3 newForce(Particle* p) override;
	Vector3 newForceSolid(SolidRigid* p) override;
};