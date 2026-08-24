//
// Created by Dmitri on 2026-08-15.
//

#ifndef GLOBAL_MARITIME_CAMERA_H
#define GLOBAL_MARITIME_CAMERA_H
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>


class Camera {
private:
    glm::vec3 position;
    glm::vec3 lookAt;
    glm::vec3 up;

    float yaw;
    float pitch;
    float distance;

public:
    Camera();

    void Update();

    void Rotate(float deltaYaw, float deltaPitch);
    void Zoom(float delta);

    glm::mat4 GetProjectionMatrix(float aspectRatio);
    glm::mat4 GetLookAtMatrix();
    float GetDistance();
    float GetYaw();
    float GetPitch();

    float ZOOM_MAX = 2000.0f;
    float ZOOM_MIN = 505.0f;
};

#endif //GLOBAL_MARITIME_CAMERA_H
