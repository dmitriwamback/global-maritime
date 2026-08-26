//
// Created by Dmitri on 2026-08-26.
//

#ifndef GLOBAL_MARITIME_COLORFRAMEBUFFER_H
#define GLOBAL_MARITIME_COLORFRAMEBUFFER_H

#include <cstdint>

class ColorFramebuffer {
public:
    ColorFramebuffer() = default;

    void Initialize();
    void Bind();
    void Unbind();
    void Update(int width, int height);

    uint32_t GetColorTextureId();

private:
    uint32_t framebufferId = 0;
    uint32_t depthBufferId = 0;
    uint32_t colorTextureId = 0;
};


#endif //GLOBAL_MARITIME_COLORFRAMEBUFFER_H
