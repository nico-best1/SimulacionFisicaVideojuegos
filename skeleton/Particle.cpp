#include "Particle.h"

void Particle::integrate(double t) {
    currentLifetime += t; 
    if (currentLifetime >= lifetime) return;

    vel += a * t;
    vel *= pow(damping, t);
    pose.p += vel * t; 
}