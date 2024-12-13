#include "SolidRigidGenerator.h"
#include "SolidRigidSystem.h"

void SolidRigidGenerator::generate(SolidRigidSystem* SRs) {
    if (currentGenerated >= maxGenerated) return;

    float mass = generateRandom(minMass, maxMass);
    float elasticity = generateRandom(minElasticity, maxElasticity);
    float friction = generateRandom(minFriction, maxFriction);
    srand(static_cast<unsigned int>(time(0)));
    float height = 10.0f + static_cast<float>(rand() % 30);
    PxBoxGeometry* cubeGeometry = new PxBoxGeometry(10.0f, height, 10.0f);
    geometryTemplate = cubeGeometry;
    physx::PxMaterial* customMaterial = SRs->createMaterial(elasticity, friction);
    physx::PxTransform transform(physx::PxVec3(generateRandom(minX, maxX), generateRandom(minY, maxY), generateRandom(minZ, maxZ)));
    physx::PxVec3 linVel(generateRandom(minVel, maxVel), generateRandom(minVel, maxVel), generateRandom(minVel, maxVel));
    physx::PxVec3 angVel(generateRandom(minAngVel, maxAngVel), generateRandom(minAngVel, maxAngVel), generateRandom(minAngVel, maxAngVel));
    //SRs->addSolid(geometryTemplate, transform, linVel, angVel, mass, customMaterial);
    //currentGenerated++;
   if (currentGenerated % 5 == 0) {
        PxBoxGeometry* cubeGeometry1 = new PxBoxGeometry(10.0f, 10.0f, 10.0f);
        physx::PxTransform transform1(Vector3(200, 80, 0));
        solid1 = SRs->addSolid(cubeGeometry1, transform1, { 0,0,0 }, { 0,0,0 }, 10, customMaterial);
        physx::PxTransform transform2(Vector3(200, 0, 0));
        solid2 = SRs->addSolid(cubeGeometry1, transform2, { 0,0,0 }, { 0,0,0 }, 10, customMaterial);

        float springConstant = 150.0f; 
        float restLength = 60.0f;    

        BidirectionalSpringForceGenerator* spring = new BidirectionalSpringForceGenerator(solid1, solid2, springConstant, restLength);
        SRs->addForceGenerator(spring);
        currentGenerated += 2;
   }
    else {
        SRs->addSolid(geometryTemplate, transform, linVel, angVel, mass, customMaterial);
        currentGenerated++;
    }
}