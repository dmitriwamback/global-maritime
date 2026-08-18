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
        layout(location = 1) in vec3 inNormal;
        layout(location = 2) in vec2 inUV;

        uniform mat4 lookAt;
        uniform mat4 projection;

        out vec2 uv;
        out vec3 normal;
        out vec3 fragp;

        void main() {
            uv = inUV;
            normal = inNormal;
            fragp = position;
            gl_Position = projection * lookAt * vec4(position, 1.0);
        }
    )";

    static constexpr const char* FRAGMENT_SHADER_SOURCE = R"(
        #version 330 core

        in vec2 uv;
        in vec3 normal;
        in vec3 fragp;

        uniform sampler2D globeTexture;
        out vec4 fragc;

        vec3 lightPosition = vec3(10.0);

        void main() {
            vec3 physical = texture(globeTexture, uv).rgb;
            float luminance = dot(physical, vec3(0.299, 0.587, 0.114));

            vec3 L = normalize(lightPosition - fragp);

            float diffuse = max(dot(normal, L), 0.2);

            physical = mix(vec3(luminance), physical, 0.5);
            fragc = vec4(physical * diffuse, 1.0);
        }
    )";
};


#endif //GLOBAL_MARITIME_SHADERSOURCES_H
