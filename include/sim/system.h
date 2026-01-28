//
// Created by tayler on 12/14/25.
//

#ifndef SIMULATION_SYSTEM_H
#define SIMULATION_SYSTEM_H
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

#include "particle.h"
#include "particletypes.h"

#include "../../include/shader.h"

class SimulationSystem {
    public:
        void CreateParticle(ParticleType type, const glm::vec2& position);
        void RenderAll(unsigned int program, const glm::mat4 & projection, const glm::mat4& view);

        Shader* shader;

    private:
        std::vector<Particle> _particles;

        Particle::Properties CreateParticleProperties(ParticleType type, const glm::vec2& position);
};


#endif //SIMULATION_SYSTEM_H