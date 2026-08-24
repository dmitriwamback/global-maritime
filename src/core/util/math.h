//
// Created by Dmitri on 2026-08-24.
//

#ifndef GLOBAL_MARITIME_MATH_H
#define GLOBAL_MARITIME_MATH_H
#include <glm/vec3.hpp>

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

#endif //GLOBAL_MARITIME_MATH_H
