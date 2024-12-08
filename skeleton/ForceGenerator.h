#pragma once
#include "Particle.h"
#include "SolidRigid.h"
class ParticleSystem;
class SolidRigidSystem;
class ForceGenerator {
protected:
    float duration;
    bool alive;
public:
    ForceGenerator(float duration) : duration(duration), alive(true) {};
    virtual Vector3 newForce(Particle* p) = 0;
    virtual Vector3 newForceSolid(SolidRigid* sr) = 0;
    virtual physx::PxVec3 newTorqueSolid(SolidRigid* sr) { return physx::PxVec3(0); }
    virtual ~ForceGenerator() {}
    bool isAlive() { return alive; };
    virtual void update(double t, ParticleSystem* pS) {};
    virtual void updateSolid(double t, SolidRigidSystem* sSys) {};
};