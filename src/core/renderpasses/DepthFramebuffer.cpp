//
// Created by Dmitri on 2026-08-26.
//

#include "DepthFramebuffer.h"

#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>

void DepthFramebuffer::Initialize() {
    auto* gl = QOpenGLContext::currentContext()->extraFunctions();

    gl->glGenFramebuffers(1, &framebufferId);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

    gl->glGenTextures(1, &depthTextureId);
    gl->glBindTexture(GL_TEXTURE_2D, depthTextureId);

    gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 1200, 800, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureId, 0);

    if (gl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Depth buffer not complete" << std::endl;
    }

    gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthFramebuffer::Bind() {

}

void DepthFramebuffer::Unbind() {

}

uint32_t DepthFramebuffer::GetDepthTexture() {
    return depthTextureId;
}

void DepthFramebuffer::Update(int width, int height) {

}
