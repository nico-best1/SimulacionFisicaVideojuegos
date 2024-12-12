#pragma once
#include <PxPhysicsAPI.h>
#include <iostream>
class Scene
{
protected:
	physx::PxScene* scene;
	physx::PxPhysics* physics;
	physx::PxMaterial* material;
public:
	Scene(physx::PxScene* scene_, physx::PxPhysics* physics_, physx::PxMaterial* material_) :
		scene(scene_), physics(physics_), material(material_) {}
};

