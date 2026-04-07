//
// Created by Tayler Parsons on 05/04/2026.
//

#include "../../../include/sim/forces/strong.h"

void StrongForce::Apply(std::vector<Particle> &particles, float dT) {
    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            auto& p1 = particles[i];
            auto& p2 = particles[j];

            if (p1.GetType() == ParticleType::ParticleType_Electron || p2.GetType() == ParticleType::ParticleType_Electron) continue;

            glm::vec2 diff = p2.GetPosition() - p1.GetPosition();
            float dist = glm::length(diff);

            if (dist < 1e-6f || dist > range * 3.0f) continue;

            float decay = std::exp(-dist / range);
            float mag = (Gs * p1.GetMass() * p2.GetMass() * decay) / (dist * dist);

            if (dist < (p1.GetRadius() + p2.GetRadius()) * 0.8f) {
                mag *= -1.0f;
            }

            glm::vec2 force = (diff / dist) * mag;

            p1.AddAcceleration(force / p1.GetMass());
            p2.AddAcceleration(-force / p2.GetMass());
        }
    }
}
