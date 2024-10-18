#include "Particle.h"
#include "ParticleSystem.h"
#include <random>

class ParticleGenerator {
private:
    ParticleSystem* system;  // El sistema de part�culas asociado
    Vector3 position;  // Posici�n de donde se generan las part�culas
    int rate;  // Tasa de generaci�n de part�culas (part�culas por segundo)
    double lastGeneratedTime;  // Tiempo desde la �ltima generaci�n

    std::default_random_engine generator;
    std::uniform_real_distribution<double> dist_pos;
    std::uniform_real_distribution<double> dist_vel;

public:
    ParticleGenerator(ParticleSystem* sys, Vector3 pos, int rate)
        : system(sys), position(pos), rate(rate), lastGeneratedTime(0),
        dist_pos(-1.0, 1.0), dist_vel(-10.0, 10.0) {}

    void update(double dt) {
        lastGeneratedTime += dt;

        // Generar part�culas a la tasa especificada
        while (lastGeneratedTime >= 1.0 / rate) {
            generateParticle();
            lastGeneratedTime -= 1.0 / rate;
        }
    }

private:
    void generateParticle() {
        // Generar posici�n y velocidad aleatorias
        Vector3 randomPos = position + Vector3(dist_pos(generator), dist_pos(generator), dist_pos(generator));
        Vector3 randomVel = Vector3(dist_vel(generator), dist_vel(generator), dist_vel(generator));

        // Crear una nueva part�cula
        Particle* newParticle = new Particle(randomPos, randomVel, Vector3(0, -9.8, 0), 0.99, 5);
        system->addParticle(newParticle);
    }
};
