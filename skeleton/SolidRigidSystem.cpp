#include "SolidRigidSystem.h"
#include "SolidRigidGenerator.h"

void SolidRigidSystem::addGenerator(SolidRigidGenerator* generator) {
    generators.push_back(generator);
}

void SolidRigidSystem::addForceGenerator(ForceGenerator* fgenerator) {
    forceGenerators.push_back(fgenerator);
}
