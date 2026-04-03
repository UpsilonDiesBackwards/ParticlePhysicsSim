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

        glm::vec2 velocity = { 0.0f, 0.0f };
        glm::vec2 acceleration = { 0.0f, 0.0f };

        glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

        ParticleType type = ParticleType::ParticleType_Neutron;
    };

    Particle(const Properties& properties);

    void CreateMesh(int segments = 50);
    void Render(unsigned int sProgram, const glm::mat4& projection) const;

    void SetColor(const glm::vec4& color) { _properties.color = color; }
    const glm::vec4& GetColor() const { return _properties.color; }

    ParticleType GetType() const { return _properties.type; }

    glm::vec2 GetPosition() const { return _properties.position; }
    void SetPosition(const glm::vec2& position) { _properties.position = position; }

    float GetRadius() const { return _properties.radius; }
    void SetRadius(float radius) { _properties.radius = radius; }

    float GetMass() const { return _properties.mass; }
    void SetMass(float mass) { _properties.mass = mass; }

    void AddAcceleration(const glm::vec2& acc);

    void Integrate(float dT);

private:
    Properties _properties;
    unsigned int _vao = 0;
    unsigned int _vbo = 0;
    int _vertexCount = 0;
};


#endif //SIMULATION_PARTICLE_H