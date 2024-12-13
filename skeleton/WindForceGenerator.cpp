#include "WindForceGenerator.h"

WindForceGenerator::WindForceGenerator(const Vector3& windVelocity, float k1, float k2, Vector3 center, float radius) :
    ForceGenerator(INT_MAX), windVelocity(windVelocity), k1(k1), k2(k2), center(center), radius(radius) {};

Vector3 WindForceGenerator::newForce(Particle* particle) {
    if ((particle->getPosition() - center).magnitude() <= radius) {
        calculateVelocity(particle);
        return k1 * (windVelocity - particle->getVelocity()) + k2 * (windVelocity - particle->getVelocity()).magnitude() * (windVelocity - particle->getVelocity());
    }
    return Vector3(0, 0, 0);
}

Vector3 WindForceGenerator::newForceSolid(SolidRigid* p)
{
    if (p)
    {
        if ((p->getSolid()->getGlobalPose().p - center).magnitude() <= radius) {
            calculateVelocitySolid(p);
            return k1 * (windVelocity - p->getSolid()->getLinearVelocity()) + k2 * (windVelocity - p->getSolid()->getLinearVelocity()).magnitude() * (windVelocity - p->getSolid()->getLinearVelocity());
        }
    }
    return physx::PxVec3(0, 0, 0);
}