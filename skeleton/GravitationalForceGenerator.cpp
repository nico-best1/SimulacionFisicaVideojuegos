#include "GravitationalForceGenerator.h"
GravitationalForceGenerator::GravitationalForceGenerator(const Vector3& gravity) :ForceGenerator(INT_MAX), gravity(gravity) {

};
physx::PxVec3 GravitationalForceGenerator::calculateForce(Particle* p) {
	return gravity * p->getMass();
}