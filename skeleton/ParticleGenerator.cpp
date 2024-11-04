#include "ParticleGenerator.h"
#include <random>

void ParticleGenerator::update(double t)
{
    float velocityX, velocityY, velocityZ;
    if (distributionType == DistributionType::Gaussian) {
        velocityX = generateGaussian(0.0, 2);
        velocityY = generateGaussian(20, 2);
        velocityZ = generateGaussian(0, 2);
    }
    else { 
        velocityX = generateUniform(-10.0, 10.0);
        velocityY = generateUniform(10, 30);
        velocityZ = generateUniform(-10.0, 10.0);
    }

    particles.push_back(new Particle(initialPosition, Vector3(velocityX, velocityY, velocityZ), Vector3(0, -9.8, 0), 0.99, particleLifetime));

    for (auto it = particles.begin(); it != particles.end(); ) {
        (*it)->integrate(t);
        if ((*it)->isDead() || (*it)->isOutOfBounds(dipersion_area_x, dipersion_area_y)) {
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