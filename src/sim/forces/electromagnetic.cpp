//
// Created by Tayler Parsons on 05/04/2026.
//

#include "../../../include/sim/forces/electromagnetic.h"

#include <glm/ext/quaternion_geometric.hpp>

#include "../../../include/application.h"

void ElectromagneticForce::Apply(std::vector<Particle>& particles, float dT) {
    const float Ke = GET_APP.simulationSystem.properties.Ke;
    const float minBohrRadius = 1.5f;
    const float softening = GET_APP.simulationSystem.properties.Softening;

    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            auto& p1 = particles[i];
            auto& p2 = particles[j];

            if (p1.GetCharge() == 0.0f || p2.GetCharge() == 0.0f) continue;

            glm::vec2 diff = p2.GetPosition() - p1.GetPosition();
            float distSq = glm::dot(diff, diff);
            if (distSq < 1e-6f) continue;
            float dist = glm::sqrt(distSq);

            float mag = 0.0f;
            bool isOppositeCharge = (p1.GetCharge() * p2.GetCharge() < 0);
            bool bothElectrons = (p1.GetType() == ParticleType::ParticleType_Electron &&
                                  p2.GetType() == ParticleType::ParticleType_Electron);

            if (bothElectrons) {
                float electronKe = Ke * 0.1f;
                mag = (electronKe * p1.GetCharge() * p2.GetCharge()) / (distSq + 50.f);
            }
            else if (isOppositeCharge) {
                if (dist < minBohrRadius) {
                    float forceAtLimit = Ke / (std::pow(minBohrRadius, 2.0f) + softening);
                    float repulsionFactor = std::pow(minBohrRadius / dist, 4.0f);
                    mag = -forceAtLimit * repulsionFactor;
                } else {
                    mag = (Ke * p1.GetCharge() * p2.GetCharge()) / (distSq + softening);
                }
            }
            else {
                mag = (Ke * p1.GetCharge() * p2.GetCharge()) / (distSq + softening);
            }

            glm::vec2 unitVec = diff / dist;
            glm::vec2 force = unitVec * mag;

            float mass1 = p1.GetMass();
            float mass2 = p2.GetMass();

            if (mass1 / mass2 > 100.0f) {
                p2.AddAcceleration(force / mass2);
            } else if (mass2 / mass1 > 100.0f) {
                p1.AddAcceleration(-force / mass1);
            } else {
                p1.AddAcceleration(-force / mass1);
                p2.AddAcceleration(force / mass2);
            }
        }
    }
}