#pragma once
#include <vector>
#include "RenderUtils.hpp"
#include "ParticleGenerator.h"
#include "ForceGenerator.h"

using namespace std;

class ParticleSystem
{
private:
	vector<ForceGenerator*> forceGenerators;
	vector<ParticleGenerator*>generators;
protected:
	std::list<Particle*>particles;
public:
	ParticleSystem() {}

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
		for (auto e : generators) {
			e->update(this, t);
		}

		for (auto it = particles.begin(); it != particles.end(); ) {
			for (auto& fg : forceGenerators) {
				fg->updateForce((*it), t);
			}
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
};
