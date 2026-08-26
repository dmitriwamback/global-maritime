//
// Created by Dmitri on 2026-08-26.
//

#ifndef GLOBAL_MARITIME_DEPTHFRAMEBUFFER_H
#define GLOBAL_MARITIME_DEPTHFRAMEBUFFER_H

#include <cstdint>

class DepthFramebuffer {
public:
    DepthFramebuffer() = default;

    void Initialize();
    void Bind();
    void Unbind();
    void Update(int width, int height);

    uint32_t GetDepthTexture();

private:
    uint32_t framebufferId = 0;
    uint32_t depthTextureId = 0;
};


#endif //GLOBAL_MARITIME_DEPTHFRAMEBUFFER_H
