#pragma once
#include "WindForceGenerator.h"
#include <cmath>

class TornadoForceGenerator : public WindForceGenerator {
private:
    Vector3 center;    
    float K;          
    float radius;      

public:
    TornadoForceGenerator(const Vector3& tornadoCenter, float intensity, float maxRadius)
        : WindForceGenerator(Vector3(0, 0, 0), 0),
        center(tornadoCenter), K(intensity), radius(maxRadius) {}

    void updateForce(Particle* particle, double t) override {
        Vector3 position = particle->getPosition();
        Vector3 direction = position - center;
        float distance = direction.magnitude();

        if (distance < radius && distance > 0.01f) {
            Vector3 tangentialDirection(-direction.z, 0, direction.x);
            tangentialDirection.normalize();

            Vector3 tornadoVelocity = tangentialDirection * (K * distance);

            Vector3 relativeVelocity = tornadoVelocity - particle->getVelocity();
            Vector3 windForce = relativeVelocity * K;

            particle->addForce(windForce);
        }
    }
};
