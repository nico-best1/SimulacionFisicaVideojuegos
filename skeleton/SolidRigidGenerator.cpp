#include "SolidRigidGenerator.h"
#include "SolidRigidSystem.h"

void SolidRigidGenerator::generate(SolidRigidSystem* SRs) {
    if (currentGenerated >= maxGenerated) return;

    float mass = generateRandom(minMass, maxMass);
    float elasticity = generateRandom(minElasticity, maxElasticity);
    float friction = generateRandom(minFriction, maxFriction);
    srand(static_cast<unsigned int>(time(0)));
    float height = 10.0f + static_cast<float>(rand() % 30);
    physx::PxTransform transform(physx::PxVec3(generateRandom(minX, maxX), generateRandom(minY, maxY), generateRandom(minZ, maxZ)));
    PxBoxGeometry* cubeGeometry = new PxBoxGeometry(10.0f, height, 10.0f);
    geometryTemplate = cubeGeometry;
    physx::PxMaterial* customMaterial = SRs->createMaterial(elasticity, friction);
    physx::PxVec3 linVel(generateRandom(minVel, maxVel), generateRandom(minVel, maxVel), generateRandom(minVel, maxVel));
    physx::PxVec3 angVel(generateRandom(minAngVel, maxAngVel), generateRandom(minAngVel, maxAngVel), generateRandom(minAngVel, maxAngVel));
    if (transform.p.y + height < 110 && transform.p.y + height > -40) {
        SRs->addSolid(geometryTemplate, transform, linVel, angVel, mass, customMaterial);
        currentGenerated++;
    }
}