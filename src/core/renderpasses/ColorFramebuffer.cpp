//
// Created by Dmitri on 2026-08-26.
//

#include "ColorFramebuffer.h"

#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>

void ColorFramebuffer::Initialize() {

    auto* gl = QOpenGLContext::currentContext()->extraFunctions();

    gl->glGenFramebuffers(1, &framebufferId);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

    gl->glGenTextures(1, &colorTextureId);
    gl->glBindTexture(GL_TEXTURE_2D, colorTextureId);

    gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureId, 0);

    gl->glGenRenderbuffers(1, &depthBufferId);
    gl->glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId);
    gl->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _width, _height);

    gl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferId);

    if (gl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }



    gl->glGenFramebuffers(1, &msaaFramebufferId);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebufferId);

    gl->glGenRenderbuffers(1, &msaaColorBufferId);
    gl->glBindRenderbuffer(GL_RENDERBUFFER, msaaColorBufferId);

    gl->glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, _width, _height);
    gl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, msaaColorBufferId);

    gl->glGenRenderbuffers(1, &msaaDepthBufferId);
    gl->glBindRenderbuffer(GL_RENDERBUFFER, msaaDepthBufferId);

    gl->glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT24, _width, _height);
    gl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, msaaDepthBufferId);

    if (gl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "MSAA Framebuffer is not complete!" << std::endl;
    }

    gl->glBindTexture(GL_TEXTURE_2D, 0);
    gl->glBindRenderbuffer(GL_RENDERBUFFER, 0);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ColorFramebuffer::Bind() {
    auto* gl = QOpenGLContext::currentContext()->extraFunctions();
    gl->glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebufferId);
}

void ColorFramebuffer::Unbind() {
    auto* gl = QOpenGLContext::currentContext()->extraFunctions();

    gl->glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFramebufferId);
    gl->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferId);

    gl->glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint32_t ColorFramebuffer::GetColorTextureId() {
    return colorTextureId;
}

void ColorFramebuffer::Update(int width, int height) {

    _width = width;
    _height = height;

    auto* gl = QOpenGLContext::currentContext()->extraFunctions();

    gl->glBindTexture(GL_TEXTURE_2D, colorTextureId);
    gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    gl->glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId);
    gl->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

    gl->glBindTexture(GL_TEXTURE_2D, 0);
    gl->glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
