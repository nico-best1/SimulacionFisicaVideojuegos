#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include "PxPhysics.h"
#include "geometry/PxGeometry.h"
class SolidRigid
{
protected:
	physx::PxShape* shape;
	physx::PxScene* scene;
	physx::PxTransform pose;
	physx::PxRigidDynamic* solid;
	float lifeTime;
	RenderItem* render_item;
public:
	SolidRigid(physx::PxScene* scene, physx::PxGeometry* geo, physx::PxTransform transform, physx::PxVec3 linVel, physx::PxVec3 angVel, float mass, physx::PxMaterial* material, Vector4 color);
	SolidRigid(SolidRigid& const s);

	~SolidRigid() {
		solid->release();
		if (render_item != nullptr) {
			DeregisterRenderItem(render_item);
		}
	}

	void integrate(double t);
	physx::PxVec3 calculateInertiaTensor(float mass);

	bool isDead() const {
		return lifeTime <= 0;
	}

	SolidRigid& operator=(const SolidRigid& p) {
		solid = p.solid;
		shape = p.shape;
		scene = p.scene;
		return *this;
	}

	// Get
	physx::PxRigidDynamic* getSolid() const { return solid; }
	physx::PxTransform getPose() const { return pose; }
	physx::PxShape* getShape() const { return shape; }
	physx::PxPhysics* getGPhysics() const { return &scene->getPhysics(); }
	physx::PxMaterial* getMaterial() const {
		if (shape != nullptr) {
			physx::PxMaterial* material = nullptr;
			shape->getMaterials(&material, 1);
			return material;
		}
		return nullptr;
	}

	// Set
	void setPose(physx::PxTransform newPose) { pose = newPose; };
	void setUpdateSolid(double inercia) { physx::PxRigidBodyExt::setMassAndUpdateInertia(*solid, inercia); }
	void setSolidInScene() { scene->addActor(*solid); };
};