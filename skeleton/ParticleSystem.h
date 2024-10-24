#pragma once
#include <vector>
#include "RenderUtils.hpp"
#include "ParticleGenerator.h"

using namespace std;

class ParticleSystem
{
private:
	vector<ParticleGenerator*>generador;
public:
	ParticleSystem() {
		generador.push_back(new ParticleGenerator(Vector3(0, 0, 0)));
	}
	void addGenerator() {
		generador.push_back(new ParticleGenerator(Vector3(0, 0, 0)));
	}
	void update(double t) {
		for (auto e : generador) {
			e->update(t);
		}
	}
};
