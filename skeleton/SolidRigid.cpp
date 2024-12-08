#include "SolidRigid.h"
SolidRigid::SolidRigid(physx::PxScene* sc, physx::PxGeometry* geo, physx::PxTransform transform, physx::PxVec3 linVel, physx::PxVec3 angVel, float mass, physx::PxMaterial* material) :scene(sc), pose(transform), lifeTime(20){
    solid = scene->getPhysics().createRigidDynamic(transform);
    solid->setLinearVelocity(linVel);
    solid->setAngularVelocity(angVel);
    shape = CreateShape(*geo, material);
    solid->attachShape(*shape);
    Vector3 inertiaTensor = calculateInertiaTensor(mass);
    solid->setMassSpaceInertiaTensor(inertiaTensor);
    solid->setMass(mass);
    render_item = new RenderItem(shape, solid, { 0.8,0.8,0.8,1 });
}

SolidRigid::SolidRigid(SolidRigid& const solid_) {
    *this = solid_;
}

physx::PxVec3 SolidRigid::calculateInertiaTensor(float objectMass) {
    physx::PxGeometryType::Enum geometryType = shape->getGeometryType();

    switch (geometryType) {
    case physx::PxGeometryType::eBOX: {
        physx::PxBoxGeometry boxGeom;
        shape->getBoxGeometry(boxGeom);
        float width = 2 * boxGeom.halfExtents.x;
        float height = 2 * boxGeom.halfExtents.y;
        float depth = 2 * boxGeom.halfExtents.z;
        float momentX = (1.0f / 12.0f) * objectMass * (height * height + depth * depth);
        float momentY = (1.0f / 12.0f) * objectMass * (width * width + depth * depth);
        float momentZ = (1.0f / 12.0f) * objectMass * (width * width + height * height);
        return physx::PxVec3(momentX, momentY, momentZ);
    }
    case physx::PxGeometryType::eSPHERE: {
        physx::PxSphereGeometry sphereGeom;
        shape->getSphereGeometry(sphereGeom);
        float sphereRadius = sphereGeom.radius;
        float inertia = (2.0f / 5.0f) * objectMass * sphereRadius * sphereRadius;
        return physx::PxVec3(inertia, inertia, inertia);
    }
    case physx::PxGeometryType::eCAPSULE: {
        physx::PxCapsuleGeometry capsuleGeom;
        shape->getCapsuleGeometry(capsuleGeom);
        float capRadius = capsuleGeom.radius;
        float capHalfHeight = capsuleGeom.halfHeight;
        float inertiaX = (1.0f / 12.0f) * objectMass * (3 * capRadius * capRadius + 4 * capHalfHeight * capHalfHeight);
        float inertiaZ = inertiaX;
        float inertiaY = (1.0f / 2.0f) * objectMass * capRadius * capRadius;
        return physx::PxVec3(inertiaX, inertiaY, inertiaZ);
    }
    default:
        return physx::PxVec3(1.0f, 1.0f, 1.0f);
    }
}
void SolidRigid::integrate(double t) {
    if (lifeTime <= 0) return;
    lifeTime -= t;
}