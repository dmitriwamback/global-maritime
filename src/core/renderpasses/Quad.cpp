//
// Created by Dmitri on 2026-08-26.
//

#include "Quad.h"

#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>

void Quad::Initialize() {

    auto* gl = QOpenGLContext::currentContext()->extraFunctions();

    constexpr float vertices[] = {
        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f,

        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f,   0.0f, 1.0f
    };

    gl->glGenVertexArrays(1, &vertexArrayObject);
    gl->glGenBuffers(1, &vertexBufferObject);

    gl->glBindVertexArray(vertexArrayObject);
    gl->glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    gl->glEnableVertexAttribArray(0);
    gl->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    gl->glEnableVertexAttribArray(1);
    gl->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    gl->glBindVertexArray(0);
}

void Quad::Render() const {
    auto* gl = QOpenGLContext::currentContext()->extraFunctions();

    gl->glBindVertexArray(vertexArrayObject);
    gl->glDrawArrays(GL_TRIANGLES, 0, 6);
    gl->glBindVertexArray(0);
}
