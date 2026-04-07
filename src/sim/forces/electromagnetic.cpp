//
// Created by Tayler Parsons on 05/04/2026.
//

#include "../../../include/sim/forces/electromagnetic.h"

#include <glm/ext/quaternion_geometric.hpp>

void ElectromagneticForce::Apply(std::vector<Particle>& particles, float dT) {
    const float Ke = 1.f;
    const float minBohrRadius = 2.f;
    const float softness = 0.1f;

    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            auto& p1 = particles[i];
            auto& p2 = particles[j];

            if (p1.GetCharge() == 0.0f || p2.GetCharge() == 0.0f) continue;

            glm::vec2 diff = p2.GetPosition() - p1.GetPosition();
            float distSq = glm::dot(diff, diff);

            if (distSq < 1e-6f) continue;
            float dist = glm::sqrt(distSq);

            // bohr model implementation
            float mag;

            // Electron * Proton pair
            bool isEPPair = (p1.GetType() == ParticleType::ParticleType_Electron && p2.GetType() == ParticleType::ParticleType_Proton) ||
                                                     (p2.GetType() == ParticleType::ParticleType_Electron && p1.GetType() == ParticleType::ParticleType_Proton);

            if (isEPPair && dist < minBohrRadius) {
                float springK = 0.1f;
                mag = (dist - minBohrRadius) * springK;

                auto& electron = (p1.GetType() == ParticleType::ParticleType_Electron) ? p1 : p2;
                glm::vec2 unitDiff = diff / dist;
                float radialVel = glm::dot(electron.GetVelocity(), unitDiff);

                electron.SetVelocity(electron.GetVelocity() - (unitDiff * radialVel * 0.05f));
            } else {
                mag = (Ke * p1.GetCharge(), p2.GetCharge()) / distSq;
            }

            glm::vec2 unitVec = diff / dist;
            glm::vec2 force = unitVec * mag;

            float mass1 = p1.GetMass();
            float mass2 = p2.GetMass();

            if (mass1 / mass2 > 100.0f) {
                p2.AddAcceleration(force / p2.GetMass());
            } else if (mass1 / mass2 < 0.01f) {
                p1.AddAcceleration(-force / p1.GetMass());
            } else {
                p1.AddAcceleration(-force / p1.GetMass());
                p2.AddAcceleration(force / p2.GetMass());
            }
        }
    }
}
