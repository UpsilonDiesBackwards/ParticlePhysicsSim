//
// Created by tayler on 12/14/25.
//

#ifndef SIMULATION_SYSTEM_H
#define SIMULATION_SYSTEM_H

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "particle.h"
#include "particletypes.h"

#include "../../include/shader.h"

class SimulationSystem {
public:
    SimulationSystem() : grid(200.0f, 200.0f, 2.0f, {-100.0f, -100.0f})
    {
        currentColors[ParticleType::ParticleType_Proton] = {0.98f, 0.12f, 0.06f, 1.0f};
        currentColors[ParticleType::ParticleType_Neutron] = {0.06f, 0.43f, 0.98f, 1.0f};
        currentColors[ParticleType::ParticleType_Electron] = {0.98f, 0.89f, 0.06f, 1.0f};
        currentColors[ParticleType::ParticleType_Photon] = {0.6f, 0.06f, 0.98f, 1.0f};
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

    void ClearAllParticles() { _particles.clear(); }

    void ResolveCollisions();
    void UpdateGrid();

    Shader* shader;

private:
    std::vector<Particle> _particles;
    std::unordered_map<ParticleType, glm::vec4> currentColors;

    Particle::Properties CreateParticleProperties(ParticleType type, const glm::vec2& position);

    ParticleColor particleColor;

    struct SpatialGrid {
        float cellSize;
        int cols, rows;
        glm::vec2 worldMin;

        std::vector<std::vector<Particle*>> cells;

        SpatialGrid(float width, float height, float size, glm::vec2 minPos = {0,0})
            : cellSize(size), worldMin(minPos) {
            cols = static_cast<int>(width / size) + 1;
            rows = static_cast<int>(height / size) + 1;
            cells.resize(cols * rows);
        }

        int GetKey(glm::vec2 pos) const {
            int x = static_cast<int>((pos.x - worldMin.x) / cellSize);
            int y = static_cast<int>((pos.y - worldMin.y) / cellSize);

            if (x < 0 || x >= cols || y < 0 || y >= rows) return -1;
            return x + (y * cols);
        }
    };

    SpatialGrid grid;
};

#endif //SIMULATION_SYSTEM_H
