//
// Created by Dmitri on 2026-08-15.
//

#include "GlobeViewer.h"

#include <iostream>
#include <QTimer>

#include "../core/shader/ShaderSources.h"

GlobeViewer::GlobeViewer() {
    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&GlobeViewer::update));
    timer->start(16);
}

void GlobeViewer::initializeGL() {
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    camera = Camera();

    globe = Globe();
    globe.Generate();
    globeShader = Shader();
    globeShader.Create(ShaderSources::VERTEX_SHADER_SOURCE, ShaderSources::FRAGMENT_SHADER_SOURCE);
}

void GlobeViewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    camera.Update();

    globeShader.Bind();
    glm::mat4 projectionMatrix = camera.GetProjectionMatrix(aspectRatio);
    glm::mat4 lookAtMatrix = camera.GetLookAtMatrix();

    globeShader.SetMat4("projection", projectionMatrix);
    globeShader.SetMat4("lookAt", lookAtMatrix);

    globe.Render();
}

void GlobeViewer::resizeGL(int width, int height) {
    aspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);
}
