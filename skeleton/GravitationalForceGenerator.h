#pragma once
#include "ForceGenerator.h"
#include "Particle.h"

class GravitationalForceGenerator : public ForceGenerator {
private:
    Vector3 gravity;
public:
    GravitationalForceGenerator(Vector3 g) : gravity(g) {}

    void updateForce(Particle* particle, double t) override {
        Vector3 force = gravity * particle->getMass();
        particle->addForce(force);
    }
};
