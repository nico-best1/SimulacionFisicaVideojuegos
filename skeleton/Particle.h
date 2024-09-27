#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
class Particle
{
private:
	Vector3 vel;
	Vector3 a;
	physx::PxTransform pose;
	RenderItem* renderItem;
public:
	Particle(Vector3 Pos, Vector3 Vel, Vector3 A) : vel(Vel), a(A){
		pose = physx::PxTransform(Pos);
		renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1)), &pose, physx::PxVec4(1, 1, 1, 1));
		RegisterRenderItem(renderItem);
	}
	~Particle() {
		DeregisterRenderItem(renderItem);
		delete renderItem;
	};

	void integrate(double t);
};

