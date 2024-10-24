#pragma once
#include "Particle.h"
#include <list>

const int AREA_Y = 20, AREA_X = 5;

class ParticleGenerator
{
public:
	ParticleGenerator(Vector3 p) :iniPos(p) {};

	~ParticleGenerator() {
		for (auto p : particulas) {
			delete p;
		}
	}

	void update(double t);

	float generateGausssian(float mean, float stddev);
private:
	Vector3 iniPos;
	std::list<Particle*>particulas;
};