//
// Created by tayler on 12/14/25.
//

#include "../../include/sim/particle.h"

#include "../../include/application.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"

Particle::Particle(const Properties &properties) : _properties(properties) {
}

void Particle::CreateMesh(int segments) {
    std::vector<glm::vec2> vertices;
    vertices.push_back({0.0f, 0.0f});

    for (int i = 0; i <= segments; ++i) {
        float angle = static_cast<float>(i) / segments * 2.0f * M_PI;
        float x = cosf(angle);
        float y = sinf(angle);
        vertices.emplace_back(x, y);
    }

    _vertexCount = static_cast<int>(vertices.size());

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void *) 0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &_trailVao);
    glGenBuffers(1, &_trailVbo);
    glBindVertexArray(_trailVao);
    glBindBuffer(GL_ARRAY_BUFFER, _trailVbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_TRAIL_POINTS * sizeof(glm::vec2), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Particle::Render(unsigned int sProgram, const glm::mat4 &projection) const {
    glUseProgram(sProgram);

    GLuint colorLoc = glGetUniformLocation(sProgram, "uColor");
    glUniform4fv(colorLoc, 1, &_properties.color[0]);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(_properties.position, 0.0f));
    model = glm::scale(model, glm::vec3(_properties.radius, _properties.radius, 1.0f));
    GLuint modelLoc = glGetUniformLocation(sProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    GLuint projLoc = glGetUniformLocation(sProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    GLuint viewLoc = glGetUniformLocation(sProgram, "view");
    glUniformMatrix4fv(
        viewLoc,
        1,
        GL_FALSE,
        glm::value_ptr(GET_APP.getCamera()->GetView())
    );

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, _vertexCount);
    glBindVertexArray(0);
}

void Particle::Transform(ParticleType newType) {
    switch (newType) {
        case ParticleType::ParticleType_Proton:
            _properties.type = ParticleType::ParticleType_Proton;
            _properties.mass = ParticleMass().PROTON;
            _properties.color = ParticleColor().PROTON;
            break;
        case ParticleType::ParticleType_Neutron:
            _properties.type = ParticleType::ParticleType_Neutron;
            _properties.mass = ParticleMass().NEUTRON;
            _properties.color = ParticleColor().NEUTRON;
            break;
        case ParticleType::ParticleType_Electron:
            _properties.type = ParticleType::ParticleType_Electron;
            _properties.mass = ParticleMass().ELECTRON;
            _properties.color = ParticleColor().ELECTRON;
            break;
        case ParticleType::ParticleType_Photon:
            _properties.type = ParticleType::ParticleType_Photon;
            _properties.mass = ParticleMass().PHOTON;
            _properties.color = ParticleColor().PHOTON;
            break;
        default: break;
    }
}

void Particle::AddAcceleration(const glm::vec2& acc) {
    _properties.acceleration += acc;
}

void Particle::Integrate(float dT) {
    _properties.velocity += _properties.acceleration * dT;

    if (_properties.type == ParticleType::ParticleType_Electron) {
        float maxSpeed = 50.0f;
        if (glm::length(_properties.velocity) > maxSpeed) {
            _properties.velocity = glm::normalize(_properties.velocity) * maxSpeed;
        }
    } else {
        _properties.velocity *= 0.95f;
    }

    _properties.position += _properties.velocity * dT;
    _properties.acceleration = glm::vec2(0.0f);

    _trail.push_back(_properties.position);
    if (_trail.size() > MAX_TRAIL_POINTS) {
        _trail.erase(_trail.begin());
    }
}

void Particle::RenderTrail(unsigned int program) const {
    if (_trail.size() < 2) return;

    glUseProgram(program);

    glm::vec4 trailColor = _properties.trailColor;
    trailColor.a = 0.5f;
    glUniform4fv(glGetUniformLocation(program, "uColor"), 1, &trailColor[0]);

    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

    glBindBuffer(GL_ARRAY_BUFFER, _trailVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _trail.size() * sizeof(glm::vec2), _trail.data());

    glBindVertexArray(_trailVao);
    glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)_trail.size());
}

void Particle::ClearTrail() {
    _trail.clear();
}
