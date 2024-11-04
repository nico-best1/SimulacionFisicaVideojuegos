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
public:
	ParticleSystem() {}

	void addGenerator(Vector3 pos, DistributionType distributionType, int dipersion_area_x_, int dipersion_area_y_, double particleLifetime) {
		generators.push_back(new ParticleGenerator(pos, distributionType, dipersion_area_x_, dipersion_area_y_, particleLifetime));
	}

	void addForceGenerator(ForceGenerator* fg) {
		forceGenerators.push_back(fg);
	}

	void update(double t) {
		for (auto e : generators) {
			e->update(t);
		}

		for (auto& gen : generators) {
			for (auto& particle : gen->getParticles()) {
				for (auto& fg : forceGenerators) {
					fg->updateForce(particle, t);
				}
			}
		}
	}
};
