//
// Created by Dmitri on 2026-08-15.
//

#include "EarthRenderer.h"

#include <iostream>
#include <QTimer>

#include <QMouseEvent>
#include <QWheelEvent>
#include <glm/ext/matrix_transform.hpp>

#include "../core/shader/ShaderSources.h"
#include "../core/util/GDELT.h"

EarthRenderer::EarthRenderer() {

    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&EarthRenderer::update));
    timer->start(16);
}

void EarthRenderer::initializeGL() {
    initializeOpenGLFunctions();

    GDELT::Search("ship hijacking", [](const QList<MaritimeIncident>& incidents) {
        for (const auto& incident : incidents) {
            qDebug() << incident.desc;
            qDebug() << incident.source;
        }
    });

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    camera = Camera();

    globe = Globe();
    globe.Generate();

    earthColorFramebuffer = ColorFramebuffer();
    borderColorFramebuffer = ColorFramebuffer();
    sceneColorFramebuffer = ColorFramebuffer();
    countryPolygonsColorFramebuffer = ColorFramebuffer();

    earthColorFramebuffer.Initialize();
    borderColorFramebuffer.Initialize();
    sceneColorFramebuffer.Initialize();
    countryPolygonsColorFramebuffer.Initialize();

    borders = Borders();
    borders.Load("../res/world.geojson");
    borders.Initialize();

    USA = CountryPolygons();
    USA.Load("../res/world.geojson", "United States of America");
    USA.Initialize();

    earthTilt = RotationMatrixDegrees(glm::vec3(0, 0, 0));

    composite = Quad();
    composite.Initialize();

    globeShader = Shader();
    globeShader.Create(ShaderSources::GLOBE_VERTEX_SHADER_SOURCE, ShaderSources::GLOBE_FRAGMENT_SHADER_SOURCE);

    borderShader = Shader();
    borderShader.Create(ShaderSources::BORDER_VERTEX_SHADER_SOURCE, ShaderSources::BORDER_FRAGMENT_SHADER_SOURCE);

    compositeShader = Shader();
    compositeShader.Create(ShaderSources::COMPOSITE_VERTEX_SHADER_SOURCE, ShaderSources::COMPOSITE_FRAGMENT_SHADER_SOURCE);
}

void EarthRenderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    camera.Update();

    RenderBorders();
    RenderGlobe();
    //RenderCountryPolygons();
    RenderComposite();

    debugRotation = 0.0f;
}

void EarthRenderer::resizeGL(int width, int height) {
    aspectRatio = (float)width / (float)height;

    earthColorFramebuffer.Update(width, height);
    sceneColorFramebuffer.Update(width, height);
    borderColorFramebuffer.Update(width, height);

    glViewport(0, 0, width, height);
}

void EarthRenderer::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePosition = event->position().toPoint();
    }
}

void EarthRenderer::mouseMoveEvent(QMouseEvent* event) {

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

void EarthRenderer::wheelEvent(QWheelEvent* event) {
    const float delta = static_cast<float>(event->angleDelta().y());
    camera.Zoom(delta);
    update();
}

void EarthRenderer::RenderGlobe() {

    glm::mat4 projectionMatrix = camera.GetProjectionMatrix(aspectRatio);
    glm::mat4 lookAtMatrix = camera.GetLookAtMatrix();

    earthColorFramebuffer.Bind();

    glViewport(0, 0, width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 rotationMatrix = RotationMatrixDegrees(glm::vec3(0, debugRotation, 0));
    glm::mat4 model = earthTilt * rotationMatrix;

    globeShader.Bind();
    globeShader.SetMat4("projection", projectionMatrix);
    globeShader.SetMat4("lookAt", lookAtMatrix);
    globeShader.SetMat4("model", model);
    globe.Render();

    earthColorFramebuffer.Unbind();
}

void EarthRenderer::RenderBorders() {

    glm::mat4 projectionMatrix = camera.GetProjectionMatrix(aspectRatio);
    glm::mat4 lookAtMatrix = camera.GetLookAtMatrix();

    glm::mat4 rotationMatrix = RotationMatrixDegrees(glm::vec3(0, debugRotation, 0));
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(500.0f)) * earthTilt * rotationMatrix;

    borderColorFramebuffer.Bind();

    glViewport(0, 0, width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    borderShader.Bind();
    borderShader.SetMat4("projection", projectionMatrix);
    borderShader.SetMat4("lookAt", lookAtMatrix);
    borderShader.SetMat4("model", model);
    borderShader.SetVec3("cameraDirection", camera.GetDirection());

    borders.Render();

    borderColorFramebuffer.Unbind();
}

void EarthRenderer::RenderCountryPolygons() {

    glm::mat4 projectionMatrix = camera.GetProjectionMatrix(aspectRatio);
    glm::mat4 lookAtMatrix = camera.GetLookAtMatrix();

    glm::mat4 rotationMatrix = RotationMatrixDegrees(glm::vec3(0, debugRotation, 0));
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(500.0f)) * earthTilt * rotationMatrix;

    countryPolygonsColorFramebuffer.Bind();

    glViewport(0, 0, width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    borderShader.Bind();
    borderShader.SetMat4("projection", projectionMatrix);
    borderShader.SetMat4("lookAt", lookAtMatrix);
    borderShader.SetMat4("model", model);
    borderShader.SetVec3("cameraDirection", camera.GetDirection());

    USA.Render();

    countryPolygonsColorFramebuffer.Unbind();
}

void EarthRenderer::RenderScene() {

}

void EarthRenderer::RenderComposite() {

    const qreal dpr = devicePixelRatioF();
    glViewport(0, 0, static_cast<GLsizei>(width() * dpr), static_cast<GLsizei>(height() * dpr));

    compositeShader.Bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earthColorFramebuffer.GetColorTextureId());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, borderColorFramebuffer.GetColorTextureId());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, countryPolygonsColorFramebuffer.GetColorTextureId());

    compositeShader.SetInt("earthTexture", 0);
    compositeShader.SetInt("borderTexture", 1);
    compositeShader.SetInt("countryPolygonsTexture", 2);

    composite.Render();
}
