//
// Created by Dmitri on 2026-08-15.
//

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera() {
    position = glm::vec3(0.0f, 0.0f, 2.0f);
    lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::Update() {
    position = glm::vec3(1.5f * sin(debugTime), 0.5f, 1.5f * cos(debugTime));
    debugTime -= 0.0025f;
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) {
    return glm::perspective(90.0f * glm::pi<float>()/180.0f, aspectRatio, 0.1f, 500.0f );
}

glm::mat4 Camera::GetLookAtMatrix() {
    return glm::lookAt(position, lookAt, up);
}