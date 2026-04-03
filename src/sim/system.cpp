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

    switch (props.type) {
        case ParticleType::ParticleType_Proton:
            GET_APP.simInterface.protonCount++;
            break;
        case ParticleType::ParticleType_Neutron:
            GET_APP.simInterface.neutronCount++;
            break;
        case ParticleType::ParticleType_Electron:
            GET_APP.simInterface.electronCount++;
            break;
        case ParticleType::ParticleType_Photon:
            GET_APP.simInterface.photonCount++;
            break;
        default: break;
    }
}

void SimulationSystem::RenderAll(unsigned int program, const glm::mat4& projection, const glm::mat4& view) {
    if (shader == nullptr) {
        shader = new Shader(
        (GET_APP.GetExecutablePath() + "/../res/shaders/circle.vert").c_str(),
        (GET_APP.GetExecutablePath() + "/../res/shaders/circle.frag").c_str()
        );
    }

    glUseProgram(program);

    GLint projLoc = glGetUniformLocation(program, "projection");
    GLint viewLoc = glGetUniformLocation(program, "view");

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


    for (const auto& particle: _particles) {
        particle.Render(shader->ID, projection);
    }
}

void SimulationSystem::Update(float dT) {
    for (auto& force : _forces) {
        force->Apply(_particles, dT);
    }

    for (auto& particle : _particles) {
        particle.Integrate(dT);
    }

    for (int i = 0; i < GET_APP.collisionResolutionCount; i++) {
        UpdateGrid();
        ResolveCollisions();
    }
}

void SimulationSystem::ResolveCollisions() {
    UpdateGrid();

    for (auto& p1 : _particles) {
        int centerX = static_cast<int>((p1.GetPosition().x - grid.worldMin.x) / grid.cellSize);
        int centerY = static_cast<int>((p1.GetPosition().y - grid.worldMin.y) / grid.cellSize);

        for (int x = centerX - 1; x <= centerX + 1; ++x) {
            for (int y = centerY - 1; y <= centerY + 1; ++y) {
                if (x < 0 || x >= grid.cols || y < 0 || y >= grid.rows) continue;

                int key = x + (y * grid.cols);
                for (Particle* p2 : grid.cells[key]) {
                    if (&p1 == p2) continue;
                    glm::vec2 dir = p1.GetPosition() - p2->GetPosition();
                    float dist = glm::length(dir);
                    float minDist = p1.GetRadius() + p2->GetRadius();

                    if (dist < minDist) {
                        if (dist == 0.0f) {
                            dir = glm::vec2(0.01f, 0.0f);
                            dist = 0.01f;
                        }

                        float overlap = (minDist - dist);
                        float slop = 0.01f;
                        float resolutionForce = std::max(0.0f, overlap + slop);

                        glm::vec2 separation = (dir / dist) * (resolutionForce * 0.5f);

                        p1.SetPosition(p1.GetPosition() + separation);
                        p2->SetPosition(p2->GetPosition() - separation);
                    }
                }
            }
        }
    }
}

void SimulationSystem::UpdateGrid() {
    for (auto& cell : grid.cells) {
        cell.clear();
    }

    for (auto& p : _particles) {
        int key = grid.GetKey(p.GetPosition());
        if (key != -1) {
            grid.cells[key].push_back(&p);
        }
    }
}

void SimulationSystem::AddForce(std::unique_ptr<IForceProvider> force) {
    _forces.push_back(std::move(force));
}

Particle::Properties SimulationSystem::CreateParticleProperties(ParticleType type, const glm::vec2 &position) {
    Particle::Properties properties;
    ParticleMass mass;

    properties.position = position;
    properties.type = type;
    properties.color = currentColors[type];

    float particleMass = 0.0f;

    switch (type) {
        case ParticleType::ParticleType_Electron:
            particleMass = mass.ELECTRON;
            break;
        case ParticleType::ParticleType_Proton:
            particleMass = mass.PROTON;
            break;
        case ParticleType::ParticleType_Neutron:
            particleMass = mass.NEUTRON;
            break;
        default:
            particleMass = 0.1f;
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
