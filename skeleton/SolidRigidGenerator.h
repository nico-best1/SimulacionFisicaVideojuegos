#pragma once
#include <random>
#include "core.hpp"
#include "RenderUtils.hpp"
#include "PxPhysics.h"
#include "geometry/PxGeometry.h"

class SolidRigidSystem;

class SolidRigidGenerator {
private:
    physx::PxGeometry* geometryTemplate;
    physx::PxMaterial* material;
    unsigned int maxGenerated;
    unsigned int currentGenerated;
    float minMass, maxMass;
    float minVel, maxVel;
    float minAngVel, maxAngVel;
    float minX, maxX, minY, maxY, minZ, maxZ;

    float generateRandom(float min, float max) {
        static std::default_random_engine generator;
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }

public:
    SolidRigidGenerator(physx::PxGeometry* geoTemplate, physx::PxMaterial* mat, unsigned int maxGen,
        float minMass_, float maxMass_,
        float minVel_, float maxVel_,
        float minAngVel_, float maxAngVel_,
        float minX_, float maxX_, float minY_, float maxY_, float minZ_, float maxZ_)
        : geometryTemplate(geoTemplate), material(mat), maxGenerated(maxGen), currentGenerated(0),
        minMass(minMass_), maxMass(maxMass_), minVel(minVel_), maxVel(maxVel_), minAngVel(minAngVel_), maxAngVel(maxAngVel_),
        minX(minX_), maxX(maxX_), minY(minY_), maxY(maxY_), minZ(minZ_), maxZ(maxZ_) {
    }

    void generate(SolidRigidSystem* SRs);
};
