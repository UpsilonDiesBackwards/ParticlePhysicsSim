//
// Created by Tayler Parsons on 05/04/2026.
//

#include "../../../include/sim/forces/weak.h"
#include "../../../include/application.h"

#include <random>

static std::mt19937 rng(std::random_device{}());
static std::uniform_real_distribution<float> dist(-3.0f, 1.0f);

float randRange(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

float randomComponent() {
    if ((float)rand() / RAND_MAX < 0.8f)
        return randRange(-2.0f, -0.8f);
    else
        return randRange(0.8f, 2.0f);
}

glm::vec2 offset(randomComponent(), randomComponent());

void WeakForce::Apply(std::vector<Particle> &particles, float dT) {
    for (int i = particles.size() - 1; i >= 0; --i) {
        auto &p = particles[i];

        if (p.GetType() == ParticleType::ParticleType_Neutron) {
            bool isBound = false;
            for (const auto &other: particles) {
                if (&p == &other) continue;
                float dist = glm::distance(p.GetPosition(), other.GetPosition());
                if (dist < 4.5f) {
                    isBound = true;
                    break;
                }
            }

            if (!isBound) {
                p.AddAge(dT);
                float meanLife = 10.0f;

                float lambda = 1.0f / meanLife;
                float probability = lambda * dT;

                if ((float) rand() / RAND_MAX < probability) {
                    glm::vec2 pos = p.GetPosition();

                    p.Transform(ParticleType::ParticleType_Proton);

                    glm::vec2 offset(dist(rng), dist(rng));

                    GET_APP.simulationSystem.CreateParticle(
                        ParticleType::ParticleType_Electron,
                        pos + offset
                    );
                }
            }
        }
    }
}
