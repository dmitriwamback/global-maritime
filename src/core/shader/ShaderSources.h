//
// Created by Dmitri on 2026-08-15.
//

#ifndef GLOBAL_MARITIME_SHADERSOURCES_H
#define GLOBAL_MARITIME_SHADERSOURCES_H


class ShaderSources {
public:
    static constexpr const char* GLOBE_VERTEX_SHADER_SOURCE = R"(
        #version 330 core

        layout(location = 0) in vec3 position;
        layout(location = 1) in vec3 inNormal;
        layout(location = 2) in vec2 inUV;

        uniform mat4 lookAt;
        uniform mat4 projection;
        uniform mat4 model;

        out vec2 uv;
        out vec3 normal;
        out vec3 fragp;

        void main() {
            uv = inUV;
            normal = (model * vec4(inNormal, 1.0)).rgb;
            fragp = (model * vec4(position, 1.0)).rgb;
            gl_Position = projection * lookAt * model * vec4(position, 1.0);
        }
    )";

    static constexpr const char* GLOBE_FRAGMENT_SHADER_SOURCE = R"(
        #version 330 core

        in vec2 uv;
        in vec3 normal;
        in vec3 fragp;

        uniform sampler2D globeTexture;
        out vec4 fragc;

        vec3 lightPosition = vec3(0.0, 0.0, 100000.0);

        vec3 sharpenTexture(sampler2D tex, vec2 uv) {
            vec2 texel = 1.0 / vec2(textureSize(tex, 0));

            vec3 center = texture(tex, uv).rgb;

            vec3 blur =
                texture(tex, uv + vec2( texel.x, 0.0)).rgb +
                texture(tex, uv + vec2(-texel.x, 0.0)).rgb +
                texture(tex, uv + vec2(0.0,  texel.y)).rgb +
                texture(tex, uv + vec2(0.0, -texel.y)).rgb;

            blur *= 0.25;

            float strength = 3.0;

            return center + (center - blur) * strength;
        }

        void main() {

            vec3 physical = sharpenTexture(globeTexture, uv);

            vec3 L = normalize(lightPosition - fragp);
            float luminance = dot(physical, vec3(0.299, 0.587, 0.114));

            float diffuse = max(dot(normal, L), 0.1);

            physical = mix(vec3(luminance), physical, 0.4);
            fragc = vec4(physical, 1.0);
        }
    )";





    static constexpr const char* BORDER_VERTEX_SHADER_SOURCE = R"(
        #version 330 core

        layout(location = 0) in vec3 position;

        uniform mat4 lookAt;
        uniform mat4 projection;
        uniform mat4 model;

        out vec3 fragp;

        void main() {
            fragp = (model * vec4(position, 1.0)).xyz;
            gl_Position = projection * lookAt * model * vec4(position, 1.0);
        }
    )";

    static constexpr const char* BORDER_FRAGMENT_SHADER_SOURCE = R"(
        #version 330 core

        out vec4 fragc;
        in vec3 fragp;

        uniform vec3 cameraDirection;

        void main() {
            vec3 norm = normalize(fragp);
            if (dot(norm, normalize(cameraDirection - fragp)) >= 0.0) {
                fragc = vec4(1.0);
            }
            else {
                discard;
            }
        }
    )";





    static constexpr const char* COMPOSITE_VERTEX_SHADER_SOURCE = R"(
        #version 330 core
        layout(location = 0) in vec2 position;
        layout(location = 1) in vec2 inUV;
        out vec2 uv;

        void main() {
            uv = inUV;
            gl_Position = vec4(position, 0.0, 1.0);
        }
    )";

    static constexpr const char* COMPOSITE_FRAGMENT_SHADER_SOURCE = R"(
        #version 330 core
        out vec4 fragc;
        in vec2 uv;

        uniform sampler2D earthTexture;
        uniform sampler2D borderTexture;
        uniform sampler2D countryPolygonsTexture;

        void main() {
            vec3 earthColor = texture(earthTexture, uv).rgb;
            vec4 borderColor = texture(borderTexture, uv);
            vec4 polygonColor = texture(countryPolygonsTexture, uv);

            vec3 composite = mix(earthColor, borderColor.rgb * 5.0, borderColor.a);
            composite = mix(composite, polygonColor.rgb * 5.0, polygonColor.a);

            fragc = vec4(composite, 1.0);
        }
    )";
};


#endif //GLOBAL_MARITIME_SHADERSOURCES_H
