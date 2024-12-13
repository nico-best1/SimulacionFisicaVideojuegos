#pragma once
#include "BidirectionalSpringForceGenerator.h"
#include <random>
#include "core.hpp"
#include "RenderUtils.hpp"
#include "PxPhysics.h"
#include "geometry/PxGeometry.h"

class SolidRigidSystem;

class SolidRigidGenerator {
private:
    SolidRigid* solid1;
    SolidRigid* solid2;
    physx::PxGeometry* geometryTemplate;
    physx::PxMaterial* material;
    unsigned int maxGenerated;
    unsigned int currentGenerated;
    float minMass, maxMass;
    float minVel, maxVel;
    float minAngVel, maxAngVel;
    float minX, maxX, minY, maxY, minZ, maxZ;
    float minElasticity, maxElasticity; 
    float minFriction, maxFriction;    

    float generateRandom(float min, float max) {
        static std::default_random_engine generator;
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }

public:
    SolidRigidGenerator(physx::PxGeometry* geoTemplate, unsigned int maxGen,
        float minMass_, float maxMass_,
        float minVel_, float maxVel_,
        float minAngVel_, float maxAngVel_,
        float minX_, float maxX_, float minY_, float maxY_, float minZ_, float maxZ_,
        float minElasticity_, float maxElasticity_, float minFriction_, float maxFriction_)
        : geometryTemplate(geoTemplate), maxGenerated(maxGen), currentGenerated(0),
        minMass(minMass_), maxMass(maxMass_), minVel(minVel_), maxVel(maxVel_),
        minAngVel(minAngVel_), maxAngVel(maxAngVel_),
        minX(minX_), maxX(maxX_), minY(minY_), maxY(maxY_), minZ(minZ_), maxZ(maxZ_),
        minElasticity(minElasticity_), maxElasticity(maxElasticity_),
        minFriction(minFriction_), maxFriction(maxFriction_) {
    }

    ~SolidRigidGenerator() {
        if (geometryTemplate != nullptr) {
            delete geometryTemplate;
            geometryTemplate = nullptr;
        }
    }


    void generate(SolidRigidSystem* SRs);
};