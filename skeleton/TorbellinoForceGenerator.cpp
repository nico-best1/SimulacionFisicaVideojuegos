#include "TorbellinoForceGenerator.h"
TorbellinoForceGenerator::TorbellinoForceGenerator(const Vector3& windVelocity, float k1, float k2, Vector3 center, float radius, float intensity) :
	WindForceGenerator(windVelocity, k1, k2, center, radius), intensity(intensity) {
}

void TorbellinoForceGenerator::calculateVelocity(Particle* p) {
	Vector3 velocity_Torbellino = { -(p->getPosition().z - center.z), 0, p->getPosition().x - center.x };
	windVelocity = intensity * velocity_Torbellino;
}