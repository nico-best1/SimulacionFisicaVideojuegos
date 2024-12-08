#pragma once
#include "ForceGenerator.h"

class TemporaryForceGenerator : public ForceGenerator {
private:
    Vector3 force;       
    float timeRemaining; 

public:
    TemporaryForceGenerator(const Vector3& force, float duration)
        : ForceGenerator(duration), force(force), timeRemaining(duration) {}

    Vector3 newForce(Particle* p) override {
        if (timeRemaining > 0) return force;
        else return Vector3(0, 0, 0);  
    }

    Vector3 newForceSolid(SolidRigid* sr) override {
        if (timeRemaining > 0) return force;
        else return Vector3(0, 0, 0);
    }

    void update(double t, ParticleSystem* pS) override {
        timeRemaining -= t;
        if (timeRemaining <= 0) alive = false; 
    }
};
