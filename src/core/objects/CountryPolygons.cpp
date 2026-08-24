//
// Created by Dmitri on 2026-08-23.
//

#include "CountryPolygons.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>

#include <glm/glm.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

struct GeoPoint {
    double latitude;
    double longitude;
};

double Cross(const GeoPoint& a, const GeoPoint& b, const GeoPoint& c) {
    return (b.longitude - a.longitude) * (c.latitude - a.latitude) - (b.latitude - a.latitude) * (c.longitude - a.longitude);
}

bool PointInTriangle(const GeoPoint& p, const GeoPoint& a, const GeoPoint& b, const GeoPoint& c) {

    const double c1 = Cross(a, b, p);
    const double c2 = Cross(b, c, p);
    const double c3 = Cross(c, a, p);

    const bool hasNegativeValue = c1 < 0.0 || c2 < 0.0 || c3 < 0.0;
    const bool hasPositiveValue = c1 > 0.0 || c2 > 0.0 || c3 > 0.0;

    return !(hasNegativeValue && hasPositiveValue);
}

double SignedArea(const std::vector<GeoPoint>& polygon) {
    double area = 0.0;

    for (size_t i = 0; i < polygon.size(); ++i) {
        const GeoPoint& a = polygon[i];
        const GeoPoint& b = polygon[(i + 1) % polygon.size()];

        area += a.longitude * b.latitude - a.latitude * b.longitude;
    }

    return area / 2.0;
}

bool PointInPolygon(const GeoPoint& p, std::vector<GeoPoint>& polygon) {
    bool inside = false;
    for (size_t i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++) {
        const GeoPoint& a = polygon[i];
        const GeoPoint& b = polygon[j];

        const bool intersects = ((a.latitude > p.latitude) != (b.latitude > p.latitude)) && (p.longitude < (b.longitude - a.longitude) * (p.latitude - a.latitude) / (b.latitude - a.latitude) + a.longitude);

        if (intersects) { inside = !inside; }
    }

    return inside;
}

void GenerateInteriorPoints(std::vector<GeoPoint>& points, double spacing) {
    if (points.size() < 3) return;

    double minLat = points[0].latitude;
    double maxLat = points[0].latitude;
    double minLon = points[0].longitude;
    double maxLon = points[0].longitude;

    for (const GeoPoint& point : points) {
        maxLat = std::max(maxLat, point.latitude);
        minLat = std::min(minLat, point.latitude);
        maxLon = std::max(maxLon, point.longitude);
        minLon = std::min(minLon, point.longitude);
    }

    std::vector<GeoPoint> interiorPoints;

    for (double lat = minLat + spacing; lat < maxLat; lat += spacing) {
        for (double lon = minLon + spacing; lon < maxLon; lon += spacing) {
            GeoPoint c = GeoPoint(lat, lon);
            if (PointInPolygon(c, points)) {
                interiorPoints.push_back(c);
            }
        }
    }
    points.insert(points.end(), interiorPoints.begin(), interiorPoints.end());
}

std::vector<uint32_t> Triangulate(const std::vector<GeoPoint>& polygon) {
    std::vector<uint32_t> indices;

    if (polygon.size() < 3) {
        return indices;
    }

    std::vector<uint32_t> remaining(polygon.size());
    std::iota(remaining.begin(), remaining.end(), 0);

    if (SignedArea(polygon) < 0.0) {
        std::reverse(remaining.begin(), remaining.end());
    }

    while (remaining.size() > 3) {
        bool found = false;

        for (size_t i = 0; i < remaining.size(); ++i) {
            const uint32_t previous = remaining[(i + remaining.size() - 1) % remaining.size()];
            const uint32_t current = remaining[i];
            const uint32_t next = remaining[(i + 1) % remaining.size()];

            const GeoPoint& a = polygon[previous];
            const GeoPoint& b = polygon[current];
            const GeoPoint& c = polygon[next];

            if (Cross(a, b, c) <= 0.0) {
                continue;
            }

            bool containsPoint = false;

            for (uint32_t vertex : remaining) {
                if (vertex == previous || vertex == current || vertex == next) {
                    continue;
                }

                if (PointInTriangle(polygon[vertex], a, b, c)) {
                    containsPoint = true;
                    break;
                }
            }

            if (containsPoint) { continue; }

            indices.push_back(previous);
            indices.push_back(current);
            indices.push_back(next);

            remaining.erase(remaining.begin() + i);
            found = true;
            break;
        }

        if (!found) break;
    }

    if (remaining.size() == 3) {
        indices.push_back(remaining[0]);
        indices.push_back(remaining[1]);
        indices.push_back(remaining[2]);
    }

    return indices;
}

bool CountryPolygons::Load(const QString& path, const QString& name) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonParseError error;
    const QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        return false;
    }

    const QJsonObject root = jsonDocument.object();
    if (root["type"].toString() != "FeatureCollection") {
        return false;
    }

    vertices.clear();
    indices.clear();

    const QJsonArray features = root["features"].toArray();

    for (const QJsonValue& featureVal : features) {

        const QJsonObject feature = featureVal.toObject();
        const QJsonObject properties = feature["properties"].toObject();

        const QString nameFound = properties["name"].toString();
        if (nameFound != name) {
            continue;
        }

        const QJsonObject geometry = feature["geometry"].toObject();
        ParseGeometry(geometry);
        break;
    }

    return !vertices.empty() && !indices.empty();
}

void CountryPolygons::ParseGeometry(const QJsonObject& geometry) {
    const QString type = geometry["type"].toString();
    const QJsonArray coordinates = geometry["coordinates"].toArray();

    if (type == "Polygon") {
        ParsePolygon(coordinates);
    }
    else if (type == "MultiPolygon") {
        for (const QJsonValue& polygonValue : coordinates) {
            ParsePolygon(polygonValue.toArray());
        }
    }
}

void CountryPolygons::ParsePolygon(const QJsonArray& polygon) {

    if (polygon.isEmpty()) {
        return;
    }

    const QJsonArray outerRing = polygon[0].toArray();
    std::vector<GeoPoint> points;
    points.reserve(outerRing.size());

    for (const QJsonValue& polygonValue : outerRing) {
        const QJsonArray coordinates = polygonValue.toArray();

        if (coordinates.size() < 2) {
            continue;
        }

        points.push_back(GeoPoint(coordinates[1].toDouble(), coordinates[0].toDouble()));

        if (points.size() > 1 && points.front().longitude == points.back().longitude && points.front().latitude == points.back().latitude) {
            points.pop_back();
        }
    }

    if (points.size() < 3) {
        return;
    }

    const uint32_t vertexOffset = static_cast<uint32_t>(vertices.size());

    for (const GeoPoint& point : points) {
        vertices.push_back(GeoToSphere(point.latitude, point.longitude));
    }

    //GenerateInteriorPoints(points, 1.0);

    const std::vector<uint32_t> localIndices = Triangulate(points);

    for (uint32_t index : localIndices) {
        indices.push_back(index + vertexOffset);
    }
}

void CountryPolygons::Initialize() {
    auto* gl = QOpenGLContext::currentContext()->extraFunctions();

    gl->glGenVertexArrays(1, &vertexArrayObject);
    gl->glGenBuffers(1, &vertexBufferObject);
    gl->glGenBuffers(1, &indexBufferObject);

    gl->glBindVertexArray(vertexArrayObject);
    gl->glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    gl->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    gl->glEnableVertexAttribArray(0);
    gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
}

void CountryPolygons::Render() const {
    auto* gl = QOpenGLContext::currentContext()->extraFunctions();
    gl->glBindVertexArray(vertexArrayObject);

    gl->glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
}