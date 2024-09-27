#include "Particle.h"

void Particle::integrate(double t) {
    vel += a * t;
    pose.p += vel * t; 
}