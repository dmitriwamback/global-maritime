//
// Created by Dmitri on 2026-08-15.
//

#ifndef GLOBAL_MARITIME_GLOBE_H
#define GLOBAL_MARITIME_GLOBE_H
#include <cstdint>
#include <vector>

#include "../texture/Texture.h"
#include "../shader/Shader.h"

struct Vertex {
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};

class Globe {
public:
    explicit Globe(uint32_t latitudeSegments = 64, uint32_t longitudeSegments = 128);

    const std::vector<Vertex>& GetVertices() const;
    const std::vector<uint32_t>& GetIndices() const;

    void Render() const;
    void Generate();

private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    unsigned int vertexArrayObject, vertexBufferObject, elementBufferObject;
    uint32_t latitudeSegments;
    uint32_t longitudeSegments;

    Texture physicalGeographyTexture;
};


#endif //GLOBAL_MARITIME_GLOBE_H
