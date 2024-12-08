#pragma once
#include "SolidRigid.h"
#include "SolidRigidGenerator.h"
#include <list>
#include <vector>
#include <random>

class SolidRigidSystem {
private:
    physx::PxScene* scene;
    physx::PxPhysics* physics;
    physx::PxMaterial* defaultMaterial;
    std::vector<SolidRigid*> solids;
    std::list<SolidRigidGenerator*> generators;
    unsigned int maxSolids;

    float generateRandom(float min, float max) {
        static std::default_random_engine generator;
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }

public:
    SolidRigidSystem(physx::PxScene* scene_, physx::PxPhysics* physics_, physx::PxMaterial* material, unsigned int maxSolids_)
        : scene(scene_), physics(physics_), defaultMaterial(material), maxSolids(maxSolids_) {
    }

    ~SolidRigidSystem() {
        for (auto solid : solids) {
            delete solid;
        }
        solids.clear();

        for (auto generator : generators) {
            delete generator;
        }
        generators.clear();
    }

    physx::PxMaterial* createMaterial(float elasticity, float friction) {
        return physics->createMaterial(friction, friction, elasticity);
    }

    void addSolid(physx::PxGeometry* geo, physx::PxTransform transform, physx::PxVec3 linVel, physx::PxVec3 angVel, float mass, physx::PxMaterial* material = nullptr) {
        if (solids.size() >= maxSolids) return;

        if (!material) material = defaultMaterial;

        SolidRigid* newSolid = new SolidRigid(scene, geo, transform, linVel, angVel, mass, material);
        newSolid->setSolidInScene();
        solids.push_back(newSolid);
    }

    void addStaticObstacle(physx::PxGeometry* geo, physx::PxTransform transform) {
        physx::PxRigidStatic* staticObstacle = physics->createRigidStatic(transform);
        physx::PxShape* shape = CreateShape(*geo, defaultMaterial);
        staticObstacle->attachShape(*shape);
        scene->addActor(*staticObstacle);
    }

    void addGenerator(SolidRigidGenerator* generator);

    void updateGenerators() {
        for (auto generator : generators) {
            generator->generate(this);
        }
    }

    void update(double deltaTime) {
        updateGenerators();
        for (auto it = solids.begin(); it != solids.end(); ) {
            (*it)->integrate(deltaTime);

            if ((*it)->isDead()) {
                delete* it;
                it = solids.erase(it);
            }
            else {
                ++it;
            }
        }
    }
};