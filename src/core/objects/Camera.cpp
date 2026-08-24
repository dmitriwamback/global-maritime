//
// Created by Dmitri on 2026-08-15.
//

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera() {
    position = glm::vec3(580.0f, 0.0f, 0.0f);
    lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    yaw = 0.0f;
    pitch = 0.0f;
    distance = 780.0f;
}

void Camera::Update() {
    pitch = glm::clamp(pitch, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f
    );

    position.x = distance * cos(pitch) * sin(yaw);
    position.y = distance * sin(pitch);
    position.z = distance * cos(pitch) * cos(yaw);
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) {
    return glm::perspective(30.0f * glm::pi<float>()/180.0f, aspectRatio, 0.1f, 2500.0f );
}

glm::mat4 Camera::GetLookAtMatrix() {
    return glm::lookAt(position, lookAt, up);
}

float Camera::GetDistance() {
    return distance;
}

float Camera::GetYaw() {
    return yaw;
}

float Camera::GetPitch() {
    return pitch;
}

void Camera::Rotate(float deltaYaw, float deltaPitch) {
    yaw += deltaYaw;
    pitch += deltaPitch;
    Update();
}

void Camera::Zoom(float delta) {
    distance -= delta;
    distance = glm::clamp(distance, ZOOM_MIN, ZOOM_MAX);
}
