#pragma once
#include <vector>
#include "RenderUtils.hpp"
#include "ParticleGenerator.h"
#include "ForceGenerator.h"

using namespace std;

class ParticleSystem
{
private:
	std::list<ForceGenerator*> forceGenerators;
	vector<ParticleGenerator*>generators;
	bool activeExplosion;
protected:
	std::list<Particle*>particles;
public:
	ParticleSystem() { activeExplosion = false; }

	void addGenerator(Vector3 pos, DistributionType distributionType, int dipersion_area_x_, int dipersion_area_y_, double particleLifetime) {
		generators.push_back(new ParticleGenerator(pos, distributionType, dipersion_area_x_, dipersion_area_y_, particleLifetime));
	}

	void addForceGenerator(ForceGenerator* fg) {
		forceGenerators.push_back(fg);
	}

	void addParticle(Vector3 InitialPosition, int velocityX, int velocityY, int velocityZ, double Damping, int lifetime_) {
		particles.push_back(new Particle(InitialPosition, Vector3(velocityX, velocityY, velocityZ), Vector3(0, 0, 0), Damping, lifetime_));
	}

	std::list<Particle*> getParticles() { return particles; }

	void update(double t) {
		if (!activeExplosion) {
			for (auto g : generators) {
				g->update(this, t);
			}
		}

		for (auto it = particles.begin(); it != particles.end(); ) {
			for (auto& fg : forceGenerators) {
				fg->update(t, this);
			}
			applyForces(*it);
			(*it)->integrate(t);
			if ((*it)->isDead() || (*it)->isOutOfBounds(100, 300)) {
				auto aux = it;
				++it;
				delete (*aux);
				particles.erase(aux);
			}
			else {
				++it;
			}

		}
	}

	void ParticleSystem::applyForces(Particle* p) {
		physx::PxVec3 totalForce(0, 0, 0);
		auto it = forceGenerators.begin();
		while (it != forceGenerators.end()) {
			if ((*it)->isAlive()) {
				totalForce += (*it)->calculateForce(p);
				++it;
			}
			else {
				auto aux = it;
				++it;
				delete* aux;
				forceGenerators.erase(aux);
			}
		}
		p->setForce(totalForce);
	}

	void setExplosion(bool active) { activeExplosion = active; };
};
