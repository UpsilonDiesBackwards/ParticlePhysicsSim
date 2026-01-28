//
// Created by tayler on 12/14/25.
//

#ifndef SIMULATION_PARTICLETYPES_H
#define SIMULATION_PARTICLETYPES_H

#include <glm/vec4.hpp>

enum class ParticleType {
    ParticleType_Electron,
    ParticleType_Proton,
    ParticleType_Neutron,
    ParticleType_Photon
};

struct ParticleColor {
    glm::vec4 ELECTRON = { 0.98f, 0.89f, 0.06f, 1.0f };
    glm::vec4 PROTON = { 0.98f, 0.12f, 0.06f, 1.0f };
    glm::vec4 NEUTRON = { 0.06f, 0.43f, 0.98f, 1.0f };
    glm::vec4 PHOTON = { 0.6f, 0.06f, 0.98f, 1.0f };
};

struct ParticleMass {
    float ELECTRON = 0.1f;
    float PROTON = 183.6f;
    float NEUTRON = 183.8f;
    float PHOTON = 0.0f;
};

#endif //SIMULATION_PARTICLETYPES_H