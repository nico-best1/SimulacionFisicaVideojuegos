#pragma once
#include "ForceGenerator.h"
#include "Particle.h"

class FloatationForceGenerator : public ForceGenerator {
private:
    float liquid_density = 1000.0f;   
    float volume;                     
    float height;                    
    float water_height;                      

public:
    FloatationForceGenerator(float volume, float height, float water_height)
        : ForceGenerator(INT_MAX), volume(volume), height(height), water_height(water_height) {}

    Vector3 newForce(Particle* p) override {
        float h = p->getPosition().y;

        float immersed;
        if (h - water_height > height * 0.5f) {
            immersed = 0.0f;
        }
        else if (water_height - h > height * 0.5f) {
            immersed = 1.0f;
        }
        else {
            immersed = (water_height - h) / height + 0.5f;
        }

        float buoyant_force = liquid_density * volume * immersed;
        Vector3 force(0, buoyant_force, 0); 

        return force;
    }

    Vector3 newForceSolid(SolidRigid* sr) override {
        float h = sr->getSolid()->getGlobalPose().p.y;

        physx::PxVec3 f(0, 0, 0);
        float inmersed = 0.0;
        if (h - water_height > height * 0.5f) {
            inmersed = 0.0f;
        }
        else if (water_height - h > height * 0.5f) {
            inmersed = 1.0f;
        }
        else {
            inmersed = (water_height - h) / height + 0.5f;
        }
        f.y = liquid_density * volume * inmersed;
        return f;
    }

    void setVolume(float increment_volume) { 
        if (volume > 0.1f) {
            volume += increment_volume;
        }
    }
};
