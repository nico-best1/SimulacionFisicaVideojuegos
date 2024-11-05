#pragma once
#include "ForceGenerator.h"
#include "Particle.h"

class WindForceGenerator : public ForceGenerator {
private:
    Vector3 windVelocity;  
    float k1;             

public:
    WindForceGenerator(const Vector3& windVel, float k1_coef)
        : windVelocity(windVel), k1(k1_coef) {}

    void updateForce(Particle* particle, double t) override {
        Vector3 particleVelocity = particle->getVelocity();
        Vector3 relativeVelocity = windVelocity - particleVelocity;

        Vector3 windForce = relativeVelocity * k1;

        particle->addForce(windForce);
    }
};
