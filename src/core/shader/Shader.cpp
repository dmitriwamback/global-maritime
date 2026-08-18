//
// Created by Dmitri on 2026-08-15.
//

#include "Shader.h"

#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <glm/gtc/type_ptr.hpp>

void Shader::Create(const char *vertexShaderSource, const char *fragmentShaderSource) {
    auto* GL = QOpenGLContext::currentContext()->extraFunctions();
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    if (!vertexShader || !fragmentShader) { return; }

    program = GL->glCreateProgram();
    GL->glAttachShader(program, vertexShader);
    GL->glAttachShader(program, fragmentShader);
    GL->glLinkProgram(program);

    int success = 0;
    GL->glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        GL->glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        std::cerr << infoLog << std::endl;
        GL->glDeleteProgram(program);
    }
    GL->glDeleteShader(vertexShader);
    GL->glDeleteShader(fragmentShader);
}

void Shader::Bind() {
    auto* GL = QOpenGLContext::currentContext()->extraFunctions();
    GL->glUseProgram(program);
}

void Shader::SetMat4(const char* name, const glm::mat4& value) {
    auto* GL = QOpenGLContext::currentContext()->extraFunctions();
    unsigned int uniform = GL->glGetUniformLocation(program, name);
    GL->glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(const char* name, const glm::vec3& value) {
    auto* GL = QOpenGLContext::currentContext()->extraFunctions();
    unsigned int uniform = GL->glGetUniformLocation(program, name);
    GL->glUniform3fv(uniform, 1, glm::value_ptr(value));
}

unsigned int Shader::CompileShader(unsigned int type, const char *source) {
    auto* GL = QOpenGLContext::currentContext()->extraFunctions();
    unsigned int shader = GL->glCreateShader(type);

    GL->glShaderSource(shader, 1, &source, nullptr);
    GL->glCompileShader(shader);
    int success = 0;
    GL->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[1024];
        GL->glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
    }
    return shader;
}