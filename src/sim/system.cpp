//
// Created by tayler on 12/14/25.
//

#include "../../include/sim/system.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "../../include/application.h"
#include "../../include/sim/particle.h"
#include "../../include/sim/forces/gravitational.h"
#include "glad/glad.h"

void SimulationSystem::CreateParticle(ParticleType type, const glm::vec2 &position) {
    Particle::Properties props = CreateParticleProperties(type, position);

    if (type == ParticleType::ParticleType_Electron && !_particles.empty()) {
        glm::vec2 centerOfMass(0.0f);
        float totalMass = 0.0f;
        for (const auto& p : _particles) {
            centerOfMass += p.GetPosition() * p.GetMass();
            totalMass += p.GetMass();
        }
        centerOfMass /= totalMass;

        glm::vec2 diff = position - centerOfMass;
        float dist = glm::length(diff);

        if (dist > 0.1f) {
            const float Ke = 0.0005f;

            float netCharge = 0.0f;
            for (const auto& p : _particles) {
                netCharge += p.GetCharge();
            }

            float pullStrength = (GET_APP.simulationSystem.properties.Gn * totalMass) + (Ke * std::abs(netCharge));
            float speed = std::sqrt(pullStrength / dist);

            glm::vec2 unitDiff = diff / dist;
            glm::vec2 tangent(-unitDiff.y, unitDiff.x);

            props.velocity = tangent * speed;
        }
    }

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
#ifdef __APPLE__
        shader = new Shader(
        (GET_APP.GetExecutablePath() + "/../res/shaders/macos/circle.vert").c_str(),
        (GET_APP.GetExecutablePath() + "/../res/shaders/macos/circle.frag").c_str()
        );
#else
        shader = new Shader(
(GET_APP.GetExecutablePath() + "/../res/shaders/circle.vert").c_str(),
(GET_APP.GetExecutablePath() + "/../res/shaders/circle.frag").c_str()
);
#endif
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

                    if ((p1.GetType() == ParticleType::ParticleType_Electron && p2->GetType() == ParticleType::ParticleType_Proton) ||
                            (p1.GetType() == ParticleType::ParticleType_Proton && p2->GetType() == ParticleType::ParticleType_Electron)) {
                            continue;
                    }

                    if (dist < minDist) {
                        if (dist == 0.0f) {
                            dir = glm::vec2(0.01f, 0.0f);
                            dist = 0.01f;
                        }

                        glm::vec2 normal = dir / dist;
                        float overlap = minDist - dist;

                        float totalMass = p1.GetMass() + p2->GetMass();
                        float ratio1 = p2->GetMass() / totalMass;
                        float ratio2 = p1.GetMass() / totalMass;

                        p1.SetPosition(p1.GetPosition() + normal * overlap * ratio1);
                        p2->SetPosition(p2->GetPosition() - normal * overlap * ratio2);

                        glm::vec2 relativeVelocity = p1.GetVelocity() - p2->GetVelocity();

                        float velocityAlongNormal = glm::dot(relativeVelocity, normal);

                        if (velocityAlongNormal < 0) {
                            float restitution = 0.5f;

                            float j = -(1.0f + restitution) * velocityAlongNormal;
                            j /= (1.0f / p1.GetMass()) + (1.0f / p2->GetMass());

                            glm::vec2 impulse = j * normal;
                            p1.SetVelocity(p1.GetVelocity() + (impulse / p1.GetMass()));
                            p2->SetVelocity(p2->GetVelocity() - (impulse / p2->GetMass()));
                        }
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
    ParticleMass massScale;
    properties.position = position;
    properties.type = type;
    properties.color = currentColors[type];

    switch (type) {
        case ParticleType::ParticleType_Electron:
            properties.mass = massScale.ELECTRON;
            properties.charge = ParticleCharge().ELECTRON;
            properties.radius = 0.15f;
            break;
        case ParticleType::ParticleType_Proton:
            properties.mass = massScale.PROTON;
            properties.charge = ParticleCharge().PROTON;
            properties.radius = 0.5f;
            break;
        case ParticleType::ParticleType_Neutron:
            properties.mass = massScale.NEUTRON;
            properties.charge = ParticleCharge().NEUTRON;
            properties.radius = 0.5f;
            break;
        default:
            properties.mass = 0.1f;
            properties.charge = ParticleCharge().NEUTRON;
            properties.radius = 0.1f;
            break;
    }

    return properties;
}
