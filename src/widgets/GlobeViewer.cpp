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

    borders = Borders();
    borders.Load("../res/world.geojson");
    borders.Initialize();

    USA = CountryPolygons();
    USA.Load("../res/world.geojson", "United States of America");
    USA.Initialize();

    globeShader = Shader();
    globeShader.Create(ShaderSources::GLOBE_VERTEX_SHADER_SOURCE, ShaderSources::GLOBE_FRAGMENT_SHADER_SOURCE);

    borderShader = Shader();
    borderShader.Create(ShaderSources::BORDER_VERTEX_SHADER_SOURCE, ShaderSources::BORDER_FRAGMENT_SHADER_SOURCE);
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

    float dy = borders.MAX_RADIUS - borders.MIN_RADIUS;
    float dx = camera.ZOOM_MAX - camera.ZOOM_MIN;

    float slope = dy/dx;
    float b = borders.MAX_RADIUS - camera.ZOOM_MAX * slope;

    float radius = slope * camera.GetDistance() + b;

    glm::mat4 borderModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(radius));

    borderShader.Bind();
    borderShader.SetMat4("projection", projectionMatrix);
    borderShader.SetMat4("lookAt", lookAtMatrix);
    borderShader.SetMat4("model", borderModelMatrix);

    borders.Render();
    USA.Render();
}

void GlobeViewer::resizeGL(int width, int height) {
    aspectRatio = (float)width / (float)height;
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