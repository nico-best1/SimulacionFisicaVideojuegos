#include "Particle.h"

void Particle::integrate(double t) {
    pose.p += vel * t; 
}