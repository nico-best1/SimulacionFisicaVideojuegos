#include "ExplosionForceGenerator.h"
#include "ParticleSystem.h"

ExplosionForceGenerator::ExplosionForceGenerator(const physx::PxVec3& explosionCenter, float explosionRadius, float forceConstant, float decayTime) :
	ForceGenerator(decayTime * 4),
	explosionCenter(explosionCenter), explosionRadius(explosionRadius), forceConstant(forceConstant), decayTime(decayTime), distance(0) {

}

void ExplosionForceGenerator::update(double deltaTime, ParticleSystem* particleSystem) {
	duration -= deltaTime;
	if (duration < 0) {
		alive = false;
		particleSystem->ActiveExplosion(false);
	}
}


Vector3 ExplosionForceGenerator::newForce(Particle* particle) {

	if (distance < explosionRadius) {
		distance = sqrt(pow(particle->getPosition().x - explosionCenter.x, 2) + pow(particle->getPosition().y - explosionCenter.y, 2) + pow(particle->getPosition().z - explosionCenter.z, 2));
		Vector3 explosionForce = (forceConstant / pow(distance, 2)) *
			Vector3(particle->getPosition().x - explosionCenter.x,
				particle->getPosition().y - explosionCenter.y,
				particle->getPosition().z - explosionCenter.z)
			* std::exp(-duration / decayTime);
		return explosionForce;
	}
	else {
		return Vector3(0, 0, 0);
	}
}