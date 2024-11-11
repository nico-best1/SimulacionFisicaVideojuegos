#pragma once
#include "ForceGenerator.h"

class SpringForceGenerator : public ForceGenerator {
private:
    Vector3 anchor; 
    float k;        
    float restLength; 

public:
    SpringForceGenerator(const Vector3& anchor, float k, float restLength)
        : ForceGenerator(INT_MAX), anchor(anchor), k(k), restLength(restLength) {}

    Vector3 newForce(Particle* p) override {
        Vector3 position = p->getPosition();
        Vector3 displacement = position - anchor;
        float length = displacement.magnitude();

        Vector3 force = displacement;
        force *= -k * (length - restLength) / length; 

        return force;
    }


    void setKConstant(float newK) { k = newK; } 

    void AddKConstant(float newK) { k += newK; } 
    void SubKConstant(float newK) { k -= newK; } 
};
