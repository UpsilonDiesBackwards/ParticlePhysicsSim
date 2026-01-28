//
// Created by tayler on 12/11/25.
//

#ifndef SIMULATION_SHADER_H
#define SIMULATION_SHADER_H
#include <string>
#include <glm/fwd.hpp>

class Shader {
public:
    unsigned int ID;

    Shader(const char *vPath, const char *fPath);

    void Use();

    void SetBool(const std::string &name, bool value) const;

    void SetInt(const std::string &name, int value) const;

    void SetFloat(const std::string &name, float value) const;

    void SetVec2(const std::string &name, const glm::vec2 &value) const;

    void SetVec3(const std::string &name, glm::vec3 value) const;

    void SetMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    void GetError(unsigned int shader, std::string type);
};


#endif //SIMULATION_SHADER_H