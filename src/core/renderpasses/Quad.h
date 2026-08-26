//
// Created by Dmitri on 2026-08-26.
//

#ifndef GLOBAL_MARITIME_QUAD_H
#define GLOBAL_MARITIME_QUAD_H

#include <cstdint>

class Quad {
public:
    Quad() = default;
    void Initialize();
    void Render() const;

private:
    uint32_t vertexArrayObject;
    uint32_t vertexBufferObject;
};


#endif //GLOBAL_MARITIME_QUAD_H
