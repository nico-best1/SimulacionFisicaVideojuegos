#pragma once
#include "ForceGenerator.h"
#include "Particle.h"

class BidirectionalSpringForceGenerator : public ForceGenerator {
private:
    Particle* particle1;
    Particle* particle2;
    float k;              
    float restLength;     

public:
    BidirectionalSpringForceGenerator(Particle* p1, Particle* p2, float k, float restLength)
        : ForceGenerator(INT_MAX), particle1(p1), particle2(p2), k(k), restLength(restLength) {}

    Vector3 newForce(Particle* p) override {
        Vector3 displacement = particle1->getPosition() - particle2->getPosition();
        float length = displacement.magnitude();

        if (p == particle1) {
            if (length > restLength) {
                Vector3 force = displacement;
                force *= -k * (length - restLength) / length;
                return force;
            }
        }
        else if (p == particle2) {
            if (length > restLength) {
                Vector3 force = displacement;
                force *= k * (length - restLength) / length;
                return force;
            }
        }
        return Vector3(0, 0, 0);
    }
};
