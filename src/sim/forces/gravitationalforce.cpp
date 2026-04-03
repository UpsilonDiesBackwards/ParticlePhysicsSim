//
// Created by tayler on 03/04/2026.
//

#include "../../../include/sim/forces/gravitationalforce.h"

#include <glm/ext/quaternion_geometric.hpp>

void GravitationalForce::Apply(std::vector<Particle>& particles, float dT) {
    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            auto& p1 = particles[i];
            auto& p2 = particles[j];
        }
    }
}
