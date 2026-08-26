//
// Created by Dmitri on 2026-08-15.
//

#include "GlobeViewer.h"

#include <iostream>
#include <QTimer>

#include <QMouseEvent>
#include <QWheelEvent>
#include <glm/ext/matrix_transform.hpp>

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

    earthColorFramebuffer = ColorFramebuffer();
    borderColorFramebuffer = ColorFramebuffer();
    sceneColorFramebuffer = ColorFramebuffer();

    earthColorFramebuffer.Initialize();
    borderColorFramebuffer.Initialize();
    sceneColorFramebuffer.Initialize();

    borders = Borders();
    borders.Load("../res/world.geojson");
    borders.Initialize();

    USA = CountryPolygons();
    USA.Load("../res/world.geojson", "United States of America");
    USA.Initialize();

    composite = Quad();
    composite.Initialize();

    globeShader = Shader();
    globeShader.Create(ShaderSources::GLOBE_VERTEX_SHADER_SOURCE, ShaderSources::GLOBE_FRAGMENT_SHADER_SOURCE);

    borderShader = Shader();
    borderShader.Create(ShaderSources::BORDER_VERTEX_SHADER_SOURCE, ShaderSources::BORDER_FRAGMENT_SHADER_SOURCE);

    compositeShader = Shader();
    compositeShader.Create(ShaderSources::COMPOSITE_VERTEX_SHADER_SOURCE, ShaderSources::COMPOSITE_FRAGMENT_SHADER_SOURCE);
}

void GlobeViewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    camera.Update();

    RenderBorders();
    RenderGlobe();
    RenderComposite();

    //USA.Render();
}

void GlobeViewer::resizeGL(int width, int height) {
    aspectRatio = (float)width / (float)height;

    earthColorFramebuffer.Update(width, height);
    sceneColorFramebuffer.Update(width, height);
    borderColorFramebuffer.Update(width, height);

    glViewport(0, 0, width, height);
}

void GlobeViewer::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePosition = event->position().toPoint();
    }
}

void GlobeViewer::mouseMoveEvent(QMouseEvent* event) {

    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }

    QPoint currentMousePosition = event->position().toPoint();
    QPoint delta = currentMousePosition - lastMousePosition;

    lastMousePosition = currentMousePosition;

    float sensitivityMax = 0.00245f;
    float sensitivityMin = 0.0000065f;

    float currentCameraDistance = camera.GetDistance();

    // f(ZOOM_MIN) = sensitivityMin
    // f(ZOOM_MAX) = sensitivityMax
    // f(x) takes the linear form y = mx + b
    float dx = camera.ZOOM_MAX - camera.ZOOM_MIN;
    float dy = sensitivityMax - sensitivityMin;
    float slope = dy/dx;

    float b = sensitivityMax - slope * camera.ZOOM_MAX;

    float sensitivity = (camera.GetDistance() * slope + b);

    camera.Rotate(-delta.x() * sensitivity, delta.y() * sensitivity);
}

void GlobeViewer::wheelEvent(QWheelEvent* event) {
    const float delta = static_cast<float>(event->angleDelta().y());
    camera.Zoom(delta);
    update();
}

void GlobeViewer::RenderGlobe() {

    glm::mat4 projectionMatrix = camera.GetProjectionMatrix(aspectRatio);
    glm::mat4 lookAtMatrix = camera.GetLookAtMatrix();

    earthColorFramebuffer.Bind();

    glViewport(0, 0, 1200, 800);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    globeShader.Bind();
    globeShader.SetMat4("projection", projectionMatrix);
    globeShader.SetMat4("lookAt", lookAtMatrix);
    globe.Render();

    earthColorFramebuffer.Unbind();
}

void GlobeViewer::RenderBorders() {

    glm::mat4 projectionMatrix = camera.GetProjectionMatrix(aspectRatio);
    glm::mat4 lookAtMatrix = camera.GetLookAtMatrix();

    glm::mat4 borderModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(500.0f));

    borderColorFramebuffer.Bind();

    glViewport(0, 0, 1200, 800);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    borderShader.Bind();
    borderShader.SetMat4("projection", projectionMatrix);
    borderShader.SetMat4("lookAt", lookAtMatrix);
    borderShader.SetMat4("model", borderModelMatrix);
    borderShader.SetVec3("cameraDirection", camera.GetDirection());

    borders.Render();

    borderColorFramebuffer.Unbind();
}

void GlobeViewer::RenderScene() {

}

void GlobeViewer::RenderComposite() {

    const qreal dpr = devicePixelRatioF();

    glViewport(0, 0, static_cast<GLsizei>(width() * dpr), static_cast<GLsizei>(height() * dpr));

    compositeShader.Bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earthColorFramebuffer.GetColorTextureId());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, borderColorFramebuffer.GetColorTextureId());

    compositeShader.SetInt("earthTexture", 0);
    compositeShader.SetInt("borderTexture", 1);

    composite.Render();
}
