#include "WindForceGenerator.h"

WindForceGenerator::WindForceGenerator(const Vector3& windVelocity, float k1, float k2, Vector3 center, float radius) :
    ForceGenerator(INT_MAX), windVelocity(windVelocity), k1(k1), k2(k2), center(center), radius(radius) {};

Vector3 WindForceGenerator::calculateForce(Particle* particle) {
    if ((particle->getPosition() - center).magnitude() <= radius) {
        calculateVelocity(particle);
        return k1 * (windVelocity - particle->getVelocity()) + k2 * (windVelocity - particle->getVelocity()).magnitude() * (windVelocity - particle->getVelocity());
    }
    return Vector3(0, 0, 0);
}