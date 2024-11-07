#pragma once
#include <PxPhysicsAPI.h>
#include <cstdlib>   
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
	double mass;
	Vector3 accumulatedForce =Vector3(0,0,0);
public:
	Particle(Vector3 Pos, Vector3 Vel, Vector3 A, double Damping, int lifetime_) 
		: vel(Vel), a(A), damping(Damping), lifetime(lifetime_){
		mass = static_cast<double>(rand()) / RAND_MAX * 50;
		pose = physx::PxTransform(Pos);
		PxShape*  shape = CreateShape(physx::PxSphereGeometry(1));
		renderItem = new RenderItem(shape, &pose, physx::PxVec4(1, 1, 1, 1));
		RegisterRenderItem(renderItem);
	}

	~Particle() {
		DeregisterRenderItem(renderItem);	
	};

	Vector3 getVelocity() { return vel; }
	Vector3 getPosition() { return pose.p; }

	void addForce(const Vector3& force) {
		accumulatedForce += force;
	}

	virtual void integrate(double t) {
		if (lifetime <= 0) return;
		a = accumulatedForce / mass;
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

	double getMass() { return mass; }

	void setForce(Vector3 newforce) {
		accumulatedForce = newforce;
	}
};

