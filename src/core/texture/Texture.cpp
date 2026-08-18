//
// Created by Dmitri on 2026-08-15.
//

#include "Texture.h"

#include <QImage>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>

Texture::~Texture() {
    if (textureID == 0) return;

    auto* context = QOpenGLContext::currentContext();
    if (context) {
        auto* GL = context->extraFunctions();
        GL->glDeleteTextures(1, &textureID);
    }
}

void Texture::Load(const QString &path) {
    auto context = QOpenGLContext::currentContext();
    if (!context) return;
    auto* GL = context->extraFunctions();

    QImage image(path);
    if (image.isNull()) return;

    image = image.convertToFormat(QImage::Format_RGBA8888);

    GL->glGenTextures(1, &textureID);
    GL->glBindTexture(GL_TEXTURE_2D, textureID);
    GL->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    GL->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GL->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    GL->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GL->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.constBits());
    GL->glGenerateMipmap(GL_TEXTURE_2D);
    GL->glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const{
    auto* GL = QOpenGLContext::currentContext()->extraFunctions();
    GL->glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::Unbind() const {
    auto* GL = QOpenGLContext::currentContext()->extraFunctions();
    GL->glBindTexture(GL_TEXTURE_2D, 0);
}