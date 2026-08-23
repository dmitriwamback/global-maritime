//
// Created by Dmitri on 2026-08-23.
//

#ifndef GLOBAL_MARITIME_BORDERS_H
#define GLOBAL_MARITIME_BORDERS_H


#pragma once

#include <vector>
#include <cstdint>

#include <QJsonArray>
#include <QJsonObject>
#include <QString>

#include <glm/glm.hpp>


struct BorderLine {
    std::vector<glm::vec3> vertices;

    uint32_t vertexArrayObject = 0;
    uint32_t vertexBufferObject = 0;
};

class Borders {
public:
    bool Load(const QString& path);
    void Initialize();
    void Render() const;

private:
    void ParseGeometry(const QJsonObject& geometry);
    void ParseLineString(const QJsonArray& coordinates);

    std::vector<BorderLine> lines;
};

#endif //GLOBAL_MARITIME_BORDERS_H
