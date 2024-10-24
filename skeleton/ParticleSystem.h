#pragma once
#include <vector>
#include "RenderUtils.hpp"
#include "ParticleGenerator.h"

using namespace std;

class ParticleSystem
{
private:
	vector<ParticleGenerator*>generators;
public:
	ParticleSystem() {}

	void addGenerator(Vector3 pos, DistributionType distributionType, int dipersion_area_x_, int dipersion_area_y_, double particleLifetime) {
		generators.push_back(new ParticleGenerator(pos, distributionType, dipersion_area_x_, dipersion_area_y_, particleLifetime));
	}

	void update(double t) {
		for (auto e : generators) {
			e->update(t);
		}
	}
};
