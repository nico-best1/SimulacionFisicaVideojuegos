#include "ParticleGenerator.h"
#include <random>

void ParticleGenerator::update(double t)
{
	particulas.push_back(new Particle(iniPos, Vector3(generateGausssian(0.0, 2), generateGausssian(20, 2), generateGausssian(0, 2)), Vector3(0, -9.8, 0), 0.998));
    for (auto it = particulas.begin(); it != particulas.end(); ) {
        Particle* p = *it; 
        p->integrate(t);          
        if (p->isDead() || p->isOutOfBounds(AREA_X, AREA_Y)) {
            delete p;              
            it = particulas.erase(it); 
        }
        else {
            ++it; 
        }
    }
}

float ParticleGenerator::generateGausssian(float mean, float stddev)
{
	static std::default_random_engine generator;
	std::normal_distribution<float>distribution(mean, stddev);
	return distribution(generator);
}