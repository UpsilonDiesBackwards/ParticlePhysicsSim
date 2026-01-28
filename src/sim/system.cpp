//
// Created by tayler on 12/14/25.
//

#include "../../include/sim/system.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "../../include/application.h"
#include "../../include/sim/particle.h"
#include "glad/glad.h"

void SimulationSystem::CreateParticle(ParticleType type, const glm::vec2 &position) {
    Particle::Properties props = CreateParticleProperties(type, position);
    Particle particle(props);

    particle.CreateMesh();

    _particles.push_back(particle);

    // std::cout << "Created a new particle" << std::endl;
}

void SimulationSystem::RenderAll(unsigned int program, const glm::mat4& projection, const glm::mat4& view) {
    shader = new Shader(
    (GET_APP.GetExecutablePath() + "/../res/shaders/circle.vert").c_str(),
    (GET_APP.GetExecutablePath() + "/../res/shaders/circle.frag").c_str()
    );

    glUseProgram(program);

    GLint projLoc = glGetUniformLocation(program, "projection");
    GLint viewLoc = glGetUniformLocation(program, "view");

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


    for (const auto& particle: _particles) {
        particle.Render(shader->ID, projection);
    }
}

Particle::Properties SimulationSystem::CreateParticleProperties(ParticleType type, const glm::vec2 &position) {
    Particle::Properties properties;
    ParticleColor color;
    ParticleMass mass;

    properties.position = position;
    properties.type = type;

    float particleMass = 0.0f;

    switch (type) {
        case ParticleType::ParticleType_Electron:
            particleMass = mass.ELECTRON;
            properties.color = color.ELECTRON;
            break;
        case ParticleType::ParticleType_Proton:
            particleMass = mass.PROTON;
            properties.color = color.PROTON;
            break;
        case ParticleType::ParticleType_Neutron:
            particleMass = mass.NEUTRON;
            properties.color = color.NEUTRON;
            break;
        default:
            particleMass = 0.1f;
            properties.color = glm::vec4(1.0f);
            break;
    }

    properties.mass = particleMass;

    constexpr float scaleFactor = 0.3f;
    properties.radius = scaleFactor * std::cbrt(particleMass);

    if (type == ParticleType::ParticleType_Electron) {
        properties.radius = scaleFactor * 2.5f;
    }

    return properties;
}
