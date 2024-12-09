#pragma once
#include <vector>
#include "RenderUtils.hpp"
#include "ParticleGenerator.h"
#include "ForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "BidirectionalSpringForceGenerator.h"

using namespace std;

class ParticleSystem {
private:
    list<ForceGenerator*> forceGenerators;
    vector<ParticleGenerator*> generators;
    bool activeExplosion;

protected:
    list<Particle*> particles;

public:
    ParticleSystem() : activeExplosion(false) {}

    void addGenerator(std::function<Vector3()> positionCallback,
        DistributionType distributionType,
        int dispersion_area_x_,
        int dispersion_area_y_,
        double particleLifetime,
        bool singleParticleGenerator,
        bool finiteParticles,
        int numMaxParticles) {
        generators.push_back(new ParticleGenerator(positionCallback, distributionType, dispersion_area_x_,
            dispersion_area_y_, particleLifetime, singleParticleGenerator,
            finiteParticles, numMaxParticles));
    }


    void addForceGenerator(ForceGenerator* fg) {
        forceGenerators.push_back(fg);
    }

    void addSpringForceBetweenParticles(Particle* p1, Particle* p2, float k, float restLength) {
        forceGenerators.push_back(new BidirectionalSpringForceGenerator(p1, p2, k, restLength));
    }

    void addParticle(Vector3 InitialPosition, int velocityX, int velocityY, int velocityZ, double Damping, int lifetime_, Vector4 color) {
        particles.push_back(new Particle(InitialPosition, Vector3(velocityX, velocityY, velocityZ), Vector3(0, 0, 0), Damping, lifetime_, color));
    }

    void addParticle(Particle* p) {
        particles.push_back(p);
    }

    void addSingleParticle(Vector3 InitialPosition, int velocityX, int velocityY, int velocityZ, double Damping, int lifetime_, double mass_) {
        particles.push_back(new Particle(InitialPosition, Vector3(velocityX, velocityY, velocityZ), Vector3(0, 0, 0), Damping, lifetime_, mass_));
    }

    std::list<Particle*> getParticles() { return particles; }

    void update(double t) {
        for (auto it = forceGenerators.begin(); it != forceGenerators.end(); ) {
            (*it)->update(t, this);
            if (!(*it)->isAlive()) {
                delete* it;
                it = forceGenerators.erase(it);
            }
            else {
                ++it;
            }
        }

        if (!activeExplosion) {
            for (auto g : generators) {
                g->update(this, t);
            }
        }

        for (auto it = particles.begin(); it != particles.end(); ) {
            applyForces(*it);
            (*it)->integrate(t);

            if ((*it)->isDead() || (*it)->isOutOfBounds(100, 300)) {
                delete* it;
                it = particles.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    void applyForces(Particle* p) {
        Vector3 totalForce(0, 0, 0);
        for (auto& fg : forceGenerators) {
            totalForce += fg->newForce(p);
        }
        p->setForce(totalForce);
    }

    void ActiveExplosion(bool active) { activeExplosion = active; }

    void clearExplosionForceGenerators() {
        for (auto it = forceGenerators.begin(); it != forceGenerators.end(); ) {
            if (dynamic_cast<ExplosionForceGenerator*>(*it) != nullptr) {
                delete* it;
                it = forceGenerators.erase(it);
            }
            else {
                ++it;
            }
        }
    }
};
