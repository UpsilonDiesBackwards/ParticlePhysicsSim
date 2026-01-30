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
