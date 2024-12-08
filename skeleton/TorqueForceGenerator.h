#include "ForceGenerator.h"

class TorqueForceGenerator : public ForceGenerator {
private:
    Vector3 force;
    Vector3 applicationPoint;

public:
    TorqueForceGenerator(float duration, const Vector3& force_, const Vector3& applicationPoint_)
        : ForceGenerator(duration), force(force_), applicationPoint(applicationPoint_) {
    }

    Vector3 newForce(Particle* p) override { return Vector3(0, 0, 0); }

    Vector3 newForceSolid(SolidRigid* sr) override {
        return Vector3(0);
    }

    Vector3 newTorqueSolid(SolidRigid* sr) override {
        if (!sr || !sr->getSolid()) return Vector3(0);

        physx::PxRigidDynamic* rigid = sr->getSolid();
        Vector3 centerOfMass = rigid->getGlobalPose().p;

        return (applicationPoint - centerOfMass).cross(force);
    }

    void updateSolid(double t, SolidRigidSystem* sSys) override {
        duration -= t;
        if (duration <= 0) alive = false;
    }
};
