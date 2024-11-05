#pragma once
#pragma once
#include "ForceGenerator.h"
#include "Particle.h"
#include <cmath>

class ExplosionForceGenerator : public ForceGenerator {
private:
    Vector3 center;     // Centro de la explosión (x_c, y_c, z_c)
    float radius;       // Radio máximo de la explosión
    float intensity;    // Intensidad de la explosión (K)
    float tau;          // Constante de tiempo (?)
    double startTime;   // Tiempo en el que comienza la explosión

public:
    ExplosionForceGenerator(Vector3 explosionCenter, float explosionRadius, float explosionIntensity, float explosionTau)
        : center(explosionCenter), radius(explosionRadius), intensity(explosionIntensity), tau(explosionTau), startTime(0) {}

    void trigger(double currentTime) {
        startTime = currentTime;
    }

    void updateForce(Particle* particle, double currentTime) override {
        double elapsedTime = currentTime - startTime;

        // La explosión solo afecta por un tiempo determinado (4 * tau)
        if (elapsedTime > 4 * tau) {
            // Deja de aplicar fuerza si ha pasado el tiempo de efecto de la explosión
            return;
        }

        Vector3 position = particle->getPosition();
        Vector3 direction = position - center;
        float distance = direction.magnitude();

        // Verifica si la partícula está dentro del radio de la explosión
        if (distance < radius) {
            // Normaliza la dirección y calcula la magnitud de la fuerza
            direction.normalize();
            float forceMagnitude = (intensity / (distance * distance)) * std::exp(-elapsedTime / tau);

            // Aplica la fuerza en la dirección radial
            Vector3 explosionForce = direction * forceMagnitude;
            particle->addForce(explosionForce);
        }
    }

};
