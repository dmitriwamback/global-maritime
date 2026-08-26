//
// Created by Dmitri on 2026-08-15.
//

#ifndef GLOBAL_MARITIME_GLOBEVIEWER_H
#define GLOBAL_MARITIME_GLOBEVIEWER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "../core/objects/Borders.h"
#include "../core/objects/Globe.h"
#include "../core/objects/Camera.h"
#include "../core/objects/CountryPolygons.h"
#include "../core/renderpasses/ColorFramebuffer.h"
#include "../core/renderpasses/DepthFramebuffer.h"
#include "../core/renderpasses/Quad.h"


class GlobeViewer : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    GlobeViewer();
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private:

    Globe globe;
    Borders borders;
    Quad composite;

    Shader globeShader, borderShader, compositeShader;

    Camera camera;
    float aspectRatio = 1;

    void RenderGlobe();
    void RenderBorders();
    void RenderScene();
    //void RenderCountryPolygons();
    void RenderComposite();

    QPoint lastMousePosition;
    CountryPolygons USA;

    DepthFramebuffer earthDepthFramebuffer, sceneDepthFramebuffer;
    ColorFramebuffer earthColorFramebuffer, sceneColorFramebuffer, borderColorFramebuffer;
};


#endif //GLOBAL_MARITIME_GLOBEVIEWER_H
