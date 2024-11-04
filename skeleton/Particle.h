#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

using namespace physx;
class Particle
{
protected:
	Vector3 vel;
	Vector3 a;
	physx::PxTransform pose;
	RenderItem* renderItem;
	double damping;
	double lifetime;
public:
	Particle(Vector3 Pos, Vector3 Vel, Vector3 A, double Damping, int lifetime_) 
		: vel(Vel), a(A), damping(Damping), lifetime(lifetime_){
		pose = physx::PxTransform(Pos);
		PxShape*  shape = CreateShape(physx::PxSphereGeometry(1));
	/*	renderItem = new RenderItem();
		renderItem->shape = shape;
		renderItem->transform = &pose;
		renderItem->color = Vector4(1,1,1,1);*/
		renderItem = new RenderItem(shape, &pose, physx::PxVec4(1, 1, 1, 1));
		RegisterRenderItem(renderItem);
	}

	~Particle() {
		DeregisterRenderItem(renderItem);	
		//delete renderItem;
		//renderItem = nullptr;
	};

	Vector3 getVelocity() { return vel; }
	Vector3 getPosition() { return pose.p; }

	virtual void integrate(double t) {
		if (lifetime <= 0) return;
		vel += a * t;
		vel *= pow(damping, t);
		pose.p += vel * t;
		lifetime -= t;
	}

	bool isDead() const { 
		return lifetime <= 0; 
	}

	bool isOutOfBounds(int area_x, int area_y) const {
		return pose.p.x > area_x || pose.p.x < -area_x ||
			pose.p.y > area_y || pose.p.y < -area_y;
	}
};

