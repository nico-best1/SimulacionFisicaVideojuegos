#pragma once
class ParticleSystem;
#include "Particle.h"
class ForceGenerator {
public:
    ForceGenerator(float duration) : duration(duration), alive(true) {};
    virtual physx::PxVec3 calculateForce(Particle* p) = 0;

    virtual ~ForceGenerator() {}
    bool isAlive() { return alive; };
    virtual void update(double t, ParticleSystem* prSys) {};
protected:
    float duration;
    bool alive;
};