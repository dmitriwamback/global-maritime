//
// Created by Dmitri on 2026-08-24.
//

#ifndef GLOBAL_MARITIME_MATH_H
#define GLOBAL_MARITIME_MATH_H
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>

constexpr double PI = 3.14159265358979323846;

static glm::vec3 GeoToSphere(double latitude, double longitude) {

    const double lat = (latitude) * PI / 180.0;
    const double lon = -longitude * PI / 180.0;

    return glm::vec3(
        static_cast<float>(std::cos(lat) * std::cos(lon)),
        static_cast<float>(std::sin(lat)),
        static_cast<float>(std::cos(lat) * std::sin(lon))
    );
}

inline glm::mat4 RotationMatrixDegrees(glm::vec3 rotation) {

    float x = rotation.x * PI / 180.0f;
    float y = rotation.y * PI / 180.0f;
    float z = rotation.z * PI / 180.0f;

    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), x, glm::vec3(1.0f, 0.0f, 0.0f))
                             * glm::rotate(glm::mat4(1.0f), y, glm::vec3(0.0f, 1.0f, 0.0f))
                             * glm::rotate(glm::mat4(1.0f), z, glm::vec3(0.0f, 0.0f, 1.0f));

    return rotationMatrix;
}

#endif //GLOBAL_MARITIME_MATH_H
