//
// Created by Dmitri on 2026-08-15.
//

#ifndef GLOBAL_MARITIME_SHADERSOURCES_H
#define GLOBAL_MARITIME_SHADERSOURCES_H


class ShaderSources {
public:
    static constexpr const char* VERTEX_SHADER_SOURCE = R"(
        #version 330 core

        layout(location = 0) in vec3 position;
        layout(location = 1) in vec2 texCoord;

        uniform mat4 lookAt;
        uniform mat4 projection;

        out vec2 uv;

        void main() {
            uv = texCoord;
            gl_Position = projection * lookAt * vec4(position, 1.0);
        }
    )";

    static constexpr const char* FRAGMENT_SHADER_SOURCE = R"(
        #version 330 core

        in vec2 uv;
        uniform sampler2D globeTexture;
        out vec4 fragc;

        void main() {
            fragc = vec4(uv, 0.0, 1.0);
        }
    )";
};


#endif //GLOBAL_MARITIME_SHADERSOURCES_H
