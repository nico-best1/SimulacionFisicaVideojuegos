#include "WindForceGenerator.h"
WindForceGenerator::WindForceGenerator(const Vector3& velocidad, float k1, float k2, physx::PxVec3 centro, float radio) :ForceGenerator(INT_MAX), velocidad(velocidad), k1(k1), k2(k2), centro(centro), radio(radio) {

};
physx::PxVec3 WindForceGenerator::calculateForce(Particle* p) {
    if ((p->getPosition() - centro).magnitude() <= radio) {
        calculateVel(p);
        return k1 * (velocidad - p->getVelocity()) + k2 * (velocidad - p->getVelocity()).magnitude() * (velocidad - p->getVelocity());
    }
    return physx::PxVec3(0, 0, 0);
}