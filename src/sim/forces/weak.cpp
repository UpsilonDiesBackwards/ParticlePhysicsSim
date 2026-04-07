//
// Created by Tayler Parsons on 05/04/2026.
//

#include "../../../include/sim/forces/weak.h"
#include "../../../include/application.h"

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

                    GET_APP.simulationSystem.CreateParticle(ParticleType::ParticleType_Electron, pos);
                }
            }
        }
    }
}
