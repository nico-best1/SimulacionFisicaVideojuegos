#pragma once
#include "Particle.h"
#include <list>
class ParticleSystem;

enum class DistributionType {
	Gaussian,
	Uniform
};

class ParticleGenerator
{
private:
	Vector3 initialPosition;
	DistributionType distributionType;
	int dipersion_area_x, dipersion_area_y;
	double particleLifetime;
	bool generateSingleParticle; 
	bool singleParticleGenerated = false;
	bool numFiniteParticles = false;
	int numParticles = 0;
	int numMaxParticles;

	float generateGaussian(float mean, float stddev);
	float generateUniform(float min, float max);
public:
	ParticleGenerator(Vector3 p, DistributionType distributionType, int dipersion_area_x_, int dipersion_area_y_, double particleLifetime_, bool generateSingleParticle_, bool numFiniteParticles_, int numMaxParticles_)
		:initialPosition(p), dipersion_area_x(dipersion_area_x_), dipersion_area_y(dipersion_area_y_), particleLifetime(particleLifetime_), generateSingleParticle(generateSingleParticle_), numFiniteParticles(numFiniteParticles_), numMaxParticles(numMaxParticles_) {};

	~ParticleGenerator() {}

	void update(ParticleSystem* Psys, double t);
};