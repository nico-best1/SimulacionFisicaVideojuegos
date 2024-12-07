#include "SolidRigidSystem.h"
#include "SolidRigidGenerator.h"

void SolidRigidSystem::addGenerator(SolidRigidGenerator* generator) {
    generators.push_back(generator);
}
