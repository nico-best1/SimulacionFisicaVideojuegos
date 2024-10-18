#include <vector>
#include "Particle.h"
class ParticleGenerator;

class ParticleSystem {
private:
    std::vector<Particle*> particles;  // Lista de partículas
    ParticleGenerator* generator;  // Generador de partículas

public:
    ParticleSystem() : generator(nullptr) {}

    void addGenerator(ParticleGenerator* gen) {
        generator = gen;
    }

    void addParticle(Particle* particle) {
        particles.push_back(particle);
    }

    void update(double dt) {
        // Actualizar el generador de partículas
        if (generator != nullptr) {
            generator->update(dt);
        }

        // Actualizar todas las partículas
        for (auto it = particles.begin(); it != particles.end(); ) {
            Particle* p = *it;
            p->integrate(dt);

            // Si la partícula ha muerto, eliminarla
            if (p->isDead()) {
                delete p;
                it = particles.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    ~ParticleSystem() {
        for (auto p : particles) {
            delete p;
        }
    }
};
