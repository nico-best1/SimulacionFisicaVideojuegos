#pragma once
#include "SolidRigid.h"
#include "TemporaryForceGenerator.h"

class Player : public SolidRigid {
private:
    TemporaryForceGenerator* jumpForce = nullptr;
    float maxSpeed = 30.0f;
    Vector3 lastVelocity;

public:
    Player(physx::PxScene* scene, physx::PxGeometry* geo, physx::PxTransform transform,
        physx::PxVec3 linVel, physx::PxVec3 angVel, float mass, physx::PxMaterial* material)
        : SolidRigid(scene, geo, transform, linVel, angVel, mass, material, {1,0,0,1}) {
        this->getSolid()->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
            physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
            physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
    }

    ~Player() {
        if (jumpForce) {
            delete jumpForce; 
            jumpForce = nullptr;
        }
    }

    void setPosition(const physx::PxTransform& newPosition) {
        setPose(newPosition);
        getSolid()->setGlobalPose(newPosition);
    }

    Vector3 getPosition() {
        physx::PxTransform currentPose = getSolid()->getGlobalPose();
        return Vector3(currentPose.p.x - 2, currentPose.p.y - 5, currentPose.p.z);
    }

    void jump(const Vector3& force, float duration) {
        Vector3 currentVelocity = getSolid()->getLinearVelocity();
        if (currentVelocity.y < maxSpeed) {
            if (jumpForce) {
                delete jumpForce;
            }
            jumpForce = new TemporaryForceGenerator(force, duration);
            Vector3 newForce = jumpForce->newForceSolid(this);
            getSolid()->addForce(Vector3(newForce.x, newForce.y, newForce.z), physx::PxForceMode::eIMPULSE);
        }
    }

    void update() {
        Vector3 currentVelocity = getSolid()->getLinearVelocity();

        if (currentVelocity.y < lastVelocity.y) {
            getSolid()->addForce(Vector3(0, -10, 0), physx::PxForceMode::eIMPULSE);
        }

        lastVelocity = currentVelocity;
    }
};
