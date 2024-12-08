#include "GravitationalForceGenerator.h"
GravitationalForceGenerator::GravitationalForceGenerator(const Vector3& gravity) 
	: ForceGenerator(INT_MAX), gravity(gravity) {

};
Vector3 GravitationalForceGenerator::newForce(Particle* p) {
	return gravity * p->getMass();
}
Vector3 GravitationalForceGenerator::newForceSolid(SolidRigid* p)
{
	return gravity * p->getSolid()->getMass();
}