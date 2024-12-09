#pragma once
#include "Particle.h"
#include <list>
#include <functional> 

class ParticleSystem;

enum class DistributionType {
	Gaussian,
	Uniform
};

class ParticleGenerator
{
private:
    std::function<Vector3()> getPositionCallback; // Callback para obtener posición dinámica
    DistributionType distributionType;
    int dispersion_area_x, dispersion_area_y;
    double particleLifetime;
    bool generateSingleParticle;
    bool singleParticleGenerated = false;
    bool numFiniteParticles = false;
    int numParticles = 0;
    int numMaxParticles;

    float generateGaussian(float mean, float stddev);
    float generateUniform(float min, float max);
    Vector4 GenerateRandomColorYR();

public:
    ParticleGenerator(std::function<Vector3()> positionCallback,
        DistributionType distributionType,
        int dispersion_area_x_,
        int dispersion_area_y_,
        double particleLifetime_,
        bool generateSingleParticle_,
        bool numFiniteParticles_,
        int numMaxParticles_)
        : getPositionCallback(positionCallback),
        dispersion_area_x(dispersion_area_x_),
        dispersion_area_y(dispersion_area_y_),
        particleLifetime(particleLifetime_),
        generateSingleParticle(generateSingleParticle_),
        numFiniteParticles(numFiniteParticles_),
        numMaxParticles(numMaxParticles_) {
    }

    ~ParticleGenerator() {}

    void update(ParticleSystem* ps, double t);
};
