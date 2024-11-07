#include "TorbellinoForceGenerator.h"
TorbellinoForceGenerator::TorbellinoForceGenerator(const physx::PxVec3& velocidad, float k1, float k2, physx::PxVec3 centro, float radio, float intensity) :
	WindForceGenerator(velocidad, k1, k2, centro, radio), intensity(intensity) {
}

void TorbellinoForceGenerator::calculateVel(Particle* p) {
	physx::PxVec3 vTor = { -(p->getPosition().z - centro.z), 0, p->getPosition().x - centro.x };
	velocidad = intensity * vTor;
}