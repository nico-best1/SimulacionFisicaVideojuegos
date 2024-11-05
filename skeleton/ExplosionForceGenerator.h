#pragma once
#pragma once
#include "ForceGenerator.h"
#include "Particle.h"
#include <cmath>

class ExplosionForceGenerator : public ForceGenerator {
private:
    Vector3 center;     // Centro de la explosi�n (x_c, y_c, z_c)
    float radius;       // Radio m�ximo de la explosi�n
    float intensity;    // Intensidad de la explosi�n (K)
    float tau;          // Constante de tiempo (?)
    double startTime;   // Tiempo en el que comienza la explosi�n

public:
    ExplosionForceGenerator(Vector3 explosionCenter, float explosionRadius, float explosionIntensity, float explosionTau)
        : center(explosionCenter), radius(explosionRadius), intensity(explosionIntensity), tau(explosionTau), startTime(0) {}

    void trigger(double currentTime) {
        startTime = currentTime;
    }

    void updateForce(Particle* particle, double currentTime) override {
        double elapsedTime = currentTime - startTime;

        // La explosi�n solo afecta por un tiempo determinado (4 * tau)
        if (elapsedTime > 4 * tau) {
            // Deja de aplicar fuerza si ha pasado el tiempo de efecto de la explosi�n
            return;
        }

        Vector3 position = particle->getPosition();
        Vector3 direction = position - center;
        float distance = direction.magnitude();

        // Verifica si la part�cula est� dentro del radio de la explosi�n
        if (distance < radius) {
            // Normaliza la direcci�n y calcula la magnitud de la fuerza
            direction.normalize();
            float forceMagnitude = (intensity / (distance * distance)) * std::exp(-elapsedTime / tau);

            // Aplica la fuerza en la direcci�n radial
            Vector3 explosionForce = direction * forceMagnitude;
            particle->addForce(explosionForce);
        }
    }

};
