//
// Created by Dmitri on 2026-08-15.
//

#include "Globe.h"

#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>

Globe::Globe(uint32_t latitudeSegments, uint32_t longitudeSegments) {
    this->latitudeSegments = latitudeSegments;
    this->longitudeSegments = longitudeSegments;
}

void Globe::Generate() {

    physicalGeographyTexture = Texture();
    physicalGeographyTexture.Load("/Users/dmitri/Documents/working/global-maritime/res/NASA_Earth_Physical_Geography.png");

    constexpr float pi = 3.14159265358979323846f;

    vertices.clear();
    indices.clear();

    for (uint32_t y = 0; y <= latitudeSegments; ++y) {

        float v = static_cast<float>(y) / static_cast<float>(latitudeSegments);
        float latitude = (v - 0.5f) * pi;

        for (uint32_t x = 0; x <= longitudeSegments; ++x) {
            float u = static_cast<float>(x) / static_cast<float>(longitudeSegments);
            float longitude = (u - 0.5f) * pi * 2;

            Vertex vertex{};
            vertex.x = cos(latitude) * cos(longitude);
            vertex.y = sin(latitude);
            vertex.z = cos(latitude) * sin(longitude);

            glm::vec3 direction = glm::normalize(glm::vec3(vertex.x, vertex.y, vertex.z));
            vertex.nx = direction.x;
            vertex.ny = direction.y;
            vertex.nz = direction.z;

            vertex.u = 1.0f - u;
            vertex.v = 1.0f - v;
            vertices.push_back(vertex);
        }
    }

    uint32_t stride = longitudeSegments + 1;

    for (uint32_t y = 0; y <= latitudeSegments; ++y) {
        for (uint32_t x = 0; x <= longitudeSegments; ++x) {

            uint32_t TL = y * stride + x;
            uint32_t TR = TL + 1;
            uint32_t BL = (y + 1) * stride + x;
            uint32_t BR = BL + 1;

            indices.push_back(TL);
            indices.push_back(BL);
            indices.push_back(TR);

            indices.push_back(TR);
            indices.push_back(BL);
            indices.push_back(BR);
        }
    }

    auto* GL = QOpenGLContext::currentContext()->extraFunctions();
    GL->glGenVertexArrays(1, &vertexArrayObject);
    GL->glGenBuffers(1, &vertexBufferObject);
    GL->glGenBuffers(1, &elementBufferObject);

    GL->glBindVertexArray(vertexArrayObject);

    GL->glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    GL->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    GL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    GL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    GL->glEnableVertexAttribArray(0);
    GL->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, x)));

    GL->glEnableVertexAttribArray(1);
    GL->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, nx)));

    GL->glEnableVertexAttribArray(2);
    GL->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, u)));

    GL->glBindVertexArray(0);
}

const std::vector<Vertex>& Globe::GetVertices() const {
    return vertices;
}

const std::vector<uint32_t>& Globe::GetIndices() const {
    return indices;
}

void Globe::Render() const {

    auto *GL = QOpenGLContext::currentContext()->extraFunctions();

    GL->glBindVertexArray(vertexArrayObject);
    physicalGeographyTexture.Bind();

    GL->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    GL->glBindVertexArray(0);
}