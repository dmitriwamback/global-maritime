//
// Created by Dmitri on 2026-08-23.
//

#ifndef GLOBAL_MARITIME_COUNTRYPOLYGONS_H
#define GLOBAL_MARITIME_COUNTRYPOLYGONS_H
#include <QJsonArray>
#include <QString>
#include <glm/vec3.hpp>
#include "../util/math.h"


class CountryPolygons {
public:
    CountryPolygons() = default;

    bool Load(const QString& path, const QString& name);
    void Initialize();
    void Render() const;

private:
    void ParseGeometry(const QJsonObject& geometry);
    void ParsePolygon(const QJsonArray& polygon);
    void ParseRing(const QJsonArray& ring);

    std::vector<glm::vec3> vertices;
    std::vector<uint32_t> indices;

    uint32_t vertexArrayObject = 0;
    uint32_t vertexBufferObject = 0;
    uint32_t indexBufferObject = 0;
};

#endif //GLOBAL_MARITIME_COUNTRYPOLYGONS_H
