#include "SolidRigidGenerator.h"
#include "SolidRigidSystem.h"

void SolidRigidGenerator::generate(SolidRigidSystem* SRs) {
    if (currentGenerated >= maxGenerated) return;

    float mass = generateRandom(minMass, maxMass);
    physx::PxTransform transform(physx::PxVec3(generateRandom(minX, maxX), generateRandom(minY, maxY), generateRandom(minZ, maxZ)));
    physx::PxVec3 linVel(generateRandom(minVel, maxVel), generateRandom(minVel, maxVel), generateRandom(minVel, maxVel));
    physx::PxVec3 angVel(generateRandom(minAngVel, maxAngVel), generateRandom(minAngVel, maxAngVel), generateRandom(minAngVel, maxAngVel));

    float elasticity = generateRandom(minElasticity, maxElasticity);
    float friction = generateRandom(minFriction, maxFriction);

    physx::PxMaterial* customMaterial = SRs->createMaterial(elasticity, friction);
    SRs->addSolid(geometryTemplate, transform, linVel, angVel, mass, customMaterial);
    currentGenerated++;
}