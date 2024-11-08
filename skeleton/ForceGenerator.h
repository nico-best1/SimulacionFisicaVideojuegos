#pragma once
#include "Particle.h"
class ParticleSystem;
class ForceGenerator {
protected:
    float duration;
    bool alive;
public:
    ForceGenerator(float duration) : duration(duration), alive(true) {};
    virtual Vector3 newForce(Particle* p) = 0;
    virtual ~ForceGenerator() {}
    bool isAlive() { return alive; };
    virtual void update(double t, ParticleSystem* pS) {};
};