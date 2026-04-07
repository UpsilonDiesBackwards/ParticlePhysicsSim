//
// Created by tayler on 03/04/2026.
//

#include "../../../include/sim/forces/gravitational.h"

#include <glm/ext/quaternion_geometric.hpp>

#include "../../../include/application.h"

void GravitationalForce::Apply(std::vector<Particle>& particles, float dT) {
    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            auto& p1 = particles[i];
            auto& p2 = particles[j];

            glm::vec2 diff = p2.GetPosition() - p1.GetPosition();
            float distSq = glm::dot(diff, diff);

            if (distSq < 1e-6f) continue;
            float dist = glm::sqrt(distSq);

            float mag = (GET_APP.simulationSystem.properties.Gn * p1.GetMass() * p2.GetMass()) / distSq;

            glm::vec2 unitVec = diff / dist;

            glm::vec2 force = unitVec * mag;

            float mass1 = p1.GetMass();
            float mass2 = p2.GetMass();

            float massRatio = mass1 / mass2;

            if (massRatio > 100.0f) {
                p2.AddAcceleration(-force / mass2);
            }
            else if (massRatio < 0.01f) {
                p1.AddAcceleration(force / mass1);
            }
            else {
                p1.AddAcceleration(force / mass1);
                p2.AddAcceleration(-force / mass2);
            }
        }
    }
}
