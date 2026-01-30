//
// Created by tayler on 12/14/25.
//

#ifndef SIMULATION_SYSTEM_H
#define SIMULATION_SYSTEM_H
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <vector>

#include "particle.h"
#include "particletypes.h"

#include "../../include/shader.h"

class SimulationSystem {
public:
    SimulationSystem() {
        currentColors[ParticleType::ParticleType_Proton]   = { 0.98f, 0.12f, 0.06f, 1.0f };
        currentColors[ParticleType::ParticleType_Neutron]  = { 0.06f, 0.43f, 0.98f, 1.0f };
        currentColors[ParticleType::ParticleType_Electron] = { 0.98f, 0.89f, 0.06f, 1.0f };
        currentColors[ParticleType::ParticleType_Photon]   = { 0.6f, 0.06f, 0.98f, 1.0f };
    }

    void CreateParticle(ParticleType type, const glm::vec2& position);
    void RenderAll(unsigned int program, const glm::mat4 & projection, const glm::mat4& view);

    glm::vec4& GetParticleColor(ParticleType type) {
        switch (type) {
            case ParticleType::ParticleType_Proton:   return particleColor.PROTON;
            case ParticleType::ParticleType_Neutron:  return particleColor.NEUTRON;
            case ParticleType::ParticleType_Electron: return particleColor.ELECTRON;
            case ParticleType::ParticleType_Photon:   return particleColor.PHOTON;
            default: return particleColor.PROTON;
        }
    }

    void UpdateParticleColors(ParticleType type, const glm::vec4& color) {
        for (auto& particle : _particles) {
            if (particle.GetType() == type) {
                particle.SetColor(color);
            }
        }

        currentColors[type] = color;
    }

    Shader* shader;

private:
    std::vector<Particle> _particles;
    std::unordered_map<ParticleType, glm::vec4> currentColors;

    Particle::Properties CreateParticleProperties(ParticleType type, const glm::vec2& position);

    ParticleColor particleColor;
};

#endif //SIMULATION_SYSTEM_H
