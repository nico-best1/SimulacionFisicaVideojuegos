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
        ps->addParticle(currentPosition, velocityX, velocityY, velocityZ, 0.98, particleLifetime, GenerateRandomColorYR());
        numParticles++;
    }
    else if (estrellas && numParticles < numMaxParticles) {
        ps->addParticle(currentPosition, velocityX, velocityY, velocityZ, 0.98, particleLifetime, { 1,1,1,1 });
        numParticles++;
    }
    else if (!generateSingleParticle && !numFiniteParticles && !estrellas) {
        ps->addParticle(currentPosition, velocityX, velocityY, velocityZ, 0.98, particleLifetime, {1,1,1,1});
    }
}

Vector4 ParticleGenerator::GenerateRandomColorYR() {
    static std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    float red = 0.8f + distribution(generator) * 0.2f;   
    float green = 0.4f + distribution(generator) * 0.4f; 
    float blue = distribution(generator) * 0.2f;        

    return Vector4(red, green, blue, 1.0f);
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