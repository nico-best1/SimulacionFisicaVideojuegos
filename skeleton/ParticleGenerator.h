#include "Particle.h"
#include "ParticleSystem.h"
#include <random>

class ParticleGenerator {
private:
    ParticleSystem* system;  // El sistema de partículas asociado
    Vector3 position;  // Posición de donde se generan las partículas
    int rate;  // Tasa de generación de partículas (partículas por segundo)
    double lastGeneratedTime;  // Tiempo desde la última generación

    std::default_random_engine generator;
    std::uniform_real_distribution<double> dist_pos;
    std::uniform_real_distribution<double> dist_vel;

public:
    ParticleGenerator(ParticleSystem* sys, Vector3 pos, int rate)
        : system(sys), position(pos), rate(rate), lastGeneratedTime(0),
        dist_pos(-1.0, 1.0), dist_vel(-10.0, 10.0) {}

    void update(double dt) {
        lastGeneratedTime += dt;

        // Generar partículas a la tasa especificada
        while (lastGeneratedTime >= 1.0 / rate) {
            generateParticle();
            lastGeneratedTime -= 1.0 / rate;
        }
    }

private:
    void generateParticle() {
        // Generar posición y velocidad aleatorias
        Vector3 randomPos = position + Vector3(dist_pos(generator), dist_pos(generator), dist_pos(generator));
        Vector3 randomVel = Vector3(dist_vel(generator), dist_vel(generator), dist_vel(generator));

        // Crear una nueva partícula
        Particle* newParticle = new Particle(randomPos, randomVel, Vector3(0, -9.8, 0), 0.99, 5);
        system->addParticle(newParticle);
    }
};
