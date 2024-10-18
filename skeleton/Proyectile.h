#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include "Particle.h"

class Proyectile : public Particle {
private:
    double mass;
    Vector3 gravity;
public:
    Proyectile(double _mass, Vector3 Pos, Vector3 Vel, Vector3 A, double Damping, double real_velocity, double simulated_velocity)
        : mass(_mass), Particle(Pos, Vel, A, Damping, 10) {
        mass = _mass * pow(real_velocity / simulated_velocity, 2);
        double gravity_value = 9.81 * pow(simulated_velocity / real_velocity, 2);
        gravity = Vector3(0, -abs(gravity_value), 0);
    }

    ~Proyectile() {}

     virtual void integrate(double t) {
         vel += (a + gravity) * t;
         vel *= pow(damping, t);
         pose.p += vel * t;
    }
};
