//
// Created by Dmitri on 2026-08-23.
//

#include "Borders.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>

#include <cmath>


static glm::vec3 GeoToSphere(double latitude, double longitude) {
    constexpr double PI = 3.14159265358979323846;
    constexpr float RADIUS = 500.01f;

    const double lat = (latitude) * PI / 180.0;
    const double lon = -longitude * PI / 180.0;

    return RADIUS * glm::vec3(
        static_cast<float>(std::cos(lat) * std::cos(lon)),
        static_cast<float>(std::sin(lat)),
        static_cast<float>(std::cos(lat) * std::sin(lon))
    );
}

bool Borders::Load(const QString& path) {
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonParseError error;

    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);

    if (error.error != QJsonParseError::NoError) {
        return false;
    }

    const QJsonObject root = document.object();

    if (root["type"].toString() != "FeatureCollection") {
        return false;
    }

    const QJsonArray features = root["features"].toArray();

    for (const QJsonValue& featureValue : features) {

        const QJsonObject feature = featureValue.toObject();
        const QJsonObject geometry = feature["geometry"].toObject();

        ParseGeometry(geometry);
    }

    return true;
}

void Borders::Initialize() {

    auto* gl = QOpenGLContext::currentContext()->extraFunctions();

    for (BorderLine& line : lines) {

        gl->glGenVertexArrays(1, &line.vertexArrayObject);
        gl->glGenBuffers(1, &line.vertexBufferObject);

        gl->glBindVertexArray(line.vertexArrayObject);

        gl->glBindBuffer(GL_ARRAY_BUFFER, line.vertexBufferObject);
        gl->glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(line.vertices.size() * sizeof(glm::vec3)), line.vertices.data(), GL_STATIC_DRAW);

        gl->glEnableVertexAttribArray(0);
        gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    }

    gl->glBindVertexArray(0);
}

void Borders::ParseGeometry(const QJsonObject& geometry) {
    const QString type = geometry["type"].toString();

    const QJsonArray coordinates = geometry["coordinates"].toArray();

    if (type == "LineString") {
        ParseLineString(coordinates);
    }
    else if (type == "MultiLineString") {

        for (const QJsonValue& line : coordinates) {
            ParseLineString(line.toArray());
        }
    }
    else if (type == "Polygon") {

        for (const QJsonValue& ring : coordinates) {
            ParseLineString(ring.toArray());
        }
    }
    else if (type == "MultiPolygon") {

        for (const QJsonValue& polygon : coordinates) {
            for (const QJsonValue& ring : polygon.toArray()) {
                ParseLineString(ring.toArray());
            }
        }
    }
}

void Borders::ParseLineString(const QJsonArray& coordinates) {
    BorderLine line;

    for (const QJsonValue& value : coordinates) {
        const QJsonArray coordinate = value.toArray();

        if (coordinate.size() < 2) continue;

        line.vertices.push_back(GeoToSphere(coordinate[1].toDouble(), coordinate[0].toDouble()));
    }

    if (!line.vertices.empty()) lines.push_back(std::move(line));
}

void Borders::Render() const {
    auto* gl = QOpenGLContext::currentContext()->extraFunctions();

    for (const BorderLine& line : lines) {
        gl->glBindVertexArray(line.vertexArrayObject);
        gl->glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(line.vertices.size()));
    }

    gl->glBindVertexArray(0);
}