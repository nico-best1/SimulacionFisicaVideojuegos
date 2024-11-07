#include "ExplosionForceGenerator.h"
#include "ParticleSystem.h"
ExplosionForceGenerator::ExplosionForceGenerator(const physx::PxVec3& centro, float radio, float k, float ct) :
	ForceGenerator(ct * 4),
	centro(centro), radio(radio), k(k), ct(ct), d(0) {

}

void ExplosionForceGenerator::update(double t, ParticleSystem* prSys) {
	duration -= t;
	if (duration < 0) {
		alive = false;
		prSys->setExplosion(false);
	}
}

physx::PxVec3 ExplosionForceGenerator::calculateForce(Particle* p) {

	if (d < radio) {
		caldulateDistance(p);
		physx::PxVec3 forceExplosion = (k / pow(d, 2)) * physx::PxVec3(p->getPosition().x - centro.x, p->getPosition().y - centro.y, p->getPosition().z - centro.z) * std::exp(-duration / ct);
		return forceExplosion;
	}
	else
		return physx::PxVec3(0, 0, 0);
}
void ExplosionForceGenerator::caldulateDistance(Particle* p) {
	d = sqrt(pow(p->getPosition().x - centro.x, 2) +
		pow(p->getPosition().y - centro.y, 2) +
		pow(p->getPosition().z - centro.z, 2));
}