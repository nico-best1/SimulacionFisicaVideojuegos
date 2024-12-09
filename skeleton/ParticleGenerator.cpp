#include "ParticleGenerator.h"
#include "ParticleSystem.h"
#include <random>

void ParticleGenerator::update(ParticleSystem* ps, double t)
{
    if (generateSingleParticle && singleParticleGenerated) return;

    Vector3 currentPosition = getPositionCallback(); // Obtener la posición dinámica
    float velocityX, velocityY, velocityZ;

    if (distributionType == DistributionType::Gaussian) {
        velocityX = generateGaussian(0.0, 2);
        velocityY = generateGaussian(0.0, 2);
        velocityZ = generateGaussian(0.0, 2);
    }
    else {
        velocityX = generateUniform(-10.0, 10.0);
        velocityY = generateUniform(-5.0, 5.0);
        velocityZ = generateUniform(-10.0, 10.0);
    }

    if (generateSingleParticle) {
        ps->addSingleParticle(currentPosition, velocityX, velocityY, velocityZ, 0.98, 100, 50);
        singleParticleGenerated = true;
    }
    else if (numFiniteParticles && numParticles < numMaxParticles) {
        ps->addParticle(currentPosition, velocityX, velocityY, velocityZ, 0.98, particleLifetime);
        numParticles++;
    }
    else if (!generateSingleParticle && !numFiniteParticles) {
        ps->addParticle(currentPosition, velocityX, velocityY, velocityZ, 0.98, particleLifetime);
    }
}



float ParticleGenerator::generateGaussian(float mean, float stddev)
{
	static std::default_random_engine generator;
	std::normal_distribution<float>distribution(mean, stddev);
	return distribution(generator);
}

float ParticleGenerator::generateUniform(float min, float max)
{
    static std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}