#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
class Particle
{
protected:
	Vector3 vel;
	Vector3 a;
	physx::PxTransform pose;
	RenderItem* renderItem;
	double damping;
	float lifetime; 
	float currentLifetime; 
public:
	Particle(Vector3 Pos, Vector3 Vel, Vector3 A, double Damping, float Lifetime) : vel(Vel), a(A), damping(Damping), lifetime(Lifetime), currentLifetime(0.0f) {
		pose = physx::PxTransform(Pos);
		renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1)), &pose, physx::PxVec4(1, 1, 1, 1));
		RegisterRenderItem(renderItem);
	}
	virtual ~Particle() {
		DeregisterRenderItem(renderItem);
		delete renderItem;
	};

	bool isDead() const {
		return currentLifetime >= lifetime;
	}

	virtual void integrate(double t);

};

