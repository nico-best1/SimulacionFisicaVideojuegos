#include "Particle.h"

void Particle::integrate(double t) {
    vel += a * t;
    vel *= pow(damping, t);
    pose.p += vel * t; 
}