//
// Created by tayler on 12/14/25.
//

#ifndef SIMULATION_PARTICLETYPES_H
#define SIMULATION_PARTICLETYPES_H

#include <glm/vec4.hpp>

enum class ParticleType {
    ParticleType_Proton,
    ParticleType_Neutron,
    ParticleType_Electron,
    ParticleType_Photon,
    ParticleType_NumItems
};

struct ParticleColor {
    glm::vec4 PROTON = { 0.98f, 0.12f, 0.06f, 1.0f };
    glm::vec4 NEUTRON = { 0.06f, 0.43f, 0.98f, 1.0f };
    glm::vec4 ELECTRON = { 0.98f, 0.89f, 0.06f, 1.0f };
    glm::vec4 PHOTON = { 0.6f, 0.06f, 0.98f, 1.0f };
};


/*
 * Because these are horrendously small numbers, small enough to cause issues, we want to calculate, and use
 * new values for mass, and the rest of the physics, based on a more suitable reference scale. For this we shall use
 * the mass of a proton.
 *
 * To calculate the new values you take the SI unit (the unit in relation to a kg) such as 9.1×10−31 and divide it
 * by our reference, the mass of a proton (1.6×10^27) so (9.1×10^31)/(1.6×10^27) = 0.0005f
 *
 */
struct ParticleMass {
    float PROTON = 1.0f;
    float NEUTRON = 1.0f;
    float ELECTRON = 0.0005f;
    float PHOTON = 0.0f;
};

struct ParticleCharge {
    float PROTON = 1.0f;
    float ELECTRON = -1.0f;
    float NEUTRON = 0.0f;
};

#endif //SIMULATION_PARTICLETYPES_H