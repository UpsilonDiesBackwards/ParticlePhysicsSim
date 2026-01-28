//
// Created by tayler on 12/14/25.
//

#ifndef SIMULATION_PARTICLE_H
#define SIMULATION_PARTICLE_H
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

#include "particletypes.h"


class Particle {
public:
    struct Properties {
        glm::vec2 position = { 0.0f, 0.0f };
        float radius = 1.0f;
        float mass = 0.1f;

        glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

        ParticleType type = ParticleType::ParticleType_Neutron;
    };

    Particle(const Properties& properties);

    void CreateMesh(int segments = 50);
    void Render(unsigned int sProgram, const glm::mat4& projection) const;

private:
    Properties _properties;
    unsigned int _vao = 0;
    unsigned int _vbo = 0;
    int _vertexCount = 0;
};


#endif //SIMULATION_PARTICLE_H