//
// Created by Dmitri on 2026-08-15.
//

#ifndef GLOBAL_MARITIME_SHADER_H
#define GLOBAL_MARITIME_SHADER_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Shader {
public:
    void Create(const char *vertexShaderSource, const char *fragmentShaderSource);
    void Bind();
    void SetVec3(const char* name, const glm::vec3& value);
    void SetMat4(const char* name, const glm::mat4& value);

private:
    int program;
    unsigned int CompileShader(unsigned int type, const char *source);
};


#endif //GLOBAL_MARITIME_SHADER_H
