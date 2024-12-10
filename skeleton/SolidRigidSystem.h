#pragma once
#include "SolidRigid.h"
#include "SolidRigidGenerator.h"
#include "ForceGenerator.h"
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
    std::list<ForceGenerator*> forceGenerators;
    unsigned int maxSolids;
    bool activeExplosion;
    float generationInterval; 
    float timeSinceLastGeneration; 
    float timeSinceLastgenerationIncrease;

    float generateRandom(float min, float max) {
        static std::default_random_engine generator;
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }

public:
    SolidRigidSystem(physx::PxScene* scene_, physx::PxPhysics* physics_, physx::PxMaterial* material, unsigned int maxSolids_, float generationInterval_)
        : scene(scene_), physics(physics_), defaultMaterial(material), maxSolids(maxSolids_), generationInterval(generationInterval_), timeSinceLastGeneration(0.0f), timeSinceLastgenerationIncrease(0.0f){
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

    SolidRigid* SolidRigidSystem::addSolid(physx::PxGeometry* geo, physx::PxTransform transform, Vector3 linVel, Vector3 angVel, float mass, physx::PxMaterial* material = nullptr) {
        if (solids.size() >= maxSolids) return nullptr;

        if (!material) material = defaultMaterial;

        SolidRigid* newSolid = new SolidRigid(scene, geo, transform, {0,0,0}, { 0,0,0 }, mass, material, {1,1,0,1});
        newSolid->setSolidInScene();

        physx::PxRigidDynamic* dynamicActor = newSolid->getSolid();
        if (dynamicActor) {
            dynamicActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
        }

        newSolid->getSolid()->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
            physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
            physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
        solids.push_back(newSolid);

        return newSolid;
    }


    void addStaticObstacle(physx::PxGeometry* geo, physx::PxTransform transform) {
        physx::PxRigidStatic* staticObstacle = physics->createRigidStatic(transform);
        physx::PxShape* shape = CreateShape(*geo, defaultMaterial);
        staticObstacle->attachShape(*shape);
        scene->addActor(*staticObstacle);
    }

    void addGenerator(SolidRigidGenerator* generator);

    void addForceGenerator(ForceGenerator* generator);

    void setExplosion(bool active) { activeExplosion = active; };

    void updateGenerators() {
        for (auto generator : generators) {
            generator->generate(this);
        }
    }

    void update(double deltaTime) {
        timeSinceLastGeneration += static_cast<float>(deltaTime);
        timeSinceLastgenerationIncrease += static_cast<float>(deltaTime);
        if (timeSinceLastGeneration >= generationInterval) {
            updateGenerators();
            timeSinceLastGeneration = 0.0f; 
        }

        if (timeSinceLastgenerationIncrease >= 10.0f && generationInterval > 2.0f) {
            generationInterval-= 0.5f;
        }

        for (auto it = solids.begin(); it != solids.end(); ) {
            applyForces(*it);
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

    void SolidRigidSystem::applyForces(SolidRigid* p) {
        Vector3 totalForce(0, 0, 0);
        Vector3 totalTorque(0, 0, 0);
        auto it = forceGenerators.begin();

        while (it != forceGenerators.end()) {
            if ((*it)->isAlive()) {
                totalForce += (*it)->newForceSolid(p);
                totalTorque += (*it)->newTorqueSolid(p); 
                ++it;
            }
            else {
                auto aux = it;
                ++it;
                delete* aux;
                forceGenerators.erase(aux);
            }
        }

        if (p->getSolid()) {
            p->getSolid()->addForce(totalForce, physx::PxForceMode::eFORCE);
            p->getSolid()->addTorque(totalTorque, physx::PxForceMode::eFORCE); 
        }
    }

};