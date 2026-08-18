//
// Created by Dmitri on 2026-08-15.
//

#ifndef GLOBAL_MARITIME_TEXTURE_H
#define GLOBAL_MARITIME_TEXTURE_H
#include <QString>


class Texture {
public:
    Texture() = default;
    ~Texture();

    void Load(const QString& path);

    void Bind() const;
    void Unbind() const;

    uint32_t GetID() const;
private:
    uint32_t textureID = 0;
};


#endif //GLOBAL_MARITIME_TEXTURE_H
