//
// Created by Dmitri on 2026-08-15.
//

#ifndef GLOBAL_MARITIME_CAMERA_H
#define GLOBAL_MARITIME_CAMERA_H
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>


class Camera {
private:
    glm::vec3 position, lookAt, up;
    float debugTime;

public:
    Camera();
    void Update();

    glm::mat4 GetProjectionMatrix(float aspectRatio);
    glm::mat4 GetLookAtMatrix();
    //glm::vec3 GetPosition();
};


#endif //GLOBAL_MARITIME_CAMERA_H
